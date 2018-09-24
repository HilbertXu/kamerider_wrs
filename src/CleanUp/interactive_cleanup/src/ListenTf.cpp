#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <string>

#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <tf2/LinearMath/Vector3.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

#define MAX_PATH 260
using namespace std;
using namespace cv;
void Mat2Quat(tf::Matrix3x3&R,double &q0,double &q1,double &q2,double &q3)
{
	//q0=cos(θ/2),q1=sin(θ/2)Kx,q2=sin(θ/2)Ky,q3=sin(θ/2)Kz
	q0=sqrt(abs(1+R[0][0]+R[1][1]+R[2][2]))/2;
	q1=(R[2][1]-R[1][2])/(4*q0);
	q2=(R[0][2]-R[2][0])/(4*q0);
	q3=(R[1][0]-R[0][1])/(4*q0);
}

int main(int argc, char **argv)
{
	string DadAxis;
	string SonAxis;
	DadAxis=argv[1];
	SonAxis=argv[2];
	
	ros::init(argc, argv, "ListenerTf");
	ros::NodeHandle n;

	tf::TransformListener Listener;
	double q0,q1,q2,q3;
	char Buffer[MAX_PATH];
	tf::StampedTransform CurrTf;//监听到的tf变换
	
	while(true)
	{
		try
		{
			Listener.lookupTransform(&(DadAxis[0]), &(SonAxis[0]),ros::Time(0), CurrTf);
		}
		catch (tf::TransformException &ex) 
		{
			ROS_ERROR("%s",ex.what());
			ros::Duration(1.0).sleep();
		}
		tf::Vector3 P=CurrTf.getOrigin();
		tf::Matrix3x3 R=CurrTf.getBasis();
		
		cout<<SonAxis<<" in "<<DadAxis<<": \n";
		Mat2Quat(R,q0,q1,q2,q3);
		sprintf(Buffer,"[%.17g %.17g %.17g %.17g %.17g %.17g %.17g]",
				P[0],P[1],P[2],q0,q1,q2,q3);
		cout<<Buffer<<endl;
		sprintf(Buffer,"[%.17g %.17g %.17g %.17g\n" \
						"%.17g %.17g %.17g %.17g\n" \
						"%.17g %.17g %.17g %.17g]",
						R[0][0],R[0][1],R[0][2],P[0],
						R[1][0],R[1][1],R[1][2],P[1],
						R[2][0],R[2][1],R[2][2],P[2]);
		cout<<Buffer<<endl;
		//依次为子坐标系在父坐标系下的位姿描述
		//[x y z cos(θ/2) sin(θ/2)Kx sin(θ/2)Ky sin(θ/2)Kz]
		
		
		ros::Duration(1.0).sleep();
	}
	return 0;
}
