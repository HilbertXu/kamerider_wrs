//Used to predicr the position Avatar point
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
#include "std_msgs/String.h"

using namespace std;
using namespace cv;

enum Step
{
	initialize,
	pick_it_up,
	clean_up,
};
int step_ = -1;
//OpenPose里检测到Pose关节对应编号(从0开始数,到17)
#define MNeckNum 1
#define RShoulderNum 2
#define RElbowNum 3
#define RWristNum 4
#define LShoulderNum 5
#define LElbowNum 6
#define LWristNum 7
//OpenPose里检测到手部关节对应编号(从0开始数,到20)
#define IFinSecNum 7

const double VeryBig=9e300;
const int MAX_LENGTH = 260;
char Buffer[MAX_LENGTH];
unsigned char floatBuffer[4];
bool pick_it_up_ =false;
bool clean_up_ = false;
bool png_is_saved = false;
bool pcl_is_saved = false;
bool pick_pcl_saved = false;
bool clean_pcl_saved = false;
bool pick_image_saved = false;
bool clean_image_saved = false;

pcl::PointCloud<pcl::PointXYZ> Cloud_Pick;//存储Pick_it_up!时头部传感器点云转到map坐标系下的描述
pcl::PointCloud<pcl::PointXYZ> Cloud_Clean;//存储Clean_up!时头部传感器点云转到map坐标系下的描述
Mat Img_Pick;//存储Pick_it_up!时头部传感器拍摄到的RGB图片
Mat Img_Clean;//存储Clean_up!时头部传感器拍摄到的RGB图片
const int ImgWidth = 640;//头部传感器拍摄到的RGB图片宽度
const int ImgHeight = 480;//头部传感器拍摄到的RGB图片高度

//png, json, xml文件保存路径
string png_dir = "/home/kamerider/sim_ws/src/CleanUp/interactive_cleanup/openpose_img";
string json_dir = "/home/kamerider/sim_ws/src/CleanUp/interactive_cleanup/json_files";
string xml_dir = "/home/kamerider/sim_ws/src/CleanUp/interactive_cleanup/MPose.xml";

//Subscriber & Publisher
ros::Subscriber img_sub;//头部传感器RGB图片的订阅器
ros::Subscriber pcl_sub;//头部传感器点云消息的订阅器
ros::Subscriber openpose_sub;
ros::Subscriber avatar_sub;//Avatar指令信息的订阅器
tf::StampedTransform CurrentTf;//监听到的tf变换
tf::TransformListener* pListener;//监听tf变换的设备指针,具体定义在main函数ros::init之后了
ros::Publisher openpose_pub;//start openpose
ros::Publisher pcl_pub;//点云转到map坐标系下坐标后的发布器
ros::Publisher predict_pub;//发布最终结果
ros::Publisher Avatar_pub;

//Matrixs & Vectors
//GCP = Grasping Candidate Position
//DCP = Destination Candidate Position
cv::Mat Unity_To_Map_Matrix;//Unity坐标系在Map坐标系下的描述
cv::Mat GCP_In_Unity;//抓取点在Unity下的位置坐标
cv::Mat DCP_In_Unity;//放置点在Unity下的坐标
cv::Mat DCP_In_Map_;
vector<string> GCP_Names;//抓取点名称
vector<string> DCP_Names;//放置点名称
vector<string> GCP_Names_sort;//抓取点名称排序
vector<string> DCP_Names_sort;//放置点名称排序

Mat M_Pos_Furniture;//家具上表面三点坐标,每行一个家具的三个点,每个点有xyz
vector<string> Furniture_Names;//家具名称
string GroundName="Ground";//地面名称

void PrintMat(Mat&X,char*Name)
{
	printf("%s=[\n",Name);
	int H=X.rows;
	int W=X.cols;
	double*p=(double*)(X.data);
	for(int i=0;i<H;i++)
	{
		for(int j=0;j<W;j++)
		{
			printf("%.17g ",p[j+i*W]);
		}
		cout<<endl;
	}
	cout<<"];\n";
}


//四舍五入函数
double round(double r)
{
	return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}

//Mat convert to vector
vector<double> convertMat2Vector(const Mat &mat)
{
	return (vector<double>)(mat.reshape(1,1));
}

void reset()
{
	//when received task_failed message
	pick_it_up_       =false;
	clean_up_         = false;
	png_is_saved      = false;
	pcl_is_saved      = false;
	pick_pcl_saved    = false;
	clean_pcl_saved   = false;
	pick_image_saved  = false;
	clean_image_saved = false;

}

int FindNearValid(char flag, int Idx)
{
	pcl::PointCloud<pcl::PointXYZ> *pclPt=NULL;
	if(flag=='p')
	{
		pclPt=&Cloud_Pick;
	}
	if(flag=='c')
	{
		pclPt=&Cloud_Clean;
	}
	if(pclPt==NULL)
		return 0;
	
	int Crow=Idx/ImgWidth;
	int Ccol=Idx%ImgWidth;
	double TmpMin=VeryBig;
	int returnIdx=Idx;
	for(int row=0;row<ImgHeight;row++)
	{
		for(int col=0;col<ImgWidth;col++)
		{
			if((!isnan(pclPt->points[row*ImgWidth+col].x))&&
				(!isnan(pclPt->points[row*ImgWidth+col].y))&&
				(!isnan(pclPt->points[row*ImgWidth+col].z)))
			{
				double Dis2=(row-Crow)*(row-Crow)+(col-Ccol)*(col-Ccol);
				if(Dis2<TmpMin)
				{
					returnIdx=row*ImgWidth+col;
					TmpMin=Dis2;
				}
			}
		}
	}
	return returnIdx;
}

