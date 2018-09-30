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
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include "std_msgs/String.h"

using namespace std;
using namespace cv;

string test_path = "/home/kamerider/sim_ws/src/CleanUp/interactive_cleanup/output_file";

int main()
{
	string fullPath = test_path + "/test.xml";
	cv::FileStorage xmlFile(fullPath, FileStorage::WRITE);

	float p_x = 0.908187;
	float p_y = -0.962155;
	float p_z = 0;

	float o_x = 0;
	float o_y = 0;
	float o_z = -0.665407;
	float o_w = 0.7464481;
	/*
	cvStartWriteStruct(xmlFile, "position", CV_NODE_MAP);
	cvWriteInt(xmlFile, "x", p_x);
	cvWriteInt(xmlFile, "y", p_y);
	cvWriteInt(xmlFile, "z", p_z);
	cvEndWriteStruct(xmlFile);

	cvStartWriteStruct(xmlFile, "orientation", CV_NODE_MAP);
	cvWriteInt(xmlFile, "x", o_x);
	cvWriteInt(xmlFile, "y", o_y);
	cvWriteInt(xmlFile, "z", o_z);
	cvWriteInt(xmlFile, "w", o_w);
	cvEndWriteStruct(xmlFile);
	*/

	xmlFile << "position" << "{";
	xmlFile << "x" << p_x;
	xmlFile << "y" << p_y;
	xmlFile << "z" << p_z;
	xmlFile << "}";

	xmlFile << "orientation" << "{";
	xmlFile << "x" << o_x;
	xmlFile << "y" << o_y;
	xmlFile << "z" << o_z;
	xmlFile << "w" << o_w;
	xmlFile << "}";
	xmlFile.release();

	//read xml
	FileStorage read(fullPath, FileStorage::READ);
	/*cout << read["position"]["x"] << " " << read["position"]["y"] << " " << read["position"]["z"] << endl;
	cout << read["orientation"]["x"] << " " << read["orientation"]["y"] << " " << 
			read["orientation"]["z"] << " " << read["orientation"]["w"] << endl;
	*/

	float x = read["position"]["x"]; cout << x << " ";
	float y = read["position"]["y"]; cout << y << " ";
	float z = read["position"]["z"]; cout << z << endl;

	float ox = read["orientation"]["x"]; cout << ox << " ";
	float oy = read["orientation"]["y"]; cout << oy << " ";
	float oz = read["orientation"]["z"]; cout << oz << " ";
	float ow = read["orientation"]["w"]; cout << ow << endl;
	read.release();
}