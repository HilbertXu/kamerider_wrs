#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <cmath>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <csignal>
#include <unistd.h>
#include <termios.h>
#include <chrono>

#include <ros/ros.h>
#include <interactive_cleanup/InteractiveCleanupMsg.h>
#include <sensor_msgs/PointCloud2.h>
#include <tf/transform_listener.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <tf2/LinearMath/Vector3.h>
#include <pcl/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include "std_msgs/String.h"
#include "std_msgs/Float64MultiArray.h"
#include <sensor_msgs/JointState.h>
#include "darknet_ros_msgs/BoundingBox.h"
#include "darknet_ros_msgs/BoundingBoxes.h"

using namespace std;
using namespace cv;
using namespace std::chrono;


const double VeryBig=9e300;
const int MAX_PATH=260;
char Buffer[MAX_PATH];//缓冲区
unsigned char floatBuffer[4];//四字节合并float数据的缓冲区

pcl::PointCloud<pcl::PointXYZ> Cloud_Frame;//存储每一帧头部传感器点云转到map坐标系下的描述
pcl::PointCloud<pcl::PointXYZ> Cloud_BFrame;//存储每一帧头部传感器点云转到base_link坐标系下的描述
const int ImgWidth = 640;//头部传感器拍摄到的RGB图片宽度
const int ImgHeight = 480;//头部传感器拍摄到的RGB图片高度

bool ifArrivedGCP         = false;
bool ifArrivedDCP         = false;
bool ifReceivedDarknet    = false;

vector<double>JointPos;//各关节在关节空间中位置

string SaveDir;//png、json文件保存路径
ros::Subscriber sub_Img;//头部传感器RGB图片的订阅器
ros::Subscriber sub_PtCloud;//头部传感器点云消息的订阅器
ros::Subscriber sub_Cup;//Avatar指令信息的订阅器
ros::Subscriber sub_darknet;
tf::StampedTransform CurrTf;//监听到的tf变换
tf::StampedTransform sensor2base_link;
tf::TransformListener *pListener;//监听tf变换的设备指针,具体定义在main函数ros::init之后了
tf::TransformListener *baseListener;
ros::Publisher pcl_pub;//点云转到map坐标系下坐标后的发布器
ros::Publisher TargetPos_pub;//发布最终结果
ros::Publisher bOBJpos_pub;//发布鼠标右键点选的物体在本体坐标系下的坐标描述
ros::Subscriber JointState_sub;//各关节信息的订阅器
ros::Subscriber nav_sub;

string InitName="LookObjPtsInImg_Map";
string base_link_Name="base_link";
string rgbdFrame_Name="head_rgbd_sensor_depth_frame";

time_point<system_clock> latest_time_of_bounding_boxes_;
time_point<system_clock> latest_time_of_point_cloud_;


darknet_ros_msgs::BoundingBoxes bounding_boxes_data_;
sensor_msgs::PointCloud2        point_cloud_data_;

string LookSaveDir;
int LookSaveNum = 0;
int data_count  = 0;
int num = 0;
float pos_x, pos_y, pos_z;
float pcl_x, pcl_y, pcl_z;
float mini_distance = 100000;
float curr_distance = 0;
string xml_dir = "/home/kamerider/sim_ws/src/CleanUp/interactive_cleanup/output_file/GCP_Nav_Pos.xml";

const int MAX_OBJECTS_NUM = 3;

int center_x[MAX_OBJECTS_NUM];
int center_y[MAX_OBJECTS_NUM];
int idx[MAX_OBJECTS_NUM];

//四舍五入函数
double round(double r)
{
	return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}

void reset()
{
	latest_time_of_point_cloud_    = system_clock::now();
	latest_time_of_bounding_boxes_ = system_clock::now(); 
	bounding_boxes_data_.boundingBoxes.reserve(MAX_OBJECTS_NUM);
}

void readNavXml()
{
	cv::FileStorage GCP_xml(xml_dir, cv::FileStorage::READ);
	pos_x = GCP_xml["position"]["x"];
	pos_y = GCP_xml["position"]["y"];
	pos_z = GCP_xml["position"]["z"];
}

void darknetCallback(const darknet_ros_msgs::BoundingBoxes::ConstPtr& msg)
{

	data_count = std::min<int>(msg->boundingBoxes.size(), MAX_OBJECTS_NUM);
	bounding_boxes_data_.boundingBoxes.resize(data_count);

	for(int i=0; i<data_count ;i++)
	{
		bounding_boxes_data_.boundingBoxes[i].Class       = msg->boundingBoxes[i].Class;
		bounding_boxes_data_.boundingBoxes[i].probability = msg->boundingBoxes[i].probability;
		bounding_boxes_data_.boundingBoxes[i].xmin        = msg->boundingBoxes[i].xmin;
		bounding_boxes_data_.boundingBoxes[i].ymin        = msg->boundingBoxes[i].ymin;
		bounding_boxes_data_.boundingBoxes[i].xmax        = msg->boundingBoxes[i].xmax;
		bounding_boxes_data_.boundingBoxes[i].ymax        = msg->boundingBoxes[i].ymax;
		center_x[i] = (bounding_boxes_data_.boundingBoxes[i].xmin + bounding_boxes_data_.boundingBoxes[i].xmax)/2;
		center_y[i] = (bounding_boxes_data_.boundingBoxes[i].ymin + bounding_boxes_data_.boundingBoxes[i].ymax)/2;
	} 
}

