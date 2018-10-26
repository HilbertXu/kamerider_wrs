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

ros::Subscriber ResMat_Sub;//Res矩阵接收器
//消息类型为std_msgs::Float64MultiArray
//前两个数为rows和cols，之后跟着rows*cols个数(行优先)

using namespace std;
using namespace cv;
const double pi=3.1415926535897932384626;
double dT=1;
const int MAX_PATH=260;

vector<string>JointTopicNames;
vector<string>JointNames;
vector<string>JointFrameNames;
vector<ros::Publisher>JointPubs;
tf::TransformListener*pListener;//监听tf变换的设备指针,具体定义在main函数ros::init之后了
tf::StampedTransform CurrTf;//监听到的tf变换

string GCP_MPos_xmlPath;
string BagDir;
vector<string> GCP_Names;//抓取点名称暨bag包名字
Mat M_Pos_GCP;

//读取xml函数
void ReadModelsXml()
{
	FileStorage xmlFile(GCP_MPos_xmlPath, FileStorage::READ);
	
	//读取GCP位置个数
	int N_GCP;
	xmlFile["N_GCP"]>>N_GCP;
	
	char Buffer[MAX_PATH];
	//读取GCP位置的名字暨bag包名字
	for(int i=1;i<=N_GCP;i++)
	{
		string TmpStr;
		sprintf(Buffer,"GCP_Name_%d",i);
		xmlFile[Buffer]>>TmpStr;
		GCP_Names.push_back(TmpStr);
	}
	
	//读取Furniture位置,每行三个坐标行向量
	M_Pos_GCP=Mat::zeros(N_GCP,3,CV_64F);
	xmlFile["M_Pos_GCP"] >> M_Pos_GCP;

	xmlFile.release();
}


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

void PubJointPos(ros::Publisher&Pub,string&Joint_name,double Joint_Pos,int duration_sec=0)
{
	trajectory_msgs::JointTrajectoryPoint Joint_point;
	Joint_point.positions.push_back(Joint_Pos);
	Joint_point.time_from_start.sec = duration_sec;
	
	trajectory_msgs::JointTrajectory Joint_trajectory;
	Joint_trajectory.joint_names.push_back(Joint_name);
	Joint_trajectory.points.push_back(Joint_point);
	
	Pub.publish(Joint_trajectory);
}

void ResMatCallback(std_msgs::Float64MultiArray msg)
{
	int rows=msg.data[0];
	int cols=msg.data[1];
	Mat Res=Mat::zeros(rows,cols,CV_64F);
	double*p=(double*)(Res.data);
	int N=rows*cols;
	for(int i=0;i<N;i++)
	{
		p[i]=msg.data[2+i];
	}
	
	int rIdx=0;
	for(int i=0;i<rows;i++)
	{
		if(p[i*cols+3]==1)
		{
			rIdx=i;
			break;
		}
	}
	
	double Dx=p[rIdx*cols+4];
	double Dy=p[rIdx*cols+5];
	double Dz=p[rIdx*cols+6];
	
	tf::Vector3 P;
	tf::Matrix3x3 R;
	
	while(true)
	{
		try
		{
			string DadName="map";
			string SonName="base_link";
			
			pListener->lookupTransform(&(DadName[0]), &(SonName[0]),ros::Time(0), CurrTf);
			P=CurrTf.getOrigin();
			R=CurrTf.getBasis();
			break;
		}
		catch (tf::TransformException &ex) 
		{
			ROS_ERROR("%s",ex.what());
			return;//听不到就再去听,不能直接往下走了
			ros::Duration(1.0).sleep();
		}
	}

	double Dx_,Dy_,Dz_;
	{
		Dx_=R[0][0]*Dx+R[0][1]*Dy+R[0][2]*Dz+P[0];
		Dy_=R[1][0]*Dx+R[1][1]*Dy+R[1][2]*Dz+P[1];
		Dz_=R[2][0]*Dx+R[2][1]*Dy+R[2][2]*Dz+P[2];
	}
	
	double a=atan2(Dy_,Dx_);
	double Tr2=(Dz_ - T_01_z)*(Dz_ - T_01_z)/(Dx_*Dx_+Dy_*Dy_);
	double r=-atan(sqrt(Tr2));
	
	printf("接收到GCP_Res：\n");
	printf("base_link下指向点坐标描述为\n\tDx_=%.17g,Dy_=%.17g,Dz_=%.17g\n",Dx_,Dy_,Dz_);
	printf("则头部旋转角a=%.17g\n",a);
	printf("则头部俯仰角r=%.17g\n",r);
	
	int i;
	i=7;PubJointPos(JointPubs[i],JointNames[i],a);
	ros::Duration(dT).sleep();
	i=8;PubJointPos(JointPubs[i],JointNames[i],r);
	ros::Duration(dT).sleep();
	
	printf("我认为当前头部RGBD相机已经能拍到GCP的物体\n");
	printf("元芳，你怎么看\n\n");
	
	
	//运行合适的bag包
	/*
	int bagIdx=0;
	double Dis2=0;
	
	int N_GCP=GCP_Names.size();
	
	double*pData=(double*)(M_Pos_GCP.data);
	{
		Dis2=(pData[0]-Dx)*(pData[0]-Dx)+
				(pData[1]-Dy)*(pData[1]-Dy)+
				(pData[2]-Dz)*(pData[2]-Dz);
		bagIdx=0;
	}
	for(int row=1;row<N_GCP;row++)
	{
		double TmpDis2=(pData[row*3+0]-Dx)*(pData[row*3+0]-Dx)+
				(pData[row*3+1]-Dy)*(pData[row*3+1]-Dy)+
				(pData[row*3+2]-Dz)*(pData[row*3+2]-Dz);
		if(TmpDis2<Dis2)
		{
			Dis2=TmpDis2;
			bagIdx=row;
		}
	}
	
	string Cmd="rosbag play '"+BagDir+"/"+GCP_Names[bagIdx]+".bag'";
	cout<<Cmd<<endl;
	system(&(Cmd[0]));
	cout<<"我认为已经抓住物体了，燃烧我的卡路里"<<endl;
	*/
}

