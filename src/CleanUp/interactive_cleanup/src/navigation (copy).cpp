//标准头文件
#include <ros/ros.h>
#include <iostream>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>
#include <string.h>
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

using namespace std;

//DestinationCandidatePosition
geometry_msgs::Pose wide_trashbox_c01;
geometry_msgs::Pose wide_trashbox_c02;
geometry_msgs::Pose wide_trashbox_c03;
geometry_msgs::Pose lowtable_A;
geometry_msgs::Pose wagon_c02_1;
geometry_msgs::Pose wagon_c02_2;
geometry_msgs::Pose sidetable_A_1;
geometry_msgs::Pose sidetable_A_2;

//GraspingCandidatesPosition
geometry_msgs::Pose GraspingCandidatesPosition01;
geometry_msgs::Pose GraspingCandidatesPosition02;
geometry_msgs::Pose GraspingCandidatesPosition03;
geometry_msgs::Pose GraspingCandidatesPosition04;
geometry_msgs::Pose GraspingCandidatesPosition05;
geometry_msgs::Pose GraspingCandidatesPosition06;
geometry_msgs::Pose GraspingCandidatesPosition07;
geometry_msgs::Pose GraspingCandidatesPosition08;
geometry_msgs::Pose GraspingCandidatesPosition09;
geometry_msgs::Pose GraspingCandidatesPosition10;
geometry_msgs::Pose GraspingCandidatesPosition11;
geometry_msgs::Pose GraspingCandidatesPosition12;
geometry_msgs::Pose GraspingCandidatesPosition13;
geometry_msgs::Pose GraspingCandidatesPosition14;
geometry_msgs::Pose GraspingCandidatesPosition15;
geometry_msgs::Pose GraspingCandidatesPosition16;
geometry_msgs::Pose GraspingCandidatesPosition17;
geometry_msgs::Pose GraspingCandidatesPosition18;

//简化类型书写为MoveBaseClient
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

//flags
bool ifNavigation                            = false;
bool ifArrived_GraspingCandidatePosition     = false;
bool ifArrived_DestinationCandidatePosition  = false;

//received position
//std_msgs::String GC_Position;
//std_msgs::String DC_Position;
std_msgs::String nav_flag;
//set goal position
geometry_msgs::Pose goal_pose;


//subscriber
//ros::Subscriber GC_Position_sub;
//ros::Subscriber DC_Position_sub;
//publisher
//ros::Publisher nav_pub;

