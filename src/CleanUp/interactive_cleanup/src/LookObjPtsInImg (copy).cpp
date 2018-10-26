#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <cmath>

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

using namespace std;
using namespace cv;

//OpenPose里检测到Pose关节对应编号(从0开始数,到17)
#define MNeckNum 1
#define RShoulderNum 2
#define RElbowNum 3
#define RWristNum 4
#define LShoulderNum 5
#define LElbowNum 6
#define LWristNum 7
//OpenPose里检测到手部关节对应编号(从0开始数,到20)
#define IFinSecNum 7

const double VeryBig=9e300;
const int MAX_PATH=260;
char Buffer[MAX_PATH];//缓冲区
unsigned char floatBuffer[4];//四字节合并float数据的缓冲区
int PngStaticNum=0;//保存图片的序号,Pick_it_up!时为1,Clean_up!时为2
int PtsStaticNum=0;//接收消息的序号,Pick_it_up!时为1,Clean_up!时为2
bool PngIsSave=false;//当前是否收到Pick_it_up!或Clean_up!消息,从而确定是否将图片写出
bool PtsIsSave=false;//当前是否收到Pick_it_up!或Clean_up!消息,从而确定是否将点云消息写出
pcl::PointCloud<pcl::PointXYZ> Cloud_Pick;//存储Pick_it_up!时头部传感器点云转到map坐标系下的描述
pcl::PointCloud<pcl::PointXYZ> Cloud_Clean;//存储Clean_up!时头部传感器点云转到map坐标系下的描述
Mat Img_Pick;//存储Pick_it_up!时头部传感器拍摄到的RGB图片
Mat Img_Clean;//存储Clean_up!
Mat Img_Frame;//存储头部传感器拍摄到的RGB图片——————————————————————————————————————————————额外添加
pcl::PointCloud<pcl::PointXYZ> Cloud_Frame;//存储每一帧头部传感器点云转到map坐标系下的描述
pcl::PointCloud<pcl::PointXYZ> Cloud_BFrame;//存储每一帧头部传感器点云转到base_link坐标系下的描述
const int ImgWidth = 640;//头部传感器拍摄到的RGB图片宽度
const int ImgHeight = 480;//头部传感器拍摄到的RGB图片高度

vector<double>JointPos;//各关节在关节空间中位置

string SaveDir;//png、json文件保存路径
ros::Subscriber sub_Img;//头部传感器RGB图片的订阅器
ros::Subscriber sub_PtCloud;//头部传感器点云消息的订阅器
ros::Subscriber sub_Cup;//Avatar指令信息的订阅器
tf::StampedTransform CurrTf;//监听到的tf变换
tf::TransformListener*pListener;//监听tf变换的设备指针,具体定义在main函数ros::init之后了
ros::Publisher pcl_pub;//点云转到map坐标系下坐标后的发布器
ros::Publisher TargetPos_pub;//发布最终结果
ros::Publisher bOBJpos_pub;//发布鼠标右键点选的物体在本体坐标系下的坐标描述
ros::Subscriber JointState_sub;//各关节信息的订阅器

string InitName="LookObjPtsInImg_Map";
string base_link_Name="base_link";
string rgbdFrame_Name="head_rgbd_sensor_depth_frame";

string SaveFileName;//初始加载xml路径
Mat M_U_T;//Unity坐标系在Map坐标系下的描述
Mat U_Pos_GCP;//抓取点在Unity下的位置坐标
Mat U_Pos_DCP;//放置点在Unity下的坐标
vector<string> GCP_Names;//抓取点名称
vector<string> DCP_Names;//放置点名称
vector<string> GCP_Names_sort;//抓取点名称
vector<string> DCP_Names_sort;//放置点名称

