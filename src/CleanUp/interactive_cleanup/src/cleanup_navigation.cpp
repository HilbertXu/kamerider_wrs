//标准头文件
#include <ros/ros.h>
#include <iostream>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <string>
//navigation中需要使用的位姿信息头文件
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/PoseWithCovariance.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Quaternion.h>
//move_base头文件
#include <move_base_msgs/MoveBaseGoal.h>
#include <move_base_msgs/MoveBaseAction.h>
//actionlib头文件
#include <actionlib/client/simple_action_client.h>
#include <stdlib.h>
#include <cstdlib>
//opencv
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

geometry_msgs::Pose GCP_Pose;
geometry_msgs::Pose DCP_Pose;
std_msgs::String nav_flag;

bool ifNavigate = false;
bool ifGo2GCP   = false;
bool ifGo2DCP   = false;

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
string nav_dir = "/home/kamerider/sim_ws/src/CleanUp/interactive_cleanup/output_file";

//ROS subscriber & publisher
ros::Subscriber DOF_sub;
ros::Publisher nav_pub;

void reset()
{
	ifNavigate    = false;
	ifGo2DCP      = false;
	ifGo2GCP      = false;
	nav_flag.data = "";
}

void printNavPose(geometry_msgs::Pose nav_pose)
{
	cout << "the navigation position is: " << endl;
	float p_x = nav_pose.position.x; cout << p_x << endl;
	float p_y = nav_pose.position.y; cout << p_y << endl;
	float p_z = nav_pose.position.z; cout << p_z << endl;

	cout << "the navigation orientation is: " << endl;
	float o_x = nav_pose.orientation.x; cout << o_x << endl;
	float o_y = nav_pose.orientation.y; cout << o_y << endl;
	float o_z = nav_pose.orientation.z; cout << o_z << endl;
	float o_w = nav_pose.orientation.w; cout << o_w << endl;
}

void DOFCallback(const geometry_msgs::PoseStamped::ConstPtr& msg)
{
	GCP_Pose = msg->pose;
	ifNavigate = true;
	ifGo2GCP   = true;
}

void ObjCallback(const std_msgs::String::ConstPtr& msg)
{
	if(msg->data == "go to DCP")
	{
		ifNavigate = true;
		ifGo2DCP   = true;
	}
}
int main(int argc, char **argv)
{
	ros::init(argc, argv, "cleanup_navigation");
	ros::NodeHandle nh;
	ROS_INFO("Initial ROS Noed");
	DOF_sub = nh.subscribe("/move_base_simple/goal", 1, DOFCallback);
	nav_pub    = nh.advertise<std_msgs::String>("/nav2obj", 1);

	ROS_INFO("Initial Move Base");
	MoveBaseClient mc_("move_base", true);
	move_base_msgs::MoveBaseGoal Goal;

	while(ros::ok())
	{
		if(ifNavigate)
		{
			if(ifGo2GCP)
			{
				ROS_INFO("Navigate to Grasping Candidate Position");
				Goal.target_pose.header.frame_id = "map";
				Goal.target_pose.header.stamp    = ros::Time::now();
				Goal.target_pose.pose            = geometry_msgs::Pose(GCP_Pose);
				while(!mc_.waitForServer(ros::Duration(5.0)))
				{
					ROS_INFO("Waiting For The Server...");
				}
				mc_.sendGoal(Goal);
				mc_.waitForResult(ros::Duration(40.0));

				if(mc_.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
				{
					ROS_INFO("Arrived Goal Position");
					nav_flag.data = "GCP_Arrived";
					nav_pub.publish(nav_flag);
					reset();
				}
			}

			if(ifGo2DCP)
			{
				ROS_INFO("Naviagte to Destination Position");
				Goal.target_pose.header.frame_id = "map";
				Goal.target_pose.header.stamp    = ros::Time::now();
				Goal.target_pose.pose            = geometry_msgs::Pose(DCP_Pose);
				while(!mc_.waitForServer(ros::Duration(5.0)))
				{
					ROS_INFO("Waiting For The Server...");
				}
				mc_.sendGoal(Goal);
				mc_.waitForResult(ros::Duration(40.0));

				if(mc_.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
				{
					ROS_INFO("Arrived Goal Position");
					nav_flag.data = "DCP_Arrived";
					nav_pub.publish(nav_flag);
					reset();
				}
			}
		}
		ros::spinOnce();
	}
}
