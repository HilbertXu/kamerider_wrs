#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <cmath>

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
#include "std_msgs/Float64MultiArray.h"
#include <sensor_msgs/JointState.h>

#include <darknet_ros_msgs/BoundingBoxes.h>

using namespace std;
using namespace cv;

const int MAX_PATH=260;
const double VeryBig=9e300;
char Buffer[MAX_PATH];//缓冲区
unsigned char floatBuffer[4];//四字节合并float数据的缓冲区
Mat Img_Frame;//存储头部传感器拍摄到的RGB图片——————————————————————————————————————————————额外添加
pcl::PointCloud<pcl::PointXYZ> Cloud_Frame;//存储每一帧头部传感器点云转到map坐标系下的描述
pcl::PointCloud<pcl::PointXYZ> Cloud_BFrame;//存储每一帧头部传感器点云转到base_link坐标系下的描述
const int ImgWidth = 640;//头部传感器拍摄到的RGB图片宽度
const int ImgHeight = 480;//头部传感器拍摄到的RGB图片高度

ros::Subscriber sub_Img;//头部传感器RGB图片的订阅器
ros::Subscriber sub_PtCloud;//头部传感器点云消息的订阅器
ros::Subscriber sub_Cup;//Avatar指令信息的订阅器
tf::StampedTransform CurrTf;//监听到的tf变换
tf::TransformListener*pListener;//监听tf变换的设备指针,具体定义在main函数ros::init之后了
ros::Publisher pcl_pub;//点云转到map坐标系下坐标后的发布器
ros::Publisher TargetPos_pub;//发布最终结果
ros::Publisher bOBJpos_pub;//发布待抓取的物体中心在本体坐标系下的坐标描述
ros::Publisher bOBJimage_pub;//发布当前头部传感器RGB图片给darknet以检测出待抓取的物体中心在图片中位置
ros::Subscriber sub_Bboxes;//darknet结果的订阅器
ros::Subscriber ResMat_Sub;//Res矩阵接收器

string InitName="GetGCPinDarknet";
string base_link_Name="base_link";
string rgbdFrame_Name="head_rgbd_sensor_depth_frame";

string xmlPath;
vector<string>GCP_Names;
Mat M_Pos_GCP;

//读取xml函数
void ReadXml()
{
	FileStorage xmlFile(xmlPath, FileStorage::READ);
	
	//读取GCP位置个数
	int N_GCP;
	xmlFile["N_GCP"]>>N_GCP;
	
	//读取GCP位置的名字
	for(int i=1;i<=N_GCP;i++)
	{
		string TmpStr;
		sprintf(Buffer,"GCP_Name_%d",i);
		xmlFile[Buffer]>>TmpStr;
		GCP_Names.push_back(TmpStr);
	}
	
	//读取GCP位置,每行一个坐标行向量
	M_Pos_GCP=Mat::zeros(N_GCP,3,CV_64F);
	xmlFile["M_Pos_GCP"] >> M_Pos_GCP;

	xmlFile.release();
}



double ResEtheld=1;//Avatar指向位置与Darknet识别位置最近的一对,若彼此距离小于ResEtheld,则认为该识别位置即为GCP位置
//将之位置用base_link描述然后发送[Gx Gy Gz]的/bOBJgcp消息
//认为此时base_link与map重合,故直接在base_link下比较
//Avatar指向位置在base_link坐标系下描述
double B_ResX=0;
double B_ResY=0;
double B_ResZ=0;
bool bOBJpos_hasPub=false;//是否已经发布了/bOBJpos消息给DOFmove_AutoGcp
bool bOBJimage_hasPub=false;//是否已经发布了/bOBJimage消息给darknet
int t_ReceiveRes=0;//接收到Res矩阵的时刻t_ReceiveRes=ros::Time::now().toSec();
double ResSetHeadT=4;//从t_ReceiveRes开始等ResSetHeadT秒即可认为GCPresPanTilt节点已将头部摆到合适位置,能看到GCP处
int DarknetShouldNum=3;//应该让Darknet识别几次后才可以相信它

