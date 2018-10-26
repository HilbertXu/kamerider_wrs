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

//OpenPose���⵽Pose�ؽڶ�Ӧ���(��0��ʼ��,��17)
#define MNeckNum 1
#define RShoulderNum 2
#define RElbowNum 3
#define RWristNum 4
#define LShoulderNum 5
#define LElbowNum 6
#define LWristNum 7
//OpenPose���⵽�ֲ��ؽڶ�Ӧ���(��0��ʼ��,��20)
#define IFinSecNum 7

const double VeryBig=9e300;
const int MAX_PATH=260;
char Buffer[MAX_PATH];//������
unsigned char floatBuffer[4];//���ֽںϲ�float���ݵĻ�����
int PngStaticNum=0;//����ͼƬ�����,Pick_it_up!ʱΪ1,Clean_up!ʱΪ2
int PtsStaticNum=0;//������Ϣ�����,Pick_it_up!ʱΪ1,Clean_up!ʱΪ2
bool PngIsSave=false;//��ǰ�Ƿ��յ�Pick_it_up!��Clean_up!��Ϣ,�Ӷ�ȷ���Ƿ�ͼƬд��
bool PtsIsSave=false;//��ǰ�Ƿ��յ�Pick_it_up!��Clean_up!��Ϣ,�Ӷ�ȷ���Ƿ񽫵�����Ϣд��
pcl::PointCloud<pcl::PointXYZ> Cloud_Pick;//�洢Pick_it_up!ʱͷ������������ת��map����ϵ�µ�����
pcl::PointCloud<pcl::PointXYZ> Cloud_Clean;//�洢Clean_up!ʱͷ������������ת��map����ϵ�µ�����
Mat Img_Pick;//�洢Pick_it_up!ʱͷ�����������㵽��RGBͼƬ
Mat Img_Clean;//�洢Clean_up!
Mat Img_Frame;//�洢ͷ�����������㵽��RGBͼƬ���������������������������������������������������������������������������������������������������
pcl::PointCloud<pcl::PointXYZ> Cloud_Frame;//�洢ÿһ֡ͷ������������ת��map����ϵ�µ�����
pcl::PointCloud<pcl::PointXYZ> Cloud_BFrame;//�洢ÿһ֡ͷ������������ת��base_link����ϵ�µ�����
const int ImgWidth = 640;//ͷ�����������㵽��RGBͼƬ���
const int ImgHeight = 480;//ͷ�����������㵽��RGBͼƬ�߶�

vector<double>JointPos;//���ؽ��ڹؽڿռ���λ��

string SaveDir;//png��json�ļ�����·��
ros::Subscriber sub_Img;//ͷ��������RGBͼƬ�Ķ�����
ros::Subscriber sub_PtCloud;//ͷ��������������Ϣ�Ķ�����
ros::Subscriber sub_Cup;//Avatarָ����Ϣ�Ķ�����
tf::StampedTransform CurrTf;//��������tf�任
tf::TransformListener*pListener;//����tf�任���豸ָ��,���嶨����main����ros::init֮����
ros::Publisher pcl_pub;//����ת��map����ϵ�������ķ�����
ros::Publisher TargetPos_pub;//�������ս��
ros::Publisher bOBJpos_pub;//��������Ҽ���ѡ�������ڱ�������ϵ�µ���������
ros::Subscriber JointState_sub;//���ؽ���Ϣ�Ķ�����

string InitName="LookObjPtsInImg_Map";
string base_link_Name="base_link";
string rgbdFrame_Name="head_rgbd_sensor_depth_frame";

string SaveFileName;//��ʼ����xml·��
Mat M_U_T;//Unity����ϵ��Map����ϵ�µ�����
Mat U_Pos_GCP;//ץȡ����Unity�µ�λ������
Mat U_Pos_DCP;//���õ���Unity�µ�����
vector<string> GCP_Names;//ץȡ������
vector<string> DCP_Names;//���õ�����
vector<string> GCP_Names_sort;//ץȡ������
vector<string> DCP_Names_sort;//���õ�����

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
//���ص�����,�ú����������и���Ŀ���ѡ��
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
//����ͷ��������RGBͼƬ�Ļص�����
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
	
	Img_Frame=Img.clone();//���������������������������������������������������������������������������������������������������
	
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

//����ͷ��������������Ϣ�Ļص�����
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
    pcl::PointCloud<pcl::PointXYZRGB> cloud;//���������������������������������������������������������������������������������������������������
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
		//�����е�����Ϣ�е�X,Y,Z��ȡ����
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
		
		//��������任����������䵽map����ϵ������
        cloud.points[i].x = R[0][0]*X+R[0][1]*Y+R[0][2]*Z+P[0];
        cloud.points[i].y = R[1][0]*X+R[1][1]*Y+R[1][2]*Z+P[1];
        cloud.points[i].z = R[2][0]*X+R[2][1]*Y+R[2][2]*Z+P[2];
		
		//�ϳ���άRGB����
		cloud.points[i].r = Img_Frame.data[i*3+2];//���������������������������������������������������������������������������������������������������
		cloud.points[i].g = Img_Frame.data[i*3+1];//���������������������������������������������������������������������������������������������������
		cloud.points[i].b = Img_Frame.data[i*3];//���������������������������������������������������������������������������������������������������

		//���浽ȫ�ֱ����﹩imageCallback����
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