string LookSaveDir;
int LookSaveNum=0;
void SaveLook()
{
	if(!LookSaveDir.empty())
	{
		LookSaveNum++;
		char Buffer[MAX_PATH*10];
		
		sprintf(Buffer,"%s/lookImg_inPos_%d.png",&(LookSaveDir[0]),LookSaveNum);
		imwrite(Buffer,Img_Frame);
		cout<<"\tSave in "<<Buffer<<"\n";
		
		sprintf(Buffer,"%s/Pose_inPos_%d.txt",&(LookSaveDir[0]),LookSaveNum);
		cout<<"\tSave in "<<Buffer<<"\n";
		{
			try
			{
				pListener->lookupTransform("/map", &(base_link_Name[0]),ros::Time(0), CurrTf);
			}
			catch (tf::TransformException &ex) 
			{
				ROS_ERROR("%s",ex.what());
				ros::Duration(1.0).sleep();
			}
			
			{
				ofstream File(Buffer);
				tf::Vector3 P=CurrTf.getOrigin();
				tf::Matrix3x3 R=CurrTf.getBasis();
				sprintf(Buffer,"%.17g %.17g %.17g %.17g\n" \
								"%.17g %.17g %.17g %.17g\n" \
								"%.17g %.17g %.17g %.17g\n" \
								"0 0 0 1",
								R[0][0],R[0][1],R[0][2],P[0],
								R[1][0],R[1][1],R[1][2],P[1],
								R[2][0],R[2][1],R[2][2],P[2]);
				File<<Buffer;
				File.close();
			}
			
			if(!JointPos.empty())
			{
				sprintf(Buffer,"%s/PoseStates.txt",&(LookSaveDir[0]));
				ofstream File(Buffer,ios::app);
				tf::Vector3 P=CurrTf.getOrigin();
				tf::Matrix3x3 R=CurrTf.getBasis();
				sprintf(Buffer,"%.17g %.17g %.17g",P[0],P[1],atan2(R[1][0],R[0][0]));
				File<<Buffer;
				int JointN=JointPos.size();
				for(int i=0;i<JointN;i++)
				{
					sprintf(Buffer," %.17g",JointPos[i]);
					File<<Buffer;
				}
				File<<"\n";
				File.close();
			}

		}
		
		sprintf(Buffer,"%s/lookXYZRGB_inPos_%d.txt",&(LookSaveDir[0]),LookSaveNum);
		cout<<"\tSave in "<<Buffer<<"\n";
		{
			ofstream File(Buffer);
			int N=Cloud_Frame.points.size();
			unsigned char*p=Img_Frame.data;
			for(int i=0;i<N;i++)
			{
				sprintf(Buffer,"%.17g %.17g %.17g %d %d %d\n",
								Cloud_Frame.points[i].x,
								Cloud_Frame.points[i].y,
								Cloud_Frame.points[i].z,
								int(p[i*3+2]),int(p[i*3+1]),int(p[i*3]));
				File<<Buffer;
			}
			File.close();
		}
		
		// sprintf(Buffer,"%s/lookXYZRGB_inPos_%d.txt.XYZRGB",&(LookSaveDir[0]),LookSaveNum);
		// cout<<"\tSave in "<<Buffer<<"\n";
		// {
			// ofstream File(Buffer,ios::binary);
			// int N=Cloud_Frame.points.size();
			// unsigned char*p=Img_Frame.data;
			// float x,y,z,r,g,b;
			// for(int i=0;i<N;i++)
			// {
				// x=Cloud_Frame.points[i].x;
				// y=Cloud_Frame.points[i].y;
				// z=Cloud_Frame.points[i].z;
				// r=float(p[i*3+2])/255.0;
				// g=float(p[i*3+1])/255.0;
				// b=float(p[i*3])/255.0;
				// File.write((char*)(&x),sizeof(float));
				// File.write((char*)(&y),sizeof(float));
				// File.write((char*)(&z),sizeof(float));
				// File.write((char*)(&r),sizeof(float));
				// File.write((char*)(&g),sizeof(float));
				// File.write((char*)(&b),sizeof(float));
			// }
			// File.close();
		// }
		
		sprintf(Buffer,"%s/lookXYZRGB_inBPos_%d.txt",&(LookSaveDir[0]),LookSaveNum);
		cout<<"\tSave in "<<Buffer<<"\n";
		{
			ofstream File(Buffer);
			int N=Cloud_BFrame.points.size();
			unsigned char*p=Img_Frame.data;
			for(int i=0;i<N;i++)
			{
				sprintf(Buffer,"%.17g %.17g %.17g %d %d %d\n",
								Cloud_BFrame.points[i].x,
								Cloud_BFrame.points[i].y,
								Cloud_BFrame.points[i].z,
								int(p[i*3+2]),int(p[i*3+1]),int(p[i*3]));
				File<<Buffer;
			}
			File.close();
		}
		
		sprintf(Buffer,"%s/lookXYZRGB_inBPos_%d.txt.XYZRGB",&(LookSaveDir[0]),LookSaveNum);
		cout<<"\tSave in "<<Buffer<<"\n";
		{
			ofstream File(Buffer,ios::binary);
			int N=Cloud_BFrame.points.size();
			unsigned char*p=Img_Frame.data;
			float x,y,z,r,g,b;
			for(int i=0;i<N;i++)
			{
				x=Cloud_BFrame.points[i].x;
				y=Cloud_BFrame.points[i].y;
				z=Cloud_BFrame.points[i].z;
				r=float(p[i*3+2])/255.0;
				g=float(p[i*3+1])/255.0;
				b=float(p[i*3])/255.0;
				File.write((char*)(&x),sizeof(float));
				File.write((char*)(&y),sizeof(float));
				File.write((char*)(&z),sizeof(float));
				File.write((char*)(&r),sizeof(float));
				File.write((char*)(&g),sizeof(float));
				File.write((char*)(&b),sizeof(float));
			}
			File.close();
		}
	}
}