void on_mouse( int event, int x, int y, int flags, void* param )
//鼠标回调函数,该函数用鼠标进行跟踪目标的选择
{
	if(Img_Frame.empty()||Cloud_Frame.empty()||Cloud_BFrame.empty())return;
	int row=y,col=x;

	switch( event )
	{
	case CV_EVENT_LBUTTONDOWN:
		{
			int Idx=row*ImgWidth+col;
			printf("[row,col]=[%d,%d],[x,y,z]=[%.17g,%.17g,%.17g]\n",
					row,col,
					Cloud_Frame.points[Idx].x,Cloud_Frame.points[Idx].y,Cloud_Frame.points[Idx].z);
			break;
		}

	case CV_EVENT_RBUTTONDOWN:
		{
			int Idx=row*ImgWidth+col;
			printf("[row,col]=[%d,%d],[x,y,z](in %s)=[%.17g,%.17g,%.17g]\n",
					row,col,&(base_link_Name[0]),
					Cloud_BFrame.points[Idx].x,Cloud_BFrame.points[Idx].y,Cloud_BFrame.points[Idx].z);
			
			std_msgs::Float64MultiArray msg;
			msg.data.push_back(Cloud_BFrame.points[Idx].x);
			msg.data.push_back(Cloud_BFrame.points[Idx].y);
			msg.data.push_back(Cloud_BFrame.points[Idx].z);
			//bOBJpos_pub.publish(msg);
			break;
		}
	}
}
int imageCallback_Num=0;
//订阅头部传感器RGB图片的回调函数
void imageCallback(sensor_msgs::Image msg)
{
	int height=msg.height;
	int width=msg.width;
	int N=msg.data.size();

	Mat Img=Mat::zeros(height,width,CV_8UC3);
	unsigned char*pData=Img.data;
	for(int i=0;i<N;i=i+3)
	{
		pData[i+2]=msg.data[i];
		pData[i+1]=msg.data[i+1];
		pData[i]=msg.data[i+2];
	}
	
	Img_Frame=Img.clone();//——————————————————————————————————————————————额外添加
	
	cv::imshow("The Lonely Observer",Img);
	int key=cv::waitKey(10);
	if(key=='s')
	{
		bOBJimage_pub.publish(msg);
	}
	if(bOBJimage_hasPub==false&&t_ReceiveRes!=0&&(ros::Time::now().toSec()-t_ReceiveRes>ResSetHeadT))
	{
		cout<<"唔……从接收到Res矩阵开始已经过了"<<ResSetHeadT<<"秒了"<<endl;
		cout<<"GCPresPanTilt节点那厮想来已将头部摆到合适位置,能看到GCP处了"<<endl;
		cout<<"把当前图片发给darknet让它去识别吧"<<endl;
		bOBJimage_pub.publish(msg);
		bOBJimage_hasPub=true;
	}
	
	if(imageCallback_Num==0)
	{
		string Wname="The Lonely Observer";
		cvSetMouseCallback(&(Wname[0]),on_mouse,0);
	}
	imageCallback_Num++;
}

