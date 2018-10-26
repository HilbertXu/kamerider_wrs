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
#include <std_msgs/Float64MultiArray.h>

using namespace std;
using namespace cv;

//OpenPose�����⵽Pose�ؽڶ�Ӧ����(��0��ʼ��,��17)
#define MNeckNum 1
#define RShoulderNum 2
#define RElbowNum 7
#define RWristNum 4
#define LShoulderNum 5
#define LElbowNum 6
#define LWristNum 7
#define MidHipNum 8
//OpenPose�����⵽�ֲ��ؽڶ�Ӧ����(��0��ʼ��,��20)
#define IFinSecNum 3


const double VeryBig=9e300;
const int MAX_PATH=260;
char Buffer[MAX_PATH];//������
unsigned char floatBuffer[4];//���ֽںϲ�float���ݵĻ�����
int PngStaticNum=0;//����ͼƬ������,Pick_it_up!ʱΪ1,Clean_up!ʱΪ2
int PtsStaticNum=0;//������Ϣ������,Pick_it_up!ʱΪ1,Clean_up!ʱΪ2
bool PngIsSave=false;//��ǰ�Ƿ��յ�Pick_it_up!��Clean_up!��Ϣ,�Ӷ�ȷ���Ƿ���ͼƬд��
bool PtsIsSave=false;//��ǰ�Ƿ��յ�Pick_it_up!��Clean_up!��Ϣ,�Ӷ�ȷ���Ƿ񽫵�����Ϣд��
pcl::PointCloud<pcl::PointXYZ> Cloud_Pick;//�洢Pick_it_up!ʱͷ������������ת��map����ϵ�µ�����
pcl::PointCloud<pcl::PointXYZ> Cloud_Clean;//�洢Clean_up!ʱͷ������������ת��map����ϵ�µ�����
Mat Img_Pick;//�洢Pick_it_up!ʱͷ�����������㵽��RGBͼƬ
Mat Img_Clean;//�洢Clean_up!
Mat Img_Frame;//�洢ͷ�����������㵽��RGBͼƬ����������������������������������������������������������������������������������������������������
const int ImgWidth = 640;//ͷ�����������㵽��RGBͼƬ����
const int ImgHeight = 480;//ͷ�����������㵽��RGBͼƬ�߶�


//png, json, xml�ļ�����·��
string png_dir      = "/home/kamerider/sim_ws/src/CleanUp/interactive_cleanup/openpose_img";
string json_dir     = "/home/kamerider/sim_ws/src/CleanUp/interactive_cleanup/json_files";
string xml_dir      = "/home/kamerider/sim_ws/src/CleanUp/interactive_cleanup/MPose.xml";
string output_dir   = "/home/kamerider/sim_ws/src/CleanUp/interactive_cleanup/output_file";

string SaveDir;//png��json�ļ�����·��
ros::Subscriber sub_Img;//ͷ��������RGBͼƬ�Ķ�����
ros::Subscriber sub_PtCloud;//ͷ��������������Ϣ�Ķ�����
ros::Subscriber sub_Cup;//Avatarָ����Ϣ�Ķ�����
ros::Subscriber sub_Openpose;

tf::StampedTransform CurrTf;//��������tf�任
tf::TransformListener*pListener;//����tf�任���豸ָ��,���嶨����main����ros::init֮����
ros::Publisher pcl_pub;//����ת��map����ϵ���������ķ�����
ros::Publisher TargetPos_pub;//�������ս���
ros::Publisher openpose_pub;
ros::Publisher Avatar_pub;
ros::Publisher nav_pub;
ros::Publisher step_pub;
ros::Publisher task_control;
ros::Publisher Mat_pub;

string SaveFileName;//��ʼ����xml·��
Mat M_U_T;//Unity����ϵ��Map����ϵ�µ�����
Mat U_Pos_GCP;//ץȡ����Unity�µ�λ������
Mat U_Pos_DCP;//���õ���Unity�µ�����
Mat M_Pos_DCP_;
vector<string> GCP_Names;//ץȡ������
vector<string> DCP_Names;//���õ�����
vector<string> GCP_Names_sort;//ץȡ������
vector<string> DCP_Names_sort;//���õ�����

Mat M_Pos_Furniture;//�Ҿ��ϱ�����������,ÿ��һ���Ҿߵ�������,ÿ������xyz
vector<string> Furniture_Names;//�Ҿ�����
string GroundName="Ground";//��������

float p_x, p_y, p_z;
float o_x, o_y, o_z, o_w;
float wrist_x, wrist_y, wrist_z;
float index_x, index_y, index_z;

void PrintMat(Mat&X,char*Name)
{
	printf("%s=[\n",Name);
	int H=X.rows;
	int W=X.cols;
	double*p=(double*)(X.data);
	for(int i=0;i<H;i++)
	{
		if(i == H-1)
		{
			p_x = p[i*W + 3];
			p_y = p[i*W + 4];
			p_z = 0;
		}
		for(int j=0;j<W;j++)
		{
			printf("%.17g ",p[j+i*W]);
		}
		cout<<endl;
	}
	cout<<"];\n";
}

void CalculateQuaternion(float w_x, float w_y, float w_z,
						 float i_x, float i_y, float i_z)
{
	//ȡ��������ʳָ�ڶ�ָ����ƽ���ϵ�ͶӰ(x,y,0)��Ϊ�����˵�����Ŀ����֮���ĳ���
	//������ֻ������Z��ת���ĽǶ�
	int x1 = w_x; int y1 = w_y;
	int x2 = i_x; int y2 = i_y;
	int z1 = 0  ; int z2 = 0  ;
}

//�������뺯��
double round(double r)
{
	return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}


