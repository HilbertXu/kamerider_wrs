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
#include <darknet_ros_msgs/BoundingBox.h>
#include <darknet_ros_msgs/BoundingBoxes.h>

using namespace std::chrono;