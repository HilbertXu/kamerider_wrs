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

string PoseDir;
double Kw = 0.001;//����������ϵ��
double eTheld = 50;//��������λ��ֵ
double Kh = 0.02;//ͷ����������������ϵ��
double hTheld = 10;//ͷ��������������λ��ֵ
double PrePitch = 0;

bool isUseFollowAvatar = true;

ros::Subscriber sub_Img;//ͷ��������RGBͼƬ�Ķ�����
ros::Subscriber switch_sub;
ros::Publisher pub_base_twist;//�����ٶ���Ϣ
ros::Publisher pub_head_trajectory;//����ͷ�������ؽ���Ϣ
ros::Publisher step_pub;

void pitchHead(ros::Publisher &publisher, const std::string &name, const double position, const int duration_sec)
{
	ROS_INFO("Adjusting to Follow Avatar");
	std::vector<std::string> names;
	names.push_back(name);

	std::vector<double> positions;
	positions.push_back(position);

	ros::Duration duration;
	duration.sec = duration_sec;

	trajectory_msgs::JointTrajectory joint_trajectory;

	trajectory_msgs::JointTrajectoryPoint arm_joint_point;

	joint_trajectory.points.push_back(arm_joint_point);

	joint_trajectory.joint_names = names;
	joint_trajectory.points[0].positions = positions;
	joint_trajectory.points[0].time_from_start = duration;

	publisher.publish(joint_trajectory);
}
void switchCallback(const std_msgs::String::ConstPtr &msg)
{
	if(msg->data == "stop")
	{
		isUseFollowAvatar = false;
	}
	if(msg->data == "start")
	{
		isUseFollowAvatar = true;
	}
}
//����ͷ��������RGBͼƬ�Ļص�����
void imageCallback(sensor_msgs::Image msg)
{
	if(isUseFollowAvatar)
	{
		int height = msg.height;
		int width = msg.width;
		int N = msg.data.size();

		Mat Img = Mat::zeros(height, width, CV_8UC3);
		unsigned char*pData = Img.data;
		for (int i = 0;i < N;i = i + 3)
		{
			pData[i + 2] = msg.data[i];
			pData[i + 1] = msg.data[i + 1];
			pData[i] = msg.data[i + 2];
		}
		cv::imshow("HeadCenterImg", Img);
		waitKey(10);

		//����Windows_Ubuntu OpenPoseͨ��,���ƻ�����ת,ʹ��Avatar�����ܵ���������м�
		imwrite(PoseDir + "/Img.png", Img);
		string Path = PoseDir + "/ManCenter.txt";
		ifstream File;
		File.open(&(Path[0]));
		while (!File.is_open())
		{
			File.open(&(Path[0]));
		}
		int X, Y;
		File >> X >> Y;
		File.close();

		//������
		double w = 0;
		{
			int W = width;
			double e = W / 2 - X;
			if (abs(e) < eTheld || X == 0 || Y == 0)
			{
				w = 0;
			}
			else
			{
				w = Kw*e;
			}
		}
		//�������ٶ�
		{
			geometry_msgs::Twist twist;
			twist.linear.x = 0.0;
			twist.linear.y = 0.0;
			twist.linear.z = 0.0;
			twist.angular.x = 0.0;
			twist.angular.y = 0.0;
			twist.angular.z = w;
			pub_base_twist.publish(twist);
		}

		//ͷ������������
		double pitch = 0;
		double pitch_max = 0.523;
		double pitch_min = -1.57;
		{
			int H = height;
			double e = H / 4 - Y;
			if (abs(e) < hTheld || X == 0 || Y == 0)
			{
				pitch = PrePitch;
			}
			else
			{
				pitch = Kh*e+PrePitch;
			}
			if (pitch > pitch_max)pitch = pitch_max;
			if (pitch < pitch_min)pitch = pitch_min;
		}
		//�����ؽ�λ��
		{
			//pitchHead(pub_head_trajectory, "head_tilt_joint", pitch, 0);
			PrePitch=pitch;
		}
		printf("X=%d,Y=%d,w=%.17g\n",X,Y,w);
	}
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "FollowAvatar");
	ros::NodeHandle nh;

	nh.param<std::string>("PoseDir", PoseDir, "/home/kamerider/sim_ws/src/CleanUp/CamShift_Avatar/build/PoseDir");
	sub_Img = nh.subscribe("/hsrb/head_rgbd_sensor/rgb/image_raw", 1, imageCallback);
	pub_base_twist = nh.advertise<geometry_msgs::Twist>("/hsrb/opt_command_velocity", 1);
	pub_head_trajectory = nh.advertise<trajectory_msgs::JointTrajectory>("/hsrb/head_trajectory_controller/command", 1);
	switch_sub = nh.subscribe("/followAvatar_switch", 1,switchCallback);
	step_pub = nh.advertise<std_msgs::String>("/task_control", 1);



	ros::spin();
	return 0;
}