//订阅头部传感器点云消息的回调函数
void PcloudCallback(sensor_msgs::PointCloud2 msg)
{
	try
	{
		pListener->lookupTransform("/map", &(rgbdFrame_Name[0]),ros::Time(0), CurrTf);
	}
	catch (tf::TransformException &ex) 
	{
		ROS_ERROR("%s",ex.what());
		return;//听不到就再去听,不能直接往下走了
		ros::Duration(1.0).sleep();
	}
	tf::Vector3 P=CurrTf.getOrigin();
	tf::Matrix3x3 R=CurrTf.getBasis();
	
	try
	{
		pListener->lookupTransform(&(base_link_Name[0]), &(rgbdFrame_Name[0]),ros::Time(0), CurrTf);
	}
	catch (tf::TransformException &ex) 
	{
		ROS_ERROR("%s",ex.what());
		return;//听不到就再去听,不能直接往下走了
		ros::Duration(1.0).sleep();
	}
	tf::Vector3 bP=CurrTf.getOrigin();
	tf::Matrix3x3 bR=CurrTf.getBasis();
	
	//pcl::PointCloud<pcl::PointXYZ> cloud;
    pcl::PointCloud<pcl::PointXYZRGB> cloud;//——————————————————————————————————————————————额外添加
    sensor_msgs::PointCloud2 oMsg;
    cloud.width  = msg.width;
    cloud.height = msg.height;
    cloud.points.resize(cloud.width * cloud.height);
	if(Cloud_Frame.empty())
	{
		Cloud_Frame.width = msg.width;
		Cloud_Frame.height = msg.height;
		Cloud_Frame.points.resize(Cloud_Frame.width * Cloud_Frame.height);
	}
	if(Cloud_BFrame.empty())
	{
		Cloud_BFrame.width = msg.width;
		Cloud_BFrame.height = msg.height;
		Cloud_BFrame.points.resize(Cloud_BFrame.width * Cloud_BFrame.height);
	}
    for (size_t i = 0; i < cloud.points.size(); ++i)
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
		
		//合成三维RGB点云
		cloud.points[i].r = Img_Frame.data[i*3+2];//——————————————————————————————————————————————额外添加
		cloud.points[i].g = Img_Frame.data[i*3+1];//——————————————————————————————————————————————额外添加
		cloud.points[i].b = Img_Frame.data[i*3];//——————————————————————————————————————————————额外添加

		//保存到全局变量里供imageCallback访问
		Cloud_Frame.points[i].x = cloud.points[i].x;
		Cloud_Frame.points[i].y = cloud.points[i].y;
		Cloud_Frame.points[i].z = cloud.points[i].z;
		
        Cloud_BFrame.points[i].x = bR[0][0]*X+bR[0][1]*Y+bR[0][2]*Z+bP[0];
        Cloud_BFrame.points[i].y = bR[1][0]*X+bR[1][1]*Y+bR[1][2]*Z+bP[1];
        Cloud_BFrame.points[i].z = bR[2][0]*X+bR[2][1]*Y+bR[2][2]*Z+bP[2];
    }
	pcl::toROSMsg(cloud, oMsg);
	oMsg.header.frame_id = "map";
	pcl_pub.publish(oMsg);
}


void ResMatCallback(std_msgs::Float64MultiArray msg)
{
	int rows=msg.data[0];
	int cols=msg.data[1];
	Mat Res=Mat::zeros(rows,cols,CV_64F);
	double*p=(double*)(Res.data);
	int N=rows*cols;
	for(int i=0;i<N;i++)
	{
		p[i]=msg.data[2+i];
	}
	
	int rIdx=0;
	for(int i=0;i<rows;i++)
	{
		if(p[i*cols+3]==1)
		{
			rIdx=i;
			break;
		}
	}
	
	double Dx=p[rIdx*cols+4];
	double Dy=p[rIdx*cols+5];
	double Dz=p[rIdx*cols+6];
	
	tf::Vector3 P;
	tf::Matrix3x3 R;
	
	while(true)
	{
		try
		{
			string DadName="map";
			string SonName="base_link";
			
			pListener->lookupTransform(&(DadName[0]), &(SonName[0]),ros::Time(0), CurrTf);
			P=CurrTf.getOrigin();
			R=CurrTf.getBasis();
			break;
		}
		catch (tf::TransformException &ex) 
		{
			ROS_ERROR("%s",ex.what());
			ros::Duration(1.0).sleep();
		}
	}

	B_ResX=R[0][0]*Dx+R[0][1]*Dy+R[0][2]*Dz+P[0];
	B_ResY=R[1][0]*Dx+R[1][1]*Dy+R[1][2]*Dz+P[1];
	B_ResZ=R[2][0]*Dx+R[2][1]*Dy+R[2][2]*Dz+P[2];
	t_ReceiveRes=ros::Time::now().toSec();
	cout<<"接收到Res矩阵￣へ￣"<<endl;
}

