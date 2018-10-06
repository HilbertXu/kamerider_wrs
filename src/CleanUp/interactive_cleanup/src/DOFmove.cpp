#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <string>

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

using namespace std;
using namespace cv;
const double pi=3.1415926535897932384626;
double baseV=0.1;
double baseW=0.1;

// WTF!!!!!全局变量怎么就不能定义vector
// vector<string>JointTopicNames;
	// JointTopicNames.push_back("/hsrb/arm_trajectory_controller/command");
	// JointTopicNames.push_back("/hsrb/arm_trajectory_controller/command");
	// JointTopicNames.push_back("/hsrb/arm_trajectory_controller/command");
	// JointTopicNames.push_back("/hsrb/gripper_trajectory_controller/command");
	// JointTopicNames.push_back("/hsrb/gripper_trajectory_controller/command");
	// JointTopicNames.push_back("/hsrb/gripper_trajectory_controller/command");
	// JointTopicNames.push_back("/hsrb/gripper_trajectory_controller/command");
	// JointTopicNames.push_back("/hsrb/head_trajectory_controller/command");
	// JointTopicNames.push_back("/hsrb/head_trajectory_controller/command");
// vector<string>JointNames;
	// JointNames.push_back("arm_lift_joint");
	// JointNames.push_back("arm_flex_joint");
	// JointNames.push_back("arm_roll_joint");
	// JointNames.push_back("wrist_flex_joint");
	// JointNames.push_back("wrist_roll_joint");
	// JointNames.push_back("hand_l_proximal_joint");
	// JointNames.push_back("hand_r_proximal_joint");
	// JointNames.push_back("head_pan_joint");
	// JointNames.push_back("head_tilt_joint");
// vector<double>JointPos;
	// JointPos.push_back(0);
	// JointPos.push_back(0);
	// JointPos.push_back(0);
	// JointPos.push_back(-pi/2);
	// JointPos.push_back(0);
	// JointPos.push_back(0);
	// JointPos.push_back(0);
	// JointPos.push_back(0);
	// JointPos.push_back(0);
// vector<double>JointPosDeltas;
	// JointPosDeltas.push_back(0.01);
	// JointPosDeltas.push_back(0.01);
	// JointPosDeltas.push_back(0.01);
	// JointPosDeltas.push_back(0.01);
	// JointPosDeltas.push_back(0.01);
	// JointPosDeltas.push_back(0.01);
	// JointPosDeltas.push_back(0.01);
	// JointPosDeltas.push_back(0.01);
	// JointPosDeltas.push_back(0.01);
// vector<int>JointUpKeys;
	// // JointUpKeys.push_back('A');
	// // JointUpKeys.push_back('S');
	// // JointUpKeys.push_back('D');
	// // JointUpKeys.push_back('F');
	// // JointUpKeys.push_back('G');
	// // JointUpKeys.push_back('H');
	// // JointUpKeys.push_back('J');
	// // JointUpKeys.push_back('K');
	// // JointUpKeys.push_back('L');
	// // WTF!!!!!!大写按键怎么就响应不了
	// JointUpKeys.push_back('q');
	// JointUpKeys.push_back('w');
	// JointUpKeys.push_back('e');
	// JointUpKeys.push_back('r');
	// JointUpKeys.push_back('t');
	// JointUpKeys.push_back('y');
	// JointUpKeys.push_back('u');
	// JointUpKeys.push_back('i');
	// JointUpKeys.push_back('o');
// vector<int>JointDownKeys;
	// JointDownKeys.push_back('a');
	// JointDownKeys.push_back('s');
	// JointDownKeys.push_back('d');
	// JointDownKeys.push_back('f');
	// JointDownKeys.push_back('g');
	// JointDownKeys.push_back('h');
	// JointDownKeys.push_back('j');
	// JointDownKeys.push_back('k');
	// JointDownKeys.push_back('l');
// vector<ros::Publisher>JointPubs;


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


