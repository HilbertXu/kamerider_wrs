#ifndef SIGVERSE_TURTLEBOT3_OPEN_MANIPULATOR_GRASPING_AUTO_HPP
#define SIGVERSE_TURTLEBOT3_OPEN_MANIPULATOR_GRASPING_AUTO_HPP

#include <cstdio>
#include <cmath>
#include <cstring>
#include <csignal>
#include <unistd.h>
#include <termios.h>
#include <chrono>
#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/CameraInfo.h>
#include <sensor_msgs/JointState.h>
#include <sensor_msgs/PointCloud2.h>
#include <trajectory_msgs/JointTrajectory.h>
#include "darknet_ros_msgs/BoundingBox.h"
#include "darknet_ros_msgs/BoundingBoxes.h"
#include <std_msgs/String.h>

using namespace std::chrono;

class SIGVerseTb3OpenManipulatorGraspingAuto
{

  const std::string CAMERA_DEPTH_OPTICAL_FRAME_NAME = "head_rgbd_sensor_link";

  const std::string MAP_NAME = "map";

  const std::string GRASPING_TARGET1_NAME = "rabbit_doll";
  const std::string GRASPING_TARGET2_NAME = "apple";
  const std::string GRASPING_TARGET3_NAME = "tumbler";
  const std::string GRASPING_TARGET4_NAME = "douburi";
  const std::string GRASPING_TARGET5_NAME = "bear_doll";
  const std::string GRASPING_TARGET6_NAME = "dog_doll";
  const std::string GRASPING_TARGET7_NAME = "ketchup";
  const std::string GRASPING_TARGET8_NAME = "soysauce";
  const std::string GRASPING_TARGET9_NAME = "toy_penguin";
  const std::string GRASPING_TARGET10_NAME = "canned_juice";
  const std::string GRASPING_TARGET11_NAME = "cup";
  const std::string GRASPING_TARGET12_NAME = "sauce";
  const std::string GRASPING_TARGET13_NAME = "sugar";

  std::string GRASPING_TARGET;
  std::string TRASNBOX_TARGET;
  const std::string TRASH1_NAME = "trash_box_for_recycle";
  const std::string TRASH2_NAME = "trash_box_for_burnable";
  const std::string TRASH3_NAME = "trash_box_for_bottle_can";

  const double PROBABILITY_THRESHOLD = 0.3;

  const int OBJECTS_INFO_UPDATING_INTERVAL = 1000; //[ms]

  const int MAX_OBJECTS_NUM = 3;

public:
  SIGVerseTb3OpenManipulatorGraspingAuto();

  void keyLoop(int argc, char** argv);

public:

  static void rosSigintHandler(int sig);

  void rgbCameraInfoCallback(const sensor_msgs::CameraInfo::ConstPtr& camera_info);
  void boundingBoxesCallback(const darknet_ros_msgs::BoundingBoxes::ConstPtr& bounding_boxes);
  void graspableObjectCallback(const std_msgs::String::ConstPtr& msg);
  void trashboxCallback(const std_msgs::String::ConstPtr& msg_trash);

  void pointCloudCallback(const sensor_msgs::PointCloud2::ConstPtr& point_cloud);
  bool findGraspingTarget(geometry_msgs::Vector3 &point_cloud_pos, const std::string &target_name);

  bool sensorToMap(tf::TransformBroadcaster &tf_broadcaster, tf::TransformListener &tf_listener, const std::string &target_name,geometry_msgs::Vector3 &map_target_pos);

  template <class T> static T clamp(const T val, const T min, const T max);
  static bool get3dPositionFromScreenPosition(geometry_msgs::Vector3 &position3d, const sensor_msgs::PointCloud2 point_cloud, const int x, const int y);

  std::string getDetectedObjectsList();
  void showDetectedObjectsList();

  int rgb_camera_height_, rgb_camera_width_;
  std::string grasp_name;
  std::string trashbox_name;
  time_point<system_clock> latest_time_of_bounding_boxes_;
  time_point<system_clock> latest_time_of_point_cloud_;

  darknet_ros_msgs::BoundingBoxes bounding_boxes_data_;
  sensor_msgs::PointCloud2        point_cloud_data_;
};


SIGVerseTb3OpenManipulatorGraspingAuto::SIGVerseTb3OpenManipulatorGraspingAuto()
{

  rgb_camera_height_ = 0;
  rgb_camera_width_  = 0;
  grasp_name = "";
  trashbox_name= "";

  latest_time_of_bounding_boxes_ = system_clock::now();
  latest_time_of_point_cloud_    = system_clock::now();

  bounding_boxes_data_.boundingBoxes.reserve(MAX_OBJECTS_NUM);
}


void SIGVerseTb3OpenManipulatorGraspingAuto::rosSigintHandler(int sig)
{
  ros::shutdown();
}

template <class T>
T SIGVerseTb3OpenManipulatorGraspingAuto::clamp(const T val, const T min, const T max)
{
  return std::min<T>(std::max<T>(min, val), max);
}



/**
 * Get the 3D position of PointCloud using the 2D position of the screen.
 * x: 0 to 479,  y: 0 to 359
 */
bool SIGVerseTb3OpenManipulatorGraspingAuto::get3dPositionFromScreenPosition(geometry_msgs::Vector3 &position3d, const sensor_msgs::PointCloud2 point_cloud, const int x, const int y)
{
  if(point_cloud.header.seq==0)
  {
    puts("No point cloud data.");
    return false;
  }

  // -- PointCloud2 memo --
  // height: 360, width: 480
  // point_step=16, row_step=7680(=16*480)
  int point_data_start_position = y * point_cloud.row_step + x * point_cloud.point_step;

  int xpos = point_data_start_position + point_cloud.fields[0].offset;
  int ypos = point_data_start_position + point_cloud.fields[1].offset;
  int zpos = point_data_start_position + point_cloud.fields[2].offset;

  float pos3d_x, pos3d_y, pos3d_z;

  memcpy(&pos3d_x, &point_cloud.data[xpos], sizeof(float));
  memcpy(&pos3d_y, &point_cloud.data[ypos], sizeof(float));
  memcpy(&pos3d_z, &point_cloud.data[zpos], sizeof(float));

  position3d.x = (double)pos3d_x;
  position3d.y = (double)pos3d_y;
  position3d.z = (double)pos3d_z;

  if(std::isnan(position3d.x) || std::isnan(position3d.y) || std::isnan(position3d.z))
  {
    puts("Point cloud data is nan.");
    return false;
  }
  return true;
}


#endif // SIGVERSE_TURTLEBOT3_OPEN_MANIPULATOR_GRASPING_AUTO_HPP