//按排序之后结果重排字符串顺序
void Quick_sort(vector<double>&Datas, vector<string>&strIdxs, int left, int right)
{//外部调用完毕后,互不重复的随机下标就存在了Idx指向的内存里
	if (left >= right)return;
	int i = left, j = right;
	double key = Datas[left];
	string TmstrIdxs;
	while (i < j)
	{
		while (i < j&&Datas[j] >= key)
		{
			j--;
		}
		Datas[i] = Datas[j];
		TmstrIdxs = strIdxs[i];
		strIdxs[i] = strIdxs[j];
		strIdxs[j] = TmstrIdxs;
		while (i < j&&Datas[i] <= key)
		{
			i++;
		}
		Datas[j] = Datas[i];
		TmstrIdxs = strIdxs[j];
		strIdxs[j] = strIdxs[i];
		strIdxs[i] = TmstrIdxs;
	}
	Datas[i] = key;
	Quick_sort(Datas, strIdxs, left, i - 1);
	Quick_sort(Datas, strIdxs, i + 1, right);
}

//读取xml函数
void ReadModelsXml(string &xml_dir, Mat &M_Pos_Furniture, vector<string> &Furniture_Names,
									Mat &DCP_In_map_, vector<string> &DCP_Names)
{
	cv::FileStorage xmlFile(xml_dir, FileStorage::READ);

	int N_Furniture;
	xmlFile["M_Pos_Furniture"]["rows"] >> N_Furniture;
	for(int i= 1; i < N_Furniture; i++)
	{
		string TmpStr;
		sprintf(Buffer,"Furniture_Names_%d", i);
		xmlFile[Buffer] >> TmpStr;
		cout << TmpStr <<endl;
		Furniture_Names.push_back(TmpStr);
	}
	std::cout << M_Pos_Furniture <<endl;
	M_Pos_Furniture = Mat::zeros(N_Furniture, 9, CV_64F);
	xmlFile["M_Pos_Furniture"] >> M_Pos_Furniture;
	xmlFile.release();

	int N_DCP = Furniture_Names.size() - 1;
	DCP_In_Map_ = Mat::ones(4, N_DCP, CV_64F);
	double *pDCP_In_Map_ = (double*)(DCP_In_Map_.data);
	double *pM_Pos_Furniture = (double*)(M_Pos_Furniture.data);
	for(int i = 0; i < N_DCP ;i++)
	{
		cout << Furniture_Names[i] << endl ;
		DCP_Names.push_back(Furniture_Names[i]);
		pDCP_In_Map_[0*DCP_In_Map_.cols+i]=(pM_Pos_Furniture[i*M_Pos_Furniture.cols+0]+pM_Pos_Furniture[i*M_Pos_Furniture.cols+6])/2;
		pDCP_In_Map_[1*DCP_In_Map_.cols+i]=(pM_Pos_Furniture[i*M_Pos_Furniture.cols+1]+pM_Pos_Furniture[i*M_Pos_Furniture.cols+7])/2;
		pDCP_In_Map_[2*DCP_In_Map_.cols+i]=(pM_Pos_Furniture[i*M_Pos_Furniture.cols+2]+pM_Pos_Furniture[i*M_Pos_Furniture.cols+8])/2;
	}
	std::cout << DCP_In_Map_ <<endl;


}
//读取json函数
void ReadXmlKeypoints(string& json_dir,vector<double>& Data,
					vector<double>& Data_L,vector<double>& Data_R,
									int flag)
{
	if(flag == 1)//process pick it up xml
	{
		ROS_INFO("READING PICK IT UP XML FILES");
		{
			ROS_INFO("READING POSE XML FILE");
			string pose_dir = json_dir + "/pick_it_up_pose.xml";
			cv::FileStorage pose_xml(pose_dir, FileStorage::READ);
			int cols, rows;
			pose_xml["pose_0"]["cols"] >> cols;
			pose_xml["pose_0"]["rows"] >> rows;
			cv::Mat Data_ = Mat::zeros(rows, cols, CV_64F);
			pose_xml["pose_0"] >> Data_;
			cout << Data_ << endl;
			Data = convertMat2Vector(Data_);
			pose_xml.release();
		}

		{
			ROS_INFO("READING RIGHT HAND XML FILE");
			string right_dir = json_dir + "/pick_it_up_hand_right.xml";
			cv::FileStorage right_xml(right_dir, FileStorage::READ);
			int cols, rows;
			right_xml["hand_right_0"]["cols"] >> cols;
			right_xml["hand_right_0"]["rows"] >> rows;
			cv::Mat Data_R_ = Mat::zeros(rows, cols, CV_64F);
			right_xml["hand_right_0"] >> Data_R_;
			cout << Data_R_ << endl;
			Data_R = convertMat2Vector(Data_R_);
			right_xml.release();
		}

		{
			ROS_INFO("READING LEFT HAND XML FILE");
			string left_dir = json_dir + "/pick_it_up_hand_left.xml";
			cv::FileStorage left_xml(left_dir, FileStorage::READ);
			int cols, rows;
			left_xml["hand_left_0"]["cols"] >> cols;
			left_xml["hand_left_0"]["rows"] >> rows;
			cv::Mat Data_L_ = Mat::zeros(rows, cols, CV_64F);
			left_xml["hand_left_0"] >> Data_L_;
			cout << Data_L_ << endl;
			Data_L = convertMat2Vector(Data_L_);
			left_xml.release();
		}


	}
	
	if(flag == 2)//process clean up xml files
	{
		ROS_INFO("READING CLEAN UP XML FILES");
		{
			ROS_INFO("READING POSE XML FILE");
			string pose_dir = json_dir + "/clean_up_pose.xml";
			cv::FileStorage pose_xml(pose_dir, FileStorage::READ);
			int cols, rows;
			pose_xml["pose_0"]["cols"] >> cols;
			pose_xml["pose_0"]["rows"] >> rows;
			cv::Mat Data_ = Mat::zeros(rows, cols, CV_64F);
			pose_xml["pose_0"] >> Data_;
			cout << Data_ << endl;
			Data = convertMat2Vector(Data_);
			pose_xml.release();
		}
		
		{
			ROS_INFO("READING RIGHT HAND XML FILE");
			string right_dir = json_dir + "/clean_up_hand_right.xml";
			cv::FileStorage right_xml(right_dir, FileStorage::READ);
			int cols, rows;
			right_xml["hand_right_0"]["cols"] >> cols;
			right_xml["hand_right_0"]["rows"] >> rows;
			cv::Mat Data_R_ = Mat::zeros(rows, cols, CV_64F);
			right_xml["hand_right_0"] >> Data_R_;
			cout << Data_R_ << endl;
			Data_R = convertMat2Vector(Data_R_);
			right_xml.release();
		}

		{
			ROS_INFO("READING LEFT HAND XML FILE");
			string left_dir = json_dir + "/clean_up_hand_left.xml";
			cv::FileStorage left_xml(left_dir, FileStorage::READ);
			int cols, rows;
			left_xml["hand_left_0"]["cols"] >> cols;
			left_xml["hand_left_0"]["rows"] >> rows;
			cv::Mat Data_L_ = Mat::zeros(rows, cols, CV_64F);
			left_xml["hand_left_0"] >> Data_L_;
			cout << Data_L_ << endl;
			Data_L = convertMat2Vector(Data_L_);
			left_xml.release();
		}
	}
}