int main(int argc, char **argv)
{
	// GCP_MPos_xmlPath=argv[1];
	// BagDir=argv[2];
	// ReadModelsXml();
	
	//vector<string>JointTopicNames;
		JointTopicNames.push_back("/hsrb/arm_trajectory_controller/command");
		JointTopicNames.push_back("/hsrb/arm_trajectory_controller/command");
		JointTopicNames.push_back("/hsrb/arm_trajectory_controller/command");
		JointTopicNames.push_back("/hsrb/gripper_trajectory_controller/command");
		JointTopicNames.push_back("/hsrb/gripper_trajectory_controller/command");
		JointTopicNames.push_back("/hsrb/gripper_trajectory_controller/command");
		JointTopicNames.push_back("/hsrb/gripper_trajectory_controller/command");
		JointTopicNames.push_back("/hsrb/head_trajectory_controller/command");
		JointTopicNames.push_back("/hsrb/head_trajectory_controller/command");
	//vector<string>JointNames;
		JointNames.push_back("arm_lift_joint");
		JointNames.push_back("arm_flex_joint");
		JointNames.push_back("arm_roll_joint");
		JointNames.push_back("wrist_flex_joint");
		JointNames.push_back("wrist_roll_joint");
		JointNames.push_back("hand_l_proximal_joint");
		JointNames.push_back("hand_r_proximal_joint");
		JointNames.push_back("head_pan_joint");
		JointNames.push_back("head_tilt_joint");
	//vector<string>JointFrameNames;
		JointFrameNames.push_back("arm_lift_link");
		JointFrameNames.push_back("arm_flex_link");
		JointFrameNames.push_back("arm_roll_link");
		JointFrameNames.push_back("wrist_flex_link");
		JointFrameNames.push_back("wrist_roll_link");
		JointFrameNames.push_back("hand_l_proximal_link");
		JointFrameNames.push_back("hand_r_proximal_link");
		JointFrameNames.push_back("head_pan_link");
		JointFrameNames.push_back("head_tilt_link");
	//vector<ros::Publisher>JointPubs;
	
	int JointN=JointNames.size();
	cout<<"我是GCPresPanTilt,我在等待Res矩阵的到来(￢_￢)"<<endl;
	cout<<"Res矩阵一来,我将控制摆头到合适角度,从而能看到Avatar指向的物体"<<endl;
	
	ros::init(argc, argv, "GCPresPanTilt");
	ros::NodeHandle n;
	ResMat_Sub=n.subscribe("/OPAros/GCPRes", 1, ResMatCallback);
	tf::TransformListener Listener;
	pListener = &Listener;
	
	for(int i=0;i<JointN;i++)
	{
		ros::Publisher Pub=n.advertise<trajectory_msgs::JointTrajectory>(JointTopicNames[i], 1);
		JointPubs.push_back(Pub);
	}

	ros::spin();
	
	return 0;
}