void on_mouse( int event, int x, int y, int flags, void* param )
//鼠标回调函数,该函数用鼠标进行跟踪目标的选择
{
	if(Img_Frame.empty()||Cloud_Frame.empty()||Cloud_BFrame.empty())return;
	int row=y,col=x;

	switch( event )
	{
	case CV_EVENT_LBUTTONDOWN:
		{
			int Idx=row*ImgWidth+col;
			printf("[row,col]=[%d,%d],[x,y,z]=[%.17g,%.17g,%.17g]\n",
					row,col,
					Cloud_Frame.points[Idx].x,Cloud_Frame.points[Idx].y,Cloud_Frame.points[Idx].z);
			break;
		}

	case CV_EVENT_RBUTTONDOWN:
		{
			int Idx=row*ImgWidth+col;
			printf("[row,col]=[%d,%d],[x,y,z](in %s)=[%.17g,%.17g,%.17g]\n",
					row,col,&(base_link_Name[0]),
					Cloud_BFrame.points[Idx].x,Cloud_BFrame.points[Idx].y,Cloud_BFrame.points[Idx].z);
			
			std_msgs::Float64MultiArray msg;
			msg.data.push_back(Cloud_BFrame.points[Idx].x);
			msg.data.push_back(Cloud_BFrame.points[Idx].y);
			msg.data.push_back(Cloud_BFrame.points[Idx].z);
			bOBJpos_pub.publish(msg);
			break;
		}
	}
}
int imageCallback_Num=0;
//订阅头部传感器RGB图片的回调函数
void imageCallback(sensor_msgs::Image msg)
{
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
	
	Img_Frame=Img.clone();//——————————————————————————————————————————————额外添加
	
	cv::imshow(InitName+"/PtsInImg",Img);
	int key=cv::waitKey(10);
	if(key=='s')
	{
		cout<<"SaveLook:\n{";
		SaveLook();
		cout<<"}\n";
	}
	
	if(imageCallback_Num==0)
	{
		string Wname=InitName+"/PtsInImg";
		cvSetMouseCallback(&(Wname[0]),on_mouse,0);
	}
	imageCallback_Num++;
}