Mat ComputePto(double Wx,double Wy,double Wz,
				double Fx,double Fy,double Fz)
{
	//返回矩阵每行对应一个Furniture,最后一行对应地面
	//各列依次为k u v Valid Dx Dy Dz
	Mat Result=Mat::zeros(Furniture_Names.size()+1,7,CV_64F);
	double*pResult=(double*)(Result.data);
	double*pM_Pos_Furniture=(double*)(M_Pos_Furniture.data);
	int i=0;
	for(;i<Furniture_Names.size();i++)
	{
		Mat WF=Mat::zeros(3,1,CV_64F);
		{
			double*p=(double*)(WF.data);
			p[0]=Fx-Wx;
			p[1]=Fy-Wy;
			p[2]=Fz-Wz;
		}
		Mat ABCW=Mat::zeros(3,3,CV_64F);
		double Ax,Ay,Az,Bx,By,Bz,Cx,Cy,Cz;
		{
			double*p=(double*)(ABCW.data);
			Ax=pM_Pos_Furniture[i*M_Pos_Furniture.cols+0];
			Ay=pM_Pos_Furniture[i*M_Pos_Furniture.cols+1];
			Az=pM_Pos_Furniture[i*M_Pos_Furniture.cols+2];
			Bx=pM_Pos_Furniture[i*M_Pos_Furniture.cols+3];
			By=pM_Pos_Furniture[i*M_Pos_Furniture.cols+4];
			Bz=pM_Pos_Furniture[i*M_Pos_Furniture.cols+5];
			Cx=pM_Pos_Furniture[i*M_Pos_Furniture.cols+6];
			Cy=pM_Pos_Furniture[i*M_Pos_Furniture.cols+7];
			Cz=pM_Pos_Furniture[i*M_Pos_Furniture.cols+8];
			
			p[0]=Bx-Wx;p[1]=Cx-Bx;p[2]=Ax-Bx;
			p[3]=By-Wy;p[4]=Cy-By;p[5]=Ay-By;
			p[6]=Bz-Wz;p[7]=Cz-Bz;p[8]=Az-Bz;
		}
		Mat Kuv=(ABCW.inv())*WF;
		double*p=(double*)(Kuv.data);
		double k=p[0];
		double u=p[1]/p[0];
		double v=p[2]/p[0];
		
		pResult[i*Result.cols]=k;
		pResult[i*Result.cols+1]=u;
		pResult[i*Result.cols+2]=v;
		if(k>0&&k<1&&u>=0&&u<=1&&v>=0&&v<=1)pResult[i*Result.cols+3]=1;
		pResult[i*Result.cols+4]=Bx+u*(Cx-Bx)+v*(Ax-Bx);
		pResult[i*Result.cols+5]=By+u*(Cy-By)+v*(Ay-By);
		pResult[i*Result.cols+6]=Bz+u*(Cz-Bz)+v*(Az-Bz);
	}
	
	double Gx,Gy,Gz=0;
	double k=(Gz-Wz)/(Fz-Wz);//==(Gy-Wy)/(Fy-Wy)==(Gx-Wx)/(Fx-Wx)
	Gx=k*(Fx-Wx)+Wx;
	Gy=k*(Fy-Wy)+Wy;
	pResult[i*Result.cols+3]=1;
	pResult[i*Result.cols+4]=Gx;
	pResult[i*Result.cols+5]=Gy;
	
	return Result;
}


void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
	if(!png_is_saved)
	{
		ROS_INFO("Head sensor image received...");
		if(!pick_image_saved && step_ == pick_it_up)//save pick it up image
		{
			ROS_INFO("PICK IT UP IMAGE RECEIVED");
			cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
			cv::Mat pick_img = cv_ptr -> image;
			cv::imwrite(png_dir + "/pick_it_up.png", pick_img);
			pick_image_saved = true;
		}
		if(!clean_image_saved && step_ == clean_up)//save clean up image and start openpose
		{
			ROS_INFO("CLEAN UP IMAGE RECEIVED");
			cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
			cv::Mat clean_img = cv_ptr -> image;
			cv::imwrite(png_dir + "/clean_up.png", clean_img);
			clean_image_saved = true;
			//start openpose
			std_msgs::String flag;
			flag.data = "start";
			png_is_saved =true;
			openpose_pub.publish(flag);
		}
	}
}

