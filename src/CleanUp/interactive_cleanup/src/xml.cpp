#include <stdio.h>
#include <iostream>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

string xml_dir = "/home/kamerider/sim_ws/src/CleanUp/interactive_cleanup/json_files/pick_it_up_hand_right.xml";

vector<double> convertMat2Vector(const Mat &mat)
{
	return (vector<double>)(mat.reshape(1,1));
}

int main()
{
	std::vector<double> Data_;
	int cols,rows;
	cv::FileStorage xml(xml_dir, FileStorage::READ);
	xml["hand_right_0"]["cols"] >> cols;
	xml["hand_right_0"]["rows"] >> rows;
	cv::Mat Data = Mat::zeros(rows, cols, CV_64F);
	cout << cols << " " << rows <<endl;
	xml["hand_right_0"] >> Data;
	cout<<Data<<endl;
	Data_ = convertMat2Vector(Data);
	for(int i =0;i<Data_.size();i++)
	{
		std::cout << Data_[i] << " ";
	}
	std::cout<<endl;
	std::vector<double> Data_test;
	float *p = (float*)(Data.data);
	int N = rows*cols*3;
	for (int i=0;i<N;i++)
	{
		cout<<p[i]<<" ";
		Data_test.push_back(double(p[i]));
	}

}
