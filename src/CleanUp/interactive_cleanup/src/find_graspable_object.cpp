//find graspable object
/*
	Author: xyc
*/
//收到来自导航的到达目标点的信息之
//分别在地上和桌子上寻找物体，在桌子上寻找时使用来自/hsrb/head_rgbd_sensor/rgb/image_raw话题的图像信息
//在地面上寻找时，先将手臂放下，然后使用来自/hsrb/hand_camera/image_raw话题的图像信息。
//同时调用darknet节点进行物体检测
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <string>

#include <ros/ros.h>
#include <interactive_cleanup/InteractiveCleanupMsg.h>
#include <sensor_msgs/PointCloud2.h>
#include <tf/transform_listener.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <tf2/LinearMath/Vector3.h>
#include <pcl/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/filters/filter.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/CameraInfo.h>
#include <sensor_msgs/JointState.h>
#include <sensor_msgs/PointCloud2.h>
#include <trajectory_msgs/JointTrajectory.h>
#include "darknet_ros_msgs/BoundingBox.h"
#include "darknet_ros_msgs/BoundingBoxes.h"
#include "std_msgs/String.h"

using namespace std;
using namespace cv;

bool find_floor = false;
bool find_table = false;

std_msgs::String find_result;
trajectory_msgs::JointTrajectory arm_joint_trajectory_;

ros::Subscriber nav_sub;
ros::Subscriber darknet_sub;
ros::Subscriber griper_sub;

ros::Publisher obj_pub;

void navCallback(const std_msgs::String::ConstPtr& msg)
{
	if(msg->data == "GCP_Arrived")
	{
		//start find obj
	}

}