void openposeCallback(const std_msgs::String::ConstPtr& msg)
{
	ROS_INFO("OPENPOSE FINISHED");
	if(msg->data == "finish")
	{
		
		//process pick it up json file
		{
			int xml_flag = 1;
			vector<int>DataNums;
			vector<double>Data;
			vector<int>DataNums_L;
			vector<double>Data_L;
			vector<int>DataNums_R;
			vector<double>Data_R;
			ReadXmlKeypoints(json_dir,  Data,  Data_L,  Data_R, xml_flag);
			std::cout<<Buffer;
			ROS_INFO("start to calculate the transform matrix");
			/*
			Mat GCP_In_Unity_ = Mat::ones(4, GCP_In_Unity.rows, CV_64F);//每列一个列向量[x y z 1]'
			std::cout<<GCP_In_Unity_.type()<<endl;
			double*pGCP_In_Unity_ = (double*)(GCP_In_Unity_.data);//4行若干列
			double*pGCP_In_Unity = (double*)(GCP_In_Unity.data);//若干行3列
			for (int i = 0; i < GCP_In_Unity.rows; i++)
			{
				pGCP_In_Unity_[i] = pGCP_In_Unity[i * 3 + 0];
				pGCP_In_Unity_[i + GCP_In_Unity.rows] = pGCP_In_Unity[i * 3 + 1];
				pGCP_In_Unity_[i + GCP_In_Unity.rows*2] = pGCP_In_Unity[i * 3 + 2];
			}
			ROS_INFO("frame transform");
			std::cout<<Unity_To_Map_Matrix.type()<<endl;
			Mat GCP_In_Map_ = Unity_To_Map_Matrix*GCP_In_Unity_;//每列存储GCP对应的地图坐标系下坐标[x y z 1]'
			double*pGCP_In_Map_ = (double*)(GCP_In_Map_.data);//4行若干列
			*/
			//当前使用起点为手腕,终点为食指第二指节构造射线
			//首先根据腕部与中轴线距离确定Avatar用的左手还是右手
			//依次为中间脖子点、左手腕、左手食指第二指节、右手腕、右手食指第二指节
			//需要变动时改此处的宏定义即可
			ROS_INFO("predict right hand or left hand");
			std::cout<<Data[MNeckNum * 3]<<" "<<Data[LWristNum * 3]<<" "<<Data_L[IFinSecNum * 3]<<" "<<Data[RWristNum * 3]<<" "<<Data_R[IFinSecNum * 3]<<endl;
			int Mark_Idxs[] = { round(Data[MNeckNum * 3]) + round(Data[MNeckNum * 3 + 1] - 1) * ImgWidth,
								round(Data[LWristNum * 3]) + round(Data[LWristNum * 3 + 1] - 1) * ImgWidth,
								round(Data_L[IFinSecNum * 3]) + round(Data_L[IFinSecNum * 3 + 1] - 1) * ImgWidth,
								round(Data[RWristNum * 3]) + round(Data[RWristNum * 3 + 1] - 1) * ImgWidth,
								round(Data_R[IFinSecNum * 3]) + round(Data_R[IFinSecNum * 3 + 1] - 1) * ImgWidth };
			//！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
			//此处最好有从邻域中找非NaN数字的操作,否则极容易导致复原到三维空间中的无效点
			//ROS_INFO("TEST");
			double M_x = Cloud_Pick.points[Mark_Idxs[0]].x;
			double M_y = Cloud_Pick.points[Mark_Idxs[0]].y;
			double M_z = Cloud_Pick.points[Mark_Idxs[0]].z;
			double Lw_x = Cloud_Pick.points[Mark_Idxs[1]].x;
			double Lw_y = Cloud_Pick.points[Mark_Idxs[1]].y;
			double Lw_z = Cloud_Pick.points[Mark_Idxs[1]].z;
			double Li_x = Cloud_Pick.points[Mark_Idxs[2]].x;
			double Li_y = Cloud_Pick.points[Mark_Idxs[2]].y;
			double Li_z = Cloud_Pick.points[Mark_Idxs[2]].z;
			double Rw_x = Cloud_Pick.points[Mark_Idxs[3]].x;
			double Rw_y = Cloud_Pick.points[Mark_Idxs[3]].y;
			double Rw_z = Cloud_Pick.points[Mark_Idxs[3]].z;
			double Ri_x = Cloud_Pick.points[Mark_Idxs[4]].x;
			double Ri_y = Cloud_Pick.points[Mark_Idxs[4]].y;
			double Ri_z = Cloud_Pick.points[Mark_Idxs[4]].z;
			
			if(isnan(M_x)||isnan(M_y)||isnan(M_z))
			{
				Mark_Idxs[0]=FindNearValid('p',Mark_Idxs[0]);
				M_x = Cloud_Pick.points[Mark_Idxs[0]].x;
				M_y = Cloud_Pick.points[Mark_Idxs[0]].y;
				M_z = Cloud_Pick.points[Mark_Idxs[0]].z;
			}
			if(isnan(Lw_x)||isnan(Lw_y)||isnan(Lw_z))
			{
				Mark_Idxs[1]=FindNearValid('p',Mark_Idxs[1]);
				Lw_x = Cloud_Pick.points[Mark_Idxs[1]].x;
				Lw_y = Cloud_Pick.points[Mark_Idxs[1]].y;
				Lw_z = Cloud_Pick.points[Mark_Idxs[1]].z;
			}
			if(isnan(Li_x)||isnan(Li_y)||isnan(Li_z))
			{
				Mark_Idxs[2]=FindNearValid('p',Mark_Idxs[2]);
				Li_x = Cloud_Pick.points[Mark_Idxs[2]].x;
				Li_y = Cloud_Pick.points[Mark_Idxs[2]].y;
				Li_z = Cloud_Pick.points[Mark_Idxs[2]].z;
			}
			if(isnan(Rw_x)||isnan(Rw_y)||isnan(Rw_z))
			{
				Mark_Idxs[3]=FindNearValid('p',Mark_Idxs[3]);
				Rw_x = Cloud_Pick.points[Mark_Idxs[3]].x;
				Rw_y = Cloud_Pick.points[Mark_Idxs[3]].y;
				Rw_z = Cloud_Pick.points[Mark_Idxs[3]].z;
			}
			if(isnan(Ri_x)||isnan(Ri_y)||isnan(Ri_z))
			{
				Mark_Idxs[4]=FindNearValid('p',Mark_Idxs[4]);
				Ri_x = Cloud_Pick.points[Mark_Idxs[4]].x;
				Ri_y = Cloud_Pick.points[Mark_Idxs[4]].y;
				Ri_z = Cloud_Pick.points[Mark_Idxs[4]].z;
			}

			/*{
				printf("%d\n%d\n%d\n%d\n%d\n", Mark_Idxs[0], Mark_Idxs[1], Mark_Idxs[2], Mark_Idxs[3], Mark_Idxs[4]);
				//调试专用
				printf("%.17g %.17g %.17g \n%.17g %.17g %.17g \n%.17g %.17g %.17g \n%.17g %.17g %.17g \n%.17g %.17g %.17g \n",
					M_x, M_y, M_z, Lw_x, Lw_y, Lw_z, Li_x, Li_y, Li_z, Rw_x, Rw_y, Rw_z, Ri_x, Ri_y, Ri_z);
			}*/
			//ROS_INFO("DATA RECORD");
			vector<double>Dis;
			if ((Lw_x - M_x)*(Lw_x - M_x) + (Lw_y - M_y)*(Lw_y - M_y) >
				(Rw_x - M_x)*(Rw_x - M_x) + (Rw_y - M_y)*(Rw_y - M_y))
			{
				cout << "Avatar used left hand" << endl;
				cv::Mat Res = ComputePto(Lw_x,Lw_y,Lw_z,Li_x,Li_y,Li_z);
				PrintMat(Res, "GCP_Res");
				cout << "\n";
				/*cout << "This is all dis:\n";
				for (int i = 0; i < GCP_Names.size(); i++)
				{
					double GCP_i_x = pGCP_In_Map_[i];
					double GCP_i_y = pGCP_In_Map_[i + GCP_In_Unity.rows];
					double GCP_i_z = pGCP_In_Map_[i + GCP_In_Unity.rows * 2];

					//向量WI叉乘向量W_GCP取模得平行四边形面积
					double dis2 = ((Li_y - Lw_y)*(GCP_i_z - Lw_z) - (GCP_i_y - Lw_y)*(Li_z - Lw_z))*((Li_y - Lw_y)*(GCP_i_z - Lw_z) - (GCP_i_y - Lw_y)*(Li_z - Lw_z)) +
								((GCP_i_x - Lw_x)*(Li_z - Lw_z) - (Li_x - Lw_x)*(GCP_i_z - Lw_z))*((GCP_i_x - Lw_x)*(Li_z - Lw_z) - (Li_x - Lw_x)*(GCP_i_z - Lw_z)) +
								((Li_x - Lw_x)*(GCP_i_y - Lw_y) - (GCP_i_x - Lw_x)*(Li_y - Lw_y))*((Li_x - Lw_x)*(GCP_i_y - Lw_y) - (GCP_i_x - Lw_x)*(Li_y - Lw_y));
					Dis.push_back(sqrt(dis2));
					
					cout << Dis[i]<<" ";
				}
				cout<< "\n";*/

			}
			else
			{
				cout << "Avatar used right hand" << endl;
				Mat Res=ComputePto(Rw_x,Rw_y,Rw_z,Ri_x,Ri_y,Ri_z);
				PrintMat(Res,"GCP_Res");
				cout << "\n";
				/*cout << "This is all dis:\n";
				for (int i = 0; i < GCP_Names.size(); i++)
				{
					double GCP_i_x = pGCP_In_Map_[i];
					double GCP_i_y = pGCP_In_Map_[i + GCP_In_Unity.rows];
					double GCP_i_z = pGCP_In_Map_[i + GCP_In_Unity.rows * 2];

					//向量WI叉乘向量W_GCP取模得平行四边形面积
					double dis2 = ((Ri_y - Rw_y)*(GCP_i_z - Rw_z) - (GCP_i_y - Rw_y)*(Ri_z - Rw_z))*((Ri_y - Rw_y)*(GCP_i_z - Rw_z) - (GCP_i_y - Rw_y)*(Ri_z - Rw_z)) +
								((GCP_i_x - Rw_x)*(Ri_z - Rw_z) - (Ri_x - Rw_x)*(GCP_i_z - Rw_z))*((GCP_i_x - Rw_x)*(Ri_z - Rw_z) - (Ri_x - Rw_x)*(GCP_i_z - Rw_z)) +
								((Ri_x - Rw_x)*(GCP_i_y - Rw_y) - (GCP_i_x - Rw_x)*(Ri_y - Rw_y))*((Ri_x - Rw_x)*(GCP_i_y - Rw_y) - (GCP_i_x - Rw_x)*(Ri_y - Rw_y));
					Dis.push_back(sqrt(dis2));

					cout << Dis[i] << " ";
					//cout << GCP_i_x << " " << GCP_i_y << " " << GCP_i_z << endl;//调试专用
				}
				cout << "\n";*/

			}

			//按Dis从小到大排序后的GCP_Names存在了GCP_Names_sort里
			/*
			for (int i = 0; i < GCP_Names.size(); i++) 
			{
				GCP_Names_sort.push_back(GCP_Names[i]); 
			}
			Quick_sort(Dis, GCP_Names_sort, 0, GCP_Names.size() - 1);
			string MsgStr = "";
			for (int i = 0; i < GCP_Names.size(); i++) 
			{ 
				MsgStr = MsgStr + GCP_Names_sort[i]+" "; 
			}
			cout << "GCP pos sort by dis:\n"<< MsgStr << endl << endl;
			std_msgs::String StrMsg;
			StrMsg.data = MsgStr;
			predict_pub.publish(StrMsg);
			*/
			cv::waitKey(1000);
		}

		//process clean up json file
		{
			//读取Clean_up!时图片对应的2_keypoints.json
			//将位置复原到三维空间map坐标系下,确定Clean_up!时指的是哪个位置
			//sprintf(Buffer, "%s/clean_up_keypoints.json", &(json_dir[0]));
			int xml_flag = 2;
			vector<int>DataNums;
			vector<double>Data;
			vector<int>DataNums_L;
			vector<double>Data_L;
			vector<int>DataNums_R;
			vector<double>Data_R;
			ReadXmlKeypoints(json_dir,  Data,  Data_L,  Data_R, xml_flag);

			

			Mat DCP_In_Unity_ = Mat::ones(4, DCP_In_Unity.rows, CV_64F);//每列一个列向量[x y z 1]'
			double*pDCP_In_Unity_ = (double*)(DCP_In_Unity_.data);//4行若干列
			double*pDCP_In_Unity = (double*)(DCP_In_Unity.data);//若干行3列
			for (int i = 0; i < DCP_In_Unity.rows; i++)
			{
				pDCP_In_Unity_[i] = pDCP_In_Unity[i * 3 + 0];
				pDCP_In_Unity_[i + DCP_In_Unity.rows] = pDCP_In_Unity[i * 3 + 1];
				pDCP_In_Unity_[i + DCP_In_Unity.rows * 2] = pDCP_In_Unity[i * 3 + 2];
			}
			double*pDCP_In_Map_ = (double*)(DCP_In_Map_.data);//4行若干列

															//当前使用起点为手腕,终点为食指第二指节构造射线
															//首先根据腕部与中轴线距离确定Avatar用的左手还是右手
															//依次为中间脖子点、左手腕、左手食指第二指节、右手腕、右手食指第二指节
															//需要变动时改此处的宏定义即可
			int Mark_Idxs[] = { round(Data[MNeckNum * 3]) + round(Data[MNeckNum * 3 + 1] - 1) * ImgWidth,
								round(Data[LWristNum * 3]) + round(Data[LWristNum * 3 + 1] - 1) * ImgWidth,
								round(Data_L[IFinSecNum * 3]) + round(Data_L[IFinSecNum * 3 + 1] - 1) * ImgWidth,
								round(Data[RWristNum * 3]) + round(Data[RWristNum * 3 + 1] - 1) * ImgWidth,
								round(Data_R[IFinSecNum * 3]) + round(Data_R[IFinSecNum * 3 + 1] - 1) * ImgWidth };
			//！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
			//此处最好有从邻域中找非NaN数字的操作,否则极容易导致复原到三维空间中的无效点
			double M_x = Cloud_Clean.points[Mark_Idxs[0]].x;
			double M_y = Cloud_Clean.points[Mark_Idxs[0]].y;
			double M_z = Cloud_Clean.points[Mark_Idxs[0]].z;
			double Lw_x = Cloud_Clean.points[Mark_Idxs[1]].x;
			double Lw_y = Cloud_Clean.points[Mark_Idxs[1]].y;
			double Lw_z = Cloud_Clean.points[Mark_Idxs[1]].z;
			double Li_x = Cloud_Clean.points[Mark_Idxs[2]].x;
			double Li_y = Cloud_Clean.points[Mark_Idxs[2]].y;
			double Li_z = Cloud_Clean.points[Mark_Idxs[2]].z;
			double Rw_x = Cloud_Clean.points[Mark_Idxs[3]].x;
			double Rw_y = Cloud_Clean.points[Mark_Idxs[3]].y;
			double Rw_z = Cloud_Clean.points[Mark_Idxs[3]].z;
			double Ri_x = Cloud_Clean.points[Mark_Idxs[4]].x;
			double Ri_y = Cloud_Clean.points[Mark_Idxs[4]].y;
			double Ri_z = Cloud_Clean.points[Mark_Idxs[4]].z;

			if(isnan(M_x)||isnan(M_y)||isnan(M_z))
			{
				Mark_Idxs[0]=FindNearValid('c',Mark_Idxs[0]);
				M_x = Cloud_Clean.points[Mark_Idxs[0]].x;
				M_y = Cloud_Clean.points[Mark_Idxs[0]].y;
				M_z = Cloud_Clean.points[Mark_Idxs[0]].z;
			}
			if(isnan(Lw_x)||isnan(Lw_y)||isnan(Lw_z))
			{
				Mark_Idxs[1]=FindNearValid('c',Mark_Idxs[1]);
				Lw_x = Cloud_Clean.points[Mark_Idxs[1]].x;
				Lw_y = Cloud_Clean.points[Mark_Idxs[1]].y;
				Lw_z = Cloud_Clean.points[Mark_Idxs[1]].z;
			}
			if(isnan(Li_x)||isnan(Li_y)||isnan(Li_z))
			{
				Mark_Idxs[2]=FindNearValid('c',Mark_Idxs[2]);
				Li_x = Cloud_Clean.points[Mark_Idxs[2]].x;
				Li_y = Cloud_Clean.points[Mark_Idxs[2]].y;
				Li_z = Cloud_Clean.points[Mark_Idxs[2]].z;
			}
			if(isnan(Rw_x)||isnan(Rw_y)||isnan(Rw_z))
			{
				Mark_Idxs[3]=FindNearValid('c',Mark_Idxs[3]);
				Rw_x = Cloud_Clean.points[Mark_Idxs[3]].x;
				Rw_y = Cloud_Clean.points[Mark_Idxs[3]].y;
				Rw_z = Cloud_Clean.points[Mark_Idxs[3]].z;
			}
			if(isnan(Ri_x)||isnan(Ri_y)||isnan(Ri_z))
			{
				Ri_x = Cloud_Clean.points[Mark_Idxs[4]].x;
				Ri_y = Cloud_Clean.points[Mark_Idxs[4]].y;
				Ri_z = Cloud_Clean.points[Mark_Idxs[4]].z;
			}
			/*{
				printf("%d\n%d\n%d\n%d\n%d\n", Mark_Idxs[0], Mark_Idxs[1], Mark_Idxs[2], Mark_Idxs[3], Mark_Idxs[4]);
				//调试专用
				printf("%.17g %.17g %.17g \n%.17g %.17g %.17g \n%.17g %.17g %.17g \n%.17g %.17g %.17g \n%.17g %.17g %.17g \n",
					M_x, M_y, M_z, Lw_x, Lw_y, Lw_z, Li_x, Li_y, Li_z, Rw_x, Rw_y, Rw_z, Ri_x, Ri_y, Ri_z);
			}*/

			vector<double>Dis;
			if ((Lw_x - M_x)*(Lw_x - M_x) + (Lw_y - M_y)*(Lw_y - M_y) >
				(Rw_x - M_x)*(Rw_x - M_x) + (Rw_y - M_y)*(Rw_y - M_y))
			{
				cout << "Avatar used left hand" << endl;
				cout << "This is all dis:\n";
				
				for (int i = 0; i < DCP_Names.size(); i++)
				{
					double DCP_i_x = pDCP_In_Map_[i];
					double DCP_i_y = pDCP_In_Map_[i + DCP_In_Unity.rows];
					double DCP_i_z = pDCP_In_Map_[i + DCP_In_Unity.rows * 2];

					//向量WI叉乘向量W_DCP取模得平行四边形面积
					double dis2 = ((Li_y - Lw_y)*(DCP_i_z - Lw_z) - (DCP_i_y - Lw_y)*(Li_z - Lw_z))*((Li_y - Lw_y)*(DCP_i_z - Lw_z) - (DCP_i_y - Lw_y)*(Li_z - Lw_z)) +
						((DCP_i_x - Lw_x)*(Li_z - Lw_z) - (Li_x - Lw_x)*(DCP_i_z - Lw_z))*((DCP_i_x - Lw_x)*(Li_z - Lw_z) - (Li_x - Lw_x)*(DCP_i_z - Lw_z)) +
						((Li_x - Lw_x)*(DCP_i_y - Lw_y) - (DCP_i_x - Lw_x)*(Li_y - Lw_y))*((Li_x - Lw_x)*(DCP_i_y - Lw_y) - (DCP_i_x - Lw_x)*(Li_y - Lw_y));
					Dis.push_back(sqrt(dis2));

					cout << Dis[i] << " ";
				}
				cout << "\n";
			}
			else
			{
				cout << "Avatar used right hand" << endl;
				cout << "This is all dis:\n";
				for (int i = 0; i < DCP_Names.size(); i++)
				{
					double DCP_i_x = pDCP_In_Map_[i];
					double DCP_i_y = pDCP_In_Map_[i + DCP_In_Unity.rows];
					double DCP_i_z = pDCP_In_Map_[i + DCP_In_Unity.rows * 2];

					//向量WI叉乘向量W_DCP取模得平行四边形面积
					double dis2 = ((Ri_y - Rw_y)*(DCP_i_z - Rw_z) - (DCP_i_y - Rw_y)*(Ri_z - Rw_z))*((Ri_y - Rw_y)*(DCP_i_z - Rw_z) - (DCP_i_y - Rw_y)*(Ri_z - Rw_z)) +
						((DCP_i_x - Rw_x)*(Ri_z - Rw_z) - (Ri_x - Rw_x)*(DCP_i_z - Rw_z))*((DCP_i_x - Rw_x)*(Ri_z - Rw_z) - (Ri_x - Rw_x)*(DCP_i_z - Rw_z)) +
						((Ri_x - Rw_x)*(DCP_i_y - Rw_y) - (DCP_i_x - Rw_x)*(Ri_y - Rw_y))*((Ri_x - Rw_x)*(DCP_i_y - Rw_y) - (DCP_i_x - Rw_x)*(Ri_y - Rw_y));
					Dis.push_back(sqrt(dis2));

					cout << Dis[i] << " ";
					//cout << DCP_i_x << " " << DCP_i_y << " " << DCP_i_z << endl;//调试专用
				}
				cout << "\n";
				cv::Mat Res=ComputePto(Rw_x,Rw_y,Rw_z,Ri_x,Ri_y,Ri_z);
				PrintMat(Res,"DCP_Res");
			}
			//按Dis从小到大排序后的DCP_Names存在了DCP_Names_sort里
			for (int i = 0; i < DCP_Names.size(); i++) { DCP_Names_sort.push_back(DCP_Names[i]); }
			Quick_sort(Dis, DCP_Names_sort, 0, DCP_Names.size() - 1);
			string MsgStr = "";
			for (int i = 0; i < DCP_Names.size(); i++) { MsgStr = MsgStr + DCP_Names_sort[i] + " "; }
			cout << "DCP pos sort by dis:\n" << MsgStr << endl << endl;
			std_msgs::String StrMsg;
			StrMsg.data = MsgStr;
			predict_pub.publish(StrMsg);
		}
	}
}


