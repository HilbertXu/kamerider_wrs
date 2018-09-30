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

class find_object
{
public:
	vector<string> GCP_Names;
	vector<string> DCP_Names;
	int rgb_camera_height_, rgb_camera_width_;
	darknet_ros_msgs::BoundingBoxes bounding_boxes_data_;
	sensor_msgs::PointCloud2 point_cloud_data_;
	
	const double Threshold = 0.3;
	const int MAX_OBJECTS_NUM = 3;
	//const std::string CAMERA_DEPTH_OPTICAL_FRAME_NAME = "head_rgbd_sensor_link";
	
	bool flag_table = true;
	bool flag_floor = true;	

public:
	find_object();
	void keyLoop(int argc, char** argv);

public:
	void rgbCameraInfoCallback(const sensor_msgs::CameraInfo::ConstPtr& camera_info);
	void boundingBoxesCallback(const darknet_ros_msgs::BoundingBoxes::ConstPtr& bounding_boxes);
	void graspableObjectCallback(const std_msgs::String::ConstPtr& msg);
	void

}