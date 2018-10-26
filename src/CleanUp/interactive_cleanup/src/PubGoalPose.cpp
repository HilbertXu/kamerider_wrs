#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
using namespace std;

int main(int argc, char **argv)
{
	int i=0;
	int N=10;
	if(argc>=9)
	{
		N=atof(argv[8]);
		cout<<"I will publish /move_base_simple/goal "<<N<<" times\n";
	}
	
	ros::init(argc, argv, "PubGoalPose");
	ros::NodeHandle n;
	ros::Publisher Goal_Pub = n.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal",100);
	while(true)
	{
		geometry_msgs::PoseStamped MsgGoal;
		ros::Time Tnow=ros::Time::now();
		MsgGoal.header.seq=0;
		MsgGoal.header.stamp=Tnow;
		MsgGoal.header.frame_id="map";
		MsgGoal.pose.position.x=atof(argv[1]);
		MsgGoal.pose.position.y=atof(argv[2]);
		MsgGoal.pose.position.z=atof(argv[3]);
		MsgGoal.pose.orientation.x=atof(argv[4]);
		MsgGoal.pose.orientation.y=atof(argv[5]);
		MsgGoal.pose.orientation.z=atof(argv[6]);
		MsgGoal.pose.orientation.w=atof(argv[7]);

		Goal_Pub.publish(MsgGoal);
		
		i++;
		if(i>N)break;

		ros::Duration(1.0).sleep();		
		ros::spinOnce();
	}
}