void pclCallback(sensor_msgs::PointCloud2 msg)
{
	try
	{
		pListener -> lookupTransform("/map", "head_rgbd_sensor_depth_frame", ros::Time(0), CurrentTf);
	}
	catch (tf::TransformException &ex)
	{
		ROS_ERROR("%s", ex.what());
		ros::Duration(1.0).sleep();
	}
	tf::Vector3 P = CurrentTf.getOrigin();
	std::cout <<"Origin Matrix P is: "<<endl;
	std::cout << P[0] <<" "<< P[1] << " " << P[2] <<endl;
	std::cout<<endl;
	tf::Matrix3x3 R = CurrentTf.getBasis();
	std::cout << " Basis Matrix R is: " <<endl;
	for(int n=0;n<3;n++)
	{
		for(int m=0;m<3;m++)
		{
			cout<<R[n][m] << " ";
		}
		cout<<endl;
	}
	std::cout<<endl;

	pcl::PointCloud<pcl::PointXYZ> cloud;
    sensor_msgs::PointCloud2 oMsg;
	cloud.width = msg.width;
	cloud.height = msg.height;
	cloud.points.resize(cloud.width * cloud.height);
	std::cout << " PointCloud size is: " << endl;
	std::cout << " " << "Width is: " << msg.width << " " << "Height is: " <<msg.height <<endl;
	 
	std::cout << "PointCloud data is: " << endl;
	if(step_ == pick_it_up)
	{
		ROS_INFO("pick it up pcl calculate");
		Cloud_Pick.width = msg.width;
		Cloud_Pick.height = msg.height;
		Cloud_Pick.points.resize(Cloud_Pick.width * Cloud_Pick.height);
	}
	if(step_ == clean_up)
	{
		ROS_INFO("clean up pcl calculate");
		Cloud_Clean.width = msg.width;
		Cloud_Clean.height = msg.height;
		Cloud_Clean.points.resize(Cloud_Clean.width * Cloud_Clean.height);
	}
	for(int i = 0; i < cloud.points.size(); i++)
	{

		
		//把已有点云消息中的X,Y,Z提取出来
		floatBuffer[0]=msg.data[i*16+0];
		floatBuffer[1]=msg.data[i*16+1];
		floatBuffer[2]=msg.data[i*16+2];
		floatBuffer[3]=msg.data[i*16+3];
		double X=*((float*)floatBuffer);
		
		floatBuffer[0]=msg.data[i*16+4];
		floatBuffer[1]=msg.data[i*16+5];
		floatBuffer[2]=msg.data[i*16+6];
		floatBuffer[3]=msg.data[i*16+7];
		double Y=*((float*)floatBuffer);
		
		floatBuffer[0]=msg.data[i*16+8];
		floatBuffer[1]=msg.data[i*16+9];
		floatBuffer[2]=msg.data[i*16+10];
		floatBuffer[3]=msg.data[i*16+11];
		double Z=*((float*)floatBuffer);
		
		//利用坐标变换将点云坐标变到map坐标系下描述
        cloud.points[i].x = R[0][0]*X+R[0][1]*Y+R[0][2]*Z+P[0];
        cloud.points[i].y = R[1][0]*X+R[1][1]*Y+R[1][2]*Z+P[1];
        cloud.points[i].z = R[2][0]*X+R[2][1]*Y+R[2][2]*Z+P[2];
        //std::cout<< cloud.points[i].x << " " << cloud.points[i].y << " " << cloud.points[i].z <<endl;
		//std::cout << "TEST" << endl;
		//std::cout << step_  << endl;
		
        if(step_ == pick_it_up && step_ != clean_up)
		{
			Cloud_Pick.points[i].x = cloud.points[i].x;
			Cloud_Pick.points[i].y = cloud.points[i].y;
			Cloud_Pick.points[i].z = cloud.points[i].z;
		}
		if(step_ == clean_up)
		{
			Cloud_Clean.points[i].x = cloud.points[i].x;
			Cloud_Clean.points[i].y = cloud.points[i].y;
			Cloud_Clean.points[i].z = cloud.points[i].z;
		}
		
	}
	pcl_is_saved = true;
	/*
	//test delete NaN points
	//by using removeNaNFromPointCloud

	std::vector<int> map_index;
	std::vector<int> pick_index;
	std::vector<int> clean_index;
	pcl::removeNaNFromPointCloud(Cloud_Pick,Cloud_Pick,pick_index);
	pcl::removeNaNFromPointCloud(Cloud_Clean,Cloud_Clean,clean_index);
	pcl::removeNaNFromPointCloud(cloud,cloud,map_index);
	*/
	pcl::toROSMsg(cloud,oMsg);
	ROS_INFO("Header frame: map");
	oMsg.header.frame_id = "map";
	pcl_pub.publish(oMsg);
	
}

