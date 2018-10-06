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
#include "std_msgs/Float64MultiArray.h"
#include <trajectory_msgs/JointTrajectory.h>
#include <trajectory_msgs/JointTrajectoryPoint.h>

using namespace std;
using namespace cv;
const double pi=3.1415926535897932384626;
const int MAX_PATH=260;
double eTheld=1e-2;
double Kv=1;
double Kw=1;
double Kvx=1;
double Kvy=1;
double dT=0.1;//每dT秒进行一拍控制
bool isNav=true;
double MaxNavT=14;//导航最多等这么多秒
double h_max=0.68;//arm_lift_joint最高上升这么多
double theta_max=1;//arm_flex_joint放平后最多再向下转这么大角度
double JointDurT=1;//各关节发送目标位置后的等待时间

vector<string>JointTopicNames;
vector<string>JointNames;
vector<string>JointFrameNames;
vector<double>JointPos;
vector<double>JointPosDeltas;
vector<int>JointUpKeys;
vector<int>JointDownKeys;
vector<ros::Publisher>JointPubs;
ros::Subscriber bOBJpos_sub;//订阅要抓取物体中心在当前帧base_link下的描述
ros::Publisher pub_base_twist;//发布速度信息

tf::StampedTransform CurrTf;//监听到的tf变换
tf::TransformListener*pListener;//监听tf变换的设备指针,具体定义在main函数ros::init之后了

//MATLAB计算出的机械臂参数
double L_02=0.1410000026226044;
double H_02=0.3392546772956848;
double L_23=0.3492376804351807;
double H_23=-0.00510203605517745;
double D=0.07800000160932541;
double Hg=0.2;
double Bg=0.6;

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