int FindNearValid(char Choice,int Idx)
{
	pcl::PointCloud<pcl::PointXYZ>*pclPt=NULL;
	if(Choice=='p')
	{
		pclPt=&Cloud_Pick;
	}
	if(Choice=='c')
	{
		pclPt=&Cloud_Clean;
	}
	if(pclPt==NULL)return 0;

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

//������֮�����������ַ���˳��
void Quick_sort(vector<double>&Datas, vector<string>&strIdxs, int left, int right)
{//�ⲿ�������Ϻ�,�����ظ��������±��ʹ�����Idxָ�����ڴ���
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


//��ȡxml����
void ReadModelsXml(string&SaveFileName,Mat&M_Pos_Furniture,vector<string>&Furniture_Names)
{
	FileStorage xmlFile(SaveFileName, FileStorage::READ);



	//��ȡ�Ҿ�λ�ø���
	int N_Furniture;
	xmlFile["M_Pos_Furniture"]["rows"]>>N_Furniture;

	//��ȡFurnitureλ�õ�����
	for(int i=1;i<=N_Furniture;i++)
	{
		string TmpStr;
		sprintf(Buffer,"Furniture_Name_%d",i);
		xmlFile[Buffer]>>TmpStr;
		Furniture_Names.push_back(TmpStr);
	}

	//��ȡFurnitureλ��,ÿ����������������
	M_Pos_Furniture=Mat::zeros(N_Furniture,9,CV_64F);
	xmlFile["M_Pos_Furniture"] >> M_Pos_Furniture;

	xmlFile.release();
}

//��ȡjson����
void ReadJsonKeypoints(char*FullPath,vector<int>&DataNums,vector<double>&Data,
									vector<int>&DataNums_L,vector<double>&Data_L,
									vector<int>&DataNums_R,vector<double>&Data_R)
{
	ifstream Json(FullPath);
	string TmpStr;
	do
	{
		Json>>TmpStr;
		if(TmpStr=="\"pose_keypoints\":[")
		{
			string DataStr;
			Json>>DataStr;
			int Nums=0;
			int L=DataStr.size();
			double Tmp=atof(&(DataStr[0]));
			Data.push_back(Tmp);Nums++;
			for(int i=1;i<L;i++)
			{
				if(DataStr[i]==',')
				{
					Tmp=atof(&(DataStr[i+1]));
					Data.push_back(Tmp);Nums++;
				}
			}
			DataNums.push_back(Nums);
		}
		if(TmpStr=="\"hand_left_keypoints\":[")
		{
			string DataStr;
			Json>>DataStr;
			int Nums=0;
			int L=DataStr.size();
			double Tmp=atof(&(DataStr[0]));
			Data_L.push_back(Tmp);Nums++;
			for(int i=1;i<L;i++)
			{
				if(DataStr[i]==',')
				{
					Tmp=atof(&(DataStr[i+1]));
					Data_L.push_back(Tmp);Nums++;
				}
			}
			DataNums_L.push_back(Nums);
		}
		if(TmpStr=="\"hand_right_keypoints\":[")
		{
			string DataStr;
			Json>>DataStr;
			int Nums=0;
			int L=DataStr.size();
			double Tmp=atof(&(DataStr[0]));
			Data_R.push_back(Tmp);Nums++;
			for(int i=1;i<L;i++)
			{
				if(DataStr[i]==',')
				{
					Tmp=atof(&(DataStr[i+1]));
					Data_R.push_back(Tmp);Nums++;
				}
			}
			DataNums_R.push_back(Nums);
		}
	}while(Json.peek() != EOF);
}
//��ȡOpenPoseд����xml��Ϣ
void ReadXmlKeypoints(char*PoseXmlPath,char*RhandXmlPath,char*LhandXmlPath,
					vector<double>&Data,vector<double>&Data_R,vector<double>&Data_L)
{
	{
		FileStorage PoseXmlFile(PoseXmlPath, FileStorage::READ);
		int rows,cols;
		PoseXmlFile["pose_0"]["rows"]>>rows;
		PoseXmlFile["pose_0"]["cols"]>>cols;
		Mat pose_0=Mat::zeros(rows,cols,CV_32FC3);
		PoseXmlFile["pose_0"]>>pose_0;
		float*p=(float*)(pose_0.data);
		int N=rows*cols*3;
		for(int i=0;i<N;i++)
		{
			Data.push_back(double(p[i]));
		}
	}

	{
		FileStorage RhandXmlFile(RhandXmlPath, FileStorage::READ);
		int rows,cols;
		RhandXmlFile["hand_right_0"]["rows"]>>rows;
		RhandXmlFile["hand_right_0"]["cols"]>>cols;
		Mat hand_right_0=Mat::zeros(rows,cols,CV_32FC3);
		RhandXmlFile["hand_right_0"]>>hand_right_0;
		float*p=(float*)(hand_right_0.data);
		int N=rows*cols*3;
		for(int i=0;i<N;i++)
		{
			Data_R.push_back(double(p[i]));
		}
	}

	{
		FileStorage LhandXmlFile(LhandXmlPath, FileStorage::READ);
		int rows,cols;
		LhandXmlFile["hand_left_0"]["rows"]>>rows;
		LhandXmlFile["hand_left_0"]["cols"]>>cols;
		Mat hand_left_0=Mat::zeros(rows,cols,CV_32FC3);
		LhandXmlFile["hand_left_0"]>>hand_left_0;
		float*p=(float*)(hand_left_0.data);
		int N=rows*cols*3;
		for(int i=0;i<N;i++)
		{
			Data_L.push_back(double(p[i]));
		}
	}

	// printf("PoseXmlPath=%s\n",PoseXmlPath);
	// for(int i=0;i<Data.size();i+=3)
	// {
		// printf("%.17g %.17g %.17g\n",Data[i],Data[i+1],Data[i+2]);
	// }
	// printf("RhandXmlPath=%s\n",RhandXmlPath);
	// for(int i=0;i<Data_R.size();i+=3)
	// {
		// printf("%.17g %.17g %.17g\n",Data_R[i],Data_R[i+1],Data_R[i+2]);
	// }
	// printf("LhandXmlPath=%s\n",LhandXmlPath);
	// for(int i=0;i<Data_L.size();i+=3)
	// {
		// printf("%.17g %.17g %.17g\n",Data_L[i],Data_L[i+1],Data_L[i+2]);
	// }
	// printf("\n");
}

void PublishMat(Mat&X)
{
	int rows=X.rows;
	int cols=X.cols;
	double*p=(double*)(X.data);
	int N=rows*cols;

	std_msgs::Float64MultiArray msg;
	msg.data.push_back(rows);
	msg.data.push_back(cols);
	for(int i=0;i<N;i++)
	{
		msg.data.push_back(p[i]);
	}
	Mat_pub.publish(msg);
}


Mat ComputePto(double Wx,double Wy,double Wz,
				double Fx,double Fy,double Fz)
{
	//���ؾ���ÿ�ж�Ӧһ��Furniture,����һ�ж�Ӧ����
	//��������Ϊk u v Valid Dx Dy Dz
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

int DealJson_FunNum=0;
void DealJson()
{
	DealJson_FunNum++;
	/*{
		//����ר��,��ȫ�ֱ��������ĵ��Ʊ�����ȥ
		ofstream Txt_Pick("Cloud_Pick.txt");
		for (int i = 0; i < Cloud_Pick.points.size(); ++i)
		{
			sprintf(Buffer, "%.17g\t%.17g\t%.17g\n", Cloud_Pick.points[i].x, Cloud_Pick.points[i].y, Cloud_Pick.points[i].z);
			Txt_Pick << Buffer;
		}
		Txt_Pick.close();

		ofstream Txt_Clean("Cloud_Clean.txt");
		for (int i = 0; i < Cloud_Clean.points.size(); ++i)
		{
			sprintf(Buffer, "%.17g\t%.17g\t%.17g\n", Cloud_Clean.points[i].x, Cloud_Clean.points[i].y, Cloud_Clean.points[i].z);
			Txt_Clean << Buffer;
		}
		Txt_Clean.close();
	}*/

	//����OpenPose���ļ����е�ͼƬ�궨��JSON�ļ�
	//�����˴�Ӧ��һ�δ����������ն���������ָ��
	//cd "~/OpenPose-master/Ubuntu/build" && "~/OpenPose-master/Ubuntu/build/OpenPose.bin" --image_dir "This is $(SaveDir)" --hand true --write_keypoint_json "This is $(SaveDir)"
	// string Json1_Path=SaveDir + "/1_keypoints.json";
	// string Json2_Path=SaveDir + "/2_keypoints.json";
	// ifstream Json1; Json1.open(&(Json1_Path[0]));
	// ifstream Json2; Json2.open(&(Json2_Path[0]));
	// while (!(Json1.is_open() && Json2.is_open()))
	// {
		// Json1.close();
		// Json2.close();
		// cout << "Wait for xml......\n";
		// cv::waitKey(1000);
		// Json1.open(&(Json1_Path[0]));
		// Json2.open(&(Json2_Path[0]));
	// }

	string xml_1=json_dir + "/pick_it_up_pose.xml";
	string xml_2=json_dir + "/pick_it_up_hand_right.xml";
	string xml_3=json_dir + "/pick_it_up_hand_left.xml";
	string xml_4=json_dir + "/clean_up_pose.xml";
	string xml_5=json_dir + "/clean_up_hand_right.xml";
	string xml_6=json_dir + "/clean_up_hand_left.xml";

	ifstream Xml_1; Xml_1.open(&(xml_1[0]));
	ifstream Xml_2; Xml_2.open(&(xml_2[0]));
	ifstream Xml_3; Xml_3.open(&(xml_3[0]));
	ifstream Xml_4; Xml_4.open(&(xml_4[0]));
	ifstream Xml_5; Xml_5.open(&(xml_5[0]));
	ifstream Xml_6; Xml_6.open(&(xml_6[0]));

	while (!(Xml_1.is_open() &&
			Xml_2.is_open() &&
			Xml_3.is_open() &&
			Xml_4.is_open() &&
			Xml_5.is_open() &&
			Xml_6.is_open()))
	{
		Xml_1.close();
		Xml_2.close();
		Xml_3.close();
		Xml_4.close();
		Xml_5.close();
		Xml_6.close();
		cout << "Wait for xml......\n";
		cv::waitKey(1000);
		Xml_1.open(&(xml_1[0]));
		Xml_2.open(&(xml_2[0]));
		Xml_3.open(&(xml_3[0]));
		Xml_4.open(&(xml_4[0]));
		Xml_5.open(&(xml_5[0]));
		Xml_6.open(&(xml_6[0]));
	}
	{
		Xml_1.close();
		Xml_2.close();
		Xml_3.close();
		Xml_4.close();
		Xml_5.close();
		Xml_6.close();
	}



	//������SaveDir�ļ������Ѿ�����1_keypoints.json��2_keypoints.json
	{
		//��ȡPick_it_up!ʱͼƬ��Ӧ��1_keypoints.json
		//��λ�ø�ԭ����ά�ռ�map����ϵ��,ȷ��Pick_it_up!ʱָ�����ĸ�λ��
		//sprintf(Buffer, "%s/1_keypoints.json", &(SaveDir[0]));
		vector<int>DataNums;
		vector<double>Data;
		vector<int>DataNums_L;
		vector<double>Data_L;
		vector<int>DataNums_R;
		vector<double>Data_R;
		//ReadJsonKeypoints(Buffer, DataNums, Data, DataNums_L, Data_L, DataNums_R, Data_R);

		char PoseXmlPath[MAX_PATH];sprintf(PoseXmlPath, "%s/pick_it_up_pose.xml", &(json_dir[0]));
		char RhandXmlPath[MAX_PATH];sprintf(RhandXmlPath, "%s/pick_it_up_hand_right.xml", &(json_dir[0]));
		char LhandXmlPath[MAX_PATH];sprintf(LhandXmlPath, "%s/pick_it_up_hand_left.xml", &(json_dir[0]));
		ReadXmlKeypoints(PoseXmlPath,RhandXmlPath,LhandXmlPath,Data,Data_R,Data_L);

		/*{
			//����ר��
			int StartIdx;
			cout << "\n\n\nPick_it_up!:\n__________________________________________" << endl;

			cout << "Pose Keypoints:" << endl;
			StartIdx = 0;
			for (int i = 0; i<DataNums.size(); i++)
			{
				for (int j = StartIdx; j<StartIdx + DataNums[i]; j = j + 3)
				{
					cout << Data[j] << "\t" << Data[j + 1] << "\t" << Data[j + 2] << "\n";
				}
				cout << "\n";
				StartIdx = StartIdx + DataNums[i];
			}

			cout << "Left Hand Keypoints:" << endl;
			StartIdx = 0;
			for (int i = 0; i<DataNums_L.size(); i++)
			{
				for (int j = StartIdx; j<StartIdx + DataNums_L[i]; j = j + 3)
				{
					cout << Data_L[j] << "\t" << Data_L[j + 1] << "\t" << Data_L[j + 2] << "\n";
				}
				cout << "\n";
				StartIdx = StartIdx + DataNums_L[i];
			}

			cout << "Right Hand Keypoints:" << endl;
			StartIdx = 0;
			for (int i = 0; i<DataNums_R.size(); i++)
			{
				for (int j = StartIdx; j<StartIdx + DataNums_R[i]; j = j + 3)
				{
					cout << Data_R[j] << "\t" << Data_R[j + 1] << "\t" << Data_R[j + 2] << "\n";
				}
				cout << "\n";
				StartIdx = StartIdx + DataNums_R[i];
			}

			cout << "__________________________________________" << endl;
		}*/

		//Mat U_Pos_GCP_ = Mat::ones(4, U_Pos_GCP.rows, CV_64F);//ÿ��һ��������[x y z 1]'
		// double*pU_Pos_GCP_ = (double*)(U_Pos_GCP_.data);//4��������
		// double*pU_Pos_GCP = (double*)(U_Pos_GCP.data);//������3��
		// for (int i = 0; i < U_Pos_GCP.rows; i++)
		// {
			// pU_Pos_GCP_[i] = pU_Pos_GCP[i * 3 + 0];
			// pU_Pos_GCP_[i + U_Pos_GCP.rows] = pU_Pos_GCP[i * 3 + 1];
			// pU_Pos_GCP_[i + U_Pos_GCP.rows*2] = pU_Pos_GCP[i * 3 + 2];
		// }
		// Mat M_Pos_GCP_ = M_U_T*U_Pos_GCP_;//ÿ�д洢GCP��Ӧ�ĵ�ͼ����ϵ������[x y z 1]'
		// double*pM_Pos_GCP_ = (double*)(M_Pos_GCP_.data);//4��������

		//��ǰʹ������Ϊ����,�յ�Ϊʳָ�ڶ�ָ�ڹ�������
		//���ȸ��������������߾���ȷ��Avatar�õ����ֻ�������
		//����Ϊ�м䲱�ӵ㡢������������ʳָ�ڶ�ָ�ڡ�������������ʳָ�ڶ�ָ��
		//��Ҫ�䶯ʱ�Ĵ˴��ĺ궨�弴��
		int Mark_Idxs[] = { round(Data[MNeckNum * 3]) + round(Data[MNeckNum * 3 + 1] - 1) * ImgWidth,
							round(Data[LWristNum * 3]) + round(Data[LWristNum * 3 + 1] - 1) * ImgWidth,
							round(Data_L[IFinSecNum * 3]) + round(Data_L[IFinSecNum * 3 + 1] - 1) * ImgWidth,
							round(Data[RWristNum * 3]) + round(Data[RWristNum * 3 + 1] - 1) * ImgWidth,
							round(Data_R[IFinSecNum * 3]) + round(Data_R[IFinSecNum * 3 + 1] - 1) * ImgWidth };
		int Hip_Idxs   =  round(Data[MidHipNum * 3]) + round(Data[MidHipNum * 3 + 1] - 1) * ImgWidth;
		//��������������������������������������������������������������������������������������
		//�˴������д��������ҷ�NaN���ֵĲ���,���������׵��¸�ԭ����ά�ռ��е���Ч��
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

		double Hip_x = Cloud_Pick.points[Hip_Idxs].x;
		double Hip_y = Cloud_Pick.points[Hip_Idxs].y;
		double Hip_z = Cloud_Pick.points[Hip_Idxs].z;

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
		if(isnan(Hip_x)||isnan(Hip_y)||isnan(Hip_z))
		{
			Hip_Idxs = FindNearValid('p',Hip_Idxs);
			Hip_x = Cloud_Pick.points[Hip_Idxs].x;
			Hip_y = Cloud_Pick.points[Hip_Idxs].y;
			Hip_z = Cloud_Pick.points[Hip_Idxs].z;
		}


		/*{
			printf("%d\n%d\n%d\n%d\n%d\n", Mark_Idxs[0], Mark_Idxs[1], Mark_Idxs[2], Mark_Idxs[3], Mark_Idxs[4]);
			//����ר��
			printf("%.17g %.17g %.17g \n%.17g %.17g %.17g \n%.17g %.17g %.17g \n%.17g %.17g %.17g \n%.17g %.17g %.17g \n",
				M_x, M_y, M_z, Lw_x, Lw_y, Lw_z, Li_x, Li_y, Li_z, Rw_x, Rw_y, Rw_z, Ri_x, Ri_y, Ri_z);
		}*/
		vector<double>Dis;
		if ((Lw_x - M_x)*(Lw_x - M_x) + (Lw_y - M_y)*(Lw_y - M_y) >
			(Rw_x - M_x)*(Rw_x - M_x) + (Rw_y - M_y)*(Rw_y - M_y))
		{
			cout << "Avatar used left hand" << endl;
			Mat Res=ComputePto(Lw_x,Lw_y,Lw_z,Li_x,Li_y,Li_z);
			PrintMat(Res,"GCP_Res");
			PublishMat(Res);
			//writeNavXml();
			cout << "\n";
		}
		else
		{
			cout << "Avatar used right hand" << endl;
			Mat Res=ComputePto(Rw_x,Rw_y,Rw_z,Ri_x,Ri_y,Ri_z);
			PrintMat(Res,"GCP_Res");
			PublishMat(Res);
			//writeNavXml();
			cout << "\n";
		}

		//��Dis��С������������GCP_Names������GCP_Names_sort��
		// for (int i = 0; i < GCP_Names.size(); i++) { GCP_Names_sort.push_back(GCP_Names[i]); }
		// Quick_sort(Dis, GCP_Names_sort, 0, GCP_Names.size() - 1);
		// string MsgStr = "";
		// for (int i = 0; i < GCP_Names.size(); i++) { MsgStr = MsgStr + GCP_Names_sort[i]+" "; }
		// cout << "GCP pos sort by dis:\n"<< MsgStr << endl << endl;
		// std_msgs::String StrMsg;
		// StrMsg.data = MsgStr;
		// TargetPos_pub.publish(StrMsg);
		cv::waitKey(1000);
	}

	{
		//��ȡClean_up!ʱͼƬ��Ӧ��2_keypoints.json
		//��λ�ø�ԭ����ά�ռ�map����ϵ��,ȷ��Clean_up!ʱָ�����ĸ�λ��
		//sprintf(Buffer, "%s/2_keypoints.json", &(SaveDir[0]));
		vector<int>DataNums;
		vector<double>Data;
		vector<int>DataNums_L;
		vector<double>Data_L;
		vector<int>DataNums_R;
		vector<double>Data_R;
		//ReadJsonKeypoints(Buffer, DataNums, Data, DataNums_L, Data_L, DataNums_R, Data_R);
		char PoseXmlPath[MAX_PATH];sprintf(PoseXmlPath, "%s/clean_up_pose.xml", &(json_dir[0]));
		char RhandXmlPath[MAX_PATH];sprintf(RhandXmlPath, "%s/clean_up_hand_right.xml", &(json_dir[0]));
		char LhandXmlPath[MAX_PATH];sprintf(LhandXmlPath, "%s/clean_up_hand_left.xml", &(json_dir[0]));
		ReadXmlKeypoints(PoseXmlPath,RhandXmlPath,LhandXmlPath,Data,Data_R,Data_L);

		/*{
			//����ר��
			int StartIdx;
			cout << "\n\n\nClean_up!:\n__________________________________________" << endl;

			cout << "Pose Keypoints:" << endl;
			StartIdx = 0;
			for (int i = 0; i<DataNums.size(); i++)
			{
				for (int j = StartIdx; j<StartIdx + DataNums[i]; j = j + 3)
				{
					cout << Data[j] << "\t" << Data[j + 1] << "\t" << Data[j + 2] << "\n";
				}
				cout << "\n";
				StartIdx = StartIdx + DataNums[i];
			}

			cout << "Left Hand Keypoints:" << endl;
			StartIdx = 0;
			for (int i = 0; i<DataNums_L.size(); i++)
			{
				for (int j = StartIdx; j<StartIdx + DataNums_L[i]; j = j + 3)
				{
					cout << Data_L[j] << "\t" << Data_L[j + 1] << "\t" << Data_L[j + 2] << "\n";
				}
				cout << "\n";
				StartIdx = StartIdx + DataNums_L[i];
			}

			cout << "Right Hand Keypoints:" << endl;
			StartIdx = 0;
			for (int i = 0; i<DataNums_R.size(); i++)
			{
				for (int j = StartIdx; j<StartIdx + DataNums_R[i]; j = j + 3)
				{
					cout << Data_R[j] << "\t" << Data_R[j + 1] << "\t" << Data_R[j + 2] << "\n";
				}
				cout << "\n";
				StartIdx = StartIdx + DataNums_R[i];
			}


		}*/

		Mat U_Pos_DCP_ = Mat::ones(4, U_Pos_DCP.rows, CV_64F);//ÿ��һ��������[x y z 1]'
		double*pU_Pos_DCP_ = (double*)(U_Pos_DCP_.data);//4��������
		double*pU_Pos_DCP = (double*)(U_Pos_DCP.data);//������3��
		for (int i = 0; i < U_Pos_DCP.rows; i++)
		{
			pU_Pos_DCP_[i] = pU_Pos_DCP[i * 3 + 0];
			pU_Pos_DCP_[i + U_Pos_DCP.rows] = pU_Pos_DCP[i * 3 + 1];
			pU_Pos_DCP_[i + U_Pos_DCP.rows * 2] = pU_Pos_DCP[i * 3 + 2];
		}



		double*pM_Pos_DCP_ = (double*)(M_Pos_DCP_.data);//4��������

														//��ǰʹ������Ϊ����,�յ�Ϊʳָ�ڶ�ָ�ڹ�������
														//���ȸ��������������߾���ȷ��Avatar�õ����ֻ�������
														//����Ϊ�м䲱�ӵ㡢������������ʳָ�ڶ�ָ�ڡ�������������ʳָ�ڶ�ָ��
														//��Ҫ�䶯ʱ�Ĵ˴��ĺ궨�弴��
		int Mark_Idxs[] = { round(Data[MNeckNum * 3]) + round(Data[MNeckNum * 3 + 1] - 1) * ImgWidth,
			round(Data[LWristNum * 3]) + round(Data[LWristNum * 3 + 1] - 1) * ImgWidth,
			round(Data_L[IFinSecNum * 3]) + round(Data_L[IFinSecNum * 3 + 1] - 1) * ImgWidth,
			round(Data[RWristNum * 3]) + round(Data[RWristNum * 3 + 1] - 1) * ImgWidth,
			round(Data_R[IFinSecNum * 3]) + round(Data_R[IFinSecNum * 3 + 1] - 1) * ImgWidth };

		int Hip_Idxs = round(Data[MidHipNum * 3]) + round(Data[MidHipNum * 3 + 1] - 1) * ImgWidth;
		//��������������������������������������������������������������������������������������
		//�˴������д��������ҷ�NaN���ֵĲ���,���������׵��¸�ԭ����ά�ռ��е���Ч��
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

		double Hip_x = Cloud_Clean.points[Hip_Idxs].x;
		double Hip_y = Cloud_Clean.points[Hip_Idxs].y;
		double Hip_z = Cloud_Clean.points[Hip_Idxs].z;

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
			Mark_Idxs[4]=FindNearValid('c',Mark_Idxs[4]);
			Ri_x = Cloud_Clean.points[Mark_Idxs[4]].x;
			Ri_y = Cloud_Clean.points[Mark_Idxs[4]].y;
			Ri_z = Cloud_Clean.points[Mark_Idxs[4]].z;
		}
		if(isnan(Hip_x)||isnan(Hip_y)||isnan(Hip_z))
		{
			Hip_Idxs = FindNearValid('c',Hip_Idxs);
			Hip_x = Cloud_Clean.points[Hip_Idxs].x;
			Hip_y = Cloud_Clean.points[Hip_Idxs].y;
			Hip_z = Cloud_Clean.points[Hip_Idxs].z;
		}
		/*{
			printf("%d\n%d\n%d\n%d\n%d\n", Mark_Idxs[0], Mark_Idxs[1], Mark_Idxs[2], Mark_Idxs[3], Mark_Idxs[4]);
			//����ר��
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
				double DCP_i_x = pM_Pos_DCP_[i];
				double DCP_i_y = pM_Pos_DCP_[i + U_Pos_DCP.rows];
				double DCP_i_z = pM_Pos_DCP_[i + U_Pos_DCP.rows * 2];

				//����WI��������W_DCPȡģ��ƽ���ı�������
				double dis2 = ((Li_y - Lw_y)*(DCP_i_z - Lw_z) - (DCP_i_y - Lw_y)*(Li_z - Lw_z))*((Li_y - Lw_y)*(DCP_i_z - Lw_z) - (DCP_i_y - Lw_y)*(Li_z - Lw_z)) +
					((DCP_i_x - Lw_x)*(Li_z - Lw_z) - (Li_x - Lw_x)*(DCP_i_z - Lw_z))*((DCP_i_x - Lw_x)*(Li_z - Lw_z) - (Li_x - Lw_x)*(DCP_i_z - Lw_z)) +
					((Li_x - Lw_x)*(DCP_i_y - Lw_y) - (DCP_i_x - Lw_x)*(Li_y - Lw_y))*((Li_x - Lw_x)*(DCP_i_y - Lw_y) - (DCP_i_x - Lw_x)*(Li_y - Lw_y));
				Dis.push_back(sqrt(dis2));

				cout << Dis[i] << " ";
			}
			cout << "\n";

			Mat Res=ComputePto(Lw_x,Lw_y,Lw_z,Li_x,Li_y,Li_z);
			PrintMat(Res,"DCP_Res");
			//writeNavXml();
		}
		else
		{
			cout << "Avatar used right hand" << endl;
			cout << "This is all dis:\n";
			for (int i = 0; i < DCP_Names.size(); i++)
			{
				double DCP_i_x = pM_Pos_DCP_[i];
				double DCP_i_y = pM_Pos_DCP_[i + U_Pos_DCP.rows];
				double DCP_i_z = pM_Pos_DCP_[i + U_Pos_DCP.rows * 2];

				//����WI��������W_DCPȡģ��ƽ���ı�������
				double dis2 = ((Ri_y - Rw_y)*(DCP_i_z - Rw_z) - (DCP_i_y - Rw_y)*(Ri_z - Rw_z))*((Ri_y - Rw_y)*(DCP_i_z - Rw_z) - (DCP_i_y - Rw_y)*(Ri_z - Rw_z)) +
					((DCP_i_x - Rw_x)*(Ri_z - Rw_z) - (Ri_x - Rw_x)*(DCP_i_z - Rw_z))*((DCP_i_x - Rw_x)*(Ri_z - Rw_z) - (Ri_x - Rw_x)*(DCP_i_z - Rw_z)) +
					((Ri_x - Rw_x)*(DCP_i_y - Rw_y) - (DCP_i_x - Rw_x)*(Ri_y - Rw_y))*((Ri_x - Rw_x)*(DCP_i_y - Rw_y) - (DCP_i_x - Rw_x)*(Ri_y - Rw_y));
				Dis.push_back(sqrt(dis2));

				cout << Dis[i] << " ";
				//cout << DCP_i_x << " " << DCP_i_y << " " << DCP_i_z << endl;//����ר��
			}
			cout << "\n";

			Mat Res=ComputePto(Rw_x,Rw_y,Rw_z,Ri_x,Ri_y,Ri_z);
			PrintMat(Res,"DCP_Res");
			//writeNavXml();
		}
		//��Dis��С������������DCP_Names������DCP_Names_sort��
		for (int i = 0; i < DCP_Names.size(); i++) { DCP_Names_sort.push_back(DCP_Names[i]); }
		Quick_sort(Dis, DCP_Names_sort, 0, DCP_Names.size() - 1);
		string MsgStr = "";
		for (int i = 0; i < DCP_Names.size(); i++) { MsgStr = MsgStr + DCP_Names_sort[i] + " "; }
		cout << "DCP pos sort by dis:\n" << MsgStr << endl << endl;
		std_msgs::String StrMsg;
		std_msgs::String str;
		StrMsg.data = MsgStr;
		TargetPos_pub.publish(StrMsg);
		int i = 1;
		//communicate with control node
		str.data = "OPAros_end";
		if(i == 1)
		{
			step_pub.publish(str);
			i++;
		}
	}
}

//����ͷ��������RGBͼƬ�Ļص�����
void imageCallback(sensor_msgs::Image msg)
{
	cout << "RECEVING IMAGES" << endl;
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
	//cv::imshow("Img",Img);
	Img_Frame=Img.clone();//����������������������������������������������������������������������������������������������������
	//cv::waitKey(10);
	if(PngIsSave)
	{
		PngIsSave = false;
		if (PngStaticNum == 1)
		{
			sprintf(Buffer,"%s/pick_it_up.png", &(png_dir[0]));
			imwrite(Buffer,Img);
			Img_Pick = Img.clone();
		}
		if (PngStaticNum == 2)
		{
			sprintf(Buffer, "%s/clean_up.png", &(png_dir[0]));
			imwrite(Buffer,Img);
			Img_Clean = Img.clone();

			//start openpose
			cout << "start openpose" <<endl;
			std_msgs::String flag;
			flag.data = "start";
			openpose_pub.publish(flag);
		}
	}
}

void openposeCallback(const std_msgs::String::ConstPtr& msg)
{
	ROS_INFO("OPENPOSE FINISHED");
	if(msg->data == "finish")
	{
		DealJson();
		std_msgs::String step_;
		step_.data = "camshift_end";
		step_pub.publish(step_);
	}
}

//����ͷ��������������Ϣ�Ļص�����
void PcloudCallback(sensor_msgs::PointCloud2 msg)
{
	// std::cout<<"I heard: "<<std::endl;
	// std::cout<<"header.seq="<<msg.header.seq<<std::endl;
	// std::cout<<"header.frame_id="<<msg.header.frame_id<<std::endl;
	// std::cout<<"height="<<msg.height<<std::endl;
	// std::cout<<"width="<<msg.width<<std::endl;
	// std::cout<<"is_bigendian="<<int(msg.is_bigendian)<<std::endl;
	// std::cout<<"point_step="<<msg.point_step<<std::endl;
	// std::cout<<"row_step="<<msg.row_step<<std::endl;
	// std::cout<<"data.size()="<<msg.data.size()<<std::endl;
	// std::cout<<"is_dense="<<int(msg.is_dense)<<std::endl<<std::endl;
	try
	{
		pListener->lookupTransform("/map", "head_rgbd_sensor_depth_frame",ros::Time(0), CurrTf);
	}
	catch (tf::TransformException &ex)
	{
		ROS_ERROR("%s",ex.what());
		ros::Duration(1.0).sleep();
	}
	tf::Vector3 P=CurrTf.getOrigin();
	tf::Matrix3x3 R=CurrTf.getBasis();

	//pcl::PointCloud<pcl::PointXYZ> cloud;
    pcl::PointCloud<pcl::PointXYZRGB> cloud;//����������������������������������������������������������������������������������������������������
    sensor_msgs::PointCloud2 oMsg;
    cloud.width  = msg.width;
    cloud.height = msg.height;
    cloud.points.resize(cloud.width * cloud.height);
	if (PtsIsSave&&PtsStaticNum == 1)
	{
		Cloud_Pick.width = msg.width;
		Cloud_Pick.height = msg.height;
		Cloud_Pick.points.resize(Cloud_Pick.width * Cloud_Pick.height);
	}
	if (PtsIsSave&&PtsStaticNum == 2)
	{
		Cloud_Clean.width = msg.width;
		Cloud_Clean.height = msg.height;
		Cloud_Clean.points.resize(Cloud_Clean.width * Cloud_Clean.height);
	}
    for (size_t i = 0; i < cloud.points.size(); ++i)
    {
		//�����е�����Ϣ�е�X,Y,Z��ȡ����
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

		//���������任�����������䵽map����ϵ������
        cloud.points[i].x = R[0][0]*X+R[0][1]*Y+R[0][2]*Z+P[0];
        cloud.points[i].y = R[1][0]*X+R[1][1]*Y+R[1][2]*Z+P[1];
        cloud.points[i].z = R[2][0]*X+R[2][1]*Y+R[2][2]*Z+P[2];

		//�ϳ���άRGB����
		cloud.points[i].r = Img_Frame.data[i*3+2];//����������������������������������������������������������������������������������������������������
		cloud.points[i].g = Img_Frame.data[i*3+1];//����������������������������������������������������������������������������������������������������
		cloud.points[i].b = Img_Frame.data[i*3];//����������������������������������������������������������������������������������������������������

		if (PtsIsSave)
		{
			if (PtsStaticNum == 1)
			{
				Cloud_Pick.points[i].x = cloud.points[i].x;
				Cloud_Pick.points[i].y = cloud.points[i].y;
				Cloud_Pick.points[i].z = cloud.points[i].z;
			}
			if (PtsStaticNum == 2)
			{
				Cloud_Clean.points[i].x = cloud.points[i].x;
				Cloud_Clean.points[i].y = cloud.points[i].y;
				Cloud_Clean.points[i].z = cloud.points[i].z;
			}
		}
    }
	pcl::toROSMsg(cloud, oMsg);
	oMsg.header.frame_id = "map";
	pcl_pub.publish(oMsg);

	if(PtsIsSave)
	{
		//sprintf(Buffer,"%s/%d.txt",&(SaveDir[0]),StaticNum);
		//ofstream Txt(Buffer);
		//for(int i=0;i<msg.data.size();i+=msg.point_step)
		//{
		//	floatBuffer[0]=msg.data[i+0];
		//	floatBuffer[1]=msg.data[i+1];
		//	floatBuffer[2]=msg.data[i+2];
		//	floatBuffer[3]=msg.data[i+3];
		//	double X=*((float*)floatBuffer);
		//
		//	floatBuffer[0]=msg.data[i+4];
		//	floatBuffer[1]=msg.data[i+5];
		//	floatBuffer[2]=msg.data[i+6];
		//	floatBuffer[3]=msg.data[i+7];
		//	double Y=*((float*)floatBuffer);
		//
		//	floatBuffer[0]=msg.data[i+8];
		//	floatBuffer[1]=msg.data[i+9];
		//	floatBuffer[2]=msg.data[i+10];
		//	floatBuffer[3]=msg.data[i+11];
		//	double Z=*((float*)floatBuffer);
		//
		//	sprintf(Buffer,"%.17g\t%.17g\t%.17g\n",X,Y,Z);
		//	Txt<<Buffer;
		//}
		//Txt.close();
		//
		//sprintf(Buffer,"%.17g\t%.17g\t%.17g\t%.17g\n" \
		//				"%.17g\t%.17g\t%.17g\t%.17g\n" \
		//				"%.17g\t%.17g\t%.17g\t%.17g\n" \
		//				"0\t0\t0\t1\n",
		//				R[0][0],R[0][1],R[0][2],P[0],
		//				R[1][0],R[1][1],R[1][2],P[1],
		//				R[2][0],R[2][1],R[2][2],P[2]);
		//cout<<Buffer;
		PtsIsSave=false;
	}
	if(DealJson_FunNum==0&&!Img_Clean.empty()&&!Cloud_Clean.empty())
	{
		DealJson();
	}
}

//����Avatarָ����Ϣ�Ļص�����
void CleanupMsgCallback(interactive_cleanup::InteractiveCleanupMsg msg)
{
	cout<<msg.detail<<"\t"<<msg.message<<endl;
	if(msg.message=="Pick_it_up!"||msg.message=="Clean_up!")
	{
		PngIsSave=true;
		PtsIsSave=true;
		PtsStaticNum++;
		PngStaticNum++;
	}
	/*if(msg.message == "Are_you_ready?")
	{
		std::cout <<"SAY I AM READY" << endl;
		interactive_cleanup::InteractiveCleanupMsg interactive_cleanup_msg;
    	interactive_cleanup_msg.message = "I_am_ready";
		Avatar_pub.publish(interactive_cleanup_msg);
		ROS_INFO("TASK START");
	}
	*/
}


int main(int argc, char **argv)
{
	ReadModelsXml(xml_dir,M_Pos_Furniture,Furniture_Names);//��ȡxml�ļ�
	int N_DCP=Furniture_Names.size()-1;//ǰ���ٸ���DCP�ļҾ�
	M_Pos_DCP_=Mat::ones(4,N_DCP,CV_64F);//ÿһ�ж���xyz1
	double*pM_Pos_DCP_=(double*)(M_Pos_DCP_.data);//ÿһ�ж���xyz xyz xyz
	double*pM_Pos_Furniture=(double*)(M_Pos_Furniture.data);
	for(int i=0;i<N_DCP;i++)
	{
		DCP_Names.push_back(Furniture_Names[i]);
		pM_Pos_DCP_[0*M_Pos_DCP_.cols+i]=(pM_Pos_Furniture[i*M_Pos_Furniture.cols+0]+pM_Pos_Furniture[i*M_Pos_Furniture.cols+6])/2;
		pM_Pos_DCP_[1*M_Pos_DCP_.cols+i]=(pM_Pos_Furniture[i*M_Pos_Furniture.cols+1]+pM_Pos_Furniture[i*M_Pos_Furniture.cols+7])/2;
		pM_Pos_DCP_[2*M_Pos_DCP_.cols+i]=(pM_Pos_Furniture[i*M_Pos_Furniture.cols+2]+pM_Pos_Furniture[i*M_Pos_Furniture.cols+8])/2;
	}

	ros::init(argc, argv, "ListenerKingNew_Xml");
	cout << "-----------ListenerKingNew_Xml online-----------" << endl;
	cout << "---------waiting for OpenPose message----------" << endl;

	ros::NodeHandle n;
	sub_PtCloud     = n.subscribe("/hsrb/head_rgbd_sensor/depth/points", 1, PcloudCallback);
	sub_Cup         = n.subscribe("/interactive_cleanup/message/to_robot", 1, CleanupMsgCallback);
	sub_Img         = n.subscribe("/hsrb/head_rgbd_sensor/rgb/image_raw", 1, imageCallback);
	sub_Openpose    = n.subscribe("/finish_openpose", 1, openposeCallback);

	pcl_pub         = n.advertise<sensor_msgs::PointCloud2>("map_points", 1);
	TargetPos_pub   = n.advertise<std_msgs::String>("TargetPosName", 1);
	openpose_pub    = n.advertise<std_msgs::String>("/start_openpose", 1);
	nav_pub         = n.advertise<std_msgs::String>("/OPAros2nav", 1);
	step_pub        = n.advertise<std_msgs::String>("/task_control", 1);
	Avatar_pub      = n.advertise<interactive_cleanup::InteractiveCleanupMsg>("/interactive_cleanup/message/to_moderator", 1);
	Mat_pub         = n.advertise<std_msgs::Float64MultiArray>("/OPAros/GCPRes", 1);

	tf::TransformListener Listener;
	pListener = &Listener;

	ros::spin();
	return 0;
}