//订阅darknet结果消息的回调函数
int BboxesCallback_Num=0;
void BboxesCallback(darknet_ros_msgs::BoundingBoxes msg)
{
	if(bOBJpos_hasPub)return;
	int N=msg.boundingBoxes.size();
	if(N==0)return;
	printf("B_Res=[%.17g %.17g %.17g]\n",B_ResX,B_ResY,B_ResZ);
	int rIdx=0;
	double TmpMinDis=VeryBig;
	double Gx=0,Gy=0,Gz=0;
	
	printf("识别到如下%d个包围盒\n",N);
	for(int i=0;i<N;i++)
	{
		printf("\t box[%d]=[Xmin Ymin Xmax Ymax]=[%d %d %d %d]\n",i,
				msg.boundingBoxes[i].xmin,
				msg.boundingBoxes[i].ymin,
				msg.boundingBoxes[i].xmax,
				msg.boundingBoxes[i].ymax);
		int BoxCrow=(msg.boundingBoxes[i].ymin+msg.boundingBoxes[i].ymax)/2;
		int BoxCcol=(msg.boundingBoxes[i].xmin+msg.boundingBoxes[i].xmax)/2;
		double Cx=Cloud_BFrame.points[BoxCrow*ImgWidth+BoxCcol].x;
		double Cy=Cloud_BFrame.points[BoxCrow*ImgWidth+BoxCcol].y;
		double Cz=Cloud_BFrame.points[BoxCrow*ImgWidth+BoxCcol].z;
		double Dis=sqrt((B_ResX-Cx)*(B_ResX-Cx)+(B_ResY-Cy)*(B_ResY-Cy)+(B_ResZ-Cz)*(B_ResZ-Cz));
		printf("\t\t Center in base_link is [%.17g %.17g %.17g]\n",Cx,Cy,Cz);
		printf("\t\t Dis from B_Res is %.17g\n",Dis);
		printf("\t\t This Object is %s\n",&(msg.boundingBoxes[i].Class[0]));
		
		if(Dis<TmpMinDis)
		{
			TmpMinDis=Dis;
			rIdx=i;
			Gx=Cx;
			Gy=Cy;
			Gz=Cz;
		}
	}
	printf("离B_Res最近的是%s-box[%d]\n",&(msg.boundingBoxes[rIdx].Class[0]),rIdx);
	BboxesCallback_Num++;
	if(BboxesCallback_Num<DarknetShouldNum)
	{
		printf("\n\n Darknet识别刚%d次,还不够%d次,孤不信它\n\n",BboxesCallback_Num,DarknetShouldNum);
		return;
	}
	if(TmpMinDis<ResEtheld)
	{
		printf("MinDis=%.17g<ResEtheld=%.17g\n",TmpMinDis,ResEtheld);
		printf("符合要求,允许向外发布消息/bOBJgcp=[Gx,Gy,Gz]=[%.17g %.17g %.17g]\n",Gx,Gy,Gz);
		std_msgs::Float64MultiArray msg;
		msg.data.push_back(Gx);
		msg.data.push_back(Gy);
		msg.data.push_back(Gz);
		bOBJpos_pub.publish(msg);
		bOBJpos_hasPub=true;
	}
	else
	{
		printf("\n\nDarknet识别出来的包围盒均离Avatar指向点过远,所以下面拿GCP盒子测试\n",N);
		double TmpMinDis=VeryBig;
		double Gx=0,Gy=0,Gz=0;
		double rIdx=0;
		double*pM_Pos_GCP=(double*)(M_Pos_GCP.data);
		for(int i=0;i<M_Pos_GCP.rows;i++)
		{
			double Cx=pM_Pos_GCP[i*3+0];
			double Cy=pM_Pos_GCP[i*3+1];
			double Cz=pM_Pos_GCP[i*3+2];
			double Dis=sqrt((B_ResX-Cx)*(B_ResX-Cx)+(B_ResY-Cy)*(B_ResY-Cy)+(B_ResZ-Cz)*(B_ResZ-Cz));
			printf("\t\t Center in base_link is [%.17g %.17g %.17g]\n",Cx,Cy,Cz);
			printf("\t\t Dis from B_Res is %.17g\n",Dis);
			if(Dis<TmpMinDis)
			{
				TmpMinDis=Dis;
				rIdx=i;
				Gx=Cx;
				Gy=Cy;
				Gz=Cz;
			}
		}
		printf("离B_Res最近的是%s-GCP_Names[%d]\n",&(GCP_Names[rIdx][0]),rIdx);
		if(TmpMinDis<ResEtheld)
		{
			printf("MinDis=%.17g<ResEtheld=%.17g\n",TmpMinDis,ResEtheld);
			printf("符合要求,允许向外发布消息/bOBJgcp=[Gx,Gy,Gz]=[%.17g %.17g %.17g]\n",Gx,Gy,Gz);
			std_msgs::Float64MultiArray msg;
			msg.data.push_back(Gx);
			msg.data.push_back(Gy);
			msg.data.push_back(Gz);
			bOBJpos_pub.publish(msg);
			bOBJpos_hasPub=true;
		}
		else
		{
			printf("MinDis=%.17g≥ResEtheld=%.17g\n",TmpMinDis,ResEtheld);
			printf("不符合要求,继续等待darknet下一次识别\n\n");
		}
	}
	
}