void GcpCallback(std_msgs::Float64MultiArray msg)
{//自制控制器，操纵机械臂抓取给定物体，机械臂无避障功能
	try
	{
		pListener->lookupTransform("/map", "base_link",ros::Time(0), CurrTf);
	}
	catch (tf::TransformException &ex) 
	{
		ROS_ERROR("%s",ex.what());
		ros::Duration(1.0).sleep();
		return;
	}
	tf::Vector3 P=CurrTf.getOrigin();
	tf::Matrix3x3 R=CurrTf.getBasis();

	double Gx=msg.data[0];
	double Gy=msg.data[1];
	double Gz=msg.data[2];
	
	printf("接收到当前帧base_link(定义为B)下描述的物体中心坐标：\n[%.17g %.17g %.17g]\n",Gx,Gy,Gz);
	printf("固有参数定义如下：\n");
	printf("\t double L_02=%.17g;\n",L_02);
	printf("\t double H_02=%.17g;\n",H_02);
	printf("\t double L_23=%.17g;\n",L_23);
	printf("\t double H_23=%.17g;\n",H_23);
	printf("\t double D=%.17g;\n",D);
	printf("\t double Hg=%.17g;\n",Hg);
	printf("\t double Bg=%.17g;\n",Bg);
	
	//最优化问题
	//变量h∈[0,h_max]，theta∈[0,theta_max]
	//约束条件为h+H_02+H_23-L_23*sin(theta)=Gz+Hg
	//求最优解使得h+theta最小
	//对于某个给定的Gz，可以画出theta为横轴h为纵轴的可行域(正弦曲线的一部分)
	//然后用线性规划的图解法求解即可
	double h,Stheta,theta;
	if(Gz+Hg<H_02+H_23)
	{
		h=0;
		Stheta=(h+H_02+H_23-Gz-Hg)/L_23;
	}
	else
	{
		Stheta=0;
		h=Gz+Hg-H_02-H_23;
	}

	if(Stheta>sin(theta_max))
	{
		theta=theta_max;
	}
	else
	{
		theta=asin(Stheta);
	}
	if(h>h_max)h=h_max;

	double L=L_02+L_23*cos(theta);
	printf("计算参数如下：\n");
	printf("\t double h=%.17g;\n",h);
	printf("\t double theta=%.17g;\n",theta);
	printf("\t double L=%.17g;\n",L);
	
	printf("\n\n开始控制抓取物体：\n");
	printf("--------------------------------\n");
	printf("Step 1:发布导航命令以平移base_link原点到B坐标系下[Gx-L,Gy-D,0]\n");
	if(isNav)
	{//导航太不靠谱了，第一次无意间成功后，录屏的好几十次没有一次成功(╯‵□′)╯︵┻━┻
		//还是自己写一个里程计闭环控制器吧，管它避障不避障
		Mat M_B_T=(Mat_<double>(4,4)<<
					R[0][0],R[0][1],R[0][2],P[0],
					R[1][0],R[1][1],R[1][2],P[1],
					R[2][0],R[2][1],R[2][2],P[2],
					0,0,0,1);
		Mat B_b_T=(Mat_<double>(4,4)<<
					1,0,0,Gx-L,
					0,1,0,Gy-D,
					0,0,1,0,
					0,0,0,1);
		Mat M_b_T=M_B_T*B_b_T;
		double*p=(double*)(M_b_T.data);
		double M_Target_x=p[3];
		double M_Target_y=p[7];
		double M_Target_theta=atan2(R[1][0],R[0][0]);
		char Buffer[MAX_PATH];
		sprintf(Buffer,"rosrun i_image PubGoalPose %.17g %.17g 0 0 0 %.17g %.17g",
				M_Target_x,M_Target_y,sin(M_Target_theta/2),cos(M_Target_theta/2));
		system(Buffer);
		ros::Duration(20).sleep();
		
		printf("\tmap坐标系下描述的位姿误差=[△x △y △theta]=[...\n");
		double t1=ros::Time::now().toSec();
		while(true)
		{
			if(ros::Time::now().toSec()-t1>MaxNavT)break;
			try
			{
				pListener->lookupTransform("/map", "base_link",ros::Time(0), CurrTf);
			}
			catch (tf::TransformException &ex) 
			{
				ROS_ERROR("%s",ex.what());
				ros::Duration(1.0).sleep();
			}
			tf::Vector3 P=CurrTf.getOrigin();
			tf::Matrix3x3 R=CurrTf.getBasis();
			double M_Curr_x=P[0];
			double M_Curr_y=P[1];
			double M_Curr_theta=atan2(R[1][0],R[0][0]);
			double M_e_x=M_Target_x-M_Curr_x;
			double M_e_y=M_Target_y-M_Curr_y;
			double M_e_theta=M_Target_theta-M_Curr_theta;
			printf("%.17g %.17g %.17g\n",M_e_x,M_e_y,M_e_theta);
			
			if(eTheld>abs(M_e_x)&&eTheld>abs(M_e_y)&&eTheld>abs(M_e_theta))break;
		}
		printf("];\n");
		printf("\t我认为已经到达目标位置了\n");
		ros::Duration(3.0).sleep();
	}
	else
	/*{//里程计前进旋转闭环控制器……晃来晃去(╯‵□′)╯︵┻━┻
		//在B坐标系下的目标位姿
		double Tx=Gx-L;
		double Ty=Gy-D;
		double Ttheta=0;
		
		//在B坐标系下的初始位姿
		double Cr=0,Ctheta=0;
		double Ta=atan(Ty/Tx);
		if(isnan(Ta))
		{
			if(Ty>0)Ta=pi/2;
			else Ta=-pi/2;
		}
		
		geometry_msgs::Twist twist;
		double Pre_t=ros::Time::now().toSec();
		while(true)
		{
			twist.linear.x = 0.0;
			twist.linear.y = 0.0;
			twist.linear.z = 0.0;
			twist.angular.x = 0.0;
			twist.angular.y = 0.0;
			twist.angular.z = (Ta-Ctheta)*Kw;
			pub_base_twist.publish(twist);
			ros::Duration(dT).sleep();
			double Curr_t=ros::Time::now().toSec();
			Ctheta=Ctheta+twist.angular.z*(Curr_t-Pre_t);
			if(abs(Ta-Ctheta)<eTheld)
			{
				twist.angular.z = 0;
				pub_base_twist.publish(twist);pub_base_twist.publish(twist);pub_base_twist.publish(twist);
				pub_base_twist.publish(twist);pub_base_twist.publish(twist);pub_base_twist.publish(twist);
				pub_base_twist.publish(twist);pub_base_twist.publish(twist);pub_base_twist.publish(twist);
				ros::Duration(dT).sleep();
				break;
			}
		}
		
		double Tr=sqrt(Tx*Tx+Ty*Ty);
		if(Tx<0)Tr=-Tr;
		while(true)
		{
			twist.linear.x = (Tr-Cr)*Kv;
			twist.linear.y = 0.0;
			twist.linear.z = 0.0;
			twist.angular.x = 0.0;
			twist.angular.y = 0.0;
			twist.angular.z = 0.0;
			pub_base_twist.publish(twist);
			ros::Duration(dT).sleep();
			double Curr_t=ros::Time::now().toSec();
			Cr=Cr+twist.linear.x*(Curr_t-Pre_t);
			if(abs(Tr-Cr)<eTheld)
			{
				twist.linear.x = 0;
				pub_base_twist.publish(twist);pub_base_twist.publish(twist);pub_base_twist.publish(twist);
				pub_base_twist.publish(twist);pub_base_twist.publish(twist);pub_base_twist.publish(twist);
				pub_base_twist.publish(twist);pub_base_twist.publish(twist);pub_base_twist.publish(twist);
				ros::Duration(dT).sleep();
				break;
			}
		}
		
		while(true)
		{
			twist.linear.x = 0.0;
			twist.linear.y = 0.0;
			twist.linear.z = 0.0;
			twist.angular.x = 0.0;
			twist.angular.y = 0.0;
			twist.angular.z = (-Ta-Ctheta)*Kw;
			pub_base_twist.publish(twist);
			ros::Duration(dT).sleep();
			double Curr_t=ros::Time::now().toSec();
			Ctheta=Ctheta+twist.angular.z*(Curr_t-Pre_t);
			if(abs(-Ta-Ctheta)<eTheld)
			{
				twist.angular.z = 0;
				pub_base_twist.publish(twist);pub_base_twist.publish(twist);pub_base_twist.publish(twist);
				pub_base_twist.publish(twist);pub_base_twist.publish(twist);pub_base_twist.publish(twist);
				pub_base_twist.publish(twist);pub_base_twist.publish(twist);pub_base_twist.publish(twist);
				ros::Duration(dT).sleep();
				break;
			}
		}
		printf("\t我认为已经到达目标位置了\n");
	}*/
	{//里程计平移闭环控制器
		//在B坐标系下的目标位置
		double Tx=Gx-L;
		double Ty=Gy-D;
		
		while(true)
		{
			try
			{
				pListener->lookupTransform("/map", "base_link",ros::Time(0), CurrTf);
				break;
			}
			catch (tf::TransformException &ex) 
			{
				ROS_ERROR("%s",ex.what());
				ros::Duration(1.0).sleep();
			}
		}
		tf::Vector3 M_B_P=CurrTf.getOrigin();
		tf::Matrix3x3 M_B_R=CurrTf.getBasis();
		
		//在B坐标系下的初始位置
		double Cx=0;
		double Cy=0;
		
		geometry_msgs::Twist twist;
		double Pre_t=ros::Time::now().toSec();

		while(true)
		{
			printf("eX=Tx-Cx=%.17g\n",Tx-Cx);
			twist.linear.x = (Tx-Cx)*Kvx;
			twist.linear.y = 0.0;
			twist.linear.z = 0.0;
			twist.angular.x = 0.0;
			twist.angular.y = 0.0;
			twist.angular.z = 0.0;
			pub_base_twist.publish(twist);
			ros::Duration(dT).sleep();
			double Curr_t=ros::Time::now().toSec();
			
			
			// try
			// {
				// pListener->lookupTransform("/map", "base_link",ros::Time(0), CurrTf);
			// }
			// catch (tf::TransformException &ex) 
			// {
				// ROS_ERROR("%s",ex.what());
				// ros::Duration(1.0).sleep();
			// }
			// tf::Vector3 M_b_P=CurrTf.getOrigin();
			// tf::Matrix3x3 M_b_R=CurrTf.getBasis();
			// Cx=(M_b_P[0]-M_B_P[0])/M_B_R[0][0];
			Cx=Cx+twist.linear.x*(Curr_t-Pre_t);
			
			if(abs(Tx-Cx)<eTheld)
			{
				twist.linear.x = 0;
				pub_base_twist.publish(twist);pub_base_twist.publish(twist);pub_base_twist.publish(twist);
				pub_base_twist.publish(twist);pub_base_twist.publish(twist);pub_base_twist.publish(twist);
				pub_base_twist.publish(twist);pub_base_twist.publish(twist);pub_base_twist.publish(twist);
				ros::Duration(dT).sleep();
				
				printf("eX=Tx-Cx=%.17g\n",Tx-Cx);
				printf("X方向已经移动到位\n");
				break;
			}
			Pre_t=Curr_t;
		}
		
		while(true)
		{
			printf("eY=Ty-Cy=%.17g\n",Ty-Cy);
			twist.linear.x = 0.0;
			twist.linear.y = (Ty-Cy)*Kvy;
			twist.linear.z = 0.0;
			twist.angular.x = 0.0;
			twist.angular.y = 0.0;
			twist.angular.z = 0.0;
			pub_base_twist.publish(twist);
			ros::Duration(dT).sleep();
			double Curr_t=ros::Time::now().toSec();
			
			// try
			// {
				// pListener->lookupTransform("/map", "base_link",ros::Time(0), CurrTf);
			// }
			// catch (tf::TransformException &ex) 
			// {
				// ROS_ERROR("%s",ex.what());
				// ros::Duration(1.0).sleep();
			// }
			// tf::Vector3 M_b_P=CurrTf.getOrigin();
			// tf::Matrix3x3 M_b_R=CurrTf.getBasis();
			// Cy=(M_b_P[0]-M_B_P[0])/M_B_R[0][1];
			Cy=Cy+twist.linear.y*(Curr_t-Pre_t);
			
			if(abs(Ty-Cy)<eTheld)
			{
				twist.linear.y = 0;
				pub_base_twist.publish(twist);pub_base_twist.publish(twist);pub_base_twist.publish(twist);
				pub_base_twist.publish(twist);pub_base_twist.publish(twist);pub_base_twist.publish(twist);
				pub_base_twist.publish(twist);pub_base_twist.publish(twist);pub_base_twist.publish(twist);
				ros::Duration(dT).sleep();
				printf("eY=Ty-Cy=%.17g\n",Ty-Cy);
				printf("Y方向已经移动到位\n");
				break;
			}
			Pre_t=Curr_t;
		}
		printf("\t我认为已经到达目标位置了\n");
	}
	printf("Step 2:升臂开爪\n");
	{
		JointPos[0]=h;
		JointPos[1]=0;
		JointPos[2]=0;
		JointPos[3]=-pi/2;
		JointPos[4]=0;
		JointPos[5]=Bg;
		JointPos[6]=-Bg;
		printf("\t当前关节空间Q=[%.17g %.17g %.17g %.17g %.17g %.17g %.17g]\n",
				JointPos[0],
				JointPos[1],
				JointPos[2],
				JointPos[3],
				JointPos[4],
				JointPos[5],
				JointPos[6]);
		int JointN=7;
		for(int i=0;i<JointN;i++)
		{
			PubJointPos(JointPubs[i],JointNames[i],JointPos[i],JointDurT);
			ros::Duration(JointDurT).sleep();
		}
	}
	printf("Step 3:平臂\n");
	{
		//JointPos[0]=h;
		JointPos[1]=-pi/2-theta;
		//JointPos[2]=0;
		JointPos[3]=-pi/2+theta;
		//JointPos[4]=0;
		//JointPos[5]=Bg;
		//JointPos[6]=-Bg;
		printf("\t当前关节空间Q=[%.17g %.17g %.17g %.17g %.17g %.17g %.17g]\n",
				JointPos[0],
				JointPos[1],
				JointPos[2],
				JointPos[3],
				JointPos[4],
				JointPos[5],
				JointPos[6]);
		int i;
		
		i=1;
		PubJointPos(JointPubs[i],JointNames[i],JointPos[i],JointDurT);
		ros::Duration(JointDurT).sleep();
		
		i=3;
		PubJointPos(JointPubs[i],JointNames[i],JointPos[i],JointDurT);
		ros::Duration(JointDurT).sleep();
	}
	printf("Step 4:合爪\n");
	{
		// JointPos[0]=h;
		// JointPos[1]=-pi/2-theta;
		// JointPos[2]=0;
		// JointPos[3]=-pi/2+theta;
		// JointPos[4]=0;
		JointPos[5]=0;
		JointPos[6]=0;
		printf("\t当前关节空间Q=[%.17g %.17g %.17g %.17g %.17g %.17g %.17g]\n",
				JointPos[0],
				JointPos[1],
				JointPos[2],
				JointPos[3],
				JointPos[4],
				JointPos[5],
				JointPos[6]);
		int i;
		
		i=5;
		PubJointPos(JointPubs[i],JointNames[i],JointPos[i],JointDurT);
		i=6;
		PubJointPos(JointPubs[i],JointNames[i],JointPos[i],JointDurT);
		ros::Duration(JointDurT).sleep();
	}
	printf("Step 5:复位\n");
	{
		JointPos[0]=h;
		JointPos[1]=0;
		JointPos[2]=0;
		JointPos[3]=-pi/2;
		JointPos[4]=0;
		JointPos[5]=0;
		JointPos[6]=0;
		printf("\t当前关节空间Q=[%.17g %.17g %.17g %.17g %.17g %.17g %.17g]\n",
				JointPos[0],
				JointPos[1],
				JointPos[2],
				JointPos[3],
				JointPos[4],
				JointPos[5],
				JointPos[6]);
		int JointN=7;
		int i=1;
		PubJointPos(JointPubs[i],JointNames[i],JointPos[i],JointDurT);
		ros::Duration(JointDurT).sleep();
		
		for(i=0;i<JointN;i++)
		{
			PubJointPos(JointPubs[i],JointNames[i],JointPos[i],JointDurT);
			ros::Duration(JointDurT).sleep();
		}
		
		i=0;
		JointPos[i]=0;
		PubJointPos(JointPubs[i],JointNames[i],JointPos[i]);
		ros::Duration(JointDurT).sleep();
	}
	
	printf("--------------------------------\n");
	printf("机械臂开环控制是一种艺术，我认为已经抓住物体了\n");
	printf("元芳，你怎么看？\n\n\n");
}