tf::TransformListener*pListener;//监听tf变换的设备指针,具体定义在main函数ros::init之后了
tf::StampedTransform CurrTf;//监听到的tf变换
void PrintTs(vector<string>&JointFrameNames,vector<double>&JointPos)
{
	printf("\nView information\n");
	printf("--------------------------------------------------\n");
	printf("JointPos:\n");
	int JointN=JointFrameNames.size();
	for(int i=0;i<JointN;i++)
	{
		printf("\t%s pos is %.17g\n",&(JointFrameNames[i][0]),JointPos[i]);
	}
	
	printf("T in tftree:\n");
	try
	{
		string DadName="base_link";
		string SonName=JointFrameNames[0];
		
		pListener->lookupTransform(&(DadName[0]), &(SonName[0]),ros::Time(0), CurrTf);
		tf::Vector3 P=CurrTf.getOrigin();
		tf::Matrix3x3 R=CurrTf.getBasis();
		printf("\tT%sT___T%sT___T=[\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t0,0,0,1];\n",
				&(DadName[0]),&(SonName[0]),
				R[0][0],R[0][1],R[0][2],P[0],
				R[1][0],R[1][1],R[1][2],P[1],
				R[2][0],R[2][1],R[2][2],P[2]);
	}
	catch (tf::TransformException &ex) 
	{
		ROS_ERROR("%s",ex.what());
		ros::Duration(1.0).sleep();
	}
	
	try
	{
		string DadName=JointFrameNames[0];
		string SonName=JointFrameNames[1];
		
		pListener->lookupTransform(&(DadName[0]), &(SonName[0]),ros::Time(0), CurrTf);
		tf::Vector3 P=CurrTf.getOrigin();
		tf::Matrix3x3 R=CurrTf.getBasis();
		printf("\tT%sT___T%sT___T=[\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t0,0,0,1];\n",
				&(DadName[0]),&(SonName[0]),
				R[0][0],R[0][1],R[0][2],P[0],
				R[1][0],R[1][1],R[1][2],P[1],
				R[2][0],R[2][1],R[2][2],P[2]);
	}
	catch (tf::TransformException &ex) 
	{
		ROS_ERROR("%s",ex.what());
		ros::Duration(1.0).sleep();
	}
	
	try
	{
		string DadName=JointFrameNames[1];
		string SonName=JointFrameNames[2];
		
		pListener->lookupTransform(&(DadName[0]), &(SonName[0]),ros::Time(0), CurrTf);
		tf::Vector3 P=CurrTf.getOrigin();
		tf::Matrix3x3 R=CurrTf.getBasis();
		printf("\tT%sT___T%sT___T=[\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t0,0,0,1];\n",
				&(DadName[0]),&(SonName[0]),
				R[0][0],R[0][1],R[0][2],P[0],
				R[1][0],R[1][1],R[1][2],P[1],
				R[2][0],R[2][1],R[2][2],P[2]);
	}
	catch (tf::TransformException &ex) 
	{
		ROS_ERROR("%s",ex.what());
		ros::Duration(1.0).sleep();
	}
	
	try
	{
		string DadName=JointFrameNames[2];
		string SonName=JointFrameNames[3];
		
		pListener->lookupTransform(&(DadName[0]), &(SonName[0]),ros::Time(0), CurrTf);
		tf::Vector3 P=CurrTf.getOrigin();
		tf::Matrix3x3 R=CurrTf.getBasis();
		printf("\tT%sT___T%sT___T=[\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t0,0,0,1];\n",
				&(DadName[0]),&(SonName[0]),
				R[0][0],R[0][1],R[0][2],P[0],
				R[1][0],R[1][1],R[1][2],P[1],
				R[2][0],R[2][1],R[2][2],P[2]);
	}
	catch (tf::TransformException &ex) 
	{
		ROS_ERROR("%s",ex.what());
		ros::Duration(1.0).sleep();
	}
	
	try
	{
		string DadName=JointFrameNames[3];
		string SonName=JointFrameNames[4];
		
		pListener->lookupTransform(&(DadName[0]), &(SonName[0]),ros::Time(0), CurrTf);
		tf::Vector3 P=CurrTf.getOrigin();
		tf::Matrix3x3 R=CurrTf.getBasis();
		printf("\tT%sT___T%sT___T=[\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t0,0,0,1];\n",
				&(DadName[0]),&(SonName[0]),
				R[0][0],R[0][1],R[0][2],P[0],
				R[1][0],R[1][1],R[1][2],P[1],
				R[2][0],R[2][1],R[2][2],P[2]);
	}
	catch (tf::TransformException &ex) 
	{
		ROS_ERROR("%s",ex.what());
		ros::Duration(1.0).sleep();
	}
	
	try
	{
		string DadName=JointFrameNames[4];
		string SonName=JointFrameNames[5];
		
		pListener->lookupTransform(&(DadName[0]), &(SonName[0]),ros::Time(0), CurrTf);
		tf::Vector3 P=CurrTf.getOrigin();
		tf::Matrix3x3 R=CurrTf.getBasis();
		printf("\tT%sT___T%sT___T=[\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t0,0,0,1];\n",
				&(DadName[0]),&(SonName[0]),
				R[0][0],R[0][1],R[0][2],P[0],
				R[1][0],R[1][1],R[1][2],P[1],
				R[2][0],R[2][1],R[2][2],P[2]);
	}
	catch (tf::TransformException &ex) 
	{
		ROS_ERROR("%s",ex.what());
		ros::Duration(1.0).sleep();
	}
	
	try
	{
		string DadName=JointFrameNames[4];
		string SonName=JointFrameNames[6];
		
		pListener->lookupTransform(&(DadName[0]), &(SonName[0]),ros::Time(0), CurrTf);
		tf::Vector3 P=CurrTf.getOrigin();
		tf::Matrix3x3 R=CurrTf.getBasis();
		printf("\tT%sT___T%sT___T=[\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t0,0,0,1];\n",
				&(DadName[0]),&(SonName[0]),
				R[0][0],R[0][1],R[0][2],P[0],
				R[1][0],R[1][1],R[1][2],P[1],
				R[2][0],R[2][1],R[2][2],P[2]);
	}
	catch (tf::TransformException &ex) 
	{
		ROS_ERROR("%s",ex.what());
		ros::Duration(1.0).sleep();
	}
	
	try
	{
		string DadName="base_link";
		string SonName=JointFrameNames[7];
		
		pListener->lookupTransform(&(DadName[0]), &(SonName[0]),ros::Time(0), CurrTf);
		tf::Vector3 P=CurrTf.getOrigin();
		tf::Matrix3x3 R=CurrTf.getBasis();
		printf("\tT%sT___T%sT___T=[\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t0,0,0,1];\n",
				&(DadName[0]),&(SonName[0]),
				R[0][0],R[0][1],R[0][2],P[0],
				R[1][0],R[1][1],R[1][2],P[1],
				R[2][0],R[2][1],R[2][2],P[2]);
	}
	catch (tf::TransformException &ex) 
	{
		ROS_ERROR("%s",ex.what());
		ros::Duration(1.0).sleep();
	}
	
	try
	{
		string DadName=JointFrameNames[7];
		string SonName=JointFrameNames[8];
		
		pListener->lookupTransform(&(DadName[0]), &(SonName[0]),ros::Time(0), CurrTf);
		tf::Vector3 P=CurrTf.getOrigin();
		tf::Matrix3x3 R=CurrTf.getBasis();
		printf("\tT%sT___T%sT___T=[\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t0,0,0,1];\n",
				&(DadName[0]),&(SonName[0]),
				R[0][0],R[0][1],R[0][2],P[0],
				R[1][0],R[1][1],R[1][2],P[1],
				R[2][0],R[2][1],R[2][2],P[2]);
	}
	catch (tf::TransformException &ex) 
	{
		ROS_ERROR("%s",ex.what());
		ros::Duration(1.0).sleep();
	}
	
	
	printf("T arms in tftree:\n");
	printf("  0 is base_link\n" \
			"  1 is %s\n" \
			"  2 is %s\n" \
			"  3 is %s\n" \
			"  4 is %s\n" \
			"  5 is %s\n" \
			"  L6 is %s\n" \
			"  R6 is %s\n",
			&(JointFrameNames[0][0]),
			&(JointFrameNames[1][0]),
			&(JointFrameNames[2][0]),
			&(JointFrameNames[3][0]),
			&(JointFrameNames[4][0]),
			&(JointFrameNames[5][0]),
			&(JointFrameNames[6][0]));
	try
	{
		string DadName="base_link";
		string SonName=JointFrameNames[0];
		
		pListener->lookupTransform(&(DadName[0]), &(SonName[0]),ros::Time(0), CurrTf);
		tf::Vector3 P=CurrTf.getOrigin();
		tf::Matrix3x3 R=CurrTf.getBasis();
		printf("\tT_01=[\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t0,0,0,1];\n",
				R[0][0],R[0][1],R[0][2],P[0],
				R[1][0],R[1][1],R[1][2],P[1],
				R[2][0],R[2][1],R[2][2],P[2]);
	}
	catch (tf::TransformException &ex) 
	{
		ROS_ERROR("%s",ex.what());
		ros::Duration(1.0).sleep();
	}
	
	try
	{
		string DadName=JointFrameNames[0];
		string SonName=JointFrameNames[1];
		
		pListener->lookupTransform(&(DadName[0]), &(SonName[0]),ros::Time(0), CurrTf);
		tf::Vector3 P=CurrTf.getOrigin();
		tf::Matrix3x3 R=CurrTf.getBasis();
		printf("\tT_12=[\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t0,0,0,1];\n",
				R[0][0],R[0][1],R[0][2],P[0],
				R[1][0],R[1][1],R[1][2],P[1],
				R[2][0],R[2][1],R[2][2],P[2]);
	}
	catch (tf::TransformException &ex) 
	{
		ROS_ERROR("%s",ex.what());
		ros::Duration(1.0).sleep();
	}
	
	try
	{
		string DadName=JointFrameNames[1];
		string SonName=JointFrameNames[2];
		
		pListener->lookupTransform(&(DadName[0]), &(SonName[0]),ros::Time(0), CurrTf);
		tf::Vector3 P=CurrTf.getOrigin();
		tf::Matrix3x3 R=CurrTf.getBasis();
		printf("\tT_23=[\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t0,0,0,1];\n",
				R[0][0],R[0][1],R[0][2],P[0],
				R[1][0],R[1][1],R[1][2],P[1],
				R[2][0],R[2][1],R[2][2],P[2]);
	}
	catch (tf::TransformException &ex) 
	{
		ROS_ERROR("%s",ex.what());
		ros::Duration(1.0).sleep();
	}
	
	try
	{
		string DadName=JointFrameNames[2];
		string SonName=JointFrameNames[3];
		
		pListener->lookupTransform(&(DadName[0]), &(SonName[0]),ros::Time(0), CurrTf);
		tf::Vector3 P=CurrTf.getOrigin();
		tf::Matrix3x3 R=CurrTf.getBasis();
		printf("\tT_34=[\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t0,0,0,1];\n",
				R[0][0],R[0][1],R[0][2],P[0],
				R[1][0],R[1][1],R[1][2],P[1],
				R[2][0],R[2][1],R[2][2],P[2]);
	}
	catch (tf::TransformException &ex) 
	{
		ROS_ERROR("%s",ex.what());
		ros::Duration(1.0).sleep();
	}
	
	try
	{
		string DadName=JointFrameNames[3];
		string SonName=JointFrameNames[4];
		
		pListener->lookupTransform(&(DadName[0]), &(SonName[0]),ros::Time(0), CurrTf);
		tf::Vector3 P=CurrTf.getOrigin();
		tf::Matrix3x3 R=CurrTf.getBasis();
		printf("\tT_45=[\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t0,0,0,1];\n",
				R[0][0],R[0][1],R[0][2],P[0],
				R[1][0],R[1][1],R[1][2],P[1],
				R[2][0],R[2][1],R[2][2],P[2]);
	}
	catch (tf::TransformException &ex) 
	{
		ROS_ERROR("%s",ex.what());
		ros::Duration(1.0).sleep();
	}
	
	try
	{
		string DadName=JointFrameNames[4];
		string SonName=JointFrameNames[5];
		
		pListener->lookupTransform(&(DadName[0]), &(SonName[0]),ros::Time(0), CurrTf);
		tf::Vector3 P=CurrTf.getOrigin();
		tf::Matrix3x3 R=CurrTf.getBasis();
		printf("\tT_5L6=[\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t0,0,0,1];\n",
				R[0][0],R[0][1],R[0][2],P[0],
				R[1][0],R[1][1],R[1][2],P[1],
				R[2][0],R[2][1],R[2][2],P[2]);
	}
	catch (tf::TransformException &ex) 
	{
		ROS_ERROR("%s",ex.what());
		ros::Duration(1.0).sleep();
	}
	
	try
	{
		string DadName=JointFrameNames[4];
		string SonName=JointFrameNames[6];
		
		pListener->lookupTransform(&(DadName[0]), &(SonName[0]),ros::Time(0), CurrTf);
		tf::Vector3 P=CurrTf.getOrigin();
		tf::Matrix3x3 R=CurrTf.getBasis();
		printf("\tT_5R6=[\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t%.17g %.17g %.17g %.17g\n" \
				"\t\t0,0,0,1];\n",
				R[0][0],R[0][1],R[0][2],P[0],
				R[1][0],R[1][1],R[1][2],P[1],
				R[2][0],R[2][1],R[2][2],P[2]);
	}
	catch (tf::TransformException &ex) 
	{
		ROS_ERROR("%s",ex.what());
		ros::Duration(1.0).sleep();
	}
	
	printf("arm pos Q=[%.17g %.17g %.17g %.17g %.17g %.17g %.17g];\n",
			JointPos[0],
			JointPos[1],
			JointPos[2],
			JointPos[3],
			JointPos[4],
			JointPos[5],
			JointPos[6]);
	printf("--------------------------------------------------\n");
}