int main(int argc, char **argv)
{//rosrun i_image GetGCPinDarknet -base_link_Name base_link -rgbdFrame_Name head_rgbd_sensor_depth_frame -ResEtheld 1 -ResSetHeadT 4 -DarknetShouldNum 3 -xmlPath *******
	{
		//Useargvtoset
		for(int i=1;i<argc;i++)
		{
			if(i+1<argc&&string(argv[i])=="-base_link_Name")
			{
				base_link_Name=argv[i+1];
			}
			if(i+1<argc&&string(argv[i])=="-rgbdFrame_Name")
			{
				rgbdFrame_Name=argv[i+1];
			}
			if(i+1<argc&&string(argv[i])=="-ResEtheld")
			{
				ResEtheld=atof(argv[i+1]);
			}
			if(i+1<argc&&string(argv[i])=="-ResSetHeadT")
			{
				ResSetHeadT=atof(argv[i+1]);
			}
			if(i+1<argc&&string(argv[i])=="-DarknetShouldNum")
			{
				DarknetShouldNum=atof(argv[i+1]);
			}
			if(i+1<argc&&string(argv[i])=="-xmlPath")
			{
				xmlPath=argv[i+1];
			}
		}
	}
	
	ReadXml();
	
	cout<<"我是"<<InitName<<"，我在等待Res矩阵的到来(￢_￢)"<<endl;
	
	ros::init(argc, argv, &(InitName[0]));
	ros::NodeHandle n;
	sub_PtCloud = n.subscribe("/hsrb/head_rgbd_sensor/depth/points", 1, PcloudCallback);
	sub_Img = n.subscribe("/hsrb/head_rgbd_sensor/rgb/image_raw", 1, imageCallback);
	string MpName=string("/")+InitName+"/map_points";
	pcl_pub = n.advertise<sensor_msgs::PointCloud2>(MpName, 1);
	bOBJpos_pub=n.advertise<std_msgs::Float64MultiArray>("/bOBJgcp", 1);
	bOBJimage_pub=n.advertise<sensor_msgs::Image>("/bOBJimage", 1);
	sub_Bboxes=n.subscribe("/darknet_ros/bounding_boxes", 1, BboxesCallback);
	ResMat_Sub=n.subscribe("/OPAros/GCPRes", 1, ResMatCallback);

	tf::TransformListener Listener;
	pListener = &Listener;
	
	ros::spin();
	return 0;
}