int main(int argc, char **argv)
{
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
	//vector<double>JointPos;
		JointPos.push_back(0);
		JointPos.push_back(0);
		JointPos.push_back(0);
		JointPos.push_back(-pi/2);
		JointPos.push_back(0);
		JointPos.push_back(0);
		JointPos.push_back(0);
		JointPos.push_back(0);
		JointPos.push_back(0);
	//vector<double>JointPosDeltas;
		JointPosDeltas.push_back(0.01);
		JointPosDeltas.push_back(0.01);
		JointPosDeltas.push_back(0.01);
		JointPosDeltas.push_back(0.01);
		JointPosDeltas.push_back(0.01);
		JointPosDeltas.push_back(0.01);
		JointPosDeltas.push_back(0.01);
		JointPosDeltas.push_back(0.01);
		JointPosDeltas.push_back(0.01);
	//vector<int>JointUpKeys;
		// JointUpKeys.push_back('A');
		// JointUpKeys.push_back('S');
		// JointUpKeys.push_back('D');
		// JointUpKeys.push_back('F');
		// JointUpKeys.push_back('G');
		// JointUpKeys.push_back('H');
		// JointUpKeys.push_back('J');
		// JointUpKeys.push_back('K');
		// JointUpKeys.push_back('L');
		// WTF!!!!!!大写按键怎么就响应不了
		JointUpKeys.push_back('q');
		JointUpKeys.push_back('w');
		JointUpKeys.push_back('e');
		JointUpKeys.push_back('r');
		JointUpKeys.push_back('t');
		JointUpKeys.push_back('y');
		JointUpKeys.push_back('u');
		JointUpKeys.push_back('i');
		JointUpKeys.push_back('o');
	//vector<int>JointDownKeys;
		JointDownKeys.push_back('a');
		JointDownKeys.push_back('s');
		JointDownKeys.push_back('d');
		JointDownKeys.push_back('f');
		JointDownKeys.push_back('g');
		JointDownKeys.push_back('h');
		JointDownKeys.push_back('j');
		JointDownKeys.push_back('k');
		JointDownKeys.push_back('l');
	//vector<ros::Publisher>JointPubs;
	
	{
		//Useargvtoset
		for(int i=1;i<argc;i++)
		{
			if(i+1<argc&&string(argv[i])=="-Kw")
			{
				Kw=atof(argv[i+1]);
			}
			if(i+1<argc&&string(argv[i])=="-eTheld")
			{
				eTheld=atof(argv[i+1]);
			}
			if(i+1<argc&&string(argv[i])=="-Kv")
			{
				Kv=atof(argv[i+1]);
			}
			if(i+1<argc&&string(argv[i])=="-Kvx")
			{
				Kvx=atof(argv[i+1]);
			}
			if(i+1<argc&&string(argv[i])=="-Kvy")
			{
				Kvy=atof(argv[i+1]);
			}
			if(i+1<argc&&string(argv[i])=="-dT")
			{
				dT=atof(argv[i+1]);
			}
			if(i+1<argc&&string(argv[i])=="-Hg")
			{
				Hg=atof(argv[i+1]);
			}
			if(i+1<argc&&string(argv[i])=="-MaxNavT")
			{
				MaxNavT=atof(argv[i+1]);
			}
			if(i+1<argc&&string(argv[i])=="-isNav")
			{
				isNav=(string(argv[i+1])=="true");
			}
			if(i+1<argc&&string(argv[i])=="-h_max")
			{
				h_max=atof(argv[i+1]);
			}
			if(i+1<argc&&string(argv[i])=="-theta_max")
			{
				theta_max=atof(argv[i+1]);
			}
			if(i+1<argc&&string(argv[i])=="-JointDurT")
			{
				JointDurT=atof(argv[i+1]);
			}
		}
	}
	
	ros::init(argc, argv, "DOFmove_AutoGcpDcp");
	ros::NodeHandle n;
	
	printf("等待darknet发布一个名为/bOBJgcp的std_msgs::Float64MultiArray类型的消息\n");
	printf("\t消息里面依次为三个数Gx Gy Gz\n");
	printf("\t代表待抓取物体中心在当前时刻base_link坐标系下的坐标\n");
	printf("\t然后本程序负责控制机器人到合适位姿并将物体抓起\n");
	
	int JointN=JointNames.size();
	for(int i=0;i<JointN;i++)
	{
		ros::Publisher Pub=n.advertise<trajectory_msgs::JointTrajectory>(JointTopicNames[i], 1);
		JointPubs.push_back(Pub);
	}
	bOBJpos_sub=n.subscribe("/bOBJgcp", 1, GcpCallback);
	pub_base_twist = n.advertise<geometry_msgs::Twist>("/hsrb/opt_command_velocity", 1);
	
	tf::TransformListener Listener;
	pListener = &Listener;
	
	ros::spin();
	return 0;
}
