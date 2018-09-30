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
ros::Subscriber OPAros_sub;
ros::Publisher nav_pub;

void reset()
{
	ifNavigate    = false;
	ifGo2DCP      = false;
	ifGo2GCP      = false;
	nav_flag.data = "";
}

void readXmlFiles(string &dir)
{
	ROS_INFO("start reading position xml files");
	string GCP_Path = dir + "/GCP_Nav_Pos.xml";
	cv::FileStorage GCP_xml(GCP_Path, cv::FileStorage::READ);
	GCP_Pose.position.x = GCP_xml["position"]["x"];
	GCP_Pose.position.y = GCP_xml["position"]["y"];
	GCP_Pose.position.z = GCP_xml["position"]["z"];

	GCP_Pose.orientation.x = GCP_xml["orientation"]["x"];
	GCP_Pose.orientation.y = GCP_xml["orientation"]["y"];
	GCP_Pose.orientation.z = GCP_xml["orientation"]["z"];
	GCP_Pose.orientation.w = GCP_xml["orientation"]["w"];
	GCP_xml.release();

	string DCP_Path = dir + "/DCP_Nav_Pos.xml";
	cv::FileStorage DCP_xml(DCP_Path, cv::FileStorage::READ);
	DCP_Pose.position.x = DCP_xml["position"]["x"];
	DCP_Pose.position.y = DCP_xml["position"]["y"];
	DCP_Pose.position.z = DCP_xml["position"]["z"];

	DCP_Pose.orientation.x = DCP_xml["orientation"]["x"];
	DCP_Pose.orientation.y = DCP_xml["orientation"]["y"];
	DCP_Pose.orientation.z = DCP_xml["orientation"]["z"];
	DCP_Pose.orientation.w = DCP_xml["orientation"]["w"];
	DCP_xml.release();

	ifNavigate = true;
	ifGo2GCP   = true;
}

void OPArosCallback(const std_msgs::String::ConstPtr& msg)
{
	if(msg->data == "start")
	{
		readXmlFiles(nav_dir);
	}
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
	OPAros_sub = nh.subscribe("/OPAros2nav", 1, OPArosCallback);
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