void AvatarMsgCallback(interactive_cleanup::InteractiveCleanupMsg msg)
{
	ROS_INFO("Avatar command received");
	cout << msg.detail << "\t" << msg.message << endl;
	if(msg.message == "Are_you_ready?")
	{
		ROS_INFO("SAY I AM READY");
		interactive_cleanup::InteractiveCleanupMsg interactive_cleanup_msg;
    	interactive_cleanup_msg.message = "I_am_ready";
		Avatar_pub.publish(interactive_cleanup_msg);
		ROS_INFO("TASK START");
	}
	if(msg.message == "Pick_it_up!")
	{
		ROS_INFO("STEP = PICK IT UP");
		step_ = pick_it_up;
	}
	if(msg.message == "Clean_up!")
	{
		ROS_INFO("STEP = CLEAN UP");
		step_ = clean_up;
	}
	if(msg.message == "Task_failed")
	{
		step_ = initialize;
		reset();
	}
}

int main(int argc, char **argv)
{
	ReadModelsXml(xml_dir, M_Pos_Furniture, Furniture_Names, DCP_In_Map_, DCP_Names);

	ros::init(argc, argv, "Position_Predict");
	ros::NodeHandle nh;
	pcl_sub         = nh.subscribe("/hsrb/head_rgbd_sensor/depth/points", 1, pclCallback);
	avatar_sub      = nh.subscribe("/interactive_cleanup/message/to_robot", 1, AvatarMsgCallback);
	img_sub         = nh.subscribe("hsrb/head_rgbd_sensor/rgb/image_raw", 1, imageCallback);
	openpose_sub    = nh.subscribe("/finish_openpose", 1, openposeCallback);

	openpose_pub    = nh.advertise<std_msgs::String>("/start_openpose", 1);
	pcl_pub         = nh.advertise<sensor_msgs::PointCloud2>("/map_points", 1);
	predict_pub     = nh.advertise<std_msgs::String>("/TargetPostion", 1);
	Avatar_pub      = nh.advertise<interactive_cleanup::InteractiveCleanupMsg>("/interactive_cleanup/message/to_moderator", 1);

	tf::TransformListener Listener;
	pListener = &Listener;

	ros::spin();
	return 0;

}