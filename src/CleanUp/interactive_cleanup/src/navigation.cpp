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
geometry_msgs::Pose GCP_Pose;
geometry_msgs::Pose DCP_Pose;

//简化类型书写为MoveBaseClient
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

//flags
bool ifNavigation                       = false;
bool ifGo2GCP                           = false;
bool ifGo2DCP                           = false;

//received position
//std_msgs::String GC_Position;
//std_msgs::String DC_Position;
std_msgs::String nav_flag;
//set goal position
geometry_msgs::Pose goal_pose;
geometry_msgs::Pose GCP;
geometry_msgs::Pose DCP;

string nav_dir = "/home/kamerider/sim_ws/src/CleanUp/interactive_cleanup/output_file";

//subscriber
//ros::Subscriber GC_Position_sub;
//ros::Subscriber DC_Position_sub;
//publisher
//ros::Publisher nav_pub;

void readXmlFiles(string &dir)
	{
		
		//read GCP
		string fullPath = dir + "/GCP_Nav_pos.xml";
		FileStorage GCP_xml(fullPath, FileStorage::READ);

		GCP_Pose.position.x = GCP_xml["position"]["x"];
		GCP_Pose.position.y = GCP_xml["position"]["y"];
		GCP_Pose.position.z = GCP_xml["position"]["z"];

		GCP_Pose.orientation.x = GCP_xml["orientation"]["x"];
		GCP_Pose.orientation.y = GCP_xml["orientation"]["y"];
		GCP_Pose.orientation.z = GCP_xml["orientation"]["z"];
		GCP_Pose.orientation.w = GCP_xml["orientation"]["w"];
		GCP_xml.release();
	

	
		//read DCP
		fullPath = dir + "/DCP_Nav_pos.xml";
		cv::FileStorage DCP_xml(fullPath, FileStorage::READ);

		DCP_Pose.position.x = DCP_xml["position"]["x"];
		DCP_Pose.position.y = DCP_xml["position"]["y"];
		DCP_Pose.position.z = DCP_xml["position"]["z"];

		DCP_Pose.orientation.x = DCP_xml["orientation"]["x"];
		DCP_Pose.orientation.y = DCP_xml["orientation"]["y"];
		DCP_Pose.orientation.z = DCP_xml["orientation"]["z"];
		DCP_Pose.orientation.w = DCP_xml["orientation"]["w"];
		DCP_xml.release();
		
	}

class navigation
{
public:
	//init position
	void initplace()
	{
		reset();
	}
	void reset()
	{
		ifNavigation                          = false;
		ifGo2DCP                              = false;
		ifGo2GCP                              = false;
		nav_flag.data                         = "";
	}

	void OPArosCallback(const std_msgs::String::ConstPtr& msg)
	{
		if(msg->data == "start")
		{
			//read xml files
			readXmlFiles(nav_dir);
		}
	}
	//init sub, pub and etc 
	int run(int argc, char **argv)
	{
		ros::init(argc, argv, "interactive_cleanup_navigation");
		ros::NodeHandle nh;
		initplace();
		ROS_INFO("Successfully load all waypoints");

		//ros::Subscriber GC_Position_sub = nh.subscribe("/GCP_Predict",1 ,&navigation::GC_PositionCallback,this);
		//ros::Subscriber DC_Position_sub = nh.subscribe("/DCP_Predict",1 ,&navigation::DC_PositionCallback,this);
		ros::Publisher  nav_pub         = nh.advertise<std_msgs::String>("/nav_pub/Arrived_Position",1);
		
		//set up navigation Client
		ROS_INFO("Initial Move Base");
		MoveBaseClient mc_("move_base", true);
		//set a goal pose
		move_base_msgs::MoveBaseGoal Goal;
		while (ros::ok())
		{
			if(ifNavigation)
			{
				if(ifGo2GCP)
				{
					ROS_INFO("Start to Navigation");
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
						navigation::reset();
					}
				}

				if(ifGo2DCP)
				{
					ROS_INFO("Start to Navigation");
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
						navigation::reset();
					}
				}
			}
			ros::spinOnce();
		}
	}
};

int main(int argc, char **argv)
{
	navigation interactive_cleanup_navigation;
	interactive_cleanup_navigation.run(argc, argv);
}








