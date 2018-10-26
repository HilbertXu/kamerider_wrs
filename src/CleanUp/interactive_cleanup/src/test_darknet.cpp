#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <cmath>

#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <tf/transform_listener.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <tf2/LinearMath/Vector3.h>
#include <pcl/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include "std_msgs/String.h"
#include <trajectory_msgs/JointTrajectory.h>
#include <trajectory_msgs/JointTrajectoryPoint.h>
#include <std_msgs/Float64MultiArray.h>

ros::Subscriber ResMat_Sub;//矩阵发布器
//消息类型为std_msgs::Float64MultiArray
//前两个数为rows和cols，之后跟着rows*cols个数(行优先)
ros::Subscriber sub_Img;
ros::Publisher bOBJimage_pub;//发布当前头部传感器RGB图片给darknet以检测出待抓取的物体中心在图片中位置


using namespace std;
using namespace cv;
string InitName="GetGCPinDarknet";
string OB_dir = "/home/kamerider/sim_ws/src/CleanUp/interactive_cleanup/Observer";
const double pi=3.1415926535897932384626;
double dT=1;
bool IF_IN_POSITION = false;

vector<string>JointTopicNames;
vector<string>JointNames;
vector<string>JointFrameNames;
vector<ros::Publisher>JointPubs;

double head_pan_angle_min=-3.8389999448327217;
double head_pan_angle_max=1.745000071766185;
double head_tilt_angle_min=-1.5700000294671668;
double head_tilt_angle_max=0.522999959775845;
// 0 is base_link
// 1 is head_pan_link
// 2 is head_tilt_link
// 3 is head_rgbd_sensor_depth_frame
double T_01_z=0.75199997425079346;
double T_23_z=0.21517200767993927;
int Count = 0 ;

Mat Img_Frame;//存储头部传感器拍摄到的RGB图片
void imageCallback(sensor_msgs::Image msg)
{
  Count++;
  if(Count == 10)
  {
    IF_IN_POSITION = true;
  }
	int height=msg.height;
	int width=msg.width;
	int N=msg.data.size();

	Mat Img=Mat::zeros(height,width,CV_8UC3);
	unsigned char*pData=Img.data;
	for(int i=0;i<N;i=i+3)
	{
		pData[i+2]=msg.data[i];
		pData[i+1]=msg.data[i+1];
		pData[i]=msg.data[i+2];
	}

	Img_Frame=Img.clone();//――――――――――――――――――――――――――――――――――――――――――――――额外添加

	cv::imshow("The Lonely Observer",Img);//it is the chosen of Steins Gate
	string PATH = OB_dir + "GCP_Observer.png";
	cv::imwrite(PATH, Img_Frame);
	int key=cv::waitKey(10);
	if(IF_IN_POSITION)
	{
		bOBJimage_pub.publish(msg);
	}
}


int main(int argc, char **argv)
{
	ros::init(argc, argv, "darknet_test");
	ros::NodeHandle n;
	sub_Img = n.subscribe("/hsrb/head_rgbd_sensor/rgb/image_raw", 1, imageCallback);

	bOBJimage_pub=n.advertise<sensor_msgs::Image>("/bOBJimage", 1);
	ros::spin();
	return 0;
}