void navCallback(const std_msgs::String::ConstPtr &msg)
{
	if(msg->data == "GCP_Arrived")
	{
		ifArrivedGCP = true;
	}
}

void pclCallback(sensor_msgs::PointCloud2 msg)
{
	try
	{
		pListener->lookupTransform("/map", "head_rgbd_sensor_depth_frame",ros::Time(0), CurrTf);
		baseListener->lookupTransform("/base_link", "head_rgbd_sensor_depth_frame", ros::Time(0), sensor2base_link);
	}
	catch (tf::TransformException &ex)
	{
		ROS_ERROR("%s",ex.what());
		ros::Duration(1.0).sleep();
	}


	tf::Vector3 P=CurrTf.getOrigin();
	tf::Matrix3x3 R=CurrTf.getBasis();

	tf::Vector3 W = sensor2base_link.getOrigin();
	tf::Matrix3x3 Q = sensor2base_link.getBasis();

	sensor_msgs::PointCloud2 oMsg;
	Cloud_Frame.width = msg.width;
	Cloud_Frame.height = msg.height;
	Cloud_Frame.points.resize(Cloud_Frame.width * Cloud_Frame.height);

	Cloud_BFrame.width = msg.width;
	Cloud_BFrame.height = msg.height;
	Cloud_BFrame.points.resize(Cloud_BFrame.width * Cloud_BFrame.height);

	for(size_t i = 0; i < Cloud_Frame.size(); i++)
	{
		//把已有点云消息中的X,Y,Z提取出来
		floatBuffer[0]=msg.data[i*16+0];
		floatBuffer[1]=msg.data[i*16+1];
		floatBuffer[2]=msg.data[i*16+2];
		floatBuffer[3]=msg.data[i*16+3];
		double X=*((float*)floatBuffer);

		floatBuffer[0]=msg.data[i*16+4];
		floatBuffer[1]=msg.data[i*16+5];
		floatBuffer[2]=msg.data[i*16+6];
		floatBuffer[3]=msg.data[i*16+7];
		double Y=*((float*)floatBuffer);

		floatBuffer[0]=msg.data[i*16+8];
		floatBuffer[1]=msg.data[i*16+9];
		floatBuffer[2]=msg.data[i*16+10];
		floatBuffer[3]=msg.data[i*16+11];
		double Z=*((float*)floatBuffer);

		//利用坐标变换将点云坐标变到map坐标系下描述
        Cloud_Frame.points[i].x = R[0][0]*X+R[0][1]*Y+R[0][2]*Z+P[0];
        Cloud_Frame.points[i].y = R[1][0]*X+R[1][1]*Y+R[1][2]*Z+P[1];
        Cloud_Frame.points[i].z = R[2][0]*X+R[2][1]*Y+R[2][2]*Z+P[2];

        Cloud_BFrame.points[i].x = Q[0][0]*X + Q[0][1]*Y + Q[0][2]*Z + W[0];
        Cloud_BFrame.points[i].y = Q[1][0]*X + Q[1][1]*Y + Q[1][2]*Z + W[1];
        Cloud_BFrame.points[i].z = Q[2][0]*X + Q[2][1]*Y + Q[2][2]*Z + W[2];
	}
}

void setMiniDis()
{
	for(int i =0; i<data_count; i++)
	{
		int center_x = round((bounding_boxes_data_.boundingBoxes[i].xmin + bounding_boxes_data_.boundingBoxes[i].xmax)/2);
		int center_y = round((bounding_boxes_data_.boundingBoxes[i].ymin + bounding_boxes_data_.boundingBoxes[i].ymax)/2);
		idx[i] = center_x + center_y * ImgWidth;
		pcl_x = Cloud_Frame.points[idx[i]].x;
		pcl_y = Cloud_Frame.points[idx[i]].y;
		pcl_z = Cloud_Frame.points[idx[i]].z;
		curr_distance = pow((pcl_x - pos_x), 2) + pow((pcl_y - pos_y), 2) + pow((pcl_z - pcl_z), 2);
		if(mini_distance > curr_distance)
		{
			mini_distance = curr_distance;
			num = i;
		}
	}
	std_msgs::Float64MultiArray msg;
	msg.data.push_back(Cloud_BFrame.points[num].x);
	msg.data.push_back(Cloud_BFrame.points[num].y);
	msg.data.push_back(Cloud_BFrame.points[num].z);
	bOBJpos_pub.publish(msg);
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "find_graspable_object");
	ros::NodeHandle nh;
	ROS_INFO("reading position file");
	readNavXml();
	sub_darknet = nh.subscribe("/darknet_ros/bounding_boxes", 1, darknetCallback);
	bOBJpos_pub = nh.advertise<std_msgs::Float64MultiArray>("/bOBJgcp", 1);
	nav_sub	    = nh.subscribe("/nav2obj", 1, navCallback);

	tf::TransformListener Listener;
	pListener = &Listener;

	tf::TransformListener BaseListener_;
	baseListener = &BaseListener_;

}