int main(int argc, char **argv)
{
	vector<string>JointTopicNames;
		JointTopicNames.push_back("/hsrb/arm_trajectory_controller/command");
		JointTopicNames.push_back("/hsrb/arm_trajectory_controller/command");
		JointTopicNames.push_back("/hsrb/arm_trajectory_controller/command");
		JointTopicNames.push_back("/hsrb/gripper_trajectory_controller/command");
		JointTopicNames.push_back("/hsrb/gripper_trajectory_controller/command");
		JointTopicNames.push_back("/hsrb/gripper_trajectory_controller/command");
		JointTopicNames.push_back("/hsrb/gripper_trajectory_controller/command");
		JointTopicNames.push_back("/hsrb/head_trajectory_controller/command");
		JointTopicNames.push_back("/hsrb/head_trajectory_controller/command");
	vector<string>JointNames;
		JointNames.push_back("arm_lift_joint");
		JointNames.push_back("arm_flex_joint");
		JointNames.push_back("arm_roll_joint");
		JointNames.push_back("wrist_flex_joint");
		JointNames.push_back("wrist_roll_joint");
		JointNames.push_back("hand_l_proximal_joint");
		JointNames.push_back("hand_r_proximal_joint");
		JointNames.push_back("head_pan_joint");
		JointNames.push_back("head_tilt_joint");
	vector<string>JointFrameNames;
		JointFrameNames.push_back("arm_lift_link");
		JointFrameNames.push_back("arm_flex_link");
		JointFrameNames.push_back("arm_roll_link");
		JointFrameNames.push_back("wrist_flex_link");
		JointFrameNames.push_back("wrist_roll_link");
		JointFrameNames.push_back("hand_l_proximal_link");
		JointFrameNames.push_back("hand_r_proximal_link");
		JointFrameNames.push_back("head_pan_link");
		JointFrameNames.push_back("head_tilt_link");
	vector<double>JointPos;
		JointPos.push_back(0);
		JointPos.push_back(0);
		JointPos.push_back(0);
		JointPos.push_back(-pi/2);
		JointPos.push_back(0);
		JointPos.push_back(0);
		JointPos.push_back(0);
		JointPos.push_back(0);
		JointPos.push_back(0);
	vector<double>JointPosDeltas;
		JointPosDeltas.push_back(0.01);
		JointPosDeltas.push_back(0.01);
		JointPosDeltas.push_back(0.01);
		JointPosDeltas.push_back(0.01);
		JointPosDeltas.push_back(0.01);
		JointPosDeltas.push_back(0.01);
		JointPosDeltas.push_back(0.01);
		JointPosDeltas.push_back(0.01);
		JointPosDeltas.push_back(0.01);
	vector<int>JointUpKeys;
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
	vector<int>JointDownKeys;
		JointDownKeys.push_back('a');
		JointDownKeys.push_back('s');
		JointDownKeys.push_back('d');
		JointDownKeys.push_back('f');
		JointDownKeys.push_back('g');
		JointDownKeys.push_back('h');
		JointDownKeys.push_back('j');
		JointDownKeys.push_back('k');
		JointDownKeys.push_back('l');
	vector<ros::Publisher>JointPubs;
	
	int JointN=JointNames.size();
	printf("按键操纵说明：\n");
	for(int i=0;i<JointN;i++)
	{
		printf("%c/%c 控制 Up/Down %s\n",JointUpKeys[i],JointDownKeys[i],&(JointNames[i][0]));
	}
	printf("ESC 控制 退出程序\n");
	printf("0 控制 各关节复位\n");
	printf("v 控制 打印各关节信息\n");
	printf("n 控制 开爪\n");
	printf("m 控制 合爪\n");
	printf("b 控制 放平手臂并保证手爪垂直于地面向下\n");
	printf("8 控制 前进\n");
	printf("2 控制 后退\n");
	printf("4 控制 左移\n");
	printf("6 控制 右移\n");
	printf("7 控制 左转\n");
	printf("9 控制 右转\n");
	printf("5 控制 急停\n");
	printf("c 控制 加速\n");
	printf("x 控制 减速\n");
	printf("\n");
	
	ros::init(argc, argv, "DOFmove");
	ros::NodeHandle n;
	for(int i=0;i<JointN;i++)
	{
		ros::Publisher Pub=n.advertise<trajectory_msgs::JointTrajectory>(JointTopicNames[i], 1);
		JointPubs.push_back(Pub);
	}
	ros::Publisher pub_base_twist = n.advertise<geometry_msgs::Twist>("/hsrb/opt_command_velocity", 1);
	tf::TransformListener Listener;
	pListener = &Listener;
	
	Mat Img=Mat::zeros(256,256,CV_8UC3);
	{
		memset(Img.data,0x66,Img.rows*Img.cols*Img.channels());
		double CenterX[]={28+40,28+40,28+40,28+100,28+100,28+100,28+160,28+160,28+160};
		double CenterY[]={40,100,160,40,100,160,40,100,160};
		int PtN=9;
		double r=20;
		unsigned char *pData=Img.data;
		for(int row=0;row<Img.rows;row++)
		{
			for(int col=0;col<Img.cols;col++)
			{
				for(int k=0;k<PtN;k++)
				{
					if((row-CenterY[k])*(row-CenterY[k])+(col-CenterX[k])*(col-CenterX[k])<=r*r)
					{
						pData[row*Img.cols*3+col*3+0]=0xCC;
						pData[row*Img.cols*3+col*3+1]=0xCC;
						pData[row*Img.cols*3+col*3+2]=0xCC;
						break;
					}
				}

			}
		}
		putText(Img,"DOF Keyboard",Point(19,255-27),FONT_HERSHEY_SIMPLEX,1,Scalar(0xCC,0xCC,0xCC));
	}

	int Key=0;
	while(true)
	{
		imshow("DOFmove_Key",Img);
		Key=waitKey(10);
		if(Key==27)break;
		
		for(int i=0;i<JointN;i++)
		{
			if(Key==JointUpKeys[i])
			{
				JointPos[i]=JointPos[i]+JointPosDeltas[i];
				PubJointPos(JointPubs[i],JointNames[i],JointPos[i]);
				//printf("Key=%d. Then %s Pos is %.17g\n",Key,&(JointNames[i][0]),JointPos[i]);
				break;
			}
			if(Key==JointDownKeys[i])
			{
				JointPos[i]=JointPos[i]-JointPosDeltas[i];
				PubJointPos(JointPubs[i],JointNames[i],JointPos[i]);
				//printf("Key=%d. Then %s Pos is %.17g\n",Key,&(JointNames[i][0]),JointPos[i]);
				break;
			}
		}
		
		switch(Key)
		{
			case '0':
			{//复位
				for(int i=0;i<JointN;i++)
				{
					JointPos[i]=0;
				}
				JointPos[3]=-pi/2;
				//printf("Key=%d. Reset Joints\n",Key);
				for(int i=0;i<JointN;i++)
				{
					PubJointPos(JointPubs[i],JointNames[i],JointPos[i]);
					//printf("\t%s Pos is %.17g\n",&(JointNames[i][0]),JointPos[i]);
				}
				break;
			}
			
			case 'v':
			{//打印信息
				PrintTs(JointFrameNames,JointPos);
				break;
			}
			
			case 'n':
			{//开爪
				int i;
				i=5;JointPos[i]=JointPos[i]+JointPosDeltas[i];
				PubJointPos(JointPubs[i],JointNames[i],JointPos[i]);
				i=6;JointPos[i]=JointPos[i]-JointPosDeltas[i];
				PubJointPos(JointPubs[i],JointNames[i],JointPos[i]);
				break;
			}
			
			case 'm':
			{//合爪
				int i;
				i=5;JointPos[i]=JointPos[i]-JointPosDeltas[i];
				PubJointPos(JointPubs[i],JointNames[i],JointPos[i]);
				i=6;JointPos[i]=JointPos[i]+JointPosDeltas[i];
				PubJointPos(JointPubs[i],JointNames[i],JointPos[i]);
				break;
			}
			
			case 'b':
			{//放平手臂并保证手爪垂直于地面向下
				if(JointPos[1]==0)
				{
					int i;
					i=1;JointPos[i]=-pi/2;
					PubJointPos(JointPubs[i],JointNames[i],JointPos[i]);
					i=3;JointPos[i]=-pi/2;
					PubJointPos(JointPubs[i],JointNames[i],JointPos[i]);
				}
				else
				{
					int i;
					i=1;JointPos[i]=JointPos[i]-JointPosDeltas[i];
					PubJointPos(JointPubs[i],JointNames[i],JointPos[i]);
					i=3;JointPos[i]=JointPos[i]+JointPosDeltas[i];
					PubJointPos(JointPubs[i],JointNames[i],JointPos[i]);
				}
				break;
			}
			
			case '8':
			{
				geometry_msgs::Twist twist;
				twist.linear.x = baseV;
				twist.linear.y = 0.0;
				twist.linear.z = 0.0;
				twist.angular.x = 0.0;
				twist.angular.y = 0.0;
				twist.angular.z = 0.0;
				pub_base_twist.publish(twist);
				break;
			}
			
			case '2':
			{
				geometry_msgs::Twist twist;
				twist.linear.x = -baseV;
				twist.linear.y = 0.0;
				twist.linear.z = 0.0;
				twist.angular.x = 0.0;
				twist.angular.y = 0.0;
				twist.angular.z = 0.0;
				pub_base_twist.publish(twist);
				break;
			}
			
			case '4':
			{
				geometry_msgs::Twist twist;
				twist.linear.x = 0.0;
				twist.linear.y = baseV;
				twist.linear.z = 0.0;
				twist.angular.x = 0.0;
				twist.angular.y = 0.0;
				twist.angular.z = 0.0;
				pub_base_twist.publish(twist);
				break;
			}
			
			case '6':
			{
				geometry_msgs::Twist twist;
				twist.linear.x = 0.0;
				twist.linear.y = -baseV;
				twist.linear.z = 0.0;
				twist.angular.x = 0.0;
				twist.angular.y = 0.0;
				twist.angular.z = 0.0;
				pub_base_twist.publish(twist);
				break;
			}
			
			case '7':
			{
				geometry_msgs::Twist twist;
				twist.linear.x = 0.0;
				twist.linear.y = 0.0;
				twist.linear.z = 0.0;
				twist.angular.x = 0.0;
				twist.angular.y = 0.0;
				twist.angular.z = baseW;
				pub_base_twist.publish(twist);
				break;
			}
			
			case '9':
			{
				geometry_msgs::Twist twist;
				twist.linear.x = 0.0;
				twist.linear.y = 0.0;
				twist.linear.z = 0.0;
				twist.angular.x = 0.0;
				twist.angular.y = 0.0;
				twist.angular.z = -baseW;
				pub_base_twist.publish(twist);
				break;
			}
			
			case '5':
			{
				geometry_msgs::Twist twist;
				twist.linear.x = 0.0;
				twist.linear.y = 0.0;
				twist.linear.z = 0.0;
				twist.angular.x = 0.0;
				twist.angular.y = 0.0;
				twist.angular.z = 0.0;
				pub_base_twist.publish(twist);
				break;
			}
			
			case 'c':
			{
				baseV=baseV*1.1;
				baseW=baseW*1.1;
				printf("baseV=%.17g,baseW=%.17g\n",baseV,baseW);
				break;
			}
			
			case 'x':
			{
				baseV=baseV/1.1;
				baseW=baseW/1.1;
				printf("baseV=%.17g,baseW=%.17g\n",baseV,baseW);
				break;
			}
		}
	}
	return 0;
}
