//标准头文件
#include <ros/ros.h>
#include <iostream>
#include <std_msgs/String.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Bool.h>
#include <string.h>
#include <exception>
//navigation中需要使用的位姿信息头文件
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/PoseWithCovariance.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Quaternion.h>
//move—base头文件
#include <move_base_msgs/MoveBaseGoal.h>
#include <move_base_msgs/MoveBaseAction.h>
//sigverse头文件
#include <trajectory_msgs/JointTrajectory.h>
#include <trajectory_msgs/JointTrajectoryPoint.h>
#include <tf/transform_listener.h>
#include <handyman/HandymanMsg.h>
#include <nodelet/nodelet.h>
//actionlib头文件
#include <actionlib/client/simple_action_client.h>
#include <stdlib.h>
#include <cstdlib>
//room_position

//string_match
#include "kmp.h"
//about arm
#include <stdio.h>
#include <signal.h>
#include <termios.h>
#include <sensor_msgs/JointState.h>
#include <handyman/HandymanMsg.h>
//opencv
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
//compute
#include <cmath>

#include <unistd.h>

//宏定义部分
#define WINDOW_NAME1 "procedure window1"
#define WINDOW_NAME2 "procedure window2"
#define Extension_Line_Distance 75

using namespace std;
using namespace cv;

cv::Mat map_image;
cv::Mat map_image_gray;

float origin_x, origin_y;
float pixel_x, pixel_y;
float resolution;
float object_x, object_y;
int center_pixel;
int rows, cols;
int edge_x, edge_y;
int extension_position_x, extension_position_y;
int extension_orientation_z, extension_orientation_w;
float worldCoordinate_x, worldCoordinate_y;
string map_dir = "/home/kamerider/sim_ws/src/CleanUp/interactive_cleanup/maps";


geometry_msgs::Pose extension_pose;
ros::Subscriber nav_sub;
ros::Subscriber obj_sub;
ros::Subscriber darknet_sub;
ros::Publisher extension_pub;


void mapCallback(const std_msgs::String::ConstPtr& msg)
{
	if(msg->data == "map_1")
	{
		string fullPath = map_dir + "/interactive_clean_up_test.pgm";
		map_image = imread(fullPath);
		if(!map_image.data)
		{
			cout << "cannot load map";
			return;
		}
		cvtColor(map_image, map_image_gray, cv_BGR2GRAY);
		cv::FileStorage mapFile(map_dir + "/interactive_clean_up_test.yaml",FileStorage::READ);
		if(!mapFile.isOpened())
		{
			cout << "cannot load para file";
			return;
		}
		cv::FileNode ori = mapFile["origin"];
		int indx = 0;
		float origin[3];
		for(cv::FileNodeIterator it = ori.begin();it != ori.end(); ++i, indx++)
		{
			*it >> (float)origin[indx];
		}
		origin_x = origin[0];
		origin_y = origin[1];
		resolution = (float)mapFile["resolution"];
		rows = map_image_gray.rows;
		cols = map_image_gray.cols;
	}
		
}

void tfCallback(const geometry_msgs::Vector3 worldCoordinate)
{
	if(resolution != 0 && worldCoordinate_x != 0 && worldCoordinate_y != 0)
	{
		cout << "start tramsform" <<endl;
		worldCoordinate_x = worldCoordinate.x/resolution;
		worldCoordinate_y = worldCoordinate.y/resolution;
		//meter to pixels
		pixel_x = origin_x / resolution;
		pixel_y = origin_y / resolution;
		object_x = -(pixel_x - worldCoordinate_x);
		object_y = pixel_y - worldCoordinate_y + rows;
		cout << "object_x: " << object_x << " " << "object_y: " << object_y <<endl;
		cout << "worldCoordinate_x: " << worldCoordinate_x << " " << "worldCoordinate_y: " << worldCoordinate_y <<endl;
		cout << "resolution: " << resolution <<endl;
		return;

	}
}

void drawCircle(int center_x, int center_y, int rows, int cols)
{
	cout << "draw a circle" << endl;
	uchar **ptr = new uchar* [rows];
	for(int i = 0; i < rows; i++)
	{
		ptr[i] =  new uchar[cols];
	}

	for(int i = 0; i < rows; i++)
	{
		for(int j = 0; j < cols; j++)
		{
			ptr[i][j] = map_image_gray.at<uchar>(i, j);
		}
	}

	int **iptr = new int*[rows];
	for(int i = 0; i < rows; i++)
	{
		iptr[i] = new int[cols];
	}

	for(int i = 0; i < rows; i++)
	{
		for(int j = 0; j < cols; j++)
		{
			iptr[i][j] = ptr[i][j];
		}
	}

	center_pixel = iptr[center_y][center_x];
	int *edge = new int[2];
	bool reach = false;
	int rads = 0;
	while(!reach)
	{
		for(int i = 0; i < rows; i++)
		{
			for(int j = 0; j < cols; j++)
			{
				if((sqrt(pow(i-center_y, 2)+pow(j-center_x, 2))) >= rads && sqrt(pow(i-center_y, 2) + pow(j-center_x)) < (rads + 0.1))
				{
					if(iptr[j][i] == 255- center_pixel)
					{
						edge_x = j;
						edge_y = i;
						return;
					}
				}
			}
		}
		rads ++;
	}
}

void Extension(int object_x, int object_y, int edge_x, int edge_y)
{
	cout << "Calculating the extension point" << endl;
	int dis;
	dis = sqrt(pow(edge_x - object_x, 2) + pow(edge_y - object_y, 2));
	
	if(center_pixel == 0)
	{
		extension_position_x = object_x + (((Extension_Line_Distance + dis)*(edge_x - object_x))/dis);
		extension_position_y = object_y + (((Extension_Line_Distance + dis)*(edge_y - object_y))/dis);
	}

	if(center_pixel == 255)
	{
		extension_position_x = edge_x -(((Extension_Line_Distance)*(edge_x - object_x))/dis);
		extension_position_y = edge_y -(((Extension_Line_Distance)*(edge_y - object_y))/dis);
	}

	std_msgs::Float64 extension_distance;
	extension_distance.data = dis + Extension_Line_Distance;
	extension_pub.publish(extension_distance);
	return;
}

void QuaternionCalculate(int object_x, int object_y, int edge_x, int edge_y)
{
	cout << "Calculating Quaternion" << endl;
	float sita;
	sita = (worldCoordinate_x)/(sqrt(pow(worldCoordinate_x, 2) + pow(worldCoordinate_y, 2)));
	extension_orientation_w = sqrt((sita/2)+0.5);
	if(worldCoordinate_y > 0)
		extension_orientation_z = sqrt(1 - pow(extension_orientation_w, 2));
	else
		extension_orientation_z = -sqrt(1 - pow(extension_orientation_w, 2));	
	extension_pose.orientation.x = 0;
	extension_pose.orientation.y = 0;
	extension_pose.orientation.z = extension_orientation_z;
	extension_pose.orientation.w = extension_orientation_w;
}
void opencv2map(int extension_position_x, int extension_position_y, int rows, int cols)
{
	cout << "convert extension position from image 2 map" << endl;
	extension_pose.position.x = (extension_position_x - cols - pixel_x) * resolution;
	extension_pose.position.y = (-extension_position_y - pixel_y) * resolution;
	extension_pose.position.z = 0;
	return;
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "position_adjust");
	ros::NodeHandle nh;
	cout << "Start position adjust node" << endl;
	ros::Subscriber darknet_sub     = nh.
}