//订阅头部传感器点云消息的回调函数
void PcloudCallback(sensor_msgs::PointCloud2 msg)
{
	try
	{
		pListener->lookupTransform("/map", &(rgbdFrame_Name[0]),ros::Time(0), CurrTf);
	}
	catch (tf::TransformException &ex) 
	{
		ROS_ERROR("%s",ex.what());
		ros::Duration(1.0).sleep();
	}
	tf::Vector3 P=CurrTf.getOrigin();
	tf::Matrix3x3 R=CurrTf.getBasis();
	
	try
	{
		pListener->lookupTransform(&(base_link_Name[0]), &(rgbdFrame_Name[0]),ros::Time(0), CurrTf);
	}
	catch (tf::TransformException &ex) 
	{
		ROS_ERROR("%s",ex.what());
		ros::Duration(1.0).sleep();
	}
	tf::Vector3 bP=CurrTf.getOrigin();
	tf::Matrix3x3 bR=CurrTf.getBasis();
	
	//pcl::PointCloud<pcl::PointXYZ> cloud;
    pcl::PointCloud<pcl::PointXYZRGB> cloud;//——————————————————————————————————————————————额外添加
    sensor_msgs::PointCloud2 oMsg;
    cloud.width  = msg.width;
    cloud.height = msg.height;
    cloud.points.resize(cloud.width * cloud.height);
	if(Cloud_Frame.empty())
	{
		Cloud_Frame.width = msg.width;
		Cloud_Frame.height = msg.height;
		Cloud_Frame.points.resize(Cloud_Frame.width * Cloud_Frame.height);
	}
	if(Cloud_BFrame.empty())
	{
		Cloud_BFrame.width = msg.width;
		Cloud_BFrame.height = msg.height;
		Cloud_BFrame.points.resize(Cloud_BFrame.width * Cloud_BFrame.height);
	}
    for (size_t i = 0; i < cloud.points.size(); ++i)
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
        cloud.points[i].x = R[0][0]*X+R[0][1]*Y+R[0][2]*Z+P[0];
        cloud.points[i].y = R[1][0]*X+R[1][1]*Y+R[1][2]*Z+P[1];
        cloud.points[i].z = R[2][0]*X+R[2][1]*Y+R[2][2]*Z+P[2];
		
		//合成三维RGB点云
		cloud.points[i].r = Img_Frame.data[i*3+2];//——————————————————————————————————————————————额外添加
		cloud.points[i].g = Img_Frame.data[i*3+1];//——————————————————————————————————————————————额外添加
		cloud.points[i].b = Img_Frame.data[i*3];//——————————————————————————————————————————————额外添加

		//保存到全局变量里供imageCallback访问
		Cloud_Frame.points[i].x = cloud.points[i].x;
		Cloud_Frame.points[i].y = cloud.points[i].y;
		Cloud_Frame.points[i].z = cloud.points[i].z;
		
        Cloud_BFrame.points[i].x = bR[0][0]*X+bR[0][1]*Y+bR[0][2]*Z+bP[0];
        Cloud_BFrame.points[i].y = bR[1][0]*X+bR[1][1]*Y+bR[1][2]*Z+bP[1];
        Cloud_BFrame.points[i].z = bR[2][0]*X+bR[2][1]*Y+bR[2][2]*Z+bP[2];
    }
	pcl::toROSMsg(cloud, oMsg);
	oMsg.header.frame_id = "map";
	pcl_pub.publish(oMsg);
}

void JointStateCallback(sensor_msgs::JointState msg)
{
	if(JointPos.empty())
	{
		int JointN=msg.position.size();
		for(int i=0;i<JointN;i++)
		{
			JointPos.push_back(msg.position[i]);
		}
	}
	else
	{
		int JointN=msg.position.size();
		for(int i=0;i<JointN;i++)
		{
			JointPos[i]=msg.position[i];
		}
	}
}


int main(int argc, char **argv)
{
	if(argc>=2)LookSaveDir=argv[1];
	cout<<LookSaveDir<<endl;
	if(argc>=5)
	{
		InitName=argv[2];
		base_link_Name=argv[3];
		rgbdFrame_Name=argv[4];
	}
	
	ros::init(argc, argv, &(InitName[0]));
	ros::NodeHandle n;
	sub_PtCloud = n.subscribe("/hsrb/head_rgbd_sensor/depth/points", 1, PcloudCallback);
	sub_Img = n.subscribe("/hsrb/head_rgbd_sensor/rgb/image_raw", 1, imageCallback);
	string MpName=string("/")+InitName+"/map_points";
	pcl_pub = n.advertise<sensor_msgs::PointCloud2>(MpName, 1);
	bOBJpos_pub=n.advertise<std_msgs::Float64MultiArray>("/bOBJgcp", 1);
	JointState_sub=n.subscribe<sensor_msgs::JointState>("/hsrb/joint_states",1,JointStateCallback);

	tf::TransformListener Listener;
	pListener = &Listener;
	
	ros::spin();
	return 0;
}