class navigation
{
public:
	//init position
	void initplace()
	{
		//DestinationCandidatePosition
		wide_trashbox_c01.position.x                  = -0.901162;
		wide_trashbox_c01.position.y                  = 1.9122;
		wide_trashbox_c01.position.z                  = 0;
		wide_trashbox_c01.orientation.x               = 0;
		wide_trashbox_c01.orientation.y               = 0;
		wide_trashbox_c01.orientation.z               = -0.999999;
		wide_trashbox_c01.orientation.w               = -0.999999;

		wide_trashbox_c02.position.x                  = -1.06872;
		wide_trashbox_c02.position.y                  = 0.0934445;
		wide_trashbox_c02.position.z                  = 0;
		wide_trashbox_c02.orientation.x               = 0;
		wide_trashbox_c02.orientation.y               = 0;
		wide_trashbox_c02.orientation.z               = 0.999897;
		wide_trashbox_c02.orientation.w               = 0.014341;

		wide_trashbox_c03.position.x                  = -1.15183;
		wide_trashbox_c03.position.y                  = -1.26388;
		wide_trashbox_c03.position.z                  = 0;
		wide_trashbox_c03.orientation.x               = 0;
		wide_trashbox_c03.orientation.y               = 0;
		wide_trashbox_c03.orientation.z               = 0.998595;
		wide_trashbox_c03.orientation.w               = 0.0529822;

		lowtable_A.position.x                         = -0.357875;
		lowtable_A.position.y                         = 3.27859;
		lowtable_A.position.z                         = 0;
		lowtable_A.orientation.x                      = 0;
		lowtable_A.orientation.y                      = 0;
		lowtable_A.orientation.z                      = -0.00168785;
		lowtable_A.orientation.w                      = 0.999999;

		wagon_c02_1.position.x                        = 2.32436;
		wagon_c02_1.position.y                        = 2.34381;
		wagon_c02_1.position.z                        = 0;
		wagon_c02_1.orientation.x                     = 0;
		wagon_c02_1.orientation.y                     = 0;
		wagon_c02_1.orientation.z                     = 0.696856;
		wagon_c02_1.orientation.w                     = 0.717212;

		wagon_c02_2.position.x                        = 4.06817;
		wagon_c02_2.position.y                        = 3.82612;
		wagon_c02_2.position.z                        = 0;
		wagon_c02_2.orientation.x                     = 0;
		wagon_c02_2.orientation.y                     = 0;
		wagon_c02_2.orientation.z                     = 0.705957;
		wagon_c02_2.orientation.w                     = 0.708255;

		sidetable_A_1.position.x                      = 1.34845;
		sidetable_A_1.position.y                      = -1.2431;
		sidetable_A_1.position.z                      = 0;
		sidetable_A_1.orientation.x                   = 0;
		sidetable_A_1.orientation.y                   = 0;
		sidetable_A_1.orientation.z                   = -0.675073;
		sidetable_A_1.orientation.w                   = 0.737751;

		sidetable_A_2.position.x                      = 4.10374;
		sidetable_A_2.position.y                      = -0.977194;
		sidetable_A_2.position.z                      = 0;
		sidetable_A_2.orientation.x                   = 0;
		sidetable_A_2.orientation.y                   = 0;
		sidetable_A_2.orientation.z                   = -0.00334431;
		sidetable_A_2.orientation.w                   = 0.999994;


		//GraspingCandidatesPosition
		GraspingCandidatesPosition01.position.x        = 1.99639;
		GraspingCandidatesPosition01.position.y        = 4.13559;
		GraspingCandidatesPosition01.position.z        = 0;
		GraspingCandidatesPosition01.orientation.x     = 0;
		GraspingCandidatesPosition01.orientation.y     = 0;
		GraspingCandidatesPosition01.orientation.z     = -0.701941;
		GraspingCandidatesPosition01.orientation.w     = 0.712235;

		GraspingCandidatesPosition02.position.x        = 1.99639;
		GraspingCandidatesPosition02.position.y        = 4.13559;
		GraspingCandidatesPosition02.position.z        = 0;
		GraspingCandidatesPosition02.orientation.x     = 0;
		GraspingCandidatesPosition02.orientation.y     = 0;
		GraspingCandidatesPosition02.orientation.z     = -0.701941;
		GraspingCandidatesPosition02.orientation.w     = 0.712235;

		GraspingCandidatesPosition03.position.x        = 3.93613;
		GraspingCandidatesPosition03.position.y        = 5.43205;
		GraspingCandidatesPosition03.position.z        = 0;
		GraspingCandidatesPosition03.orientation.x     = 0;
		GraspingCandidatesPosition03.orientation.y     = 0;
		GraspingCandidatesPosition03.orientation.z     = -0.812144;
		GraspingCandidatesPosition03.orientation.w     = 0.583457;

		GraspingCandidatesPosition04.position.x        = 1.37426;
		GraspingCandidatesPosition04.position.y        = 4.34523;
		GraspingCandidatesPosition04.position.z        = 0;
		GraspingCandidatesPosition04.orientation.x     = 0;
		GraspingCandidatesPosition04.orientation.y     = 0;
		GraspingCandidatesPosition04.orientation.z     = -0.976022;
		GraspingCandidatesPosition04.orientation.w     = 0.217673;

		GraspingCandidatesPosition05.position.x        = -0.323212;
		GraspingCandidatesPosition05.position.y        = 2.67396;
		GraspingCandidatesPosition05.position.z        = 0;
		GraspingCandidatesPosition05.orientation.x     = 0;
		GraspingCandidatesPosition05.orientation.y     = 0;
		GraspingCandidatesPosition05.orientation.z     = 0.999567;
		GraspingCandidatesPosition05.orientation.w     = 0.0294403;

		GraspingCandidatesPosition06.position.x        = -0.105577;
		GraspingCandidatesPosition06.position.y        = 2.64013;
		GraspingCandidatesPosition06.position.z        = 0;
		GraspingCandidatesPosition06.orientation.x     = 0;
		GraspingCandidatesPosition06.orientation.y     = 0;
		GraspingCandidatesPosition06.orientation.z     = 0.437581;
		GraspingCandidatesPosition06.orientation.w     = 0.899179;

		GraspingCandidatesPosition07.position.x        = 0.998312;
		GraspingCandidatesPosition07.position.y        = -1.0911;
		GraspingCandidatesPosition07.position.z        = 0;
		GraspingCandidatesPosition07.orientation.x     = 0;
		GraspingCandidatesPosition07.orientation.y     = 0;
		GraspingCandidatesPosition07.orientation.z     = -0.71058;
		GraspingCandidatesPosition07.orientation.w     = 0.703617;

		GraspingCandidatesPosition08.position.x        = 1.58827;
		GraspingCandidatesPosition08.position.y        = -1.23085;
		GraspingCandidatesPosition08.position.z        = 0;
		GraspingCandidatesPosition08.orientation.x     = 0;
		GraspingCandidatesPosition08.orientation.y     = 0;
		GraspingCandidatesPosition08.orientation.z     = -0.694874;
		GraspingCandidatesPosition08.orientation.w     = 0.719131;

		GraspingCandidatesPosition09.position.x        = 2.26516;
		GraspingCandidatesPosition09.position.y        = -1.14646;
		GraspingCandidatesPosition09.position.z        = 0;
		GraspingCandidatesPosition09.orientation.x     = 0;
		GraspingCandidatesPosition09.orientation.y     = 0;
		GraspingCandidatesPosition09.orientation.z     = -0.678557;
		GraspingCandidatesPosition09.orientation.w     = 0.734548;

		GraspingCandidatesPosition10.position.x        = -0.00737533;
		GraspingCandidatesPosition10.position.y        = -1.00818;
		GraspingCandidatesPosition10.position.z        = 0;
		GraspingCandidatesPosition10.orientation.x     = 0;
		GraspingCandidatesPosition10.orientation.y     = 0;
		GraspingCandidatesPosition10.orientation.z     = -0.844642;
		GraspingCandidatesPosition10.orientation.w     = 0.535332;

		GraspingCandidatesPosition11.position.x        = 0.180915;
		GraspingCandidatesPosition11.position.y        = 2.06416;
		GraspingCandidatesPosition11.position.z        = 0;
		GraspingCandidatesPosition11.orientation.x     = 0;
		GraspingCandidatesPosition11.orientation.y     = 0;
		GraspingCandidatesPosition11.orientation.z     = 0.524546;
		GraspingCandidatesPosition11.orientation.w     = 0.851382;

		GraspingCandidatesPosition12.position.x        = 3.31411;
		GraspingCandidatesPosition12.position.y        = 2.33665;
		GraspingCandidatesPosition12.position.z        = 0;
		GraspingCandidatesPosition12.orientation.x     = 0;
		GraspingCandidatesPosition12.orientation.y     = 0;
		GraspingCandidatesPosition12.orientation.z     = -0.0390031;
		GraspingCandidatesPosition12.orientation.w     = 0.999239;

		GraspingCandidatesPosition13.position.x        = 3.05935;
		GraspingCandidatesPosition13.position.y        = -0.412672;
		GraspingCandidatesPosition13.position.z        = 0;
		GraspingCandidatesPosition13.orientation.x     = 0;
		GraspingCandidatesPosition13.orientation.y     = 0;
		GraspingCandidatesPosition13.orientation.z     = -0.740275;
		GraspingCandidatesPosition13.orientation.w     = 0.672304;

		GraspingCandidatesPosition14.position.x        = -0.734811;
		GraspingCandidatesPosition14.position.y        = 0.784799;
		GraspingCandidatesPosition14.position.z        = 0;
		GraspingCandidatesPosition14.orientation.x     = 0;
		GraspingCandidatesPosition14.orientation.y     = 0;
		GraspingCandidatesPosition14.orientation.z     = 0.991783;
		GraspingCandidatesPosition14.orientation.w     = 0.127933;

		GraspingCandidatesPosition15.position.x        = 1.2971;
		GraspingCandidatesPosition15.position.y        = 0.776394;
		GraspingCandidatesPosition15.position.z        = 0;
		GraspingCandidatesPosition15.orientation.x     = 0;
		GraspingCandidatesPosition15.orientation.y     = 0;
		GraspingCandidatesPosition15.orientation.z     = 0.0115142;
		GraspingCandidatesPosition15.orientation.w     = 0.999934;

		GraspingCandidatesPosition16.position.x        = 3.3855;
		GraspingCandidatesPosition16.position.y        = 0.528056;
		GraspingCandidatesPosition16.position.z        = 0;
		GraspingCandidatesPosition16.orientation.x     = 0;
		GraspingCandidatesPosition16.orientation.y     = 0;
		GraspingCandidatesPosition16.orientation.z     = 0.0699747;
		GraspingCandidatesPosition16.orientation.w     = 0.997549;

		GraspingCandidatesPosition17.position.x        = 4.00584;
		GraspingCandidatesPosition17.position.y        = -0.345885;
		GraspingCandidatesPosition17.position.z        = 0;
		GraspingCandidatesPosition17.orientation.x     = 0;
		GraspingCandidatesPosition17.orientation.y     = 0;
		GraspingCandidatesPosition17.orientation.z     = -0.0486058;
		GraspingCandidatesPosition17.orientation.w     = 0.998818;

		GraspingCandidatesPosition18.position.x        = 4.1002;
		GraspingCandidatesPosition18.position.y        = -1.49241;
		GraspingCandidatesPosition18.position.z        = 0;
		GraspingCandidatesPosition18.orientation.x     = 0;
		GraspingCandidatesPosition18.orientation.y     = 0;
		GraspingCandidatesPosition18.orientation.z     = -0.0168006;
		GraspingCandidatesPosition18.orientation.w     = 0.999859;

		reset();
	}
	void reset()
	{
		ifNavigation                               = false;
		ifArrived_DestinationCandidatePosition     = false;
		ifArrived_GraspingCandidatePosition        = false;
		nav_flag.data                              = "";
	}
	void DC_PositionCallback(const std_msgs::String::ConstPtr& msg)
	{
		ifNavigation                           = true;
		ifArrived_DestinationCandidatePosition = true;
		ROS_INFO("DestinationPosition Received");
		if(msg->data == "wide_trashbox_c01")
		{
			goal_pose = wide_trashbox_c01;
		}
		if(msg->data == "wide_trashbox_c02")
		{
			goal_pose = wide_trashbox_c02;
		}
		if(msg->data == "wide_trashbox_c03")
		{
			goal_pose = wide_trashbox_c03;
		}
		if(msg->data == "lowtable_A")
		{
			goal_pose = lowtable_A;
		}
		if(msg->data == "wagon_c02_1")
		{
			goal_pose = wagon_c02_1;
		}
		if(msg->data == "wagon_c02_2")
		{
			goal_pose = wagon_c02_2;
		}
		if(msg->data == "sidetable_A_1")
		{
			goal_pose = sidetable_A_1;
		}
		if(msg->data == "sidetable_A_2")
		{
			goal_pose = sidetable_A_2;
		}
		else
			ROS_INFO("The Destination Does not Exist");

	}
	void GC_PositionCallback(const std_msgs::String::ConstPtr& msg)
	{
		ifNavigation                        = true;
		ifArrived_GraspingCandidatePosition = true;
		ROS_INFO("GraspingPosition Received");
		if(msg->data == "GraspingPosition01")
		{
			goal_pose = GraspingCandidatesPosition01;
		}
		if(msg->data == "GraspingPosition02")
		{
			goal_pose = GraspingCandidatesPosition02;
		}
		if(msg->data == "GraspingPosition03")
		{
			goal_pose = GraspingCandidatesPosition03;
		}
		if(msg->data == "GraspingPosition04")
		{
			goal_pose = GraspingCandidatesPosition04;
		}
		if(msg->data == "GraspingPosition05")
		{
			goal_pose = GraspingCandidatesPosition05;
		}
		if(msg->data == "GraspingPosition06")
		{
			goal_pose = GraspingCandidatesPosition06;
		}
		if(msg->data == "GraspingPosition07")
		{
			goal_pose = GraspingCandidatesPosition07;
		}
		if(msg->data == "GraspingPosition08")
		{
			goal_pose = GraspingCandidatesPosition08;
		}
		if(msg->data == "GraspingPosition09")
		{
			goal_pose = GraspingCandidatesPosition09;
		}
		if(msg->data == "GraspingPosition10")
		{
			goal_pose = GraspingCandidatesPosition10;
		}
		if(msg->data == "GraspingPosition11")
		{
			goal_pose = GraspingCandidatesPosition11;
		}
		if(msg->data == "GraspingPosition12")
		{
			goal_pose = GraspingCandidatesPosition12;
		}
		if(msg->data == "GraspingPosition13")
		{
			goal_pose = GraspingCandidatesPosition13;
		}
		if(msg->data == "GraspingPosition14")
		{
			goal_pose = GraspingCandidatesPosition14;
		}
		if(msg->data == "GraspingPosition15")
		{
			goal_pose = GraspingCandidatesPosition15;
		}
		if(msg->data == "GraspingPosition16")
		{
			goal_pose = GraspingCandidatesPosition16;
		}
		if(msg->data == "GraspingPosition17")
		{
			goal_pose = GraspingCandidatesPosition17;
		}
		if(msg->data == "GraspingPosition18")
		{
			goal_pose = GraspingCandidatesPosition18;
		}
	}
	//init sub, pub and etc 
	int run(int argc, char **argv)
	{
		ros::init(argc, argv, "interactive_cleanup_navigation");
		ros::NodeHandle nh;
		initplace();
		ROS_INFO("Successfully load all waypoints");

		ros::Subscriber GC_Position_sub = nh.subscribe("/GC_Position_Predict",1 ,&navigation::GC_PositionCallback,this);
		ros::Subscriber DC_Position_sub = nh.subscribe("/DC_Position_Predict",1 ,&navigation::DC_PositionCallback,this);
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
				ROS_INFO("Start to Navigation");
				Goal.target_pose.header.frame_id = "map";
				Goal.target_pose.header.stamp    = ros::Time::now();
				Goal.target_pose.pose            = geometry_msgs::Pose(goal_pose);

				while(!mc_.waitForServer(ros::Duration(5.0)))
				{
					ROS_INFO("Waiting For The Server...");
				}
				mc_.sendGoal(Goal);
				mc_.waitForResult(ros::Duration(40.0));

				if(mc_.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
				{
					ROS_INFO("Arrived Goal Position");
					if(ifArrived_GraspingCandidatePosition == true)
					{
						nav_flag.data = "GraspingPositionArrived";
						nav_pub.publish(nav_flag);
						navigation::reset();
					}

					if(ifArrived_DestinationCandidatePosition == true)
					{
						nav_flag.data = "DestinationPositionArrived";
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








