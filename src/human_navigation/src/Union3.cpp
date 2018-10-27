#include <ros/ros.h>
#include <human_navigation/HumanNaviObjectInfo.h>
#include <human_navigation/HumanNaviDestination.h>
#include <human_navigation/HumanNaviTaskInfo.h>
#include <human_navigation/HumanNaviMsg.h>
#include <human_navigation/HumanNaviGuidanceMsg.h>
#include <human_navigation/HumanNaviAvatarStatus.h>
#include <human_navigation/HumanNaviObjectStatus.h>
#include <math.h>
#include <string>


using namespace std;

class HumanNavigationSample
{
private:
	////////ZLH//start/////
	bool isMoveDirection = false;
	int directionCount = 0;
	////////ZLH//end///////
	enum Step
	{
		Initialize,
		Ready,
		WaitTaskInfo,
		GuideForTakingObject,
		GuideForPlacement,
		WaitTaskFinished,
		TaskFinished
	};

	enum class SpeechState
	{
		None,
		WaitingState,
		Speaking,
		Speakable
	};

	// human navigation message from/to the moderator
	const std::string MSG_ARE_YOU_READY      = "Are_you_ready?";
	const std::string MSG_TASK_SUCCEEDED     = "Task_succeeded";
	const std::string MSG_TASK_FAILED        = "Task_failed";
	const std::string MSG_TASK_FINISHED      = "Task_finished";
	const std::string MSG_GO_TO_NEXT_SESSION = "Go_to_next_session";
	const std::string MSG_MISSION_COMPLETE   = "Mission_complete";
	const std::string MSG_REQUEST            = "Guidance_request";
	const std::string MSG_SPEECH_STATE       = "Speech_state";
	const std::string MSG_SPEECH_RESULT      = "Speech_result";

	const std::string MSG_I_AM_READY        = "I_am_ready";
	const std::string MSG_GET_AVATAR_STATUS = "Get_avatar_status";
	const std::string MSG_GET_OBJECT_STATUS = "Get_object_status";
	const std::string MSG_GET_SPEECH_STATE  = "Get_speech_state";

	// display type of guidance message panels for the avatar (test subject)
	const std::string DISPLAY_TYPE_ALL         = "All";
	const std::string DISPLAY_TYPE_ROBOT_ONLY  = "RobotOnly";
	const std::string DISPLAY_TYPE_AVATAR_ONLY = "AvatarOnly";
	const std::string DISPLAY_TYPE_NONE        = "None";

	int step;
	SpeechState speechState;

	bool isStarted;
	bool isFinished;

	bool isTaskInfoReceived;
	bool isRequestReceived;

	ros::Time timePrevSpeechStateConfirmed;

	bool isSentGetAvatarStatus;
	bool isSentGetObjectStatus;

	//自己加的，判断是否到达家具附近
	bool isArrived;//寻找目标时用

	bool isCatched;//寻找目标时用
	
	bool isNear;//寻找目的地时用

	bool isCaught;//寻找目的地时用

	

	human_navigation::HumanNaviTaskInfo taskInfo;
	std::string guideMsg;

	human_navigation::HumanNaviAvatarStatus avatarStatus;
	human_navigation::HumanNaviObjectStatus objectStatus;



	////////////////////////////////////////ZLH//start////////////////////////////////////////////////
	double offsetAngel= 3.1415926535/4;//45度
	double nearDistance = 1;
	double targetDistance ;
	double leftAngel;
	double rightAngel;
	double leftDistance;
	double rightDistance;
	double headToTarget[2];

	bool isOnTheWay = true;
	bool isStart = false;

	//bool isSentGetAvatarStatus = false;

	bool turnLeft = false;
	bool turnRight = false;
	bool turnFront = false;
	bool turnBack = false;

	double nearTargetDistance = 0.15;
	bool moveLeftHand = false;
	bool moveRightHand = false;
	bool moveHandLeft = false;
	bool moveHandRight = false;
	bool moveHandUp = false;
	bool moveHandDown = false;
	bool moveHandFront = false;
	bool moveHandBack = false;
	double offsetHight = 0.3;

	int directionInt = 0;
	string directionString ;
	bool stopMove = false;

	bool LeftHandCatch = false;
	bool RightHandCatch = false;

	bool isGoDestination = false;
	double destinationSizeArea;
	bool targetInLeftHand = false;

	int myNumOfNonTargetObjects;
	
	int myFurniCount ;
	bool isLeftMayWrong = false;
	bool isRightMayWrong = false;

	bool isGoDestinationFirstSpeak = true;

	class player
	{
		public:
		double head[3] ;
		double chest[3];
		double lefthand[3] ;
		double righthand[3] ;
		double towards[3];
		double chestToTarget[3];
		double chestToLeft[2];
		double chestToRight[2];
		bool is_target_object_in_left_hand;
	    bool is_target_object_in_right_hand;
	    string object_in_left_hand;
	    string object_in_right_hand;
	};

	enum MoveStep
	{
		moveDirectionReady,
		goRight,
		goWrong,
		moveWhitchHand,
		moveHand,
		canCatch,
		catchWrong,
		catchRight,
		goRightToDestination,
		goWrongToDestination,
		canPut,
		putRight,
		putWrong,
		mayCatchWrong
	};

	class nonObject
	{
	public:
		string name;
		double x;
		double y;
		double z;
	};

	class myFurni
	{
public:
string name;
double x;
double y;
		double z;
	};

	nonObject myObject[100];

	myFurni myfurni[100];	
	
	int moveStep;

	player Avatar;

	bool isNearTheTarget ;

	double targetFuniture[3] ;//0:x   1:y   2:z

	double targetObject[3] ;//0:x   1:y   2:z

	double destinationFuniture[3];

	double destinationSize[3];
	
	//human_navigation::HumanNaviTaskInfo taskInfo;
	
	//human_navigation::HumanNaviAvatarStatus avatarStatus;

	string targetObjectName;

	string targetFunitureName;
	////////////////////////////////////////ZLH//end///////////////////////////////////////////////



	void init()
	{
		step = Initialize;
		speechState = SpeechState::None;

		reset();
	}

	void reset()
	{
		isStarted             = false;
		isFinished            = false;
		isTaskInfoReceived    = false;
		isRequestReceived     = false;
		isSentGetAvatarStatus = false;
		isSentGetObjectStatus = false;
		isArrived			  = false;
		isCatched			  = false;
		isNear 				  = false;
		isCaught			  = false;
		furnitureString       = " ";
		destinationString     = " ";
		t_object              = " ";
		directionCount = 0;

		////////////////////////////////////////ZLH//start////////////////////////////////////////////////
		 
		 isMoveDirection = false;
		 moveStep = moveDirectionReady;
		 isOnTheWay = true;
		 isStart = false;

	
		 turnLeft = false;
		 turnRight = false;
		 turnFront = false;
		 turnBack = false;

		 nearTargetDistance = 0.1;
		 moveLeftHand = false;
		 moveRightHand = false;
		 moveHandLeft = false;
		 moveHandRight = false;
		 moveHandUp = false;
		 moveHandDown = false;
		 moveHandFront = false;
		 moveHandBack = false;
		 directionInt = 0;
		 directionString = " ";
		furnitureString = " ";

		 stopMove = false;

		 LeftHandCatch = false;
		 RightHandCatch = false;

		 isGoDestination = false;
		 targetInLeftHand = false;

		 isLeftMayWrong = false;
		 isRightMayWrong = false;
		 myNumOfNonTargetObjects = 0;
		 myFurniCount = 0;
		 isGoDestinationFirstSpeak = true;
		 //if(myObject != NULL)
		 //{
			//delete []myObject;
		 //}
	 	////////////////////////////////////////ZLH//end////////////////////////////////////////////////
	}

	// send humanNaviMsg to the moderator (Unity)
	void sendMessage(ros::Publisher &publisher, const std::string &message)
	{
		human_navigation::HumanNaviMsg human_navi_msg;
		human_navi_msg.message = message;
		publisher.publish(human_navi_msg);

		//ROS_INFO("Send message:%s", message.c_str());
	}

	void sendGuidanceMessage(ros::Publisher &publisher, const std::string &message, const std::string displayType)
	{
		human_navigation::HumanNaviGuidanceMsg guidanceMessage;
		guidanceMessage.message = message;
		guidanceMessage.display_type = displayType;
		publisher.publish(guidanceMessage);

		speechState = SpeechState::Speaking;

		ROS_INFO("Send guide message: %s : %s", guidanceMessage.message.c_str(), guidanceMessage.display_type.c_str());
	}


	// receive humanNaviMsg from the moderator (Unity)
	void messageCallback(const human_navigation::HumanNaviMsg::ConstPtr& message)
	{
		ROS_INFO("Subscribe message: %s : %s", message->message.c_str(), message->detail.c_str());

		if(message->message==MSG_ARE_YOU_READY)
		{
			isStarted = true;
		}
		else if(message->message==MSG_REQUEST)
		{
			if(isTaskInfoReceived && !isFinished)
			{
				isRequestReceived = true;
			}
		}
		else if(message->message==MSG_TASK_SUCCEEDED)
		{
		}
		else if(message->message==MSG_TASK_FAILED)
		{
		}
		else if(message->message==MSG_TASK_FINISHED)
		{
			isFinished = true;
		}
		else if(message->message==MSG_GO_TO_NEXT_SESSION)
		{
			ROS_INFO("Go to next session");
			isMoveDirection = false;
			step = Initialize;
		}
		else if(message->message==MSG_MISSION_COMPLETE)
		{
			//exit(EXIT_SUCCESS);
		}
		else if(message->message==MSG_SPEECH_STATE)
		{
			if(message->detail=="Is_speaking")
			{
				speechState = SpeechState::Speaking;
			}
			else
			{
				speechState = SpeechState::Speakable;
			}
		}
		else if(message->message==MSG_SPEECH_RESULT)
		{
			ROS_INFO("Speech result: %s", message->detail.c_str());
		}
	}

	// receive taskInfo from the moderator (Unity)
	void taskInfoMessageCallback(const human_navigation::HumanNaviTaskInfo::ConstPtr& message)
	{
		taskInfo = *message;

		ROS_INFO_STREAM(
			"Subscribe task info message:" << std::endl <<
			"Environment ID: " << taskInfo.environment_id << std::endl <<
			"Target object: " << std::endl << taskInfo.target_object <<
			"Destination: " << std::endl << taskInfo.destination
		);

		int numOfNonTargetObjects = taskInfo.non_target_objects.size();
		std::cout << "Number of non-target objects: " << numOfNonTargetObjects << std::endl;
		std::cout << "Non-target objects:" << std::endl;
		for(int i=0; i<numOfNonTargetObjects; i++)
		{
			std::cout << taskInfo.non_target_objects[i] << std::endl;
		}

		int numOfFurniture = taskInfo.furniture.size();
		std::cout << "Number of furniture: " << numOfFurniture << std::endl;
		std::cout << "Furniture objects:" << std::endl;
		for(int i=0; i<numOfFurniture; i++)
		{
			std::cout << taskInfo.furniture[i] << std::endl;
		}

		isTaskInfoReceived = true;

		//////////////////////////////////////ZLH//START///////////////////////////////////////////
		myNumOfNonTargetObjects = numOfNonTargetObjects;
		myFurniCount = numOfFurniture;
		//myObject = new nonObject[myNumOfNonTargetObjects];
		for(int i=0;i<myNumOfNonTargetObjects;i++)
		{
			myObject[i].name = taskInfo.non_target_objects[i].name;
			myObject[i].x = taskInfo.non_target_objects[i].position.x;
			myObject[i].y = taskInfo.non_target_objects[i].position.y;
			myObject[i].z = taskInfo.non_target_objects[i].position.z;
		}
		string describe=" ";
		for(int i=0;i<myFurniCount;i++)
		{
			if(taskInfo.furniture[i].name.find("chair_")!=std::string::npos)
			{
				describe = " chair";
			}
			//myfurni[i].name = taskInfo.furniture[i].name;
			myfurni[i].name = describe;
			myfurni[i].x = taskInfo.furniture[i].position.x;
			myfurni[i].y = taskInfo.furniture[i].position.y;
			myfurni[i].z = taskInfo.furniture[i].position.z;
		}

		targetObject[0] = taskInfo.target_object.position.x;
		targetObject[1] = taskInfo.target_object.position.y;
		targetObject[2] = taskInfo.target_object.position.z;

		destinationFuniture[0] = taskInfo.destination.position.x;
		destinationFuniture[1] = taskInfo.destination.position.y;
		destinationFuniture[2] = taskInfo.destination.position.z;

		destinationSize[0] = fabs(taskInfo.destination.size.x);
		destinationSize[1] = fabs(taskInfo.destination.size.y);
		destinationSize[2] = taskInfo.destination.size.z;

		if(destinationSize[0]<=destinationSize[1])
		{
			destinationSizeArea = destinationSize[0]/2;
		}
		else
		{
			destinationSizeArea = destinationSize[1]/2;
		}
		moveStep = goWrong;
		//////////////////////////////////////ZLH//END/////////////////////////////////////////////

	}

	void avatarStatusMessageCallback(const human_navigation::HumanNaviAvatarStatus::ConstPtr& message)
	{
		avatarStatus = *message;

		/*ROS_INFO_STREAM(
			"Subscribe avatar status message:" << std::endl <<
			"Head: " << std::endl << avatarStatus.head << 
			"LeftHand: " << std::endl << avatarStatus.left_hand << 
			"rightHand: " << std::endl << avatarStatus.right_hand << std::endl
			//"objctInLeftHand: " << avatarStatus.object_in_left_hand << std::endl <<
			//"objectInRightHand: " << avatarStatus.object_in_right_hand << std::endl <<
			//"isTargetObjectInLeftHand: " << std::boolalpha << (bool)avatarStatus.is_target_object_in_left_hand << std::endl <<
			//"isTargetObjectInRightHand: " << std::boolalpha << (bool)avatarStatus.is_target_object_in_right_hand << std::endl
		);*/

		/*ROS_INFO_STREAM(
			"左手："<< std::boolalpha << (bool)avatarStatus.is_target_object_in_left_hand << std::endl <<
			"右手："<< std::boolalpha << (bool)avatarStatus.is_target_object_in_right_hand << std::endl
			);*/
			////////////////////////////////////////ZLH//start///////////////////////////////////////////////
			
			//head		
			Avatar.head[0] = avatarStatus.head.position.x;
			Avatar.head[1] = avatarStatus.head.position.y;
			Avatar.head[2] = avatarStatus.head.position.z;

			//chest
			Avatar.chest[0] = avatarStatus.body.position.x;
			Avatar.chest[1] = avatarStatus.body.position.y;
			Avatar.chest[2] = avatarStatus.body.position.z;


			//lefthand
			Avatar.lefthand[0] = avatarStatus.left_hand.position.x;
			Avatar.lefthand[1] = avatarStatus.left_hand.position.y;
			Avatar.lefthand[2] = avatarStatus.left_hand.position.z;

			//righthand
			Avatar.righthand[0] = avatarStatus.right_hand.position.x;
			Avatar.righthand[1] = avatarStatus.right_hand.position.y;
			Avatar.righthand[2] = avatarStatus.right_hand.position.z;			


			//double chestToLeft[2];
			//double chestToRight[2];

			Avatar.chestToLeft[0] = avatarStatus.left_hand.position.x - avatarStatus.body.position.x ;
			Avatar.chestToLeft[1] = avatarStatus.left_hand.position.y - avatarStatus.body.position.y ;

			Avatar.chestToRight[0] = avatarStatus.right_hand.position.x - avatarStatus.body.position.x ;
			Avatar.chestToRight[1] = avatarStatus.right_hand.position.y - avatarStatus.body.position.y ;

			//将左右向量转化成单位向量
		    double length = sqrt(pow(Avatar.chestToLeft[0],2)+pow(Avatar.chestToLeft[1],2));
			Avatar.chestToLeft[0] = Avatar.chestToLeft[0]/length;	
			Avatar.chestToLeft[1] = Avatar.chestToLeft[1]/length;			
			
			length = sqrt(pow(Avatar.chestToRight[0],2)+pow(Avatar.chestToRight[1],2));
			Avatar.chestToRight[0] = Avatar.chestToRight[0]/length;
			Avatar.chestToRight[1] = Avatar.chestToRight[1]/length;
			
			Avatar.towards[0] = Avatar.chestToLeft[0] + Avatar.chestToRight[0];
			Avatar.towards[1] = Avatar.chestToLeft[1] + Avatar.chestToRight[1];
			//Avatar.towards[2] = temp.z;
		
			
		
			Avatar.is_target_object_in_left_hand = avatarStatus.is_target_object_in_left_hand;
			Avatar.is_target_object_in_right_hand = avatarStatus.is_target_object_in_right_hand;
		
			Avatar.object_in_left_hand = avatarStatus.object_in_left_hand;
			Avatar.object_in_right_hand = avatarStatus.object_in_right_hand;
			////////////////////////////////////////ZLH//end///////////////////////////////////////////////

		isSentGetAvatarStatus = false;
		//cout<<"Avatar"<<endl;
	}

	void objectStatusMessageCallback(const human_navigation::HumanNaviObjectStatus::ConstPtr& message)
	{
		objectStatus = *message;

		ROS_INFO_STREAM(
			"Subscribe object status message:" << std::endl <<
			"Target object: " << std::endl << taskInfo.target_object
		);

		int numOfNonTargetObjects = taskInfo.non_target_objects.size();
		std::cout << "Number of non-target objects: " << numOfNonTargetObjects << std::endl;
		std::cout << "Non-target objects:" << std::endl;
		for(int i=0; i<numOfNonTargetObjects; i++)
		{
			std::cout << taskInfo.non_target_objects[i] << std::endl;
		}

		isSentGetObjectStatus = false;
	}

	bool speakGuidanceMessage(ros::Publisher pubHumanNaviMsg, ros::Publisher pubGuidanceMsg, std::string message, int interval = 1)
	{	
		if(speechState == SpeechState::Speakable)
		{	
			sendGuidanceMessage(pubGuidanceMsg, message, DISPLAY_TYPE_ALL);
			speechState = SpeechState::None;
			directionCount++;
			return true;
		}
		else if(speechState == SpeechState::None || speechState == SpeechState::Speaking)
		{	
			//cout<<"1:00000000000"<<endl;
			if(timePrevSpeechStateConfirmed.sec + interval < ros::Time::now().sec)
			{	
				//cout<<"1:1111111111"<<endl;
				sendMessage(pubHumanNaviMsg, MSG_GET_SPEECH_STATE);
				timePrevSpeechStateConfirmed = ros::Time::now();
				speechState = SpeechState::WaitingState;
			}
		}
		//cout<<"1:2222222222"<<endl;
		return false;
	}


	//以下函数为自己添加
	bool in_something(double locate[3], double room[4])
	{
		if ((locate[0] > room[2] && locate[0]<room[0]) && (locate[2]>room[3] && locate[2] < room[1]))
			return true;
		else
		{
			return false;
		}
	}


	//判断是否在某圆形物体内
	bool in_something_round(double locate[3], double location[5])
	{
		double r = location[3];
		double a[2];
		a[0] = location[0];
		a[1] = location[2];
		
		double distance;
		distance = pow(locate[0] - a[0], 2) + pow(locate[2] - a[1],2);
		distance = pow(distance,0.5);



		if(distance < r)
			return true;
		else
			return false;
	}

	//自己加的函数，用于判断人和目标物体的距离
	bool arrived(double target_locate[3], double head_locate[3])
	{
		double distance;
		distance = pow(target_locate[0] - head_locate[0], 2) + pow(target_locate[2] - head_locate[2] , 2);
		distance = pow(distance,0.5);


		if(distance < 1)
			return true;
		else
			return false;
	}



	//识别某个家具并说出指示语句
	string furniture_sentence_out(string a, human_navigation::HumanNaviObjectInfo target)
	{
		string describe;
		if(target.name.find("pink_cup")!=std::string::npos)
		{
			describe = " pink cup";
		}
		if(target.name.find("empty_plastic_bottle")!=std::string::npos)
		{
			describe = " empty plastic bottle";
		}
		if(target.name.find("pepper_01")!=std::string::npos)
		{
			describe = "brown pepper box";
		}
		if(target.name.find("coffee_press_01")!=std::string::npos)
		{
			describe = " coffee press";
		}
		if(target.name.find("plastic_bottle_01")!=std::string::npos)
		{
			describe = " full plastic bottle";
		}


		if(target.name.find("apple")!=std::string::npos)
		{
			describe = " apple";
		}

		if(target.name.find("banana")!=std::string::npos)
		{
			describe = " banana";
		}

		if(target.name.find("cannedjuice_200ml")!=std::string::npos)
		{
			describe = " little green canned juice";
		}

		if(target.name.find("cannedjuice_350ml")!=std::string::npos)
		{
			describe = " large green canned juice";
		}

		if(target.name.find("cigarette")!=std::string::npos)
		{
			describe = " blue box of cigarettes";
		}

		if(target.name.find("coffee_press")!=std::string::npos)
		{
			describe = " coffee press";
		}

		if(target.name.find("cup_c01")!=std::string::npos)
		{
			describe = " white cup";
		}

		if(target.name.find("cup_c02")!=std::string::npos)
		{
			describe = " pink cup";
		}

		if(target.name.find("cup_c02")!=std::string::npos)
		{
			describe = " pink cup";
		}

		if(target.name.find("dish_A_curry")!=std::string::npos)
		{
			describe = " dish of curry";
		}

		if(target.name.find("dish_A_spaghetti")!=std::string::npos)
		{
			describe = " dish of spaghetti";
		}

		if(target.name.find("doll_bear")!=std::string::npos)
		{
			describe = " door bear";
		}

		if(target.name.find("doll_dog")!=std::string::npos)
		{
			describe = " door dog";
		}

		if(target.name.find("doll_bear")!=std::string::npos)
		{
			describe = " door bear";
		}

		if(target.name.find("doll_rabbit")!=std::string::npos)
		{
			describe = " door rabbit";
		}

		if(target.name.find("fork_A")!=std::string::npos)
		{
			describe = " fork";
		}

		if(target.name.find("fryingpan")!=std::string::npos)
		{
			describe = " frying pan";
		}

		if(target.name.find(" ketchup_full")!=std::string::npos)
		{
			describe = " red full bottle of ketchup";
		}


		if(target.name.find("kettle")!=std::string::npos)
		{
			describe = " kettle";
		}

		if(target.name.find("mayonaise_empty")!=std::string::npos)
		{
			describe = " empty bottle of mayonnaise";
		}
		

		if(target.name.find("mayonaise_full")!=std::string::npos)
		{
			describe = " full bottle of mayonnaise";
		}

		if(target.name.find("nursingbottle")!=std::string::npos)
		{
			describe = " nursing bottle";
		}

		if(target.name.find("orange")!=std::string::npos)
		{
			describe = " orange";
		}

		if(target.name.find("pepper_full")!=std::string::npos)
		{
			describe = " brown pepper can";
		}

		

		if(target.name.find("petbottle_2l_empty_c01")!=std::string::npos)
		{
			describe = " two liter empty pet bottle";
		}

		if(target.name.find("petbottle_2l_full_c01")!=std::string::npos)
		{
			describe = " two liter full pet bottle";
		}

		if(target.name.find("petbottle_500ml_empty_c01")!=std::string::npos)
		{
			describe = " five hundrend milliliter empty pet bottle";
		}

		if(target.name.find("petbottle_500ml_full_c01")!=std::string::npos)
		{
			describe = " five hundrend milliliter full pet bottle";
		}

		if(target.name.find("salt_full")!=std::string::npos)
		{
			describe = " white can of salt";
		}

		if(target.name.find("sauce_full")!=std::string::npos)
		{
			describe = " black bottle of sauce";
		}

		if(target.name.find("soysauce_full")!=std::string::npos)
		{
			describe = " black pot of soysauce";
		}

		if(target.name.find("spoon_A")!=std::string::npos)
		{
			describe = " spoon";
		}

		if(target.name.find("sugar")!=std::string::npos)
		{
			describe = " white can of sugar";
		}

		if(target.name.find("toy_duck")!=std::string::npos)
		{
			describe = " toy duck";
		}


		if(target.name.find("toy_penguin")!=std::string::npos)
		{
			describe = " toy penguin";
		}


		if(target.name.find("tumblerglass_c02")!=std::string::npos)
		{
			describe = " big blue glass";
		}

		if(target.name.find("tumiki_car")!=std::string::npos)
		{
			describe = " toy car";
		}
		





		t_object = describe;

		string outstc = "Our target object is a" + describe + ". ";
		if(a == " ")
		{
			outstc = outstc + "Please go to find where the target furniture is. ";
		}
		else
		{
			outstc = outstc + "It is " + a + ".  " + "Please go to find the " + furnitureString + ". ";
		}
		outstc = outstc+"Before you pick something up please put your hand above the object and I'll tell you if it is the right object. ";
		return outstc;
	}

	string location_sentence_out(string a, string b,string c)
	{
		string output;

		output = "Now you have found the " + furnitureString + ". ";

		if(a == " "&& b == " "&& c == " ")
		{
			
			output = output + "I can't find the specific location of the target. ";
		}
		else if(b == " "&& c == " ")
		{
		
			output = output + "The" + t_object + " is " + a  + ". ";
		}
		else if(c == " ")
		{
			
			output = output + "The" + t_object +  " is " + b + ". ";
		}
		else
		{
		
			output = output + "The" + t_object + " is " + b + " and " + c + ". ";
		}


		output = output + "  When you put your hand on the right object we will remind you. ";

		return output;
	}

	string destinationSentenceOut(string a , string b , string c)
	{
		string output;

		output = "Now you have got the" + t_object + ". ";
		if (a == " "&& b == " "&&c == " ")
		{
			output = output + "I can't find the specific location of the destination. ";
		}
		else if (b == " "&& c == " ")
		{
			output = output +"Please put it " + a;
		}
		else if (c == " ")
		{
			output = output +"Please put it " + b;
		}
		else
		{
			output = output + "Please put it " + b + ", "+c;
		}


		return output;
	}

	string destinationArrivedOut(string a, string b, string c)
	{
		string output;

		output = "Now you can see the " + destinationString + ". ";

		if (a == " "&& b == " "&&c == " ")
		{
			output = output + "I can't find the specific location of the destination. ";
		}
		else if (b == " "&& c == " ")
		{
			output = output +"Please put it " + a;
		}
		else if (c == " ")
		{
			output = output +"Please put it " + b;
		}
		else
		{
			output = output + "Please put it " + b + ", "+c;
		}


		return output;
	}


public:


	string furnitureString = "hhhhhhh ";
	string destinationString;
	string t_object;

	////////////////////////////////////////ZLH//start////////////////////////////////////////////////

	//判断Avator是否在正确的方向上 偏移角不超过


	bool is_on_the_way()
	{
		Avatar.chestToTarget[0] = targetObject[0] - Avatar.chest[0];
		Avatar.chestToTarget[1] = targetObject[1] - Avatar.chest[1];
		double v1 = sqrt(pow(Avatar.chestToTarget[0],2)+pow(Avatar.chestToTarget[1],2));
		double v2 = sqrt(pow(Avatar.towards[0],2)+pow(Avatar.towards[1],2));
		double offsetAngelNew = acos((Avatar.towards[0]*Avatar.chestToTarget[0]+Avatar.towards[1]*Avatar.chestToTarget[1])/v1*v2);
		if(offsetAngelNew<=offsetAngel)
		{
			isOnTheWay = true;
			moveStep = 	goRight;
		}
		else
		{
			isOnTheWay = false;
			moveStep = goWrong;
		}//roslaunch human_navigation Union.launch

		return isOnTheWay;
	}



	//计算头与目标物体的距离（只看XOY平面），距离小于nearDistance判定为“近”
	bool is_near()
	{
		targetDistance = sqrt(pow(Avatar.head[0]-targetObject[0],2)+pow(Avatar.head[1]-targetObject[1],2));
		if(targetDistance<=nearDistance)
		{
			isNearTheTarget = true;
			//isMoveDirection = false;
			moveStep = moveWhitchHand;//可以开始移手了
		}
		else//又走错了
		{
			isNearTheTarget = false;
			moveStep = goWrong;
		}	
		return isNearTheTarget;
	}


	//Avatar与目标较远时，给出目标在左还是在右的指示(若is_near()==false,则循环使用这个函数)
	void left_or_right()
	{
		
		//计算距两手距离，大致判断左右
		leftDistance = sqrt(pow(Avatar.lefthand[0]-targetObject[0],2)+pow(Avatar.lefthand[1]-targetObject[1],2));
		rightDistance = sqrt(pow(Avatar.righthand[0]-targetObject[0],2)+pow(Avatar.righthand[1]-targetObject[1],2));
		//cout<<"左距离"<<leftDistance<<endl;
		bool leftSide = false;
		bool rightSide = false;
		if(leftDistance<rightDistance)
		{
			leftSide = true;
			rightSide = false;
		}
		else
		{
			leftSide = false;
			rightSide = true;
		}

		
		//计算向量点积
	
		Avatar.chestToTarget[0] = targetObject[0] - Avatar.chest[0];
		Avatar.chestToTarget[1] = targetObject[1] - Avatar.chest[1];


		
		double v1 = sqrt(pow(Avatar.chestToTarget[0],2)+pow(Avatar.chestToTarget[1],2));
		double v2 = sqrt(pow(Avatar.towards[0],2)+pow(Avatar.towards[1],2));
		double offsetAngelNew = acos((Avatar.towards[0]*Avatar.chestToTarget[0]+Avatar.towards[1]*Avatar.chestToTarget[1])/(v1*v2));
				
		//cout<<"夹角"<<180*(offsetAngelNew/3.1415926535)<<endl;
		offsetAngelNew = 180*(offsetAngelNew/3.1415926535);
		if(leftSide)
		{
			if(offsetAngelNew<15)//12
			{
				directionInt = 12;
				directionString = "12 ";
			}
			if((offsetAngelNew>=15)&&(offsetAngelNew<45))//11
			{
				directionInt = 11;
				directionString = "11 ";
			}
			if((offsetAngelNew>=45)&&(offsetAngelNew<75))//10
			{
				directionInt = 10;
				directionString = "10 ";
				//cout<<"10的位置"<<directionString<<endl;
			}
			if((offsetAngelNew>=75)&&(offsetAngelNew<105))//9
			{
				directionInt = 9;
				directionString = "9 ";
			}
			if((offsetAngelNew>=105)&&(offsetAngelNew<135))//8
			{
				directionInt = 8;
				directionString = "8 ";
			}
			if((offsetAngelNew>=135)&&(offsetAngelNew<165))//7
			{
				directionInt = 7;
				directionString = "7 ";
			}
			if((offsetAngelNew>=165)&&(offsetAngelNew<180))//6
			{
				directionInt = 6;
				directionString = "6 ";
				//cout<<"6的位置"<<directionString<<endl;
			}

		}


		else
		{
		
			if(offsetAngelNew<15)//12
			{
				directionInt = 12;
				directionString = "12 ";
			}
			if((offsetAngelNew>=15)&&(offsetAngelNew<45))//1
			{
				directionString = "1 ";
				directionInt = 1;
			}
			if((offsetAngelNew>=45)&&(offsetAngelNew<75))//2
			{
				directionInt = 2;
				directionString = "2 ";
			}
			if((offsetAngelNew>=75)&&(offsetAngelNew<105))//3
			{
				directionInt = 3;
				directionString = "3 ";
			}
			if((offsetAngelNew>=105)&&(offsetAngelNew<135))//4
			{
				directionInt = 4;
				directionString = "4 ";
			}
			if((offsetAngelNew>=135)&&(offsetAngelNew<165))//5
			{
				directionString = "5 ";
				directionInt = 5;
			}
			if((offsetAngelNew>=165)&&(offsetAngelNew<=180))//6
			{
				directionString = "6 ";
				directionInt = 6;
			}

		}
/*
	
		if(leftSide)
		{
			if(offsetAngelNew<(15/180)*3.1415926535)//12
			{
				directionInt = 12;
				directionString = "12";
			}
			if((offsetAngelNew>=(15/180)*3.1415926535)&&(offsetAngelNew<(45/180)*3.1415926535))//11
			{
				directionInt = 11;
				directionString = "11";
			}
			if((offsetAngelNew>=(45/180)*3.1415926535)&&(offsetAngelNew<(75/180)*3.1415926535))//10
			{
				directionInt = 10;
				directionString = "10";
				cout<<"10的位置"<<directionString<<endl;
			}
			if((offsetAngelNew>=(75/180)*3.1415926535)&&(offsetAngelNew<(105/180)*3.1415926535))//9
			{
				directionInt = 9;
				directionString = "9";
			}
			if((offsetAngelNew>=(105/180)*3.1415926535)&&(offsetAngelNew<(135/180)*3.1415926535))//8
			{
				directionInt = 8;
				directionString = "8";
			}
			if((offsetAngelNew>=(135/180)*3.1415926535)&&(offsetAngelNew<(165/180)*3.1415926535))//7
			{
				directionInt = 7;
				directionString = "7";
			}
			if((offsetAngelNew>=(165/180)*3.1415926535)&&(offsetAngelNew<(180/180)*3.1415926535))//6
			{
				directionInt = 6;
				directionString = "6";
				cout<<"6的位置"<<directionString<<endl;
			}

		}


		else
		{
		
			if(offsetAngelNew<(15/180)*3.1415926535)//12
			{
				directionInt = 12;
				directionString = "12";
			}
			if((offsetAngelNew>=(15/180)*3.1415926535)&&(offsetAngelNew<(45/180)*3.1415926535))//1
			{
				directionString = "1";
				directionInt = 1;
			}
			if((offsetAngelNew>=(45/180)*3.1415926535)&&(offsetAngelNew<(75/180)*3.1415926535))//2
			{
				directionInt = 2;
				directionString = "2";
			}
			if((offsetAngelNew>=(75/180)*3.1415926535)&&(offsetAngelNew<(105/180)*3.1415926535))//3
			{
				directionInt = 3;
				directionString = "3";
			}
			if((offsetAngelNew>=(105/180)*3.1415926535)&&(offsetAngelNew<(135/180)*3.1415926535))//4
			{
				directionInt = 4;
				directionString = "4";
			}
			if((offsetAngelNew>=(135/180)*3.1415926535)&&(offsetAngelNew<(165/180)*3.1415926535))//5
			{
				directionString = "5";
				directionInt = 5;
			}
			if((offsetAngelNew>=(165/180)*3.1415926535)&&(offsetAngelNew<=(180/180)*3.1415926535))//6
			{
				directionString = "6";
				directionInt = 6;
			}

		}
	

*/






/*
		if(dianzi>0)
		{
			turnFront = true;
			turnBack = false;
		}
		if(dianzi<0)
		{
			turnFront = false;
			turnBack = true;
		}
		
		if(offsetAngelNew<=offsetAngel)
		{
			turnFront = true;
			turnBack = false;
		}
		if(offsetAngelNew>=3.1415926535-offsetAngel)
		{
			turnFront = false;
			turnBack = true;
		}*/
		
	}
	
	string is_may_near_furni()
	{
		int i=0;
		double TFDistance;
		double temp;
		
		//if(target.name.find("chair")!=std::string::npos)
		//{
		//	describe = " pink cup";
		//}
		string result = myfurni[0].name;
		TFDistance = sqrt(pow(targetObject[0]-myfurni[0].x,2)+pow(targetObject[1]-myfurni[0].y,2)+pow(targetObject[2]-myfurni[0].z,2));
		for(i=0;i<myFurniCount;i++)
		{
			temp = sqrt(pow(targetObject[0]-myfurni[i].x,2)+pow(targetObject[1]-myfurni[i].y,2)+pow(targetObject[2]-myfurni[i].z,2));
			if(TFDistance>temp)
			{
				//if(.name.find("chair")!=std::string::npos)
				//{
				//	result = " pink cup";
				//}
				result = myfurni[i].name;
				TFDistance = temp;
			}
		}
		
		return result;	
	}
	
	bool is_may_catch_wrong()
	{
		//cout<<"---------in is_may_catch_wrong()-----------"<<endl;
		int i=0;
		double LDistance;
		double RDistance;
		for(i=0;i<myNumOfNonTargetObjects;i++)
		{
			LDistance = sqrt(pow(Avatar.lefthand[0]-myObject[i].x,2)+pow(Avatar.lefthand[1]-myObject[i].y,2));
			RDistance = sqrt(pow(Avatar.righthand[0]-myObject[i].x,2)+pow(Avatar.righthand[1]-myObject[i].y,2));
		//cout<<"---------in is_may_catch_wrong()-----------"<<endl;
			if(LDistance<=nearTargetDistance)
			{
				cout<<"--------------LEFT-------------------"<<endl;
				isLeftMayWrong = true;
				isRightMayWrong = false;
				moveStep = mayCatchWrong;
				return true;
			}
			if(RDistance<=nearTargetDistance)
			{
				cout<<"--------------right-------------------"<<endl;
				isLeftMayWrong = false;
				isRightMayWrong = true;
				moveStep = mayCatchWrong;
				return true;
			}
		}
		return false;
		
	}


	//手是否与目标很近,哪只手可以抓取（即是否可以停止调整）
	bool is_stop_move_hand()
	{
		//此处应更新左右手坐标
		leftDistance = sqrt(pow(Avatar.lefthand[0]-targetObject[0],2)+pow(Avatar.lefthand[1]-targetObject[1],2));
		rightDistance = sqrt(pow(Avatar.righthand[0]-targetObject[0],2)+pow(Avatar.righthand[1]-targetObject[1],2));

		
		if(!isGoDestination)//抓物体
		{
			if(leftDistance <= nearTargetDistance)
			{
				LeftHandCatch = true;
				RightHandCatch = false;
				moveStep = canCatch;
				return true;
			}

			//右手可抓取
			if(rightDistance <= nearTargetDistance)
			{
				LeftHandCatch = false;
				RightHandCatch = true;
				moveStep = canCatch;
				return true;
			}
			return false;
		}
		
		else//放物体
		{
			if(targetInLeftHand)//在左手
			{
				if(leftDistance <= destinationSizeArea )//&& (Avatar.lefthand[3]>=targetObject[3])&&(Avatar.lefthand[3]<=targetObject[3]+destinationSize[3]))
				{
					moveStep = canPut;
					return true;
				}
				else
				{
					moveStep = moveHand;
					return false;
				}
			}
			else//在右手
			{
				if(rightDistance <= destinationSizeArea)//&& (Avatar.righthand[3]>=targetObject[3])&&(Avatar.righthand[3]<=targetObject[3]+destinationSize[3]))
				{
					moveStep = canPut;
					return true;
				}
				else
				{
					moveStep = moveHand;
					return false;
				}
			}
		}
	}


	//Avatar与目标较近时，决定微调哪个手部//(若is_stop_move_hand()==false，则循环使用这个函数)
	void move_whitch_hand()
	{
		//计算距两手距离，判断移哪只手
		//此处应更新左右手坐标
		if(!isGoDestination)//抓物体
		{
			leftDistance = sqrt(pow(Avatar.lefthand[0]-targetObject[0],2)+pow(Avatar.lefthand[1]-targetObject[1],2));
			rightDistance = sqrt(pow(Avatar.righthand[0]-targetObject[0],2)+pow(Avatar.righthand[1]-targetObject[1],2));

			if(leftDistance<rightDistance)
			{
				moveLeftHand = true;
				moveRightHand = false;

			}
			else
			{
				moveLeftHand = false;
				moveRightHand = true;
			}
			moveStep = moveHand;
		}
		else//放物体
		{
			if(targetInLeftHand)//在左手
			{
				moveLeftHand = true;
				moveRightHand = false;
			}
			else
			{
				moveLeftHand = false;
				moveRightHand = true;
			}
		}
		/*
		leftDistance = sqrt(pow(Avatar.lefthand[0]-targetObject[0],2)+pow(Avatar.lefthand[1]-targetObject[1],2));
		rightDistance = sqrt(pow(Avatar.righthand[0]-targetObject[0],2)+pow(Avatar.righthand[1]-targetObject[1],2));

		if(leftDistance<rightDistance)
		{
			moveLeftHand = true;
			moveRightHand = false;

		}
		else
		{
			moveLeftHand = false;
			moveRightHand = true;
		}
		moveStep = moveHand;*/
	}


	void move_hand()
	{
		//double leftToRight[2];//左手点到右手点的向量

		//leftToRight[0] = Avatar.righthand[0] - Avatar.lefthand[0];
		//leftToRight[1] = Avatar.righthand[1] - Avatar.lefthand[1];

		double handToTarget[2];

		bool leftSide = false;
		bool rightSide = false;



		
		if(moveLeftHand == true)//移左手
		{
			//
			handToTarget[0] = targetObject[0] - Avatar.lefthand[0];
			handToTarget[1] = targetObject[1] - Avatar.lefthand[1];
			double LeftToChest[2];
			LeftToChest[0] = Avatar. chest[0] - Avatar.lefthand[0];
			LeftToChest[1] = Avatar. chest[1] - Avatar.lefthand[1];

			//平移点坐标
			double moveTarget[2];
			moveTarget[0] = targetObject[0] + LeftToChest[0]; 
			moveTarget[1] = targetObject[1] + LeftToChest[1];

			double newLeftDistance = sqrt(pow(Avatar.lefthand[0]-moveTarget[0],2)+pow(Avatar.lefthand[1]-moveTarget[1],2));
			double newRightDistance = sqrt(pow(Avatar.righthand[0]-moveTarget[0],2)+pow(Avatar.righthand[1]-moveTarget[1],2));

			if(newLeftDistance<=newRightDistance)//
			{
				leftSide = true;
				rightSide = false;
			}
			else
			{
				leftSide = false;
				rightSide = true;
			}

			double v1 = sqrt(pow(handToTarget[0],2)+pow(handToTarget[1],2));
			double v2 = sqrt(pow(Avatar.towards[0],2)+pow(Avatar.towards[1],2));
			double offsetAngelNew = acos((Avatar.towards[0]*handToTarget[0]+Avatar.towards[1]*handToTarget[1])/(v1*v2));
			offsetAngelNew = 180*(offsetAngelNew/3.1415926535);
			if(leftSide)
			{
				if(offsetAngelNew<=22.5)//前
				{
					moveHandLeft = false;
					moveHandRight = false;
					moveHandFront = true;
					moveHandBack = false;
				}
				if((offsetAngelNew>22.5)&&(offsetAngelNew<=67.5))//前左
				{
					moveHandLeft = true;
					moveHandRight = false;
					moveHandFront = true;
					moveHandBack = false;
				}
				if((offsetAngelNew>67.5)&&(offsetAngelNew<=112.5))//左
				{
					moveHandLeft = true;
					moveHandRight = false;
					moveHandFront = false;
					moveHandBack = false;
				}
				if((offsetAngelNew>112.5)&&(offsetAngelNew<=157.5))//左后
				{
					moveHandLeft = true;
					moveHandRight = false;
					moveHandFront = false;
					moveHandBack = true;
				}
				if((offsetAngelNew>157.5)&&(offsetAngelNew<=180))//后
				{
					moveHandLeft = false;
					moveHandRight = false;
					moveHandFront = false;
					moveHandBack = true;
				}

			}
			else
			{

				if(offsetAngelNew<=22.5)//前
				{
					moveHandLeft = false;
					moveHandRight = false;
					moveHandFront = true;
					moveHandBack = false;
				}
				if((offsetAngelNew>22.5)&&(offsetAngelNew<=67.5))//前右
				{
					moveHandLeft = false;
					moveHandRight = true;
					moveHandFront = true;
					moveHandBack = false;
				}
				if((offsetAngelNew>67.5)&&(offsetAngelNew<=112.5))//右
				{
					moveHandLeft = false;
					moveHandRight = true;
					moveHandFront = false;
					moveHandBack = false;
				}
				if((offsetAngelNew>112.5)&&(offsetAngelNew<=157.5))//右后
				{
					moveHandLeft = false;
					moveHandRight = true;
					moveHandFront = false;
					moveHandBack = true;
				}
				if((offsetAngelNew>157.5)&&(offsetAngelNew<=180))//后
				{
					moveHandLeft = false;
					moveHandRight = false;
					moveHandFront = false;
					moveHandBack = true;
				}
			}
			
			if(Avatar.lefthand[2]<=targetObject[2])//向上移
			{
				moveHandUp = true;
				moveHandDown = false;
			}
			if(Avatar.lefthand[2]>=targetObject[2]+offsetHight)//向下移
			{
				moveHandUp = false;
				moveHandDown = true;
			}
			if(isGoDestination)
			{
				if(Avatar.lefthand[2]<=targetObject[2])
				{
					moveHandUp = true;
					moveHandDown = false;
				}
				if(Avatar.lefthand[2]>=targetObject[2]+destinationSize[2])
				{
					moveHandUp = false;
					moveHandDown = true;
				}
			}
			

		}
		else//移右手
		{
			handToTarget[0] = targetObject[0] - Avatar.righthand[0];
			handToTarget[1] = targetObject[1] - Avatar.righthand[1];

			double rightToChest[2];
			rightToChest[0] = Avatar. chest[0] - Avatar.righthand[0];
			rightToChest[1] = Avatar. chest[1] - Avatar.righthand[1];

			//平移点坐标
			double moveTarget[2];
			moveTarget[0] = targetObject[0] + rightToChest[0]; 
			moveTarget[1] = targetObject[1] + rightToChest[1];

			double eleftHand[2] ;
			double erightHand[2];

			eleftHand[0] = Avatar.chestToLeft[0] + Avatar.chest[0];
			eleftHand[1] = Avatar.chestToLeft[1] + Avatar.chest[1];
			erightHand[0] = Avatar.chestToRight[0] + Avatar.chest[0];
			erightHand[1] = Avatar.chestToRight[1] + Avatar.chest[1];


			double newLeftDistance = sqrt(pow(eleftHand[0]-moveTarget[0],2)+pow(eleftHand[1]-moveTarget[1],2));
			double newRightDistance = sqrt(pow(erightHand[0]-moveTarget[0],2)+pow(erightHand[1]-moveTarget[1],2));

			if(newLeftDistance<=newRightDistance)//
			{
				leftSide = true;
				rightSide = false;
			}
			else
			{
				leftSide = false;
				rightSide = true;
			}

			double v1 = sqrt(pow(handToTarget[0],2)+pow(handToTarget[1],2));
			double v2 = sqrt(pow(Avatar.towards[0],2)+pow(Avatar.towards[1],2));
			double offsetAngelNew = acos((Avatar.towards[0]*handToTarget[0]+Avatar.towards[1]*handToTarget[1])/(v1*v2));
			offsetAngelNew = 180*(offsetAngelNew/3.1415926535);
			if(leftSide)
			{
				if(offsetAngelNew<=22.5)//前
				{
					moveHandLeft = false;
					moveHandRight = false;
					moveHandFront = true;
					moveHandBack = false;
				}
				if((offsetAngelNew>22.5)&&(offsetAngelNew<=67.5))//前左
				{
					moveHandLeft = true;
					moveHandRight = false;
					moveHandFront = true;
					moveHandBack = false;
				}
				if((offsetAngelNew>67.5)&&(offsetAngelNew<=112.5))//左
				{
					moveHandLeft = true;
					moveHandRight = false;
					moveHandFront = false;
					moveHandBack = false;
				}
				if((offsetAngelNew>112.5)&&(offsetAngelNew<=157.5))//左后
				{
					moveHandLeft = true;
					moveHandRight = false;
					moveHandFront = false;
					moveHandBack = true;
				}
				if((offsetAngelNew>157.5)&&(offsetAngelNew<=180))//后
				{
					moveHandLeft = false;
					moveHandRight = false;
					moveHandFront = false;
					moveHandBack = true;
				}

			}
			else
			{

				if(offsetAngelNew<=22.5)//前
				{
					moveHandLeft = false;
					moveHandRight = false;
					moveHandFront = true;
					moveHandBack = false;
				}
				if((offsetAngelNew>22.5)&&(offsetAngelNew<=67.5))//前右
				{
					moveHandLeft = false;
					moveHandRight = true;
					moveHandFront = true;
					moveHandBack = false;
				}
				if((offsetAngelNew>67.5)&&(offsetAngelNew<=112.5))//右
				{
					moveHandLeft = false;
					moveHandRight = true;
					moveHandFront = false;
					moveHandBack = false;
				}
				if((offsetAngelNew>112.5)&&(offsetAngelNew<=157.5))//右后
				{
					moveHandLeft = false;
					moveHandRight = true;
					moveHandFront = false;
					moveHandBack = true;
				}
				if((offsetAngelNew>157.5)&&(offsetAngelNew<=180))//后
				{
					moveHandLeft = false;
					moveHandRight = false;
					moveHandFront = false;
					moveHandBack = true;
				}
			}

			if(Avatar.righthand[2]<=targetObject[2])//向上移
			{
				moveHandUp = true;
				moveHandDown = false;
			}
			if(Avatar.righthand[2]>=targetObject[2]+offsetHight)//向下移
			{
				moveHandUp = false;
				moveHandDown = true;
			}

			if(isGoDestination)
			{
				if(Avatar.righthand[2]<=targetObject[2])
				{
					moveHandUp = true;
					moveHandDown = false;
				}
				if(Avatar.righthand[2]>=targetObject[2]+destinationSize[2])
				{
					moveHandUp = false;
					moveHandDown = true;
				}
			}
		}
		

		
	}

	string clockToDirection()
	{
		string output;
		switch(directionInt)
		{
			case 12:
			{
				output = "front";
				break;
			}
			case 11:
			{
				output = "left front";
				break;
			}
			case 10:
			{
				output = "left";
				break;
			}
			case 9:
			{
				output = "left";
				break;
			}
			case 8:
			{
				output = "left back";
				break;
			}
			case 7:
			{
				output = "left back";
				break;
			}
			case 6:
			{
				output = "back";
				break;
			}
			case 5:
			{
				output = "right back";
				break;
			}
			case 4:
			{
				output = "rigth back";
				break;
			}
			case 3:
			{
				output = "right ";
				break;
			}
			case 2:
			{
				output = "right ";
				break;
			}
			case 1:
			{
				output = "right front";
				break;
			}
			
			
		}
		return output;
	}


	//发送指导消息
	string speak()
	{
		/*
		ROS_INFO_STREAM(
			"Subscribe task info message:" << std::endl <<
			"Environment ID: " << taskInfo.environment_id << std::endl <<
			"Target object: " << std::endl << taskInfo.target_object <<
			"Destination: " << std::endl << taskInfo.destination
		);
		*/
		//ROS_INFO_STREAM("speaking"<<std::endl);
		targetDistance = sqrt(pow(Avatar.head[0]-targetObject[0],2)+pow(Avatar.head[1]-targetObject[1],2));
		string stringDistance = std::to_string(int(targetDistance));
		string output ="nothing_speak()";
		string LRdirection = clockToDirection();
		string tempTarget;
		if(!isGoDestination)
		{tempTarget = t_object;}
		else
		{tempTarget = "destination";}
		//cout<<"moveStep"<<moveStep<<endl;
		switch(moveStep)
			{
				case goRight:
				{
					//ROS_INFO_STREAM("step:goright"<<std::endl);
					//ROS_INFO_STREAM("You are on the right direction."<<std::endl);
					//output = "You are on the right direction.";
					output = "The "+furnitureString +" is at your "+LRdirection+", "+directionString+ "o'clock direction. You are " + stringDistance +" meters away from the "+tempTarget+".";
					break;
					//is_on_the_way();
				}
				case goWrong:
				{
					//ROS_INFO_STREAM("You are on the wrong direction."<<std::endl);
					//ROS_INFO_STREAM("The "+targetObjectName+" is ");
					//ROS_INFO_STREAM("Please go ");
					output = "The "+furnitureString +" is at your "+LRdirection+", "+directionString+ "o'clock direction. You are " + stringDistance +" meters away from the "+tempTarget+".";
					
					/*if(turnLeft)//左转
					{
						//ROS_INFO_STREAM("left ");
						output = output + "left ";
					}
					if(turnRight)//右转
					{
						//ROS_INFO_STREAM("right ");
						output = output + "right ";
					}
					if(turnFront)//左转
					{
						//ROS_INFO_STREAM("front ");
						output = output + "front ";
					}
					if(turnBack)//右转
					{
						//ROS_INFO_STREAM("back ");
						output = output + "back ";
					}*/
					
					//ROS_INFO_STREAM("to find the "+targetObjectName<<std::endl);
					//output = output + ".";
					//speakGuidanceMessage(pubHumanNaviMsg, pubGuidanceMsg, output);
					break;

				}
				/*
				case moveWhitchHand:
				{
					output = "whichHand_nothing.";
					break;
				}
				*/
				case moveHand:
				{
					
					if(moveLeftHand)//移左手
					{
						//ROS_INFO_STREAM("Move your left hand ");
						output = "Move your left hand ";
						if(moveHandLeft)
						{
							//ROS_INFO_STREAM("left");
							output = output + "left ";
						}
						if(moveHandRight)
						{
							//ROS_INFO_STREAM("right");
							output = output + "right ";
						}
						if(moveHandUp)
						{
							//ROS_INFO_STREAM("up");
							output = output +"up ";
						}
						if(moveHandDown)
						{
							//ROS_INFO_STREAM("down");
							output = output +"down ";
						}
						if(moveHandFront)
						{
							//ROS_INFO_STREAM("front");
							output = output +"front ";
						}
						if(moveHandBack)
						{
							//ROS_INFO_STREAM("back");
							output = output +"back ";
						}

					}
					if(moveRightHand)//移右手
					{
						//ROS_INFO_STREAM("Move your right hand ");
						output = "Move your right hand ";
						if(moveHandLeft)
						{
							//ROS_INFO_STREAM("left");
							output = output + "left ";
						}
						if(moveHandRight)
						{
							//ROS_INFO_STREAM("right");
							output = output + "right ";
						}
						if(moveHandUp)
						{
							//ROS_INFO_STREAM("up");
							output = output +"up ";
						}
						if(moveHandDown)
						{
							//ROS_INFO_STREAM("down");
							output = output +"down ";
						}
						if(moveHandFront)
						{
							//ROS_INFO_STREAM("front");
							output = output +"front ";
						}
						if(moveHandBack)
						{
							//ROS_INFO_STREAM("back");
							output = output +"back ";
						}
					}
					if(isGoDestination){output = output+ ".";}
					else
					{output = output + "to catch the "+t_object+".";}
					break;
				}
				case canCatch:
				{
					output = "Pick the "+t_object+" up with your";
					if(!isGoDestination)
					{
						if(LeftHandCatch)
						{
							//ROS_INFO_STREAM("Pick it up with your left hand!");
							output = output + "left hand! ";
						}
						if(RightHandCatch)
						{
							//ROS_INFO_STREAM("Pick it up with your right hand!");
							output = output + " right hand! ";
						}
					}
					if(isGoDestination)
					{
						if(LeftHandCatch)
						{
							//ROS_INFO_STREAM("Pick it up with your left hand!");
							output = output + " left hand! ";
						}
						if(RightHandCatch)
						{
							//ROS_INFO_STREAM("Pick it up with your right hand!");
							output = output + " right hand! ";
						}
					}
					break;
				}
				case catchWrong:
				{
					//ROS_INFO_STREAM("You have picked the wrong thing. Please put it down!");
					//cout<<"000002---------------speak catchWrong--------------"<<endl;
					output = "You have picked the wrong thing. Please put it down!";
					break;
				}
				case catchRight:
				{
					//ROS_INFO_STREAM("Good job! You get the target!");
					output = "Good job! You get the target!";
					//isStrat = false;
					break;
				}
				/*
				case goRightToDestination:
				{

				}
				case goWrongToDestination:
				{
					output = "Please go ";
					if(turnLeft)//左转
					{
						//ROS_INFO_STREAM("left ");
						output = output + "left ";
					}
					if(turnRight)//右转
					{
						//ROS_INFO_STREAM("right ");
						output = output + "right ";
					}
					if(turnFront)//左转
					{
						//ROS_INFO_STREAM("front ");
						output = output + "front ";
					}
					if(turnBack)//右转
					{
						//ROS_INFO_STREAM("back ");
						output = output + "back ";
					}
					//ROS_INFO_STREAM("to find the "+targetObjectName<<std::endl);
					output = output + ".";
					//speakGuidanceMessage(pubHumanNaviMsg, pubGuidanceMsg, output);
					break;
				}*/
				case canPut:
				{
					output = "Please put it down.";
					break;
				}
				case putRight:
				{
					output = "Win!";
					break;	
				}
				case putWrong:
				{
					output = "You misplace the target, please pick it up."  ;
					break;
				}
				case mayCatchWrong:
				{
					if(isLeftMayWrong)
					{output = "Don't pick the object near your left hand which is not the target! Please keep finding.";}
					if(isRightMayWrong)
					{output = "Don't pick the object near your right hand which is not the target! Please keep finding.";}
					break;
				}
				default:
				{output = "nothing_default.";}
			}
	//cout<<"speak()"<<output<<endl;
			if(output =="nothing_speak()")
				//cout<<"moveStep: "<<moveStep<<endl;
	return output;
		
	}

	void turn_to_destination()
	{
		cout<<"------in turn to destination-----------"<<endl;
		targetObject[0] = destinationFuniture[0];
		targetObject[1] = destinationFuniture[1];
		targetObject[2] = destinationFuniture[2];
		furnitureString = destinationString;
		moveStep = goWrong;
	}


	///////////////////////////////////////////
	string runMoveDirection()
	{
		//ros::init(argc, argv, "MoveDirection");

		//ros::NodeHandle nodeHandle;

		//ros::Rate loopRate(1);

		//init();

		//ROS_INFO("Move Direction Is On!");

		//ros::Subscriber subTaskInfoMsg = nodeHandle.subscribe<human_navigation::HumanNaviTaskInfo>("/human_navigation/message/task_info", 1, &MoveDirection::taskInfoMessageCallback, this);
		//ros::Subscriber subAvatarStatusMsg = nodeHandle.subscribe<human_navigation::HumanNaviAvatarStatus>("/human_navigation/message/avatar_status", 1, &MoveDirection::avatarStatusMessageCallback, this);
		//ros::Publisher pubHumanNaviMsg = nodeHandle.advertise<human_navigation::HumanNaviMsg>("/human_navigation/message/to_moderator", 10);
		//ros::Time time;
		

		//while (ros::ok())
		//{ 
			//ROS_INFO_STREAM("    run step:"+step<<std::endl);
			//sendMessage(pubHumanNaviMsg, MSG_GET_AVATAR_STATUS);
			//cout<<"moveStep: "<<moveStep<<endl;
			switch(moveStep)
			{
				case moveDirectionReady:
				{
					if(isStart)
					{
						moveStep = goWrong;
					}
					//step = goRight;
					break;
				}
				case goRight:
				{
					if(isGoDestination)
					{
						if(isGoDestinationFirstSpeak)
						{isGoDestinationFirstSpeak = false;
						 isMoveDirection = false;
						}
						else
						{left_or_right();}
						
					}
					else{
					if(Avatar.object_in_left_hand!=""||Avatar.object_in_right_hand!="")//抓了东西
					{	
						//cout<<"000000---------------ininini--------------"<<endl;
						if(Avatar.is_target_object_in_left_hand||Avatar.is_target_object_in_right_hand)
						{
							//cout<<"moveHand 2:"<<"2222222222"<<endl;
							moveStep = catchRight;
							if(Avatar.is_target_object_in_left_hand)
								targetInLeftHand = true;
							else
								targetInLeftHand = false;
								//turn_to_destination();
							isMoveDirection = false;
							//cout<<"333333333"<<endl;
							isGoDestination = true;
							moveStep = goWrong;
							step= GuideForPlacement;
						}
						else
						{
							//cout<<"000001---------------catchWrong--------------"<<endl;
							moveStep = catchWrong;	
						}
					}	
					else
					{	
						if(is_may_catch_wrong())
						{
							moveStep = mayCatchWrong;
						}
						else{
							if(!is_near())
							{
							//is_on_the_way();
								left_or_right();
							}
							else
                    					{
								if(!isGoDestination)
                    						{
                    							isMoveDirection = false;
                        		
                    						}
                    						else
                    						{
                    							isMoveDirection = true;
                    						}
                   		 			}
						}
					}
					}
					//is_on_the_way();
					//ROS_INFO_STREAM("goRight");
					break;
				}
				case goWrong:
				{
					if(isGoDestination)
					{
						if(!is_near())
						{
							//is_on_the_way();
							if(isGoDestinationFirstSpeak)
							{isGoDestinationFirstSpeak = false;
							 isMoveDirection = false;
							}
							else
							{left_or_right();}
						}
					}
					else{
						if(Avatar.object_in_left_hand!=""||Avatar.object_in_right_hand!="")//抓了东西
						{	
							//cout<<"000000---------------ininini--------------"<<endl;
							if(Avatar.is_target_object_in_left_hand||Avatar.is_target_object_in_right_hand)
							{
								//cout<<"moveHand 2:"<<"2222222222"<<endl;
								moveStep = catchRight;
								if(Avatar.is_target_object_in_left_hand)
									targetInLeftHand = true;
								else
									targetInLeftHand = false;
									//turn_to_destination();
								isMoveDirection = false;
								//cout<<"333333333"<<endl;
								isGoDestination = true;
								moveStep = goWrong;
								step= GuideForPlacement;
							}
							else
							{
								//cout<<"000001---------------catchWrong--------------"<<endl;
								moveStep = catchWrong;	
							}
						}
						else
						{	
							if(is_may_catch_wrong())
							{
								moveStep = mayCatchWrong;
							}
							else
							{
								if(!is_near())
								{
								
									left_or_right();
							
								}
								else
                    						{
                    							isMoveDirection = false;	
                   		 				}
							}
						}
					}
					//ROS_INFO_STREAM("goWrong");
					break;
				}
				case moveWhitchHand:
				{
					if(!isGoDestination)
					{
						if(is_near())
     						{move_whitch_hand();}
     				}
     				else
     				{
     					if(targetInLeftHand)//左手拿着物体
     						moveLeftHand = true;
     					else
     						moveRightHand = true;
     					moveStep = moveHand;
     				}
					//move_whitch_hand();
					break;
				}
				case moveHand:
				{
					if(!isGoDestination)
					{
						if(is_may_catch_wrong())
						{
							
						}
						else{
						if(is_near()&&!is_stop_move_hand())
						{
							//cout<<"isGoDestination: "<<isGoDestination<<endl;
							move_hand();
						}
						//判断是否抓住
						if(Avatar.object_in_left_hand!=""||Avatar.object_in_right_hand!="")//抓了东西
						{	
							//cout<<"000000---------------ininini--------------"<<endl;
							if(Avatar.is_target_object_in_left_hand||Avatar.is_target_object_in_right_hand)
							{


								//cout<<"moveHand 2:"<<"2222222222"<<endl;
								moveStep = catchRight;
								if(Avatar.is_target_object_in_left_hand)
									targetInLeftHand = true;
								else
									targetInLeftHand = false;
								//turn_to_destination();
								isMoveDirection = false;
								//cout<<"333333333"<<endl;
								isGoDestination = true;
								moveStep = goWrong;
								step= GuideForPlacement;
							}
							else
							{
								//cout<<"000001---------------catchWrong--------------"<<endl;
								moveStep = catchWrong;	
							}
						}
						}
					}
					else
					{
						
							if(is_near()&&!is_stop_move_hand())
							{
								//cout<<"isGoDestination: "<<isGoDestination<<endl;
								move_hand();
							}
							else
							{
								if(!is_near())
								{
									moveStep = goWrong;
								} 
								if(is_stop_move_hand())
								{
									moveStep = canPut;
								}
							}
					}

					//move_hand();
					break;
				}
				case canCatch:
				{
					if(is_near()&&is_stop_move_hand())
					{
						//cout<<"object_in_left_hand: "<<Avatar.object_in_left_hand<<endl;
						if(Avatar.object_in_left_hand!=""||Avatar.object_in_right_hand!="")//抓了东西
						{	

							//cout<<"moveHand 3:"<<"33333333333"<<endl;
							if(Avatar.is_target_object_in_left_hand||Avatar.is_target_object_in_right_hand)
							{	



								//cout<<"moveHand 4:"<<"4444444444"<<endl;
								moveStep = catchRight;
								if(Avatar.is_target_object_in_left_hand)
									targetInLeftHand = true;
								else
									targetInLeftHand = false;
								//turn_to_destination();
								isMoveDirection = false;
								//cout<<"444444444444"<<endl;
								isGoDestination = true;
								moveStep = goWrong;
								step= GuideForPlacement;
							}
							else
							{
								moveStep = catchWrong;	
							}
						}
					}
					else
					{
						if(!is_near())
						{moveStep = goWrong;}
						if(!is_stop_move_hand())
						{moveStep = moveHand;}
						
					}
					
					//ROS_INFO_STREAM("canCatch");
					break;
				}
				case catchWrong:
				{
					//cout<<"000004---------------runmoveCatchWrong--------------"<<endl;
					if(Avatar.object_in_left_hand!=""||Avatar.object_in_right_hand!="")//抓了东西
					{	
						//cout<<"000000---------------ininini--------------"<<endl;
						if(Avatar.is_target_object_in_left_hand||Avatar.is_target_object_in_right_hand)
						{
							//cout<<"moveHand 2:"<<"2222222222"<<endl;
							moveStep = catchRight;
							if(Avatar.is_target_object_in_left_hand)
								targetInLeftHand = true;
							else
								targetInLeftHand = false;
								//turn_to_destination();
							isMoveDirection = false;
							//cout<<"333333333"<<endl;
							isGoDestination = true;
							moveStep = goWrong;
							step = GuideForPlacement;
						}
						else
						{
							//cout<<"000001---------------catchWrong--------------"<<endl;
							moveStep = catchWrong;	
						}
					}
					else
					{
						if(!is_near())
						{moveStep = goWrong;}
						else
						{
							if(!is_stop_move_hand())
							{moveStep = moveHand;}
						}
					}
					break;
				}
				case catchRight:
				{
					break;
				}
				case canPut:
				{
					if(is_near()&&!is_stop_move_hand())
					{
							//cout<<"isGoDestination: "<<isGoDestination<<endl;
							move_hand();
					}
					else
					{
						if(!is_near())
						{
							moveStep = goWrong;
						} 
						if(is_stop_move_hand())
						{
							moveStep = canPut;
						}
					}
					break;
				}
				case putRight:
				{
					break;
				}
				case putWrong:
				{
					break; 
				}
				case mayCatchWrong:
				{
					//cout<<"000004---------------runmoveCatchWrong--------------"<<endl;
					if(Avatar.object_in_left_hand!=""||Avatar.object_in_right_hand!="")//抓了东西
					{	
						//cout<<"000000---------------ininini--------------"<<endl;
						if(Avatar.is_target_object_in_left_hand||Avatar.is_target_object_in_right_hand)
						{
							//cout<<"moveHand 2:"<<"2222222222"<<endl;
							moveStep = catchRight;
							if(Avatar.is_target_object_in_left_hand)
								targetInLeftHand = true;
							else
								targetInLeftHand = false;
								//turn_to_destination();
							isMoveDirection = false;
							//cout<<"333333333"<<endl;
							isGoDestination = true;
							moveStep = goWrong;
							step = GuideForPlacement;
						}
						else
						{
							//cout<<"000001---------------catchWrong--------------"<<endl;
							moveStep = catchWrong;	
						}
					}
					else
					{
						if(!is_near())
						{
							if(is_may_catch_wrong())
							{moveStep = mayCatchWrong;}
							else
							{moveStep = goWrong;}
						}
						else
						{
							if(!is_stop_move_hand())
							{
								if(is_may_catch_wrong())
								{moveStep = mayCatchWrong;}
								else
								{moveStep = moveHand;}
							}
						}
					}
					break;
				}	
				/*
				case goRightToDestination:
				{
					if(!is_near())
					{
						is_on_the_way();
					}
					break;
				}
				case goWrongToDestination:
				{
					if(!is_near())
					{
						if (!is_on_the_way())
						{
							left_or_right();
						}
					}
				}
				case */
			
			
		}
		//string output = "nothing_runMove";
		string output = speak();
		//cout<<"runMove()"<<moveStep<<endl;
		//ROS_INFO("555555555");
		return output;
	}

	///////////////////////////////////////////



	int run(int argc, char **argv)
	{
		ros::init(argc, argv, "human_navi_sample");

		ros::NodeHandle nodeHandle;

		ros::Rate loopRate(10);

		init();

		ROS_INFO("Human Navi sample start!");

		ros::Subscriber subHumanNaviMsg = nodeHandle.subscribe<human_navigation::HumanNaviMsg>("/human_navigation/message/to_robot", 100, &HumanNavigationSample::messageCallback, this);
		ros::Subscriber subTaskInfoMsg = nodeHandle.subscribe<human_navigation::HumanNaviTaskInfo>("/human_navigation/message/task_info", 1, &HumanNavigationSample::taskInfoMessageCallback, this);
		ros::Subscriber subAvatarStatusMsg = nodeHandle.subscribe<human_navigation::HumanNaviAvatarStatus>("/human_navigation/message/avatar_status", 1, &HumanNavigationSample::avatarStatusMessageCallback, this);
		//ros::Subscriber subObjectStatusMsg = nodeHandle.subscribe<human_navigation::HumanNaviObjectStatus>("/human_navigation/message/object_status", 1, &HumanNavigationSample::objectStatusMessageCallback, this);
		ros::Publisher pubHumanNaviMsg = nodeHandle.advertise<human_navigation::HumanNaviMsg>("/human_navigation/message/to_moderator", 10);
		ros::Publisher pubGuidanceMsg  = nodeHandle.advertise<human_navigation::HumanNaviGuidanceMsg>("/human_navigation/message/guidance_message", 10);

		ros::Time time;


		while (ros::ok())
		{	

			////////ZLH//start///
			//time = ros::Time::now();
			//int wait = 10;
			

			string output = " ";
			sendMessage(pubHumanNaviMsg, MSG_GET_AVATAR_STATUS);
			if(isMoveDirection)
			{	

				//cout<<"hhhhhhhhhhhhhhh"<<endl;
				output = runMoveDirection();////////
				int wait = 3;
				if(moveStep == goRight||moveStep == goWrong)
				{
					wait = 3;
				}
				if(moveStep == mayCatchWrong||moveStep == catchWrong||moveStep == canCatch||moveStep == canPut)
				{wait =0;}
				if(moveStep ==moveHand)
				{
					wait =2;
      				}
				if((time.sec + wait < ros::Time::now().sec))
				{	
				if(output !="nothing_default.")
				{
					if(directionCount<15)
					{speakGuidanceMessage(pubHumanNaviMsg, pubGuidanceMsg, output);
					time = ros::Time::now();}
					//cout<<output<<endl;
					//time = ros::Time::now();
					//cout<<"step："<<step<<endl;

					//cout<<"家具"<<furnitureString<<endl;
				}
				//ROS_INFO_STREAM(output);
				}				

			}
			else
			{
				//cout<<"aaaaaaaaaaaaaaa"<<endl;
			switch(step)
			{
				case Initialize:
				{
					reset();

					ROS_INFO("##### Initialized ######");
					//furnitureString = "1";
					//cout<<"F:"<<furnitureString<<endl;
					//cout<<"11111111111"<<endl;
					step++;
					break;
				}
				case Ready:
				{	//cout<<"iiiiiiiiiii"<<endl;
					if(isStarted)
					{
						step++;

						sendMessage(pubHumanNaviMsg, MSG_I_AM_READY);

						//cout<<"2222222222"<<endl;
						//furnitureString = "2";
						//cout<<"F:"<<furnitureString<<endl;;
						ROS_INFO("Task start");
					}
					break;
				}
				case WaitTaskInfo:
				{
					if(isTaskInfoReceived){ step++; }
					//cout<<"3333333333"<<endl;
					//furnitureString = "3";

					//cout<<"F:"<<furnitureString<<endl;;
					break;
				}
				case GuideForTakingObject:
				{	

					//cout<<"1:"<<std::boolalpha<<(bool)isRequestReceived<<endl;

					if(isRequestReceived)
					{
						isRequestReceived = false;
					}

					//cout<<"44444444444"<<endl;

					//furnitureString = "4";
					//cout<<"F:"<<furnitureString<<endl;;


					std::string targetObjectName;

					string environment = taskInfo.environment_id;


					double locate[3] ;


					string location_1 = " ";
					string location_2 = " ";
					string location_3 = " ";

					string try_ = " ";


					if (environment == "LayoutHN01")
{
	double sidetable_and_mat[4] = { 0.77606,5.20635,-0.720915,3.81716};//r
	double table_level[2] = {0.493,0.226};//r

	double bed[4] = { 3.76808,6.41943,2.40386,4.64314 };//r

	double fridge[4] = { -3.1212,4.18648,-3.69847,3.49011 };//r
	double fridge_level[3] = { 0.927,0.742,0.38 };//r
	double fridge_center_line = 0.214;//r

	double counter_and_cabinet[4] = { -2.9743, 6.64847, -3.75563, 4.34638 };//r
	double counter_and_cabinet_level[3] = { 1.3839, 0.8565, 0.35 };//r
	double counter[2] = { 3.06622 ,2.212 };
	double cabinet[3] = { 2.04622 ,2.6556 ,3.2664 };
	double cabinet_line = 1.50785;//r

	double bookshelf[4] = { 0,0,0,0 };
	double bookshelf_level[3] = { 1.0281,0.6708,0.3512 }; //r

	double shelf_1[4] = { -3.23593,1.56046,-3.75125,-0.0923617 };//r
	double shelf_2[4] = { -3.20721,3.23087,-3.26121,1.58291 };//r
	double shelf_level[3] = {1.0259,0.624,0.221 };//r

	double TVbass[4] = { 1.00351,6.72734,-0.895564,-0.895564 };//r
	double TVbass_level[2] = { 0.756,0.49 };//r

	double glass_wagon_1[4] = { -1.30828,6.55556,-1.62333,5.8725 };//r
	double glass_wagon_2[4] = { 1.67445,6.70748,1.13121,6.1508 };//r
	double glass_wagon_3[4] = { -1.7524,6.73146,-2.28742,6.13601 };//r

	double wagon_level[2] = { 0.7286,0.394 };

	double wooden_table_and_chair[4] = { 3.18773,3.63173,1.88067, 2.34327};//r
	double chair_level = 0.563;//椅子上面桌子的高度//r
	double trashcan[5] = {  3.294,0,3.927,0.344,0.437 };//r

	double brown_mat[4] = { 2.629,0.179,1.005,-0.856 };//r


	geometry_msgs::Point targetPosition = taskInfo.target_object.position;


	locate[0] = -targetPosition.y;
	locate[1] = targetPosition.z;
	locate[2] = targetPosition.x;

	if (in_something(locate, shelf_1))
	{
		location_1 = "on the left wooden shelf";
		furnitureString = "wooden shelf";

		if (locate[1] >= shelf_level[0])
		{
			location_2 = "on the first floor of the shelf";
		}
		else if (locate[1] >= shelf_level[1])
		{
			location_2 = "on the second floor of the shelf";
		}
		else if (locate[1] >= shelf_level[2])
		{
			location_2 = "on the third floor of the shelf";
		}
		else
		{
			location_2 = "on the bottom floor of the shelf";
		}
	}


	if (in_something(locate, shelf_2))
	{
		location_1 = "on the wooden shelf next to the fridge";
		furnitureString = "wooden shelf next to the fridge";

		if (locate[1] >= shelf_level[0])
		{
			location_2 = "on the first floor of the shelf";
		}
		else if (locate[1] >= shelf_level[1])
		{
			location_2 = "on the second floor of the shelf";
		}
		else if (locate[1] >= shelf_level[2])
		{
			location_2 = "on the third floor of the shelf";
		}
		else
		{
			location_2 = "on the bottom floor of the shelf";
		}
	}


	if (in_something(locate, counter_and_cabinet))
	{

		if (locate[1] >= counter_and_cabinet_level[0])
		{
			location_1 = "in the wall cabinet";
			furnitureString = "wall cabinet ";

			if (locate[1]>cabinet_line)
			{
				location_3 = "on the upper layer ";
			}
			else
			{
				location_3 = "on the lower layer ";
			}


			if (locate[2]>cabinet[0])
			{
				location_2 = "in the first cabinet on the right";
			}
			else if (locate[2]>cabinet[1])
			{
				location_2 = "in the second cabinet on the right";
			}
			else if (locate[2]>cabinet[2])
			{
				location_2 = "in the third cabniet on the right";
			}

			else
			{
				location_2 = "in the first cabniet on the left";
			}
		}


		else if (locate[1] >= counter_and_cabinet_level[1])
		{
			location_1 = "on the top of the drawer and under the cabinet";
			furnitureString = "drawer under the wall cabinet";
		}

		else if (locate[1] >= counter_and_cabinet_level[2])
		{
			location_1 = "in the drawer";
			furnitureString = "drawer";

			if (locate[2] > counter[0])
			{
				location_2 = "in the first floor of the first drawer on the right";
			}

			else if (locate[2]>counter[1])
			{
				location_2 = "in the first floor of the second drawer on the right";

			}
			else
			{
				location_2 = "in the first floor of the third drawer on the right";
			}
		}

		else if (locate[1] >= counter_and_cabinet_level[3])
		{
			if (locate[2] > counter[0])
			{
				location_2 = "in the second floor of the first drawer on the right";
			}

			else if (locate[2]>counter[1])
			{
				location_2 = "in the second floor of the second drawer on the right";

			}

			else
			{
				location_2 = "in the second floor of the third drawer on the right";
			}
		}
	}


	if (in_something(locate, fridge))
	{
		location_1 = "in the fridge";
		furnitureString = "fridge";

		if (locate[1] > fridge_level[0])
		{
			location_2 = "in the upper part of the fridge";
		}
		else if (locate[1] > fridge_level[1])
		{
			if (locate[2] > fridge_center_line)
			{
				location_2 = "in the right center drawer";
			}
			else
			{
				location_2 = "in the left center drawer";
			}
		}
		else if (locate[1] >= fridge_level[2])
		{
			location_2 = "in the second drawer from the floor";
		}
		else
		{
			location_2 = "in the bottom drawer";
		}
	}


	if (in_something(locate, bookshelf))
	{
		location_1 = "on the white bookshelf";
		furnitureString = "white bookshelf";

		if (locate[1] >= bookshelf_level[0])
		{
			location_2 = "on the first floor of the bookshelf";
		}
		else if (locate[1] >= bookshelf_level[1])
		{
			location_2 = "on the second floor of the bookshelf";
		}
		else if (locate[1] >= bookshelf_level[2])
		{
			location_2 = "on the third floor of the bookshelf";
		}
		else
		{
			location_2 = "on the bottom floor of the bookshelf";
		}
	}



	if (in_something(locate, wooden_table_and_chair))
	{
		if (locate[1] >= chair_level)
		{
			furnitureString = "wooden table";
			location_1 = "on the wooden table";
		}

		else
		{
			furnitureString = "the chair under the wooden table";
			location_1 = "on the chair under the wooden table";
		}
	}


	if (in_something(locate, bed))
	{
		location_1 = "on the bed";
		furnitureString = "bed";
	}

	if (in_something(locate, sidetable_and_mat))
	{
		if (locate[1] >= table_level[0])
		{
			furnitureString = "white sidetable";
			location_1 = "on the white sidetable";
		}
		else
		{
			if (locate[1] >= table_level[1])
			{
				furnitureString = "white sidetable";
				location_1 = "on the second layer of the white sidetable";
			}
			else
			{
				furnitureString = "blue mat";
				location_1 = " on the blue mat under the sidetable";
			}
		}
	}


	if (in_something(locate, glass_wagon_1))
	{
		location_1 = "on the glass wagon near the kitchen drawer";
		furnitureString = "glass wagon near the kitchen drawer";

		if (locate[1] >= wagon_level[0])
		{
			location_2 = "on the top of the glass wagon";
		}
		else if (locate[1] >= wagon_level[1])
		{
			location_2 = "on the first floor of the glass wagon";
		}
		else
		{
			location_2 = "on the second floor of the glass wagon";
		}
	}

	if (in_something(locate, glass_wagon_2))
	{
		location_1 = "on the second glass wagon near the bed";
		furnitureString = "second glass wagon near the bed";

		if (locate[1] >= wagon_level[0])
		{
			location_2 = "on the top of the glass wagon";
		}
		else if (locate[1] >= wagon_level[1])
		{
			location_2 = "on the first floor of the glass wagon";
		}
		else
		{
			location_2 = "on the second floor of the glass wagon";
		}
	}

	if (in_something(locate, glass_wagon_3))
	{
		location_1 = "on the glass wagon near the bed";
		furnitureString = "glass wagon near the bed";

		if (locate[1] >= wagon_level[0])
		{
			location_2 = "on the top of the glass wagon";
		}
		else if (locate[1] >= wagon_level[1])
		{
			location_2 = "on the first floor of the glass wagon";
		}
		else
		{
			location_2 = "on the second floor of the glass wagon";
		}
	}


	if (in_something(locate, TVbass))
	{
		location_1 = "on the TV bass";

		furnitureString = "TV bass";

		if (locate[1] > TVbass_level[0])
		{
			location_2 = "on the top of the TV bass";
		}

		else
		{
			location_2 = "on the second floor of the TV bass";
		}
	}

	if (in_something_round(locate, trashcan))
	{
		location_1 = "in the trashcan near the wooden table";

		furnitureString = "trashcan near the wooden table";
	}


}//layout1 end

if (environment == "LayoutHN02")
{
	double bed[4] = { 3.879,6.68,2.393,4.591 };

	double trashcan_1[5] = {  -3.293,0, 6.346,0.1655,0.437 };//r
	double trashcan_2[5] = {  3.64,0,4.27,0.1655,0.437 };//r

	double wooden_table_and_chair[4] = { -1.006,4.808,-1.938,-1.4485 };
	double chair_level = 0.4152;

	double counter_and_cabinet[4] = { 1.005, 6.8361, -1.46, 6.246 };
	double counter_and_cabinet_level[3] = { 1.3839, 0.8565, 0.35 };//r
	double counter[2] = { 0.173 ,-0.624 };
	double cabinet[3] = { 0.381 ,-0.242 ,-0.849};
	double cabinet_line = 1.50785;//r

	double blue_cupboard[4] = { -1.55,6.806,-3.0833,6.2 };
	double blue_cupboard_level[2] = { 1.229,0.72 };//2
	double cupboard[3] = { 0,0,0 };
	double cupboard_centerline = -2.3;
	double cupboard_leftside_line = 0.3;//r
	double cupboard_rightside_line = 0.555;//r
	double cupboard_rightside_cut = -1.937;
	double oven[4] = { -1.924,6.627,-2.1575, 6.448 };

	double TVbass[4] = { -3.348,2.932,-3.808,2.932 };
	double TVbass_level[2] = { 0.756,0.49 };//r

	double paper_box[4] = { -3.306,2.628,-3.77,2.078 };

	double glass_wagon[4] = { 3.884,-0.253,3.258,-0.865 };
	double wagon_level[2] = { 0.7286,0.394 };//r

	double shelf_1[4] = { 3.903,2.129,3.287,1.286 };
	double shelf_2[4] = { 2.348,6.813,1.504,6.178 };
	double shelf_level[3] = { 1.0259,0.624,0.221 };//r

	double sidetable[4] = { 3.955,3.713,3.261,2.166 };
	double table_level[2] = {0.493,0.226};//r

	double brown_mat[4] = { 0.261,6.207,-1.322,5.201 };    
	geometry_msgs::Point targetPosition = taskInfo.target_object.position;


	locate[0] = -targetPosition.y;
	locate[1] = targetPosition.z;
	locate[2] = targetPosition.x;

	if (in_something(locate, bed))
	{
		location_1 = "on the bed";
		furnitureString = "bed";
	}

	if (in_something_round(locate, trashcan_1))
	{
		location_1 = "in the trashcan near the bed";

		furnitureString = "trashcan near the bed";
	}

	if (in_something(locate, wooden_table_and_chair))
	{
		if (locate[1] >= chair_level)
		{
			furnitureString = "wooden table";
			location_1 = "on the wooden table";
		}

		else
		{
			furnitureString = "the chair under the wooden table";
			location_1 = "on the chair under the wooden table";
		}
	}


	if (in_something(locate, counter_and_cabinet))
	{

		if (locate[1] >= counter_and_cabinet_level[0])
		{
			location_1 = "in the wall cabinet";
			furnitureString = "wall cabinet ";

			if (locate[1]>cabinet_line)
			{
				location_3 = "on the upper layer ";
			}
			else
			{
				location_3 = "on the lower layer ";
			}


			if (locate[0]>cabinet[0])
			{
				location_2 = "in the first cabinet on the right";
			}
			else if (locate[0]>cabinet[1])
			{
				location_2 = "in the second cabinet on the right";
			}
			else if (locate[0]>cabinet[2])
			{
				location_2 = "in the third cabniet on the right";
			}

			else
			{
				location_2 = "in the first cabniet on the left";
			}
		}


		else if (locate[1] >= counter_and_cabinet_level[1])
		{
			location_1 = "on the top of the drawer and under the cabinet";
			furnitureString = "drawer";
		}

		else if (locate[1] >= counter_and_cabinet_level[2])
		{
			location_1 = "in the drawer";
			furnitureString = "drawer";

			if (locate[0] > counter[0])
			{
				location_2 = "in the first floor of the first drawer on the right";
			}

			else if (locate[0]>counter[1])
			{
				location_2 = "in the first floor of the second drawer on the right";

			}
			else
			{
				location_2 = "in the first floor of the third drawer on the right";
			}
		}

		else if (locate[1] >= counter_and_cabinet_level[3])
		{
			if (locate[0] > counter[0])
			{
				location_2 = "in the second floor of the first drawer on the right";
			}

			else if (locate[0]>counter[1])
			{
				location_2 = "in the second floor of the second drawer on the right";

			}

			else
			{
				location_2 = "in the second floor of the third drawer on the right";
			}
		}
	}

	if (in_something(locate, blue_cupboard))
	{
		location_1 = "in the blue cupboard";
		furnitureString = "blue cupboard";

		if (locate[1] >= blue_cupboard_level[0])
		{
			if (locate[0] > cupboard_centerline)
			{
				location_2 = "in the upper and right part of the cupboard";
			}
			else
			{
				location_2 = "in the upper and left part of the cupboard";
			}
		}

		else if (locate[1] >= blue_cupboard_level[1])
		{
			location_2 = "in the middle part of the cupboard";

			if (in_something(locate, oven))
			{
				location_3 = "in the small oven";

				furnitureString = "oven";
			}
		}

		else
		{
			if (locate[0] > cupboard_centerline)
			{
				location_2 = "in the lower and right part of the cupboard";
				if (locate[2] > cupboard_rightside_cut)
				{
					if (locate[1] > cupboard_rightside_line)
					{
						location_3 = "in the right drawer";
					}
					else
					{
						location_3 = "in the right chest";
					}
				}
				else
				{
					if (locate[1] > cupboard_rightside_line)
					{
						location_3 = "in the left drawer";
					}
					else
					{
						location_3 = "in the left chest";
					}
				}
			}

			else
			{
				location_2 = "in the lower and left part of the cupboard";

				if (locate[1] < cupboard_leftside_line)
				{
					location_3 = "in the bottom drawer";
				}
			}
		}
	}


	if (in_something_round(locate, trashcan_2))
	{
		location_1 = "in the trashcan near the blue capboard";

		furnitureString = "trashcan near the blue capboard";
	}


	if (in_something(locate, TVbass))
	{
		location_1 = "on the TV bass";

		furnitureString = "TV bass";

		if (locate[1] > TVbass_level[0])
		{
			location_2 = "on the top of the TV bass";
		}

		else
		{
			location_2 = "on the second floor of the TV bass";
		}
	}

	if (in_something(locate, paper_box))
	{
		location_1 = "in the paper box";
		furnitureString = "paper box";
	}

	if (in_something(locate, glass_wagon))
	{
		location_1 = "on the glass_wagon in the corner";
		furnitureString = "glass_wagon in the corner";

		if (locate[1] >= wagon_level[0])
		{
			location_2 = "on the top of the glass wagon";
		}
		else if (locate[1] >= wagon_level[1])
		{
			location_2 = "on the first floor of the glass wagon";
		}
		else
		{
			location_2 = "on the second floor of the glass wagon";
		}
	}


	if (in_something(locate, shelf_1))
	{
		location_1 = "on the wooden shelf near the white sidetable";
		furnitureString = "wooden shelf near the white side table";

		if (locate[1] >= shelf_level[0])
		{
			location_2 = "on the first floor of the shelf";
		}
		else if (locate[1] >= shelf_level[1])
		{
			location_2 = "on the second floor of the shelf";
		}
		else if (locate[1] >= shelf_level[2])
		{
			location_2 = "on the third floor of the shelf";
		}
		else
		{
			location_2 = "on the bottom floor of the shelf";
		}
	}

	if (in_something(locate, shelf_2))
	{
		location_1 = "on the wooden shelf near the bed";
		furnitureString = "wooden shelf near the bed";

		if (locate[1] >= shelf_level[0])
		{
			location_2 = "on the first floor of the shelf";
		}
		else if (locate[1] >= shelf_level[1])
		{
			location_2 = "on the first floor of the shelf";
		}
		else if (locate[1] >= shelf_level[2])
		{
			location_2 = "on the second floor of the shelf";
		}
		else
		{
			location_2 = "on the bottom floor of the shelf";
		}
	}

	/*if (in_something(locate, sidetable))
	{
		location_1 = "on the white sidetable";

		furnitureString = "white sidetable";
	}*/
	if (in_something(locate, sidetable))
	{
		if (locate[1] >= table_level[0])
		{
			furnitureString = "white sidetable";
			location_1 = "on the white sidetable";
		}
		else
		{
			
				furnitureString = "white sidetable";
				location_1 = "on the second layer of the white sidetable";
			
			
		}
	}
}

if (environment == "LayoutHN03")
{
	double bed[4] = { -0.08,6.708,-2.179,5.205 };

	double shelf_1[4] = { -3.179,6.746,-3.8137, 5.106 };
	double shelf_2[4] = { -3.184,5.104,-3.8137,3.459 };
	double shelf_level[3] = {1.0259,0.624,0.221 };//r
	double counter_and_cabinet[4] = { -3.255, 3.151, -3.867, 0.6804 };
	double counter_and_cabinet_level[3] = { 1.3839, 0.8565, 0.35 };//r
	double counter[2] = { 2.343 ,1.535 };
	double cabinet[3] = { 2.535 ,1.923 ,1.322 };
	double cabinet_line = 1.50785;//r

	double fridge[4] = { -3.178,0.644,-3.852,-0.034 };
	double fridge_level[3] = { 0.927,0.742,0.38 };//r
	double fridge_center_line = 0.214;

	double paper_box_1[4] = { 3.71,1.23,3.395,0.661 };//离门远的
	//double paper_box_1[4] = { 0,0,0,0 };
	double paper_box_2[4] = { 3.744,0.29,3.375,-0.273 };//离门近的

	double bookshelf[4] = { 3.9409,4.5673,3.475,3.6 };
	double bookshelf_level[3] = { 1.0281,0.6708,0.3512 };//r

	double TVbass[4] = { 3.79,6.722,2.803,4.78 };
	double TVbass_level[2] = { 0.756,0.49 };//r

	double sidetable_and_mat[4] = { 1.856,6.747,0.552,5.142 };
	double table_level[2] = {0.493,0.226};//r

	geometry_msgs::Point targetPosition = taskInfo.target_object.position;


	locate[0] = -targetPosition.y;
	locate[1] = targetPosition.z;
	locate[2] = targetPosition.x;


	if (in_something(locate, sidetable_and_mat))
	{
		if (locate[1] >= table_level[0])
		{
			furnitureString = "white sidetable";
			location_1 = "on the white sidetable";
		}
		else
		{
			if (locate[1] >= table_level[1])
			{
				furnitureString = "white sidetable";
				location_1 = "on the second layer of the white sidetable";
			}
			else
			{
				furnitureString = "blue mat";
				location_1 = " on the blue mat under the sidetable";
			}
		}
	}

	if (in_something(locate, bed))
	{
		location_1 = "on the bed";
		furnitureString = "bed";
	}

	if (in_something(locate, shelf_1))
	{
		location_1 = "on the wooden shelf inside";
		furnitureString = "wooden shelf inside";

		if (locate[1] >= shelf_level[0])
		{
			location_2 = "on the first floor of the shelf";
		}
		else if (locate[1] >= shelf_level[1])
		{
			location_2 = "on the second floor of the shelf";
		}
		else if (locate[1] >= shelf_level[2])
		{
			location_2 = "on the third floor of the shelf";
		}
		else
		{
			location_2 = "on the bottom floor of the shelf";
		}
	}

	if (in_something(locate, shelf_2))
	{
		location_1 = "on the wooden shelf outside";
		furnitureString = "wooden shelf outside";

		if (locate[1] >= shelf_level[0])
		{
			location_2 = "on the first floor of the shelf";
		}
		else if (locate[1] >= shelf_level[1])
		{
			location_2 = "on the second floor of the shelf";
		}
		else if (locate[1] >= shelf_level[2])
		{
			location_2 = "on the third floor of the shelf";
		}
		else
		{
			location_2 = "on the bottom floor of the shelf";
		}
	}

	if (in_something(locate, counter_and_cabinet))
	{

		if (locate[1] >= counter_and_cabinet_level[0])
		{
			location_1 = "in the wall cabinet";
			furnitureString = "wall cabinet ";

			if (locate[1]>cabinet_line)
			{
				location_3 = "on the upper layer ";
			}
			else
			{
				location_3 = "on the lower layer ";
			}


			if (locate[2]>cabinet[0])
			{
				location_2 = "in the first cabinet on the right";
			}
			else if (locate[2]>cabinet[1])
			{
				location_2 = "in the second cabinet on the right";
			}
			else if (locate[2]>cabinet[2])
			{
				location_2 = "in the third cabniet on the right";
			}

			else
			{
				location_2 = "in the first cabniet on the left";
			}
		}


		else if (locate[1] >= counter_and_cabinet_level[1])
		{
			location_1 = "on the top of the drawer and under the cabinet";
			furnitureString = "drawer";
		}

		else if (locate[1] >= counter_and_cabinet_level[2])
		{
			location_1 = "in the drawer";
			furnitureString = "drawer";

			if (locate[2] > counter[0])
			{
				location_2 = "in the first floor of the first drawer on the left";
			}

			else if (locate[2]>counter[1])
			{
				location_2 = "in the first floor of the second drawer on the left";

			}
			else
			{
				location_2 = "in the first floor of the third drawer on the left";
			}
		}

		else if (locate[1] >= counter_and_cabinet_level[3])
		{
			if (locate[2] > counter[0])
			{
				location_2 = "in the second floor of the first drawer on the left";
			}

			else if (locate[2]>counter[1])
			{
				location_2 = "in the second floor of the second drawer on the left";

			}

			else
			{
				location_2 = "in the second floor of the third drawer on the left";
			}
		}
	}

	if (in_something(locate, fridge))
	{
		location_1 = "in the fridge";
		furnitureString = "fridge";

		if (locate[1] > fridge_level[0])
		{
			location_2 = "in the upper part of the fridge";
		}
		else if (locate[1] > fridge_level[1])
		{
			if (locate[2] > fridge_center_line)
			{
				location_2 = "in the right center drawer";
			}
			else
			{
				location_2 = "in the left center drawer";
			}
		}
		else if (locate[1] >= fridge_level[2])
		{
			location_2 = "in the second drawer from the floor";
		}
		else
		{
			location_2 = "in the bottom drawer";
		}
	}


	if (in_something(locate,paper_box_1))
	{
		location_1 = "in the paper box near the door";
		furnitureString = "paper box near the door";
	}

	if (in_something(locate,paper_box_2))
	{
		location_1 = "in the paper box far from the door";
		furnitureString = "paper box far from the door";
	}

	if (in_something(locate, bookshelf))
	{
		location_1 = "on the white bookshelf";
		furnitureString = "white bookshelf";

		if (locate[1] >= bookshelf_level[0])
		{
			location_2 = "on the first floor of the bookshelf";
		}
		else if (locate[1] >= bookshelf_level[1])
		{
			location_2 = "on the second floor of the bookshelf";
		}
		else if (locate[1] >= bookshelf_level[2])
		{
			location_2 = "on the third floor of the bookshelf";
		}
		else
		{
			location_2 = "on the bottom floor of the bookshelf";
		}
	}

	if (in_something(locate, TVbass))
	{
		location_1 = "on the TV bass";

		furnitureString = "TV bass";

		if (locate[1] > TVbass_level[0])
		{
			location_2 = "on the top of the TV bass";
		}

		else
		{
			location_2 = "on the second floor of the TV bass";
		}
	}
}//layout3 end

if (environment == "LayoutHN04")
{
	double fridge[4] = { 2.535,6.849,1.8463,6.14 };
	double fridge_level[3] = { 0.927,0.742,0.38 };//r
	double fridge_center_line = 2.107;

	double counter_and_cabinet[4] = { 1.806, 6.847, -0.662, 6.24 };
	double counter_and_cabinet_level[3] = { 1.3839, 0.8565, 0.35 };//r
	double counter[2] = { 0.98 ,0.175 };
	double cabinet[3] = { 1.171 ,0.565 ,-0.041 };
	double cabinet_line = 1.50785;//r

	double glass_dining_table_with_chair[4] = { -0.309,5.544,-2.076,4.441 };
	double table_level = 0.756;

	double sidetable[4] = { -3.165,5.46,-3.885,3.905 };
	double sidetable_level[2] = {0.493,0.226};//r
	
	double brown_mat[4] = { 2.616,0.161,1.016,-0.851 };

	double blue_cupboard[4] = { 3.973,5.231,3.334,3.701 };
	double blue_cupboard_level[2] = { 1.229,0.72 };
	double cupboard[3] = { 0,0,0 };
	double cupboard_centerline = 2.249;
	double cupboard_leftside_line = 0.268;//高度
	double cupboard_rightside_line = 0.536;//
	double cupboard_rightside_cut = 2.623;

	double trashcan[5] = { 3.727,0,5.473,0.1655,0.437 };//r

	double paper_box[4] = { 3.745,6.626,3.382,6.031 };

	geometry_msgs::Point targetPosition = taskInfo.target_object.position;


	locate[0] = -targetPosition.y;
	locate[1] = targetPosition.z;
	locate[2] = targetPosition.x;

	if (in_something(locate, sidetable))
	{
		if (locate[1] >= sidetable_level[0])
		{
			furnitureString = "white sidetable";
			location_1 = "on the white sidetable";
		}
		else
		{
			
				furnitureString = "white sidetable";
				location_1 = "on the second layer of the white sidetable";
			
		}
	}

	if (in_something(locate, fridge))
	{
		location_1 = "in the fridge";
		furnitureString = "fridge";

		if (locate[1] > fridge_level[0])
		{
			location_2 = "in the upper part of the fridge";
		}
		else if (locate[1] > fridge_level[1])
		{
			if (locate[0] > fridge_center_line)
			{
				location_2 = "in the right center drawer";
			}
			else
			{
				location_2 = "in the left center drawer";
			}
		}
		else if (locate[1] >= fridge_level[2])
		{
			location_2 = "in the second drawer from the floor";
		}
		else
		{
			location_2 = "in the bottom drawer";
		}
	}

	if (in_something(locate, counter_and_cabinet))
	{

		if (locate[1] >= counter_and_cabinet_level[0])
		{
			location_1 = "in the wall cabinet";
			furnitureString = "wall cabinet ";

			if (locate[1]>cabinet_line)
			{
				location_3 = "on the upper layer ";
			}
			else
			{
				location_3 = "on the lower layer ";
			}


			if (locate[0]>cabinet[0])
			{
				location_2 = "in the first cabinet on the right";
			}
			else if (locate[0]>cabinet[1])
			{
				location_2 = "in the second cabinet on the right";
			}
			else if (locate[0]>cabinet[2])
			{
				location_2 = "in the third cabniet on the right";
			}

			else
			{
				location_2 = "in the first cabniet on the left";
			}
		}


		else if (locate[1] >= counter_and_cabinet_level[1])
		{
			location_1 = "on the top of the drawer and under the cabinet";
			furnitureString = "drawer";
		}

		else if (locate[1] >= counter_and_cabinet_level[2])
		{
			location_1 = "in the drawer";
			furnitureString = "drawer";

			if (locate[0] > counter[0])
			{
				location_2 = "in the first floor of the first drawer on the left";
			}

			else if (locate[0]>counter[1])
			{
				location_2 = "in the first floor of the second drawer on the left";

			}
			else
			{
				location_2 = "in the first floor of the third drawer on the left";
			}
		}

		else if (locate[1] >= counter_and_cabinet_level[3])
		{
			if (locate[0] > counter[0])
			{
				location_2 = "in the second floor of the first drawer on the left";
			}

			else if (locate[0]>counter[1])
			{
				location_2 = "in the second floor of the second drawer on the left";

			}

			else
			{
				location_2 = "in the second floor of the third drawer on the left";
			}
		}
	}



	if (in_something(locate, glass_dining_table_with_chair))
	{
		if (locate[1] >= table_level)
		{
			location_1 = "on the glass dining table";
			furnitureString = "glass dining table";
		}
		else
		{
			location_1 = "on the chair under the glass table";
			furnitureString = "on the chair under the table";
		}
	}

	if (in_something(locate, sidetable))
	{
		location_1 = "on the short sidetable";

		furnitureString = "short sidetable";
	}

	if (in_something(locate, blue_cupboard))
	{
		location_1 = "in the blue cupboard";
		furnitureString = "blue cupboard";

		if (locate[1] >= blue_cupboard_level[0])
		{
			if (locate[2] > cupboard_centerline)
			{
				location_2 = "in the upper and left part of the cupboard";
			}
			else
			{
				location_2 = "in the upper and right part of the cupboard";
			}
		}

		else if (locate[1] >= blue_cupboard_level[1])
		{
			location_2 = "in the middle part of the cupboard";
		}

		else
		{
			if (locate[2] < cupboard_centerline)
			{
				location_2 = "in the lower and right part of the cupboard";
				if (locate[2] < cupboard_rightside_cut)
				{
					if (locate[1] > cupboard_rightside_line)
					{
						location_3 = "in the right drawer";
					}
					else
					{
						location_3 = "in the right chest";
					}
				}
				else
				{
					if (locate[1] > cupboard_rightside_line)
					{
						location_3 = "in the left drawer";
					}
					else
					{
						location_3 = "in the left chest";
					}
				}
			}

			else
			{
				location_2 = "in the lower and left part of the cupboard";

				if (locate[1] < cupboard_leftside_line)
				{
					location_3 = "in the bottom drawer";
				}
			}
		}
	}

	if (in_something_round(locate, trashcan))
	{
		location_1 = "in the trashcan near the blue cupboard";
		furnitureString = "trashcan near the blue cupboard";
	}

	if (in_something(locate, paper_box))
	{
		location_1 = "in the paper box near fridge";
		furnitureString = "paper box near fridge";
	}
}
//layout4 end

if (environment == "LayoutHN05")
{
	double bed_1[4] = { -2.338,6.678, -3.823,4.59 };
	double bed_2[4] = { 3.857,6.69,2.37,4.6 };

	double bookshelf_1[4] = { -3.426,4.09,-3.65,3.606 };
	double bookshelf_2[4] = {  3.93,4.55,0.02699965,2.371  };
	double bookshelf_level[3] = { 1.0281,0.6708,0.3512 };//r

	double paper_box_1[4] = { -3.367,2.992,-3.711,2.435 };
	double paper_box_2[4] = { 3.782,2.764,3.407,2.225 };

	double glass_wagon_1[4] = { -1.65,6.764,-2.271,6.192 };
	double glass_wagon_2[4] = { 2.303,6.781,1.726,6.146 };
	double wagon_level[2] = { 0.7286,0.394 };//r

	double wooden_table_and_chair_1[4] = { 1.146,6.769,0.341,6.34 };//hei色的
	double wooden_table[4] = { -1.066,6.782,-0.357,5.532 };
	double chair[4] = { -1.013,6.54,-1.55,6.095 };
	double chair_level = 0.4152;

	//double wooden_table_and_chair_2[4] = { 0,0,0,0 };//黑色的

	double wooden_wagon_1[4] = { -0.383,5.442,-0.94,4.84};
	double wooden_wagon_2[4] = { 0.929,5.445,0.357,4.804 };

	double shelf[4] = { 0.317,6.776,-0.324,5.137 };
	double shelf_level[3] = { 1.0259,0.624,0.221 };//r

	geometry_msgs::Point targetPosition = taskInfo.target_object.position;


	locate[0] = -targetPosition.y;
	locate[1] = targetPosition.z;
	locate[2] = targetPosition.x;

	if (in_something(locate, paper_box_1))
	{
		location_1 = "in the left paper box ";
		furnitureString = "paper box on the left";
	}

	if (in_something(locate, paper_box_2))
	{
		location_1 = "in the right paper box";
		furnitureString = "paper box on the right";
	}

	if (in_something(locate, bed_1))
	{
		location_1 = "on the left bed";
		furnitureString = "bed on the left";
	}

	if (in_something(locate, bed_2))
	{
		location_1 = "on the right bed";
		furnitureString = "bed on the right";
	}

	if (in_something(locate, glass_wagon_1))
	{
		location_1 = "on the left glass wagon";
		furnitureString = "glass wagon on the left";

		if (locate[1] >= wagon_level[0])
		{
			location_2 = "on the top of the glass wagon";
		}
		else if (locate[1] >= wagon_level[1])
		{
			location_2 = "on the first floor of the glass wagon";
		}
		else
		{
			location_2 = "on the second floor of the glass wagon";
		}
	}

	if (in_something(locate, glass_wagon_2))
	{
		location_1 = "on the right glass wagon";
		furnitureString = "glass wagon on the right";

		if (locate[1] >= wagon_level[0])
		{
			location_2 = "on the top of the glass wagon";
		}
		else if (locate[1] >= wagon_level[1])
		{
			location_2 = "on the first floor of the glass wagon";
		}
		else
		{
			location_2 = "on the second floor of the glass wagon";
		}
	}

	if (in_something(locate, bookshelf_1))
	{
		location_1 = "on the left white bookshelf ";
		furnitureString = "white bookshelf on the left";

		if (locate[1] >= bookshelf_level[0])
		{
			location_2 = "on the first floor of the bookshelf";
		}
		else if (locate[1] >= bookshelf_level[1])
		{
			location_2 = "on the second floor of the bookshelf";
		}
		else if (locate[1] >= bookshelf_level[2])
		{
			location_2 = "on the third floor of the bookshelf";
		}
		else
		{
			location_2 = "on the bottom floor of the bookshelf";
		}
	}

	if (in_something(locate, bookshelf_2))
	{
		location_1 = "on the right white bookshelf ";
		furnitureString = "white bookshelf on the right";

		if (locate[1] >= bookshelf_level[0])
		{
			location_2 = "on the first floor of the bookshelf";
		}
		else if (locate[1] >= bookshelf_level[1])
		{
			location_2 = "on the second floor of the bookshelf";
		}
		else if (locate[1] >= bookshelf_level[2])
		{
			location_2 = "on the third floor of the bookshelf";
		}
		else
		{
			location_2 = "on the bottom floor of the bookshelf";
		}
	}

	if (in_something(locate, wooden_wagon_1))
	{
		location_1 = "on the left wooden wagon";
		furnitureString = "wooden wagon on the left";

		if (locate[1] >= wagon_level[0])
		{
			location_2 = "on the top of the wooden  wagon";
		}
		else if (locate[1] >= wagon_level[1])
		{
			location_2 = "on the first floor of the wooden wagon";
		}
		else
		{
			location_2 = "on the second floor of the wooden wagon";
		}
	}

	if (in_something(locate, wooden_wagon_2))
	{
		location_1 = "on the right wooden wagon";
		furnitureString = "wooden wagon on the right";

		if (locate[1] >= wagon_level[0])
		{
			location_2 = "on the top of the wooden  wagon";
		}
		else if (locate[1] >= wagon_level[1])
		{
			location_2 = "on the first floor of the wooden wagon";
		}
		else
		{
			location_2 = "on the second floor of the wooden wagon";
		}
	}

	if (in_something(locate, wooden_table_and_chair_1))//白色的椅子和桌子
	{
		if (locate[1] >= chair_level)
		{
			furnitureString = "wooden table near the white chair";
			location_1 = "on the right wooden table near the white chair";
		}

		else
		{
			furnitureString = "the white chair under the wooden table";
			location_1 = "on the white chair under the wooden table";
		}
	}

	

	if (in_something(locate, wooden_table))
	{
		location_1 = "on the left wooden table near the black chair";
		furnitureString = "the wooden table on the left near the black chair";
	}

	if (in_something(locate, chair))
	{
		location_1 = "on the white chair";
		furnitureString = "white chair on the left";
	}

	if (in_something(locate, shelf))
	{
		location_1 = "on the wooden shelf";
		furnitureString = "wooden shelf";

		if (locate[1] >= shelf_level[0])
		{
			location_2 = "on the first floor of the shelf";
		}
		else if (locate[1] >= shelf_level[1])
		{
			location_2 = "on the second floor of the shelf";
		}
		else if (locate[1] >= shelf_level[2])
		{
			location_2 = "on the third floor of the shelf";
		}
		else
		{
			location_2 = "on the bottom floor of the shelf";
		}
	}

}//layout5 end

if (environment == "LayoutHN06")
{
	double table_1[4] = { -0.598,5.693,-1.829,4.284 };
	double table_2[4] = { 1.369,5.693,0.188,4.274 };

	double glass_wagon[4] = { 0.191,5.666,-0.602,4.274 };
	double wagon_level[2] = { 0.713,0.41 };

	double shelf[4] = { -3.171,6.735,-3.8,3.363 };
	double shelf_level[3] = { 1.413,1.028,0.618 };

	double counter_and_cabinet[4] = { -3.227, 2.186, -3.815, -0.234 };
	double counter_and_cabinet_level[3] = { 1.3839, 0.8565, 0.35 };
	double counter[2] = { 1.401 ,0.592 };
	double cabinet[3] = { 1.6 ,0.997 ,0.393 };
	double cabinet_line = 1.50785;

	double paper_box_1[4] = { 3.859,-0.171,3.324,-0.687 };
	double paper_box_2[4] = { 3.88,0.765,0.324,0.012 };

	double trashcan_1[5] = { 3.774,0,2.327,0.2,0 };
	double trashcan_2[5] = { 3.763,0,1.589,0.2,0 };

	double wooden_dining_table[4] = { 3.88,5.246,2.886,2.959 };

	double bookshelf[4] = { 3.927,6.765,2.955,6.337 };
	double bookshelf_level[3] = { 1.029,0.678,0.361 };

	double paper_box_3[4] = { 2.838,6.691,2.119,6.12 };

	geometry_msgs::Point targetPosition = taskInfo.target_object.position;


	locate[0] = -targetPosition.y;
	locate[1] = targetPosition.z;
	locate[2] = targetPosition.x;

	if (in_something(locate, table_1))
	{
		location_1 = "on the left wooden table near shelf";
		furnitureString = "table near shelf";
	}

	if (in_something(locate, glass_wagon))
	{
		location_1 = "on the glass_wagon";
		furnitureString = "glass_wagon";

		if (locate[1] >= wagon_level[0])
		{
			location_2 = "on the top of the glass wagon";
		}
		else if (locate[1] >= wagon_level[1])
		{
			location_2 = "on the first floor of the glass wagon";
		}
		else
		{
			location_2 = "on the second floor of the glass wagon";
		}
	}

	if (in_something(locate, table_2))
	{
		location_1 = "on the right wooden table";
		furnitureString = "table on the right";
	}

	if (in_something(locate, shelf))
	{
		location_1 = "on the four-in-one shelf";
		furnitureString = "four-in-one shelf";
	}

	if (in_something(locate, counter_and_cabinet))
	{

		if (locate[1] >= counter_and_cabinet_level[0])
		{
			location_1 = "in the wall cabinet";
			furnitureString = "wall cabinet ";

			if (locate[1]>cabinet_line)
			{
				location_3 = "on the upper layer ";
			}
			else
			{
				location_3 = "on the lower layer ";
			}


			if (locate[2]>cabinet[0])
			{
				location_2 = "in the first cabinet on the right";
			}
			else if (locate[2]>cabinet[1])
			{
				location_2 = "in the second cabinet on the right";
			}
			else if (locate[2]>cabinet[2])
			{
				location_2 = "in the third cabniet on the right";
			}

			else
			{
				location_2 = "in the first cabniet on the left";
			}
		}

		
		else if (locate[1] >= counter_and_cabinet_level[1])
		{
			location_1 = "on the top of the drawer and under the cabinet";
			furnitureString = "drawer";
		}

		else if (locate[1] >= counter_and_cabinet_level[2])
		{
			location_1 = "in the drawer";
			furnitureString = "drawer";

			if (locate[2] > counter[0])
			{
				location_2 = "in the first floor of the first drawer on the right";
			}

			else if (locate[2]>counter[1])
			{
				location_2 = "in the first floor of the second drawer on the right";

			}
			else
			{
				location_2 = "in the first floor of the third drawer on the right";
			}
		}

		else if (locate[1] >= counter_and_cabinet_level[3])
		{
			if (locate[2] > counter[0])
			{
				location_2 = "in the second floor of the first drawer on the right";
			}

			else if (locate[2]>counter[1])
			{
				location_2 = "in the second floor of the second drawer on the right";

			}

			else
			{
				location_2 = "in the second floor of the third drawer on the right";
			}
		}
	}

	if (in_something(locate, paper_box_1))
	{
		location_1 = "in the paper box near the door";
		furnitureString = "paper box near the door";
	}

	if (in_something(locate, paper_box_2))
	{
		location_1 = "in the paper box near the trashcan";
		furnitureString = "paper box near the trashcan";
	}

	if (in_something_round(locate, trashcan_1))
	{
		location_1 = " in the trashcan on the left";
		furnitureString = "trashcan on the left";
	}

	if (in_something_round(locate, trashcan_2))
	{
		location_1 = " in the trashcan on the right";
		furnitureString = "trashcan on the right";
	}

	if (in_something(locate, wooden_dining_table))
	{
		location_1 = "on the wooden dinning table";
		furnitureString = "wooden dining table";
	}


	if (in_something(locate, bookshelf))
	{
		location_1 = "on the white bookshelf";
		furnitureString = "white bookshelf";

		if (locate[1] >= bookshelf_level[0])
		{
			location_2 = "on the first floor of the bookshelf";
		}
		else if (locate[1] >= bookshelf_level[1])
		{
			location_2 = "on the second floor of the bookshelf";
		}
		else if (locate[1] >= bookshelf_level[2])
		{
			location_2 = "on the third floor of the bookshelf";
		}
		else
		{
			location_2 = "on the bottom floor of the bookshelf";
		}
	}

	if (in_something(locate, paper_box_3))
	{
		location_1 = "in the paper box near the white bookshelf";

		furnitureString = "paper box near the white bookshelf";
	}


}

if (environment == "LayoutHN07")
{
	double blue_cupboard[4] = { -0.194,6.669,-1.739,6.196 };
	double blue_cupboard_level[2] = { 1.229,0.72 };
	double cupboard[3] = { 0,0,0 };
	double cupboard_centerline = -0.992;
	double cupboard_leftside_line = 0.268;
	double cupboard_rightside_line = 0.536;
	double cupboard_rightside_cut = -0.596;

	double counter_and_cabinet[4] = { 2.319,6.749,-0.14,6.222 };
	double counter_and_cabinet_level[3] = { 1.3839, 0.8565, 0.35 };
	double counter[2] = { 1.505 ,0.709 };
	double cabinet[3] = { 1.505 ,1.08 ,1.696 };
	double cabinet_line = 1.50785;

	double trashcan_1[5] = { -2.394,0,6.616,0.2,0 };
	double trashcan_2[5] = { -2.806,0,6.611,0.2,0 };
	double trashcan_3[5] = { -3.209,0,6.605,0.2,0 };

	double bed[4] = { -3.861,6.681,-5.381,4.621 };

	double TVbass[4] = { -3.48,3.393,-5.456,2.456 };
	double TVbass_level[2] = { 0.77,0.502 };

	double paper_box[4] = { -4.697,1.757,-5.395,-3.41 };

	double glass_wagon[4] = { -4.832,-0.276,-5.404,-0.849 };
	double wagon_level[2] = { 0.715,0.461 };

	double brown_mat[4] = { 0.988,0.161,-0.561,-0.844 };

	double sidetable[4] = { 2.35,4.265,1.666,2.754 };
	double sidetable_level = 0.471;

	double oven[4] = { -0.341,6.637,-0.83,0.267 };

	double glass_dining_table[4] = { -0.532,5.291,-2.832,4.135 };

	geometry_msgs::Point targetPosition = taskInfo.target_object.position;


	locate[0] = -targetPosition.y;
	locate[1] = targetPosition.z;
	locate[2] = targetPosition.x;

	if (in_something(locate, glass_dining_table))
	{
		//cout<<"11111111111111111"<<endl;
		location_1 = "on the glass dinning table";
		furnitureString = "glass dining table";
	}

	if (in_something(locate, brown_mat))
	{
		location_1 = "on the brown mat";

		furnitureString = "brown mat";
	}

	if (in_something(locate, sidetable))
	{
		location_1 = "on the short sidetable";

		furnitureString = "short sidetable";
		if (locate[1] > sidetable_level)
		{
			location_2 = "on the top of the sidetable";
		}
		else
		{
			location_2 = "on the second floor of the side table";
		}
	}

	if (in_something_round(locate, trashcan_1))
	{
		location_1 = "in the first trashcan on the right";

		furnitureString = "first trashcan on the right";
	}

	if (in_something_round(locate, trashcan_2))
	{
		location_1 = "in the second trashcan on the right";

		furnitureString = "second trashcan on the right";
	}

	if (in_something_round(locate, trashcan_3))
	{
		location_1 = "in the first trashcan on the left";

		furnitureString = "first trashcan on the left";
	}

	if (in_something(locate, TVbass))
	{
		location_1 = "on the TV bass";

		furnitureString = "TV bass";

		if (locate[1] > TVbass_level[0])
		{
			location_2 = "on the top of the TV bass";
		}

		else
		{
			location_2 = "on the second floor of the TV bass";
		}
	}

	if (in_something(locate, glass_wagon))
	{
		location_1 = "on the glass_wagon";
		furnitureString = "glass_wagon";

		if (locate[1] >= wagon_level[0])
		{
			location_2 = "on the top of the glass wagon";
		}
		else if (locate[1] >= wagon_level[1])
		{
			location_2 = "on the first floor of the glass wagon";
		}
		else
		{
			location_2 = "on the second floor of the glass wagon";
		}
	}

	if (in_something(locate, paper_box))
	{
		location_1 = "in the paper box behind the TV bass";
		furnitureString = "paper box behind the TV bass";
	}

	if (in_something(locate, counter_and_cabinet))
	{

		if (locate[1] >= counter_and_cabinet_level[0])
		{
			location_1 = "in the wall cabinet";
			furnitureString = "wall cabinet ";

			if (locate[1]>cabinet_line)
			{
				location_3 = "on the upper layer ";
			}
			else
			{
				location_3 = "on the lower layer ";
			}


			if (locate[0]>cabinet[0])
			{
				location_2 = "in the first cabinet on the right";
			}
			else if (locate[0]>cabinet[1])
			{
				location_2 = "in the second cabinet on the right";
			}
			else if (locate[0]>cabinet[2])
			{
				location_2 = "in the third cabniet on the right";
			}

			else
			{
				location_2 = "in the first cabniet on the left";
			}
		}


		else if (locate[1] >= counter_and_cabinet_level[1])
		{
			location_1 = "on the top of the drawer and under the cabinet";
			furnitureString = "drawer";
		}

		else if (locate[1] >= counter_and_cabinet_level[2])
		{
			location_1 = "in the drawer";
			furnitureString = "drawer";

			if (locate[0] > counter[0])
			{
				location_2 = "in the first floor of the first drawer on the right";
			}

			else if (locate[0]>counter[1])
			{
				location_2 = "in the first floor of the second drawer on the right";

			}
			else
			{
				location_2 = "in the first floor of the third drawer on the right";
			}
		}

		else if (locate[1] >= counter_and_cabinet_level[3])
		{
			if (locate[0] > counter[0])
			{
				location_2 = "in the second floor of the first drawer on the right";
			}

			else if (locate[0]>counter[1])
			{
				location_2 = "in the second floor of the second drawer on the right";

			}

			else
			{
				location_2 = "in the second floor of the third drawer on the right";
			}
		}
	}

	if (in_something(locate, blue_cupboard))
	{
		location_1 = "in the blue cupboard";
		furnitureString = "blue cupboard";

		if (locate[1] >= blue_cupboard_level[0])
		{
			if (locate[0] > cupboard_centerline)
			{
				location_2 = "in the upper and right part of the cupboard";
			}
			else
			{
				location_2 = "in the upper and left part of the cupboard";
			}
		}

		else if (locate[1] >= blue_cupboard_level[1])
		{
			location_2 = "in the middle part of the cupboard";
			if (in_something(locate, oven))
			{
				location_1 = "in the oven";
				furnitureString = "oven";
			}
		}

		else
		{
			if (locate[0] > cupboard_centerline)
			{
				location_2 = "in the lower and right part of the cupboard";
				if (locate[2] > cupboard_rightside_cut)
				{
					if (locate[1] > cupboard_rightside_line)
					{
						location_3 = "in the right drawer";
					}
					else
					{
						location_3 = "in the right chest";
					}
				}
				else
				{
					if (locate[1] > cupboard_rightside_line)
					{
						location_3 = "in the left drawer";
					}
					else
					{
						location_3 = "in the left chest";
					}
				}
			}

			else
			{
				location_2 = "in the lower and left part of the cupboard";

				if (locate[1] < cupboard_leftside_line)
				{
					location_3 = "in the bottom drawer";
				}
			}
		}
	}
}

if (environment == "LayoutHN08")
{
	double round_table_1[5] = { 3.241,0.767,3.21,0.32,0 };
	double round_table_2[5] = { 3.118,0,0.767,0.32,0 };
	double round_table_3[5] = { 3.26,0,0.767,0.32,0 };
	double round_table_4[5] = { 03.231,0,0.767,0.32,0 };

	double cornersofa_1[4] = { -3.032,0.798,-3.756,-0.039 };
	double cornersofa_2[4] = { -2.973,3.211,-3.824,2.325 };
	
	double glass_wagon[4] = { -3.222,1.887,-3.824,1.273 };
	double wagon_level[2] = { 0.679,0.362 };

	double drawer_alone_1[4] = { 3.96,4.649,1.462,4.03 };
	double drawer_1[2] = { -2.179,-2.993 };
	double drawer_alone_level[2]  = { 0.89,0.373 };

	double drawer_alone_2[4] = { 3.925,4.649,1.498,4.03 };
	double drawer_2[2] = { 3.112,2.319 };

	double shelf[4] = { -1.318,6.549,-3.802,5.919 };
	double shelf_level[3] = { 0,0,0 };

	double fridge_1[4] = { -0.522,6.584,-1.217,5.856 };
	double fridge_level[3] = { 0.927,0.742,0.38 };
	double fridge_1_center_line = -0.951;

	double fridge_2[4] = { 0.215,6.576,-0.474,5.856 };
	double fridge_2_center_line = -0.202;

	double wall_cabinet[4] = { 3.985,6.584,1.507,6.087 };
	double wall_cabinet_level = 1.401;
	double wall_cabinet_line = 0;
	double cabinet[3] = { 3.347,2.76,2.108 };
	double kitchen_drawer[4] = { 3.95,6.576,0.263,5.982 };
	double kitchen_drawer_cut[3] = { 3.012,2.164,1.227 };
	double kitchen_drawer_level[3] = { 0.86,0.608,0.329 };

	geometry_msgs::Point targetPosition = taskInfo.target_object.position;


	locate[0] = -targetPosition.y;
	locate[1] = targetPosition.z;
	locate[2] = targetPosition.x;

	if (in_something(locate, shelf))
	{
		location_1 = "on the wooden shelf";
		furnitureString = "wooden shelf";

		if (locate[1] >= shelf_level[0])
		{
			location_2 = "on the first floor of the shelf";
		}
		else if (locate[1] >= shelf_level[1])
		{
			location_2 = "on the second floor of the shelf";
		}
		else if (locate[1] >= shelf_level[2])
		{
			location_2 = "on the third floor of the shelf";
		}
		else
		{
			location_2 = "on the bottom floor of the shelf";
		}
	}

	if (in_something(locate, drawer_alone_1))
	{	
		furnitureString = "drawer next to the shelf";
		if (locate[1] > drawer_alone_level[0])
		{
			location_1 = "on the top of the drawer next to the shelf";
			location_2 = "on the top of the drawer";
			furnitureString = "drawer next to the shelf";
		}
		else if (locate[1] > drawer_alone_level[1])
		{
			location_1 = "in the drawer next to the shelf";
			if (locate[0] >= drawer_1[0])
			{
				location_2 = "in the first floor and the first drawer on the left";
			}
			else if (locate[0] >= drawer_1[1])
			{
				location_2 = "in the first floor and the second drawer on the left";
			}
			else
			{
				location_2 = "in the first floor and the first drawer on the right";
			}
		}

		else
		{
			location_1 = "in the drawer next to the shelf";
			if (locate[0] >= drawer_1[0])
			{
				location_2 = "in the second floor and the first drawer on the left";
			}
			else if (locate[0] >= drawer_1[1])
			{
				location_2 = "in the second floor and the second drawer on the left";
			}
			else
			{
				location_2 = "in the second floor and the first drawer on the right";
			}
		}
	}

	if (in_something(locate, drawer_alone_2))
	{
		furnitureString = "drawer next to the cabinet";
		if (locate[1] > drawer_alone_level[0])
		{
			location_1 = "on the top of the drawer next to the cabniet";
			location_2 = "on the top of the drawer";
			furnitureString = "drawer next to the cabinet";
		}
		else if (locate[1] > drawer_alone_level[1])
		{
			location_1 = "in the drawer next to the cabinet";
			if (locate[0] >= drawer_2[0])
			{
				location_2 = "in the first floor and the first drawer on the left";
			}
			else if (locate[0] >= drawer_2[1])
			{
				location_2 = "in the first floor and the second drawer on the left";
			}
			else
			{
				location_2 = "in the first floor and the first drawer on the right";
			}
		}

		else
		{
			location_1 = "in the drawer next to the cabinet";
			if (locate[0] >= drawer_2[0])
			{
				location_2 = "in the second floor and the first drawer on the left";
			}
			else if (locate[0] >= drawer_2[1])
			{
				location_2 = "in the second floor and the second drawer on the left";
			}
			else
			{
				location_2 = "in the second floor and the first drawer on the right";
			}
		}
	}

	if (in_something(locate, fridge_2))
	{
		location_1 = "in the fridge near the oven";
		furnitureString = "fridge near the oven";

		if (locate[1] > fridge_level[0])
		{
			location_2 = "in the upper part of the fridge";
		}
		else if (locate[1] > fridge_level[1])
		{
			if (locate[2] > fridge_2_center_line)
			{
				location_2 = "in the right center drawer";
			}
			else
			{
				location_2 = "in the left center drawer";
			}
		}
		else if (locate[1] >= fridge_level[2])
		{
			location_2 = "in the second drawer from the floor";
		}
		else
		{
			location_2 = "in the bottom drawer";
		}
	}

	if (in_something(locate, fridge_1))
	{
		location_1 = "in the fridge near the shelf";
		furnitureString = "fridge near the shelf";

		if (locate[1] > fridge_level[0])
		{
			location_2 = "in the upper part of the fridge";
		}
		else if (locate[1] > fridge_level[1])
		{
			if (locate[2] > fridge_1_center_line)
			{
				location_2 = "in the right center drawer";
			}
			else
			{
				location_2 = "in the left center drawer";
			}
		}
		else if (locate[1] >= fridge_level[2])
		{
			location_2 = "in the second drawer from the floor";
		}
		else
		{
			location_2 = "in the bottom drawer";
		}
	}


	if (in_something(locate, kitchen_drawer))
	{
		if (locate[1] > wall_cabinet_level)
		{
			location_1 = "in the wall cabinet";
			furnitureString = "wall cabinet";
			if (locate[0] > cabinet[0])
			{
				location_2 = "in the first wall cabinet on the right";
			}
			else if (locate[0] > cabinet[1])
			{
				location_2 = "in the second wall cabinet on the right";
			}
			else if (locate[0] > cabinet[2])
			{
				location_2 = "in the third wall cabinet on the right";
			}
			else
			{
				location_2 = "in the first wall cabinet on the left";
			}
		}

		else if (locate[1] > kitchen_drawer[0])
		{
			location_1 = "on the top of the kitchen drawer";
			furnitureString = "kitchen drawer";
			location_2 = "on the top of the drawer and under the cabinet";
		}

		else if (locate[1] > kitchen_drawer[1])
		{
			location_1 = "in the kitchen drawer under the cabinet";
			furnitureString = "kitchen drawer";
			if (locate[0] > kitchen_drawer_cut[0])
			{
				location_2 = "in the first floor and the first drawer on the right";
			}
			else if (locate[0] > kitchen_drawer_cut[1])
			{
				location_2 = "in the first floor and the second drawer on the right";
			}
			else if (locate[0] > kitchen_drawer_cut[2])
			{
				location_2 = "in the first floor and the third drawer on the right";
			}
			else
			{
				location_2 = "in the first floor and the first drawer on the left";
			}
		}

		else if (locate[1] > kitchen_drawer[2])
		{
			location_1 = "in the kitchen drawer under the cabinet";
			furnitureString = "kitchen drawer";
			if (locate[0] > kitchen_drawer_cut[0])
			{
				location_2 = "in the first floor and the first drawer on the right";
			}
			else if (locate[0] > kitchen_drawer_cut[1])
			{
				location_2 = "in the second floor and the second drawer on the right";
			}
			else if (locate[0] > kitchen_drawer_cut[2])
			{
				location_2 = "in the second floor and the third drawer on the right";
			}
			else
			{
				location_2 = "in the second floor and the first drawer on the left";
			}
		}
		else
		{
			location_1 = "in the kitchen drawer under the cabinet";
			furnitureString = "kitchen drawer";
			if (locate[0] > kitchen_drawer_cut[0])
			{
				location_2 = "in the third floor and the first drawer on the right";
			}
			else if (locate[0] > kitchen_drawer_cut[1])
			{
				location_2 = "in the third floor and the second drawer on the right";
			}
			else if (locate[0] > kitchen_drawer_cut[2])
			{
				location_2 = "in the third floor and the third drawer on the right";
			}
			else
			{
				location_2 = "in the third floor and the first drawer on the left";
			}
		}
	}

	if (in_something(locate, glass_wagon))
	{
		location_1 = "on the glass_wagon";
		furnitureString = "glass_wagon";

		if (locate[1] >= wagon_level[0])
		{
			location_2 = "on the top of the glass wagon";
		}
		else if (locate[1] >= wagon_level[1])
		{
			location_2 = "on the first floor of the glass wagon";
		}
		else
		{
			location_2 = "on the second floor of the glass wagon";
		}
	}

	if (in_something(locate, cornersofa_1))
	{
		location_1 = "on the corner sofa on the left";

		furnitureString = "corner sofa on the left";
	}

	if (in_something(locate, cornersofa_2))
	{
		location_1 = "on the corner sofa on the left";

		furnitureString = "corner sofa on the left";
	}

	if (in_something_round(locate, round_table_1))
	{
		location_1 = "on the first round table on the left";
		furnitureString = "first round table on the left";
	}

	if (in_something_round(locate, round_table_2))
	{
		location_1 = "on the second round table on the left";
		furnitureString = "second round table on the left";
	}

	if (in_something_round(locate, round_table_3))
	{
		location_1 = "on the third round table on the left";
		furnitureString = "third round table on the left";
	}

	if (in_something_round(locate, round_table_4))
	{
		location_1 = "on the first round table on the right";
		furnitureString = "first round table on the right";
	}
}

if (environment == "LayoutHN09")
{
	double round_table_1[5] = { 0.376,0,6.412,0.32,0 };
	double round_table_2[5] = { 1.63,0,5.27,0.32,0 };
	double round_table_3[5] = { 0.338,0,4.261,0.32,0 };
	double round_table_4[5] = { 0.338,0,4.261,0.32,0 };

	double wooden_table_1[4] = { -4.526,0.724,-5.792,0.004 };
	double wooden_table_2[4] = { -4.526,2.541,-5.792,1.821 };

	double trashcan_1[5] = { -5.522,0,6.455,0.2,0 };
	double trashcan_2[5] = { -5.014,0,6.455,0.2,0 };

	double counter_and_cabinet[4] = { -3.346, 4.176, -5.82, 3.561 };
	double counter_and_cabinet_level[3] = { 1.3839, 0.8565, 0.35 };
	double counter[2] = { -4.178 ,-4.967 };
	double cabinet[3] = { -4.024 ,-4.606 ,-5.16 };
	double cabinet_line = 1.401;

	double wooden_wagon[4] = { -1.642,4.912,-2.249,4.297 };
	double wagon_level[2] = { 0.679,0.362 };

	double fridge[4] = { -4.009,6.789,-4.697,6.086 };
	double fridge_level[3] = { 0.927,0.742,0.38 };
	double fridge_center_line = -4.429;

	double brown_mat[4] = { 0.666,0.116,-0.928,-0.897 };

	double wall_cabinet[4] = { -1.731,4.912,-2.249,4.297 };
	double wall_cabinet_level = 1.401;
	double wall_cabinet_line = 0;
	double cabinetWall[3] = { 6.062,5.459,4.856 };
	double kitchen_drawer[4] = { -1.6758,6.766,-2.225,4.9391 };
	double kitchen_drawer_cut = 5.856;
	double kitchen_drawer_level[3] = { 0.914,0.661,0.417 };

	geometry_msgs::Point targetPosition = taskInfo.target_object.position;


	locate[0] = -targetPosition.y;
	locate[1] = targetPosition.z;
	locate[2] = targetPosition.x;

	if (in_something(locate, fridge))
	{
		location_1 = "in the fridge";
		furnitureString = "fridge";

		if (locate[1] > fridge_level[0])
		{
			location_2 = "in the upper part of the fridge";
		}
		else if (locate[1] > fridge_level[1])
		{
			if (locate[0] > fridge_center_line)
			{
				location_2 = "in the right center drawer";
			}
			else
			{
				location_2 = "in the left center drawer";
			}
		}
		else if (locate[1] >= fridge_level[2])
		{
			location_2 = "in the second drawer from the floor";
		}
		else
		{
			location_2 = "in the bottom drawer";
		}
	}

	if (in_something_round(locate, trashcan_1))
	{
		location_1 = "in the trashcan on the left";

		furnitureString = "trashcan on the left";
	}

	if (in_something_round(locate, trashcan_2))
	{
		location_1 = "in the trashcan on the right";

		furnitureString = "trashcan on the right";
	}

	if (in_something(locate, counter_and_cabinet))
	{

		if (locate[1] >= counter_and_cabinet_level[0])
		{
			location_1 = "in the wall cabinet on the faucet";
			furnitureString = "wall cabinet on the faucet";

			if (locate[1]>cabinet_line)
			{
				location_3 = "on the upper layer ";
			}
			else
			{
				location_3 = "on the lower layer ";
			}


			if (locate[0]>cabinet[0])
			{
				location_2 = "in the first cabinet on the left";
			}
			else if (locate[0]>cabinet[1])
			{
				location_2 = "in the second cabinet on the left";
			}
			else if (locate[0]>cabinet[2])
			{
				location_2 = "in the third cabniet on the left";
			}

			else
			{
				location_2 = "in the first cabniet on the right";
			}
		}


		else if (locate[1] >= counter_and_cabinet_level[1])
		{
			location_1 = "on the top of the drawer and under the cabinet";
			furnitureString = "drawer under the cabinet";
		}

		else if (locate[1] >= counter_and_cabinet_level[2])
		{
			location_1 = "in the drawer";
			furnitureString = "drawer under the cabinet";

			if (locate[0] > counter[0])
			{
				location_2 = "in the first floor of the first drawer on the left";
			}

			else if (locate[0]>counter[1])
			{
				location_2 = "in the first floor of the second drawer on the left";

			}
			else
			{
				location_2 = "in the first floor of the third drawer on the left";
			}
		}

		else if (locate[1] >= counter_and_cabinet_level[3])
		{
			if (locate[0] > counter[0])
			{
				location_2 = "in the second floor of the first drawer on the left";
			}

			else if (locate[0]>counter[1])
			{
				location_2 = "in the second floor of the second drawer on the left";

			}

			else
			{
				location_2 = "in the second floor of the third drawer on the left";
			}
		}
	}

	if (in_something(locate, wooden_table_1))
	{
		location_1 = "on the left wooden table";

		furnitureString = "wooden table on the left";
	}

	if (in_something(locate, wooden_table_2))
	{
		location_1 = "on the right wooden table";

		furnitureString = "wooden table on the right";
	}

	if (in_something(locate, brown_mat))
	{
		location_1 = "on the brown mat";

		furnitureString = "brown mat";
	}


	if (in_something_round(locate, round_table_1))
	{
		location_1 = "on the round table";
		furnitureString = "round table";
	}

	if (in_something_round(locate, round_table_2))
	{
		location_1 = "on the round table";
		furnitureString = "round table";
	}

	if (in_something_round(locate, round_table_3))
	{
		location_1 = "on the round table";
		furnitureString = "round table";
	}

	if (in_something_round(locate, round_table_4))
	{
		location_1 = "on the round table";
		furnitureString = "round table";
	}


	if (in_something(locate, wall_cabinet))
	{
		if (locate[1] > wall_cabinet_level)
		{
			location_1 = "in the wall cabinet above the wagon";
			furnitureString = "wall cabinet above the wagon";

			if (locate[2] > cabinetWall[0])
			{
				location_2 = "in the first cabinet on the left";
			}
			else if (locate[2] > cabinetWall[1])
			{
				location_2 = "in the second cabinet on the left";
			}
			else if (locate[2] > cabinetWall[2])
			{
				location_2 = "in the third cabinet on the left";
			}
			else
			{
				location_2 = "in the first cabinet on the right";
			}
		}
		else
		{
			if (in_something(locate, kitchen_drawer))
			{
				location_1 = "in the kitchen drawer near the wagon";

				furnitureString = "kitchen drawer near the wagon";

				if (locate[1] > kitchen_drawer_level[0])
				{
					location_2 = "on the top of the kitchen drawer";
				}
				else if(locate[1] > kitchen_drawer_level[1])
				{
					if (locate[3] > kitchen_drawer_cut)
					{
						location_2 = "in the first floor and the left drawer";

					}
					else
					{
						location_2 = "in the first floor and the right drawer";
					}

				}
				else if (locate[1] > kitchen_drawer_level[2])
				{
					if (locate[3] > kitchen_drawer_cut)
					{
						location_2 = "in the second floor and the left drawer";

					}
					else
					{
						location_2 = "in the second floor and the right drawer";
					}
				}
				else
				{
					if (locate[3] > kitchen_drawer_cut)
					{
						location_2 = "in the third floor and the left drawer";

					}
					else
					{
						location_2 = "in the third floor and the right drawer";
					}
				}
			}
			if (in_something(locate, wooden_wagon))
			{
				location_1 = "on the wooden wagon";
				furnitureString = "wooden wagon";

				if (locate[1] >= wagon_level[0])
				{
					location_2 = "on the top of the wooden wagon";
				}
				else if (locate[1] >= wagon_level[1])
				{
					location_2 = "on the first floor of the wooden wagon";
				}
				else
				{
					location_2 = "on the second floor of the wooden wagon";
				}
			}
		}
	}
}

//ÓÐÉ¾¼õ
if (environment == "LayoutHN10")
{
	double wooden_table_1[4] = { -4.581,2.556,-5.886,1.447 };
	double wooden_table_2[4] = { -4.588,4.341,-5.886,3.261 };
	double wooden_table_3[4] = { -4.588,0.471,-5.886,-0.138 };

	double shelf[4] = { -5.054,6.675,-5.897,1.833 };
	double shelf_level[3] = { 1.424,1.024,0.647 };

	double fridge[4] = { -3.106,6.721,-3.847,5.96 };
	double fridge_level[3] = { 0.927,0.742,0.38 };
	double fridge_center_line = -3.551;

	double paper_box[4] = { -3.847,6.675,-4.689,5.96 };

	double glass_wagon[4] = { -1.838,6.721,-3.106,6.082 };
	double wagon_level[2] = { 0.72,0.41 };

	double wall_cabinet[4] = { 1.838,6.674,-2.777,6.403 };
	double wall_cabinet_level = 1.401;
	double wall_cabinet_line = 0;
	double cabinetWall[7] = { 1.277,0.665,0.073,-0.565,-1.198,-1.795,-2.408 };
	double kitchen_drawer_level[3] = { 0.85,0.558,0.349 };
	double kitchen_drawer_line[3] = { 0.957,0.047,-0.899 };

	double counter_and_cabinet[4] = { 1.866,4.779,-3.052,4.17 };
	double counter_and_cabinet_level[3] = { 1.3839, 0.8565, 0.35 };
	double counter[5] = { 1.034,0.231,-0.601,-1.43,-2.243 };
	double cabinet[7] = { 1.255,0.649,0.056,-0.582,-1.214,-1.803,-2.42 };
	double cabinet_line = 1.50785;

	

	geometry_msgs::Point targetPosition = taskInfo.target_object.position;


	locate[0] = -targetPosition.y;
	locate[1] = targetPosition.z;
	locate[2] = targetPosition.x;

	if (in_something(locate, wooden_table_1))
	{
		location_1 = "on the short wooden table";

		furnitureString = "wooden table";
	}

	if (in_something(locate, counter_and_cabinet))
	{

		if (locate[1] >= counter_and_cabinet_level[0])
		{
			location_1 = "in the wall cabinet";
			furnitureString = "wall cabinet ";

			if (locate[1]>cabinet_line)
			{
				location_3 = "on the upper layer ";
			}
			else
			{
				location_3 = "on the lower layer ";
			}


			if (locate[0]>cabinet[0])
			{
				location_2 = "in the first cabinet on the left";
			}
			else if (locate[0]>cabinet[1])
			{
				location_2 = "in the second cabinet on the left";
			}
			else if (locate[0]>cabinet[2])
			{
				location_2 = "in the third cabniet on the left";
			}
			else if (locate[0]>cabinet[3])
			{
				location_2 = "in the fourth cabniet on the left";
			}
			else if (locate[0]>cabinet[4])
			{
				location_2 = "in the fifth cabniet on the left";
			}
			else if (locate[0]>cabinet[5])
			{
				location_2 = "in the sixth cabniet on the left";
			}
			else if (locate[0]>cabinet[6])
			{
				location_2 = "in the seventh cabniet on the left";
			}
			else
			{
				location_2 = "in the first cabniet on the right";
			}
		}


		else if (locate[1] >= counter_and_cabinet_level[1])
		{
			location_1 = "on the top of the drawer and under the cabinet";
			furnitureString = "drawer";
		}

		else if (locate[1] >= counter_and_cabinet_level[2])
		{
			location_1 = "in the drawer";
			furnitureString = "drawer";

			if (locate[0] > counter[0])
			{
				location_2 = "in the first floor of the first drawer on the left";
			}

			else if (locate[0]>counter[1])
			{
				location_2 = "in the first floor of the second drawer on the left";

			}
			else if (locate[0]>counter[2])
			{
				location_2 = "in the first floor of the third drawer on the left";

			}
			else if (locate[0]>counter[3])
			{
				location_2 = "in the first floor of the fourth drawer on the left";

			}
			else if (locate[0]>counter[4])
			{
				location_2 = "in the first floor of the fifth drawer on the left";

			}
			else
			{
				location_2 = "in the first floor of the sixth drawer on the left";
			}
		}

		else if (locate[1] >= counter_and_cabinet_level[3])
		{
			if (locate[0] > counter[0])
			{
				location_2 = "in the second floor of the first drawer on the left";
			}

			else if (locate[0]>counter[1])
			{
				location_2 = "in the second floor of the second drawer on the left";

			}
			else if (locate[0]>counter[2])
			{
				location_2 = "in the second floor of the third drawer on the left";

			}
			else if (locate[0]>counter[3])
			{
				location_2 = "in the second floor of the fourth drawer on the left";

			}
			else if (locate[0]>counter[4])
			{
				location_2 = "in the second floor of the fifth drawer on the left";

			}
			else
			{
				location_2 = "in the second floor of the sixth drawer on the left";
			}
		}
	}

	if (in_something(locate, wooden_table_2))
	{
		location_1 = "on the short wooden table";

		furnitureString = "wooden table";
	}

	if (in_something(locate, wooden_table_3))
	{
		location_1 = "on the short wooden table";

		furnitureString = "wooden table";
	}

	if (in_something(locate, shelf))
	{
		location_1 = "on the wooden shelf";
		furnitureString = "wooden shelf";

		if (locate[1] >= shelf_level[0])
		{
			location_2 = "on the first floor of the shelf";
		}
		else if (locate[1] >= shelf_level[1])
		{
			location_2 = "on the first floor of the shelf";
		}
		else if (locate[1] >= shelf_level[2])
		{
			location_2 = "on the second floor of the shelf";
		}
		else
		{
			location_2 = "on the bottom floor of the shelf";
		}
	}

	if (in_something(locate, fridge))
	{
		location_1 = "in the fridge";
		furnitureString = "fridge";

		if (locate[1] > fridge_level[0])
		{
			location_2 = "in the upper part of the fridge";
		}
		else if (locate[1] > fridge_level[1])
		{
			if (locate[0] > fridge_center_line)
			{
				location_2 = "in the right center drawer";
			}
			else
			{
				location_2 = "in the left center drawer";
			}
		}
		else if (locate[1] >= fridge_level[2])
		{
			location_2 = "in the second drawer from the floor";
		}
		else
		{
			location_2 = "in the bottom drawer";
		}
	}

	if (in_something(locate, paper_box))
	{
		location_1 = "in the paper box";
		furnitureString = "paper box";
	}

	if (in_something(locate, wall_cabinet))
	{
		if (locate[1] > wall_cabinet_level)
		{
			location_1 = "in the cabinet above the wagon";
			furnitureString = "cabinet above the wagon";

			if (locate[0] > cabinetWall[0])
			{
				location_2 = "in the first cabinet on the right";
			}
			else if(locate[0] > cabinetWall[1])
			{
				location_2 = "in the second cabinet on the right";

			}
			else if (locate[0] > cabinetWall[2])
			{
				location_2 = "in the third cabinet on the right";

			}
			else if (locate[0] > cabinetWall[3])
			{
				location_2 = "in the forth cabinet on the right";

			}
			else if (locate[0] > cabinetWall[4])
			{
				location_2 = "in the fifth cabinet on the right";

			}

			else if (locate[0] > cabinetWall[5])
			{
				location_2 = "in the sixth cabinet on the right";

			}

			else if (locate[0] > cabinetWall[6])
			{
				location_2 = "in the seventh cabinet on the right";

			}
			else
			{
				location_2 = "in the first cabinet on the left";

			}
		}
		else
		{
			if (in_something(locate, glass_wagon))
			{
				location_1 = "on the glass_wagon";
				furnitureString = "glass_wagon";

				if (locate[1] >= wagon_level[0])
				{
					location_2 = "on the top of the glass wagon";
				}
				else if (locate[1] >= wagon_level[1])
				{
					location_2 = "on the first floor of the glass wagon";
				}
				else
				{
					location_2 = "on the second floor of the glass wagon";
				}
			}
			else
			{
				location_1 = "in the kitchen drawer next to the wagon";
				furnitureString = "kitchen drawer next to the wagon";
				if (locate[1] > kitchen_drawer_level[0])
				{
					location_2 = "on the top of the drawer which has a toaster";
				}
				else if (locate[1] > kitchen_drawer_level[1])
				{
					if (locate[0] > kitchen_drawer_line[0])
					{
						location_2 = "in the first floor and the first drawer on the right";
					}
					else if (locate[0] > kitchen_drawer_line[1])
					{
						location_2 = "in the first floor and the second drawer on the right";
					}
					else if (locate[0] > kitchen_drawer_line[2])
					{
						location_2 = "in the first floor and the third drawer on the right";
					}
					else
					{
						location_2 = "in the first floor and the first drawer on the left";
					}
				}
				else if (locate[1] > kitchen_drawer_level[2])
				{
					if (locate[0] > kitchen_drawer_line[0])
					{
						location_2 = "in the second floor and the first drawer on the right";
					}
					else if (locate[0] > kitchen_drawer_line[1])
					{
						location_2 = "in the second floor and the second drawer on the right";
					}
					else if (locate[0] > kitchen_drawer_line[2])
					{
						location_2 = "in the second floor and the third drawer on the right";
					}
					else
					{
						location_2 = "in the second floor and the first drawer on the left";
					}
				}
				else
				{
					if (locate[0] > kitchen_drawer_line[0])
					{
						location_2 = "in the third floor and the first drawer on the right";
					}
					else if (locate[0] > kitchen_drawer_line[1])
					{
						location_2 = "in the third floor and the second drawer on the right";
					}
					else if (locate[0] > kitchen_drawer_line[2])
					{
						location_2 = "in the third floor and the third drawer on the right";
					}
					else
					{
						location_2 = "in the third floor and the first drawer on the left";
					}
				}
			}
		}
	}

}

if (environment == "LayoutHN11")
{
	double wooden_table_1[4] = { -1.1651,5.84,-2.4379,5.1187 };
	double wooden_table_2[4] = { 1.229,5.843,-0.058,5.108 };

	double shelf[4] = { -3.16,6.63,-3.849,5.056 };
	double shelf_level[3] = { 1.328,0.95,0.56 };

	double brown_mat[4] = { 0,0,0,0 };

	double counter_and_cabinet_1[4] = { -3.189, 5.042, -3.812, 2.551 };
	double counter_and_cabinet_level[3] = { 1.3839, 0.8565, 0.35 };
	double counter_1[2] = { 0.567 ,-0.17 };
	double cabinet_1[3] = { 0.751 ,0.195 ,-0.412 };
	double cabinet_line = 1.401;

	double counter_and_cabinet_2[4] = { 0, 3.87101, 3.30582, 1.44405 };
	//double counter_and_cabinet_level[3] = { 1.3839, 0.8565, 0.35 };
	double counter_2[2] = { 4.179 ,3.383 };
	double cabinet_2[3] = { 4.391 ,3.788 ,3.202 };

	double fridge[4] = { -5.173,4.732,-5.852,4.015 };
	double fridge_level[3] = { 0.927,0.742,0.38 };
	double fridge_center_line = 4.262;

	double glass_wagon[4] = { -5.304,3.885,-5.881,2.603 };
	double wagon_level[2] = { 0.589,0.295 };

	double wall_cabinet[4] = { -5.317,3.896,-5.88,-976 };
	double wall_cabinet_level = 1.401;
	double wall_cabinet_line = 0;
	double cabinet[7] = { 3.341,2.692,2.078,1.451,0.855,0.252,-0.36 };
	double kitchen_drawer_level[3] = { 0.822,0.536,0.279 };
	double kitchen_drawer_line[3] = { 1.698,0.771,-0.14 };

	double trashcan[5] = { -5.687,0,5.802,0.2,0 };

	double paper_box[4] = { -5.224,5.611,-5.882,4.783 };

	geometry_msgs::Point targetPosition = taskInfo.target_object.position;


	locate[0] = -targetPosition.y;
	locate[1] = targetPosition.z;
	locate[2] = targetPosition.x;

	if (in_something(locate, wall_cabinet))
	{
		if (locate[1] > wall_cabinet_level)
		{
			location_1 = "in the cabinet above the wagon";
			furnitureString = "cabinet above the wagon";

			if (locate[2] > cabinet[0])
			{
				location_2 = "in the first cabinet on the right";
			}
			else if (locate[2] > cabinet[1])
			{
				location_2 = "in the second cabinet on the right";

			}
			else if (locate[2] > cabinet[2])
			{
				location_2 = "in the third cabinet on the right";

			}
			else if (locate[2] > cabinet[3])
			{
				location_2 = "in the forth cabinet on the right";

			}
			else if (locate[2] > cabinet[4])
			{
				location_2 = "in the fifth cabinet on the right";

			}

			else if (locate[2] > cabinet[5])
			{
				location_2 = "in the sixth cabinet on the right";

			}

			else if (locate[2] > cabinet[6])
			{
				location_2 = "in the seventh cabinet on the right";

			}
			else
			{
				location_2 = "in the first cabinet on the left";

			}
		}
		else
		{
			if (in_something(locate, glass_wagon))
			{
				location_1 = "on the glass_wagon";
				furnitureString = "glass_wagon";

				if (locate[1] >= wagon_level[0])
				{
					location_2 = "on the top of the glass wagon";
				}
				else if (locate[1] >= wagon_level[1])
				{
					location_2 = "on the first floor of the glass wagon";
				}
				else
				{
					location_2 = "on the second floor of the glass wagon";
				}
			}
			else
			{
				location_1 = "in the kitchen drawer next to the wagon";
				furnitureString = "kitchen drawer next to the wagon";
				if (locate[1] > kitchen_drawer_level[0])
				{
					location_2 = "on the top of the drawer which has a toaster";
				}
				else if (locate[1] > kitchen_drawer_level[1])
				{
					if (locate[2] > kitchen_drawer_line[0])
					{
						location_2 = "in the first floor and the first drawer on the right";
					}
					else if (locate[2] > kitchen_drawer_line[1])
					{
						location_2 = "in the first floor and the second drawer on the right";
					}
					else if (locate[2] > kitchen_drawer_line[2])
					{
						location_2 = "in the first floor and the third drawer on the right";
					}
					else
					{
						location_2 = "in the first floor and the first drawer on the left";
					}
				}
				else if (locate[1] > kitchen_drawer_level[2])
				{
					if (locate[2] > kitchen_drawer_line[0])
					{
						location_2 = "in the second floor and the first drawer on the right";
					}
					else if (locate[2] > kitchen_drawer_line[1])
					{
						location_2 = "in the second floor and the second drawer on the right";
					}
					else if (locate[2] > kitchen_drawer_line[2])
					{
						location_2 = "in the second floor and the third drawer on the right";
					}
					else
					{
						location_2 = "in the second floor and the first drawer on the left";
					}
				}
				else
				{
					if (locate[0] > kitchen_drawer_line[0])
					{
						location_2 = "in the third floor and the first drawer on the right";
					}
					else if (locate[2] > kitchen_drawer_line[1])
					{
						location_2 = "in the third floor and the second drawer on the right";
					}
					else if (locate[2] > kitchen_drawer_line[2])
					{
						location_2 = "in the third floor and the third drawer on the right";
					}
					else
					{
						location_2 = "in the third floor and the first drawer on the left";
					}
				}
			}
		}
	}

	if (in_something(locate, counter_and_cabinet_1))
	{

		if (locate[1] >= counter_and_cabinet_level[0])
		{
			location_1 = "in the wall cabinet";
			furnitureString = "wall cabinet ";

			if (locate[1]>cabinet_line)
			{
				location_3 = "on the upper layer ";
			}
			else
			{
				location_3 = "on the lower layer ";
			}


			if (locate[2]>cabinet_1[0])
			{
				location_2 = "in the first cabinet on the left";
			}
			else if (locate[2]>cabinet_1[1])
			{
				location_2 = "in the second cabinet on the left";
			}
			else if (locate[2]>cabinet_1[2])
			{
				location_2 = "in the third cabniet on the left";
			}

			else
			{
				location_2 = "in the first cabniet on the right";
			}
		}


		else if (locate[1] >= counter_and_cabinet_level[1])
		{
			location_1 = "on the top of the drawer and under the cabinet";
			furnitureString = "drawer";
		}

		else if (locate[1] >= counter_and_cabinet_level[2])
		{
			location_1 = "in the drawer";
			furnitureString = "drawer";

			if (locate[2] > counter_1[0])
			{
				location_2 = "in the first floor of the first drawer on the left";
			}

			else if (locate[2]>counter_1[1])
			{
				location_2 = "in the first floor of the second drawer on the left";

			}
			else
			{
				location_2 = "in the first floor of the third drawer on the left";
			}
		}

		else if (locate[1] >= counter_and_cabinet_level[3])
		{
			if (locate[2] > counter_1[0])
			{
				location_2 = "in the second floor of the first drawer on the left";
			}

			else if (locate[2]>counter_1[1])
			{
				location_2 = "in the second floor of the second drawer on the left";

			}

			else
			{
				location_2 = "in the second floor of the third drawer on the left";
			}
		}
	}

	if (in_something(locate, counter_and_cabinet_2))
	{

		if (locate[1] >= counter_and_cabinet_level[0])
		{
			location_1 = "in the wall cabinet";
			furnitureString = "wall cabinet ";

			if (locate[1]>cabinet_line)
			{
				location_3 = "on the upper layer ";
			}
			else
			{
				location_3 = "on the lower layer ";
			}


			if (locate[2]>cabinet_2[0])
			{
				location_2 = "in the first cabinet on the left";
			}
			else if (locate[2]>cabinet_2[1])
			{
				location_2 = "in the second cabinet on the left";
			}
			else if (locate[2]>cabinet_2[2])
			{
				location_2 = "in the third cabniet on the left";
			}

			else
			{
				location_2 = "in the first cabniet on the right";
			}
		}


		else if (locate[1] >= counter_and_cabinet_level[1])
		{
			location_1 = "on the top of the drawer and under the cabinet";
			furnitureString = "drawer";
		}

		else if (locate[1] >= counter_and_cabinet_level[2])
		{
			location_1 = "in the drawer";
			furnitureString = "drawer";

			if (locate[2] > counter_2[0])
			{
				location_2 = "in the first floor of the first drawer on the left";
			}

			else if (locate[2]>counter_2[1])
			{
				location_2 = "in the first floor of the second drawer on the left";

			}
			else
			{
				location_2 = "in the first floor of the third drawer on the left";
			}
		}

		else if (locate[1] >= counter_and_cabinet_level[3])
		{
			if (locate[2] > counter_2[0])
			{
				location_2 = "in the second floor of the first drawer on the left";
			}

			else if (locate[2]>counter_2[1])
			{
				location_2 = "in the second floor of the second drawer on the left";

			}

			else
			{
				location_2 = "in the second floor of the third drawer on the left";
			}
		}
	}

	if (in_something(locate, fridge))
	{
		location_1 = "in the fridge";
		furnitureString = "fridge";

		if (locate[1] > fridge_level[0])
		{
			location_2 = "in the upper part of the fridge";
		}
		else if (locate[1] > fridge_level[1])
		{
			if (locate[2] > fridge_center_line)
			{
				location_2 = "in the right center drawer";
			}
			else
			{
				location_2 = "in the left center drawer";
			}
		}
		else if (locate[1] >= fridge_level[2])
		{
			location_2 = "in the second drawer from the floor";
		}
		else
		{
			location_2 = "in the bottom drawer";
		}
	}


	if (in_something(locate, shelf))
	{
		location_1 = "on the wooden shelf";
		furnitureString = "wooden shelf";

		if (locate[1] >= shelf_level[0])
		{
			location_2 = "on the first floor of the shelf";
		}
		else if (locate[1] >= shelf_level[1])
		{
			location_2 = "on the second floor of the shelf";
		}
		else if (locate[1] >= shelf_level[2])
		{
			location_2 = "on the third floor of the shelf";
		}
		else
		{
			location_2 = "on the bottom floor of the shelf";
		}
	}

	if (in_something(locate, wooden_table_1))
	{
		location_1 = "on the left wooden table";

		furnitureString = "wooden table on the left";
	}

	if (in_something(locate, wooden_table_2))
	{
		location_1 = "on the right wooden table";

		furnitureString = "wooden table on the right";
	}
}

if (environment == "LayoutHN12")
{
	double shelf[4] = { 0.964,6.843,-1.558,6.227 };
	double shelf_level[3] = { 1.43,1.04,0.65 };

	double fridge[4] = { 2.143,6.876,1.453,6.167 };
	double fridge_level[3] = { 0.927,0.742,0.38 };
	double fridge_center_line = 1.694;

	double blue_cupboard[4] = { 3.746,6.903,2.191,6.249 };
	double blue_cupboard_level[2] = { 1.229,0.72 };
	double cupboard[3] = { 0,0,0 };
	double cupboard_centerline = 2.952;
	double cupboard_leftside_line = 0.268;
	double cupboard_rightside_line = 0.536;
	double cupboard_rightside_cut = 3.32;

	double counter_and_cabinet[4] = { 6.203, 6.91, 3.746, 6.283 };
	double counter_and_cabinet_level[3] = { 1.3839, 0.8565, 0.35 };
	double counter[2] = { 5.384 ,4.583 };
	double cabinet[3] = { 5.585 ,4.944 ,4.343 };
	double cabinet_line = 1.50785;

	double round_table_1[5] = { 5.199,0,3.128,0.32,0 };
	double round_table_2[5] = { 4.323,0,1.835,0.32,0 };
	double round_table_3[5] = { 5.14,0,0.521,0.32,0 };

	double kitchen_drawer[4] = { 6.171,4.65,0.7,4.078 };
	double kitchen_drawer_level[3] = { 0.878,0.624,0.364 };
	double kitchen_drawer_cut[5] = { 5.241,4.355,3.426,2.521,1.605 };

	double wall_cabinet[7] = { 5.558,4.939,4.351,3.715,3.08,2.486,1.89 };
	double wall_cabinet_line = 1.401;

	geometry_msgs::Point targetPosition = taskInfo.target_object.position;


	locate[0] = -targetPosition.y;
	locate[1] = targetPosition.z;
	locate[2] = targetPosition.x;

	if (in_something(locate, kitchen_drawer))
	{
		if (locate[1] > cabinet_line)
		{
			location_1 = "in the wall cabinet";
			furnitureString = "wall cabinet";

			if (locate[0] > wall_cabinet[0])
			{
				location_2 = "in the first wall cabinet on the left";
			}
			else if (locate[0] > wall_cabinet[1])
			{
				location_2 = "in the second wall cabinet on the left";
			}
			else if (locate[0] > wall_cabinet[2])
			{
				location_2 = "in the third wall cabinet on the left";
			}
			else if (locate[0] > wall_cabinet[3])
			{
				location_2 = "in the forth wall cabinet on the left";
			}
			else if (locate[0] > wall_cabinet[4])
			{
				location_2 = "in the fifth wall cabinet on the left";
			}
			else if (locate[0] > wall_cabinet[5])
			{
				location_2 = "in the sixth wall cabinet on the left";
			}
			else if (locate[0] > wall_cabinet[6])
			{
				location_2 = "in the second wall cabinet on the right";
			}
			else
			{
				location_2 = "in the first wall cabinet on the right";
			}
		}
		else
		{
			location_1 = "in the kitchen drawer under the cabinet";
			furnitureString = "kitchen drawer under the cabinet";

			if (locate[1] > kitchen_drawer_level[0])
			{
				location_2 = "on the top of the kitchen drawer";
			}
			else if (locate[1] > kitchen_drawer_level[1])
			{
				if (locate[0] > kitchen_drawer_cut[0])
				{
					location_2 = "in the first floor and the first drawer on the left";
				}
				else if (locate[0] > kitchen_drawer_cut[1])
				{
					location_2 = "in the first floor and the second drawer on the left";
				}
				else if (locate[0] > kitchen_drawer_cut[2])
				{
					location_2 = "in the first floor and the third drawer on the left";
				}
				else if (locate[0] > kitchen_drawer_cut[3])
				{
					location_2 = "in the first floor and the forth drawer on the left";
				}
				else if (locate[0] > kitchen_drawer_cut[4])
				{
					location_2 = "in the first floor and the fifth drawer on the left";
				}
				else
				{
					location_2 = "in the first floor and the first drawer on the right";
				}
			}
			else if (locate[1] > kitchen_drawer_level[2])
			{
				if (locate[0] > kitchen_drawer_cut[0])
				{
					location_2 = "in the second floor and the first drawer on the left";
				}
				else if (locate[0] > kitchen_drawer_cut[1])
				{
					location_2 = "in the second floor and the second drawer on the left";
				}
				else if (locate[0] > kitchen_drawer_cut[2])
				{
					location_2 = "in the second floor and the third drawer on the left";
				}
				else if (locate[0] > kitchen_drawer_cut[3])
				{
					location_2 = "in the second floor and the forth drawer on the left";
				}
				else if (locate[0] > kitchen_drawer_cut[4])
				{
					location_2 = "in the second floor and the fifth drawer on the left";
				}
				else
				{
					location_2 = "in the second floor and the first drawer on the right";
				}
			}
			else
			{
				if (locate[0] > kitchen_drawer_cut[0])
				{
					location_2 = "in the third floor and the first drawer on the left";
				}
				else if (locate[0] > kitchen_drawer_cut[1])
				{
					location_2 = "in the third floor and the second drawer on the left";
				}
				else if (locate[0] > kitchen_drawer_cut[2])
				{
					location_2 = "in the third floor and the third drawer on the left";
				}
				else if (locate[0] > kitchen_drawer_cut[3])
				{
					location_2 = "in the third floor and the forth drawer on the left";
				}
				else if (locate[0] > kitchen_drawer_cut[4])
				{
					location_2 = "in the third floor and the fifth drawer on the left";
				}
				else
				{
					location_2 = "in the third floor and the first drawer on the right";
				}
			}
		}
	}

	if (in_something_round(locate, round_table_1))
	{
		location_1 = "on the left round table";
		furnitureString = "round table on the left";
	}
	if (in_something_round(locate, round_table_2))
	{
		location_1 = "on the middle round table";
		furnitureString = "round table in the middle";
	}

	if (in_something_round(locate, round_table_3))
	{
		location_1 = "on the right round table";
		furnitureString = "round table on the right";
	}

	if (in_something(locate, fridge))
	{
		location_1 = "in the fridge";
		furnitureString = "fridge";

		if (locate[1] > fridge_level[0])
		{
			location_2 = "in the upper part of the fridge";
		}
		else if (locate[1] > fridge_level[1])
		{
			if (locate[0] > fridge_center_line)
			{
				location_2 = "in the right center drawer";
			}
			else
			{
				location_2 = "in the left center drawer";
			}
		}
		else if (locate[1] >= fridge_level[2])
		{
			location_2 = "in the second drawer from the floor";
		}
		else
		{
			location_2 = "in the bottom drawer";
		}
	}

	if (in_something(locate, blue_cupboard))
	{
		location_1 = "in the blue cupboard";
		furnitureString = "blue cupboard";

		if (locate[1] >= blue_cupboard_level[0])
		{
			if (locate[0] > cupboard_centerline)
			{
				location_2 = "in the upper and right part of the cupboard";
			}
			else
			{
				location_2 = "in the upper and left part of the cupboard";
			}
		}

		else if (locate[1] >= blue_cupboard_level[1])
		{
			location_2 = "in the middle part of the cupboard";
		}

		else
		{
			if (locate[0] > cupboard_centerline)
			{
				location_2 = "in the lower and right part of the cupboard";
				if (locate[0] > cupboard_rightside_cut)
				{
					if (locate[1] > cupboard_rightside_line)
					{
						location_3 = "in the right drawer";
					}
					else
					{
						location_3 = "in the right chest";
					}
				}
				else
				{
					if (locate[1] > cupboard_rightside_line)
					{
						location_3 = "in the left drawer";
					}
					else
					{
						location_3 = "in the left chest";
					}
				}
			}

			else
			{
				location_2 = "in the lower and left part of the cupboard";

				if (locate[1] < cupboard_leftside_line)
				{
					location_3 = "in the bottom drawer";
				}
			}
		}
	}

	if (in_something(locate, counter_and_cabinet))
	{

		if (locate[1] >= counter_and_cabinet_level[0])
		{
			location_1 = "in the wall cabinet";
			furnitureString = "wall cabinet ";

			if (locate[1]>cabinet_line)
			{
				location_3 = "on the upper layer ";
			}
			else
			{
				location_3 = "on the lower layer ";
			}


			if (locate[0]>cabinet[0])
			{
				location_2 = "in the first cabinet on the right";
			}
			else if (locate[0]>cabinet[1])
			{
				location_2 = "in the second cabinet on the right";
			}
			else if (locate[0]>cabinet[2])
			{
				location_2 = "in the third cabniet on the right";
			}

			else
			{
				location_2 = "in the first cabniet on the left";
			}
		}


		else if (locate[1] >= counter_and_cabinet_level[1])
		{
			location_1 = "on the top of the drawer and under the cabinet";
			furnitureString = "drawer";
		}

		else if (locate[1] >= counter_and_cabinet_level[2])
		{
			location_1 = "in the drawer";
			furnitureString = "drawer";

			if (locate[0] > counter[0])
			{
				location_2 = "in the first floor of the first drawer on the right";
			}

			else if (locate[0]>counter[1])
			{
				location_2 = "in the first floor of the second drawer on the right";

			}
			else
			{
				location_2 = "in the first floor of the third drawer on the right";
			}
		}

		else if (locate[1] >= counter_and_cabinet_level[3])
		{
			if (locate[0] > counter[0])
			{
				location_2 = "in the second floor of the first drawer on the right";
			}

			else if (locate[0]>counter[1])
			{
				location_2 = "in the second floor of the second drawer on the right";

			}

			else
			{
				location_2 = "in the second floor of the third drawer on the right";
			}
		}
	}

}

if (environment == "LayoutHN13")
{
	double round_table_1[5] = { -4.04,0.92,2.2,0.32,0 };
	double round_table_2[5] = { -4.05,0.92,1.341,0.32,0 };
	double round_table_3[5] = { -5.46,0.92,-0.57,0.32,0 };
	double round_table_4[5] = { -5.437,0.92,1.341,0.32,0 };
	double round_table_5[5] = { -1.351,0.92,5.11,0.32,0 };
	double round_table_6[5] = { 1.602,0.92,5.146,0.32,0 };


	double glass_wagon_1[4] = { -0.294,6.786,-0.897,6.193 };
	double glass_wagon_2[4] = { 0.943,6.79,0.34,6.193 };
	double wooden_wagon[4] = { -1.642,4.912,-2.249,4.297 };
	double wooden_wagon_1[4] = { -3.843,6.752,-4.443,6.149 };
	double wagon_level[2] = { 0.679,0.362 };

	double fridge[4] = { -4.491,6.778,-5.176,6.09 };
	double fridge_level[3] = { 0.927,0.742,0.38 };
	double fridge_center_line = -4.942;

	double counter_and_cabinet[4] = { -3.346,4.176,-5.82,3.561 };
	double counter_and_cabinet_level[3] = { 1.3839, 0.8565, 0.35 };
	double counter[2] = { -4.185 ,-4.987 };
	double cabinet[3] = { -3.979 ,-4.558 ,-5.169 };
	double cabinet_line = 1.673;

	

	double wall_cabinet[4] = { -1.731,4.912,-2.249,4.297 };
	double wall_cabinet_level = 1.401;
	double wall_cabinet_line = 0;
	double cabinetWall[3] = { 6.062,5.459,4.856 };
	double kitchen_drawer[4] = { -1.6758,6.766,-2.225,4.9391 };
	double kitchen_drawer_cut = 5.856;
	double kitchen_drawer_level[3] = { 0.914,0.661,0.417 };

	double trashcan[5] = { -3.612,0,6.577,0.2,0 };

	geometry_msgs::Point targetPosition = taskInfo.target_object.position;


	locate[0] = -targetPosition.y;
	locate[1] = targetPosition.z;
	locate[2] = targetPosition.x;

	if (in_something_round(locate, round_table_1))
	{
		location_1 = "on the round cafe set";
		furnitureString = "round cafe set";
	}

	if (in_something_round(locate, round_table_2))
	{
		location_1 = "on the round cafe set";
		furnitureString = "round cafe set";
	}

	if (in_something_round(locate, round_table_3))
	{
		location_1 = "on the round cafe set";
		furnitureString = "round cafe set";
	}

	if (in_something_round(locate, round_table_4))
	{
		location_1 = "on the round cafe set";
		furnitureString = "round cafe set";
	}

	if (in_something_round(locate, round_table_5))
	{
		location_1 = "on the round cafe set";
		furnitureString = "round cafe set";
	}

	if (in_something_round(locate, round_table_6))
	{
		location_1 = "on the round cafe set";
		furnitureString = "round cafe set";
	}

	if (in_something(locate, glass_wagon_1))
	{
		location_1 = "on the left glass wagon";
		furnitureString = "glass wagon on the left";

		if (locate[1] >= wagon_level[0])
		{
			location_2 = "on the top of the glass wagon";
		}
		else if (locate[1] >= wagon_level[1])
		{
			location_2 = "on the first floor of the glass wagon";
		}
		else
		{
			location_2 = "on the second floor of the glass wagon";
		}
	}

	if (in_something(locate, glass_wagon_2))
	{
		location_1 = "on the right glass wagon";
		furnitureString = "glass wagon on the right";

		if (locate[1] >= wagon_level[0])
		{
			location_2 = "on the top of the glass wagon";
		}
		else if (locate[1] >= wagon_level[1])
		{
			location_2 = "on the first floor of the glass wagon";
		}
		else
		{
			location_2 = "on the second floor of the glass wagon";
		}
	}

	if (in_something(locate, fridge))
	{
		location_1 = "in the fridge";
		furnitureString = "fridge";

		if (locate[1] > fridge_level[0])
		{
			location_2 = "in the upper part of the fridge";
		}
		else if (locate[1] > fridge_level[1])
		{
			if (locate[0] > fridge_center_line)
			{
				location_2 = "in the right center drawer";
			}
			else
			{
				location_2 = "in the left center drawer";
			}
		}
		else if (locate[1] >= fridge_level[2])
		{
			location_2 = "in the second drawer from the floor";
		}
		else
		{
			location_2 = "in the bottom drawer";
		}
	}

	if (in_something(locate, wooden_wagon_1))
	{
		location_1 = "on the wooden wagon near the fridge";
		furnitureString = "wooden wagon near the fridge ";

		if (locate[1] >= wagon_level[0])
		{
			location_2 = "on the top of the wooden wagon";
		}
		else if (locate[1] >= wagon_level[1])
		{
			location_2 = "on the first floor of the wooden wagon";
		}
		else
		{
			location_2 = "on the second floor of the wooden wagon";
		}
	}

	if (in_something(locate, wall_cabinet))
	{
		if (locate[1] > wall_cabinet_level)
		{
			location_1 = "in the wall cabinet above the wagon";
			furnitureString = "wall cabinet above the wagon";

			if (locate[2] > cabinetWall[0])
			{
				location_2 = "in the first cabinet on the left";
			}
			else if (locate[2] > cabinetWall[1])
			{
				location_2 = "in the second cabinet on the left";
			}
			else if (locate[2] > cabinetWall[2])
			{
				location_2 = "in the third cabinet on the left";
			}
			else
			{
				location_2 = "in the first cabinet on the right";
			}
		}
		else
		{
			if (in_something(locate, kitchen_drawer))
			{
				location_1 = "in the kitchen drawer near the wagon";

				furnitureString = "kitchen drawer near the wagon";

				if (locate[1] > kitchen_drawer_level[0])
				{
					location_2 = "on the top of the kitchen drawer";
				}
				else if (locate[1] > kitchen_drawer_level[1])
				{
					if (locate[3] > kitchen_drawer_cut)
					{
						location_2 = "in the first floor and the left drawer";

					}
					else
					{
						location_2 = "in the first floor and the right drawer";
					}

				}
				else if (locate[1] > kitchen_drawer_level[2])
				{
					if (locate[3] > kitchen_drawer_cut)
					{
						location_2 = "in the second floor and the left drawer";

					}
					else
					{
						location_2 = "in the second floor and the right drawer";
					}
				}
				else
				{
					if (locate[3] > kitchen_drawer_cut)
					{
						location_2 = "in the third floor and the left drawer";

					}
					else
					{
						location_2 = "in the third floor and the right drawer";
					}
				}
			}
			if (in_something(locate, wooden_wagon))
			{
				location_1 = "on the wooden wagon";
				furnitureString = "wooden wagon";

				if (locate[1] >= wagon_level[0])
				{
					location_2 = "on the top of the wooden wagon";
				}
				else if (locate[1] >= wagon_level[1])
				{
					location_2 = "on the first floor of the wooden wagon";
				}
				else
				{
					location_2 = "on the second floor of the wooden wagon";
				}
			}
		}
	}

	if (in_something(locate, counter_and_cabinet))
	{

		if (locate[1] >= counter_and_cabinet_level[0])
		{
			location_1 = "in the wall cabinet";
			furnitureString = "wall cabinet ";

			if (locate[1]>cabinet_line)
			{
				location_3 = "on the upper layer ";
			}
			else
			{
				location_3 = "on the lower layer ";
			}


			if (locate[0]>cabinet[0])
			{
				location_2 = "in the first cabinet on the left";
			}
			else if (locate[0]>cabinet[1])
			{
				location_2 = "in the second cabinet on the left";
			}
			else if (locate[0]>cabinet[2])
			{
				location_2 = "in the third cabniet on the left";
			}

			else
			{
				location_2 = "in the first cabniet on the right";
			}
		}


		else if (locate[1] >= counter_and_cabinet_level[1])
		{
			location_1 = "on the top of the drawer and under the cabinet";
			furnitureString = "drawer";
		}

		else if (locate[1] >= counter_and_cabinet_level[2])
		{
			location_1 = "in the drawer";
			furnitureString = "drawer";

			if (locate[0] > counter[0])
			{
				location_2 = "in the first floor of the first drawer on the left";
			}

			else if (locate[0]>counter[1])
			{
				location_2 = "in the first floor of the second drawer on the left";

			}
			else
			{
				location_2 = "in the first floor of the third drawer on the left";
			}
		}

		else if (locate[1] >= counter_and_cabinet_level[3])
		{
			if (locate[0] > counter[0])
			{
				location_2 = "in the second floor of the first drawer on the left";
			}

			else if (locate[0]>counter[1])
			{
				location_2 = "in the second floor of the second drawer on the left";

			}

			else
			{
				location_2 = "in the second floor of the third drawer on the left";
			}
		}
	}

	if (in_something_round(locate, trashcan))
	{
		location_1 = "in the trashcan";

		furnitureString = "trashcan";
	}
}

					try_ = furniture_sentence_out(location_1, taskInfo.target_object);

					guideMsg = location_sentence_out(location_1 , location_2 , location_3);

					if(furnitureString==" ")
					{furnitureString = is_may_near_furni();}

					cout<<try_<<endl;
					cout<<guideMsg<<endl;

					//furnitureString = "6";
					//cout<<"F:"<<furnitureString<<endl;;
					
					sendMessage(pubHumanNaviMsg, MSG_GET_AVATAR_STATUS);

					geometry_msgs::Point avatarPosition = avatarStatus.head.position;

					//cout<<std::boolalpha<<(bool)isCatched<<endl;

					isCatched = (avatarStatus.is_target_object_in_left_hand||avatarStatus.is_target_object_in_right_hand);
/*
					cout<<std::boolalpha<<(bool)isCatched<<endl;
					cout<<std::boolalpha<<(bool)avatarStatus.is_target_object_in_left_hand<<endl;
					cout<<std::boolalpha<<(bool)avatarStatus.is_target_object_in_right_hand<<endl;
					cout<<std::boolalpha<<(bool)(avatarStatus.is_target_object_in_left_hand||avatarStatus.is_target_object_in_right_hand)<<endl;

*/
					double avatarLocation[3];
					avatarLocation[0] = -avatarPosition.y;
					avatarLocation[1] = avatarPosition.z;
					avatarLocation[2] = avatarPosition.x;


					isArrived = arrived(locate,avatarLocation);


					//cout<<isArrived<<endl;


					//sendMessage(pubHumanNaviMsg, MSG_GET_SPEECH_STATE);

					speechState = SpeechState::Speakable;//取巧的方法！！！！！				

					if(isArrived == false)						
					{


						//int WaitTime = 15;
						
							//cout<<"0000000000000000000000000000000000000000000000000"<<endl;
							if(speakGuidanceMessage(pubHumanNaviMsg, pubGuidanceMsg, try_))
							{
							//	time = ros::Time::now();0
								//cout<<"0000000000000000000000000000000000000000000000000"<<endl;
								isMoveDirection = true;
							}
						
					}

					else
					{
						//int WaitTime = 0;
						//if(time.sec + WaitTime < ros::Time::now().sec)
						{
							if(speakGuidanceMessage(pubHumanNaviMsg, pubGuidanceMsg, guideMsg))
							{
								//time = ros::Time::now();
								isMoveDirection = true;
								//cout<<"isMoveDirection: "<<isMoveDirection<<endl;
								
								/*if(isCatched == true)
								{
									step++;
									
								}*/
							}
						}
					}

					/*if(isCatched == true)
								{
									step++;
								}*/
					

					break;
				}
				case GuideForPlacement:
				{
					
					if(isRequestReceived)
					{
						if(speakGuidanceMessage(pubHumanNaviMsg, pubGuidanceMsg, guideMsg))
						{
							isRequestReceived = false;
						}
					}
					//cout<<"4:"<<"44444444444"<<endl;
					//int WaitTime = 0;
					//if(time.sec + WaitTime < ros::Time::now().sec)
					{
						geometry_msgs::Point destinationPosition = taskInfo.destination.position;

						//cout<<"4:"<<"555555555"<<endl;
						string location_1 = " ";
						string location_2 = " ";
						string location_3 = " ";


						string try_ = " ";


						string environment = taskInfo.environment_id;

						double locate[3];


						if (environment == "LayoutHN01")
{
	double sidetable_and_mat[4] = { 0.77606,5.20635,-0.720915,3.81716};//r
	double table_level[2] = {0.493,0.226};//r

	double bed[4] = { 3.76808,6.41943,2.40386,4.64314 };//r

	double fridge[4] = { -3.1212,4.18648,-3.69847,3.49011 };//r
	double fridge_level[3] = { 0.927,0.742,0.38 };//r
	double fridge_center_line = 0.214;//r

	double counter_and_cabinet[4] = { -2.9743, 6.64847, -3.75563, 4.34638 };//r
	double counter_and_cabinet_level[3] = { 1.3839, 0.8565, 0.35 };//r
	double counter[2] = { 3.06622 ,2.212 };
	double cabinet[3] = { 2.04622 ,2.6556 ,3.2664 };
	double cabinet_line = 1.50785;//r

	double bookshelf[4] = { 0,0,0,0 };
	double bookshelf_level[3] = { 1.0281,0.6708,0.3512 }; //r

	double shelf_1[4] = { -3.23593,1.56046,-3.75125,-0.0923617 };//r
	double shelf_2[4] = { -3.20721,3.23087,-3.26121,1.58291 };//r
	double shelf_level[3] = {1.0259,0.624,0.221 };//r

	double TVbass[4] = { 1.00351,6.72734,-0.895564,-0.895564 };//r
	double TVbass_level[2] = { 0.756,0.49 };//r

	double glass_wagon_1[4] = { -1.30828,6.55556,-1.62333,5.8725 };//r
	double glass_wagon_2[4] = { 1.67445,6.70748,1.13121,6.1508 };//r
	double glass_wagon_3[4] = { -1.7524,6.73146,-2.28742,6.13601 };//r

	double wagon_level[2] = { 0.7286,0.394 };

	double wooden_table_and_chair[4] = { 3.18773,3.63173,1.88067, 2.34327};//r
	double chair_level = 0.563;//椅子上面桌子的高度//r
	double trashcan[5] = {  3.294,0,3.927,0.344,0.437 };//r

	double brown_mat[4] = { 2.629,0.179,1.005,-0.856 };//r


	geometry_msgs::Point targetPosition = taskInfo.target_object.position;


	locate[0] = -targetPosition.y;
	locate[1] = targetPosition.z;
	locate[2] = targetPosition.x;

	if (in_something(locate, shelf_1))
	{
		location_1 = "on the left wooden shelf";
		destinationString= "wooden shelf";
		//destinationString
		if (locate[1] >= shelf_level[0])
		{
			location_2 = "on the first floor of the shelf";
		}
		else if (locate[1] >= shelf_level[1])
		{
			location_2 = "on the second floor of the shelf";
		}
		else if (locate[1] >= shelf_level[2])
		{
			location_2 = "on the third floor of the shelf";
		}
		else
		{
			location_2 = "on the bottom floor of the shelf";
		}
	}


	if (in_something(locate, shelf_2))
	{
		location_1 = "on the wooden shelf next to the fridge";
		destinationString = "wooden shelf next to the fridge";

		if (locate[1] >= shelf_level[0])
		{
			location_2 = "on the first floor of the shelf";
		}
		else if (locate[1] >= shelf_level[1])
		{
			location_2 = "on the second floor of the shelf";
		}
		else if (locate[1] >= shelf_level[2])
		{
			location_2 = "on the third floor of the shelf";
		}
		else
		{
			location_2 = "on the bottom floor of the shelf";
		}
	}


	if (in_something(locate, counter_and_cabinet))
	{

		if (locate[1] >= counter_and_cabinet_level[0])
		{
			location_1 = "in the wall cabinet";
			destinationString = "wall cabinet ";

			if (locate[1]>cabinet_line)
			{
				location_3 = "on the upper layer ";
			}
			else
			{
				location_3 = "on the lower layer ";
			}


			if (locate[2]>cabinet[0])
			{
				location_2 = "in the first cabinet on the right";
			}
			else if (locate[2]>cabinet[1])
			{
				location_2 = "in the second cabinet on the right";
			}
			else if (locate[2]>cabinet[2])
			{
				location_2 = "in the third cabniet on the right";
			}

			else
			{
				location_2 = "in the first cabniet on the left";
			}
		}


		else if (locate[1] >= counter_and_cabinet_level[1])
		{
			location_1 = "on the top of the drawer and under the cabinet";
			destinationString = "drawer under the wall cabinet";
		}

		else if (locate[1] >= counter_and_cabinet_level[2])
		{
			location_1 = "in the drawer";
			destinationString = "drawer";

			if (locate[2] > counter[0])
			{
				location_2 = "in the first floor of the first drawer on the right";
			}

			else if (locate[2]>counter[1])
			{
				location_2 = "in the first floor of the second drawer on the right";

			}
			else
			{
				location_2 = "in the first floor of the third drawer on the right";
			}
		}

		else if (locate[1] >= counter_and_cabinet_level[3])
		{
			if (locate[2] > counter[0])
			{
				location_2 = "in the second floor of the first drawer on the right";
			}

			else if (locate[2]>counter[1])
			{
				location_2 = "in the second floor of the second drawer on the right";

			}

			else
			{
				location_2 = "in the second floor of the third drawer on the right";
			}
		}
	}


	if (in_something(locate, fridge))
	{
		location_1 = "in the fridge";
		destinationString = "fridge";

		if (locate[1] > fridge_level[0])
		{
			location_2 = "in the upper part of the fridge";
		}
		else if (locate[1] > fridge_level[1])
		{
			if (locate[2] > fridge_center_line)
			{
				location_2 = "in the right center drawer";
			}
			else
			{
				location_2 = "in the left center drawer";
			}
		}
		else if (locate[1] >= fridge_level[2])
		{
			location_2 = "in the second drawer from the floor";
		}
		else
		{
			location_2 = "in the bottom drawer";
		}
	}


	if (in_something(locate, bookshelf))
	{
		location_1 = "on the white bookshelf";
		destinationString = "white bookshelf";

		if (locate[1] >= bookshelf_level[0])
		{
			location_2 = "on the first floor of the bookshelf";
		}
		else if (locate[1] >= bookshelf_level[1])
		{
			location_2 = "on the second floor of the bookshelf";
		}
		else if (locate[1] >= bookshelf_level[2])
		{
			location_2 = "on the third floor of the bookshelf";
		}
		else
		{
			location_2 = "on the bottom floor of the bookshelf";
		}
	}



	if (in_something(locate, wooden_table_and_chair))
	{
		if (locate[1] >= chair_level)
		{
			destinationString = "wooden table";
			location_1 = "on the wooden table";
		}

		else
		{
			destinationString = "the chair under the wooden table";
			location_1 = "on the chair under the wooden table";
		}
	}


	if (in_something(locate, bed))
	{
		location_1 = "on the bed";
		destinationString = "bed";
	}

	if (in_something(locate, sidetable_and_mat))
	{
		if (locate[1] >= table_level[0])
		{
			destinationString = "white sidetable";
			location_1 = "on the white sidetable";
		}
		else
		{
			if (locate[1] >= table_level[1])
			{
				destinationString = "white sidetable";
				location_1 = "on the second layer of the white sidetable";
			}
			else
			{
				destinationString = "blue mat";
				location_1 = " on the blue mat under the sidetable";
			}
		}
	}


	if (in_something(locate, glass_wagon_1))
	{
		location_1 = "on the glass wagon near the kitchen drawer";
		destinationString = "glass wagon near the kitchen drawer";

		if (locate[1] >= wagon_level[0])
		{
			location_2 = "on the top of the glass wagon";
		}
		else if (locate[1] >= wagon_level[1])
		{
			location_2 = "on the first floor of the glass wagon";
		}
		else
		{
			location_2 = "on the second floor of the glass wagon";
		}
	}

	if (in_something(locate, glass_wagon_2))
	{
		location_1 = "on the second glass wagon near the bed";
		destinationString = "second glass wagon near the bed";

		if (locate[1] >= wagon_level[0])
		{
			location_2 = "on the top of the glass wagon";
		}
		else if (locate[1] >= wagon_level[1])
		{
			location_2 = "on the first floor of the glass wagon";
		}
		else
		{
			location_2 = "on the second floor of the glass wagon";
		}
	}

	if (in_something(locate, glass_wagon_3))
	{
		location_1 = "on the glass wagon near the bed";
		destinationString = "glass wagon near the bed";

		if (locate[1] >= wagon_level[0])
		{
			location_2 = "on the top of the glass wagon";
		}
		else if (locate[1] >= wagon_level[1])
		{
			location_2 = "on the first floor of the glass wagon";
		}
		else
		{
			location_2 = "on the second floor of the glass wagon";
		}
	}


	if (in_something(locate, TVbass))
	{
		location_1 = "on the TV bass";

		destinationString = "TV bass";

		if (locate[1] > TVbass_level[0])
		{
			location_2 = "on the top of the TV bass";
		}

		else
		{
			location_2 = "on the second floor of the TV bass";
		}
	}

	if (in_something_round(locate, trashcan))
	{
		location_1 = "in the trashcan near the wooden table";

		destinationString = "trashcan near the wooden table";
	}


}//layout1 end

if (environment == "LayoutHN02")
{
	double bed[4] = { 3.879,6.68,2.393,4.591 };

	double trashcan_1[5] = {  -3.293,0, 6.346,0.1655,0.437 };//r
	double trashcan_2[5] = {  3.64,0,4.27,0.1655,0.437 };//r

	double wooden_table_and_chair[4] = { -1.006,4.808,-1.938,-1.4485 };
	double chair_level = 0.4152;

	double counter_and_cabinet[4] = { 1.005, 6.8361, -1.46, 6.246 };
	double counter_and_cabinet_level[3] = { 1.3839, 0.8565, 0.35 };//r
	double counter[2] = { 0.173 ,-0.624 };
	double cabinet[3] = { 0.381 ,-0.242 ,-0.849};
	double cabinet_line = 1.50785;//r

	double blue_cupboard[4] = { -1.55,6.806,-3.0833,6.2 };
	double blue_cupboard_level[2] = { 1.229,0.72 };//2
	double cupboard[3] = { 0,0,0 };
	double cupboard_centerline = -2.3;
	double cupboard_leftside_line = 0.3;//r
	double cupboard_rightside_line = 0.555;//r
	double cupboard_rightside_cut = -1.937;
	double oven[4] = { -1.924,6.627,-2.1575, 6.448 };

	double TVbass[4] = { -3.348,2.932,-3.808,2.932 };
	double TVbass_level[2] = { 0.756,0.49 };//r

	double paper_box[4] = { -3.306,2.628,-3.77,2.078 };

	double glass_wagon[4] = { 3.884,-0.253,3.258,-0.865 };
	double wagon_level[2] = { 0.7286,0.394 };//r

	double shelf_1[4] = { 3.903,2.129,3.287,1.286 };
	double shelf_2[4] = { 2.348,6.813,1.504,6.178 };
	double shelf_level[3] = { 1.0259,0.624,0.221 };//r

	double sidetable[4] = { 3.955,3.713,3.261,2.166 };
	double table_level[2] = {0.493,0.226};//r

	double brown_mat[4] = { 0.261,6.207,-1.322,5.201 };    
	geometry_msgs::Point targetPosition = taskInfo.target_object.position;


	locate[0] = -targetPosition.y;
	locate[1] = targetPosition.z;
	locate[2] = targetPosition.x;

	if (in_something(locate, bed))
	{
		location_1 = "on the bed";
		destinationString = "bed";
	}

	if (in_something_round(locate, trashcan_1))
	{
		location_1 = "in the trashcan near the bed";

		destinationString = "trashcan near the bed";
	}

	if (in_something(locate, wooden_table_and_chair))
	{
		if (locate[1] >= chair_level)
		{
			destinationString = "wooden table";
			location_1 = "on the wooden table";
		}

		else
		{
			destinationString = "the chair under the wooden table";
			location_1 = "on the chair under the wooden table";
		}
	}


	if (in_something(locate, counter_and_cabinet))
	{

		if (locate[1] >= counter_and_cabinet_level[0])
		{
			location_1 = "in the wall cabinet";
			destinationString = "wall cabinet ";

			if (locate[1]>cabinet_line)
			{
				location_3 = "on the upper layer ";
			}
			else
			{
				location_3 = "on the lower layer ";
			}


			if (locate[0]>cabinet[0])
			{
				location_2 = "in the first cabinet on the right";
			}
			else if (locate[0]>cabinet[1])
			{
				location_2 = "in the second cabinet on the right";
			}
			else if (locate[0]>cabinet[2])
			{
				location_2 = "in the third cabniet on the right";
			}

			else
			{
				location_2 = "in the first cabniet on the left";
			}
		}


		else if (locate[1] >= counter_and_cabinet_level[1])
		{
			location_1 = "on the top of the drawer and under the cabinet";
			destinationString = "drawer";
		}

		else if (locate[1] >= counter_and_cabinet_level[2])
		{
			location_1 = "in the drawer";
			destinationString = "drawer";

			if (locate[0] > counter[0])
			{
				location_2 = "in the first floor of the first drawer on the right";
			}

			else if (locate[0]>counter[1])
			{
				location_2 = "in the first floor of the second drawer on the right";

			}
			else
			{
				location_2 = "in the first floor of the third drawer on the right";
			}
		}

		else if (locate[1] >= counter_and_cabinet_level[3])
		{
			if (locate[0] > counter[0])
			{
				location_2 = "in the second floor of the first drawer on the right";
			}

			else if (locate[0]>counter[1])
			{
				location_2 = "in the second floor of the second drawer on the right";

			}

			else
			{
				location_2 = "in the second floor of the third drawer on the right";
			}
		}
	}

	if (in_something(locate, blue_cupboard))
	{
		location_1 = "in the blue cupboard";
		destinationString = "blue cupboard";

		if (locate[1] >= blue_cupboard_level[0])
		{
			if (locate[0] > cupboard_centerline)
			{
				location_2 = "in the upper and right part of the cupboard";
			}
			else
			{
				location_2 = "in the upper and left part of the cupboard";
			}
		}

		else if (locate[1] >= blue_cupboard_level[1])
		{
			location_2 = "in the middle part of the cupboard";

			if (in_something(locate, oven))
			{
				location_3 = "in the small oven";

				destinationString = "oven";
			}
		}

		else
		{
			if (locate[0] > cupboard_centerline)
			{
				location_2 = "in the lower and right part of the cupboard";
				if (locate[2] > cupboard_rightside_cut)
				{
					if (locate[1] > cupboard_rightside_line)
					{
						location_3 = "in the right drawer";
					}
					else
					{
						location_3 = "in the right chest";
					}
				}
				else
				{
					if (locate[1] > cupboard_rightside_line)
					{
						location_3 = "in the left drawer";
					}
					else
					{
						location_3 = "in the left chest";
					}
				}
			}

			else
			{
				location_2 = "in the lower and left part of the cupboard";

				if (locate[1] < cupboard_leftside_line)
				{
					location_3 = "in the bottom drawer";
				}
			}
		}
	}


	if (in_something_round(locate, trashcan_2))
	{
		location_1 = "in the trashcan near the blue capboard";

		destinationString = "trashcan near the blue capboard";
	}


	if (in_something(locate, TVbass))
	{
		location_1 = "on the TV bass";

		destinationString = "TV bass";

		if (locate[1] > TVbass_level[0])
		{
			location_2 = "on the top of the TV bass";
		}

		else
		{
			location_2 = "on the second floor of the TV bass";
		}
	}

	if (in_something(locate, paper_box))
	{
		location_1 = "in the paper box";
		destinationString = "paper box";
	}

	if (in_something(locate, glass_wagon))
	{
		location_1 = "on the glass_wagon in the corner";
		destinationString = "glass_wagon in the corner";

		if (locate[1] >= wagon_level[0])
		{
			location_2 = "on the top of the glass wagon";
		}
		else if (locate[1] >= wagon_level[1])
		{
			location_2 = "on the first floor of the glass wagon";
		}
		else
		{
			location_2 = "on the second floor of the glass wagon";
		}
	}


	if (in_something(locate, shelf_1))
	{
		location_1 = "on the wooden shelf near the white sidetable";
		destinationString = "wooden shelf near the white side table";

		if (locate[1] >= shelf_level[0])
		{
			location_2 = "on the first floor of the shelf";
		}
		else if (locate[1] >= shelf_level[1])
		{
			location_2 = "on the second floor of the shelf";
		}
		else if (locate[1] >= shelf_level[2])
		{
			location_2 = "on the third floor of the shelf";
		}
		else
		{
			location_2 = "on the bottom floor of the shelf";
		}
	}

	if (in_something(locate, shelf_2))
	{
		location_1 = "on the wooden shelf near the bed";
		destinationString = "wooden shelf near the bed";

		if (locate[1] >= shelf_level[0])
		{
			location_2 = "on the first floor of the shelf";
		}
		else if (locate[1] >= shelf_level[1])
		{
			location_2 = "on the first floor of the shelf";
		}
		else if (locate[1] >= shelf_level[2])
		{
			location_2 = "on the second floor of the shelf";
		}
		else
		{
			location_2 = "on the bottom floor of the shelf";
		}
	}

	/*if (in_something(locate, sidetable))
	{
		location_1 = "on the white sidetable";

		destinationString = "white sidetable";
	}*/
	if (in_something(locate, sidetable))
	{
		if (locate[1] >= table_level[0])
		{
			destinationString = "white sidetable";
			location_1 = "on the white sidetable";
		}
		else
		{
			
				destinationString = "white sidetable";
				location_1 = "on the second layer of the white sidetable";
			
			
		}
	}
}

if (environment == "LayoutHN03")
{
	double bed[4] = { -0.08,6.708,-2.179,5.205 };

	double shelf_1[4] = { -3.179,6.746,-3.8137, 5.106 };
	double shelf_2[4] = { -3.184,5.104,-3.8137,3.459 };
	double shelf_level[3] = {1.0259,0.624,0.221 };//r
	double counter_and_cabinet[4] = { -3.255, 3.151, -3.867, 0.6804 };
	double counter_and_cabinet_level[3] = { 1.3839, 0.8565, 0.35 };//r
	double counter[2] = { 2.343 ,1.535 };
	double cabinet[3] = { 2.535 ,1.923 ,1.322 };
	double cabinet_line = 1.50785;//r

	double fridge[4] = { -3.178,0.644,-3.852,-0.034 };
	double fridge_level[3] = { 0.927,0.742,0.38 };//r
	double fridge_center_line = 0.214;

	double paper_box_1[4] = { 3.71,1.23,3.395,0.661 };//离门远的
	//double paper_box_1[4] = { 0,0,0,0 };
	double paper_box_2[4] = { 3.744,0.29,3.375,-0.273 };//离门近的

	double bookshelf[4] = { 3.9409,4.5673,3.475,3.6 };
	double bookshelf_level[3] = { 1.0281,0.6708,0.3512 };//r

	double TVbass[4] = { 1.856,6.747,0.552,4.7534 };
	double TVbass_level[2] = { 0.756,0.49 };//r

	double sidetable_and_mat[4] = { 1.856,6.747,0.552,5.142 };
	double table_level[2] = {0.493,0.226};//r

	geometry_msgs::Point targetPosition = taskInfo.target_object.position;


	locate[0] = -targetPosition.y;
	locate[1] = targetPosition.z;
	locate[2] = targetPosition.x;


	if (in_something(locate, sidetable_and_mat))
	{
		if (locate[1] >= table_level[0])
		{
			destinationString = "white sidetable";
			location_1 = "on the white sidetable";
		}
		else
		{
			if (locate[1] >= table_level[1])
			{
				destinationString = "white sidetable";
				location_1 = "on the second layer of the white sidetable";
			}
			else
			{
				destinationString = "blue mat";
				location_1 = " on the blue mat under the sidetable";
			}
		}
	}

	if (in_something(locate, bed))
	{
		location_1 = "on the bed";
		destinationString = "bed";
	}

	if (in_something(locate, shelf_1))
	{
		location_1 = "on the wooden shelf inside";
		destinationString = "wooden shelf inside";

		if (locate[1] >= shelf_level[0])
		{
			location_2 = "on the first floor of the shelf";
		}
		else if (locate[1] >= shelf_level[1])
		{
			location_2 = "on the second floor of the shelf";
		}
		else if (locate[1] >= shelf_level[2])
		{
			location_2 = "on the third floor of the shelf";
		}
		else
		{
			location_2 = "on the bottom floor of the shelf";
		}
	}

	if (in_something(locate, shelf_2))
	{
		location_1 = "on the wooden shelf outside";
		destinationString = "wooden shelf outside";

		if (locate[1] >= shelf_level[0])
		{
			location_2 = "on the first floor of the shelf";
		}
		else if (locate[1] >= shelf_level[1])
		{
			location_2 = "on the second floor of the shelf";
		}
		else if (locate[1] >= shelf_level[2])
		{
			location_2 = "on the third floor of the shelf";
		}
		else
		{
			location_2 = "on the bottom floor of the shelf";
		}
	}

	if (in_something(locate, counter_and_cabinet))
	{

		if (locate[1] >= counter_and_cabinet_level[0])
		{
			location_1 = "in the wall cabinet";
			destinationString = "wall cabinet ";

			if (locate[1]>cabinet_line)
			{
				location_3 = "on the upper layer ";
			}
			else
			{
				location_3 = "on the lower layer ";
			}


			if (locate[2]>cabinet[0])
			{
				location_2 = "in the first cabinet on the right";
			}
			else if (locate[2]>cabinet[1])
			{
				location_2 = "in the second cabinet on the right";
			}
			else if (locate[2]>cabinet[2])
			{
				location_2 = "in the third cabniet on the right";
			}

			else
			{
				location_2 = "in the first cabniet on the left";
			}
		}


		else if (locate[1] >= counter_and_cabinet_level[1])
		{
			location_1 = "on the top of the drawer and under the cabinet";
			destinationString = "drawer";
		}

		else if (locate[1] >= counter_and_cabinet_level[2])
		{
			location_1 = "in the drawer";
			destinationString = "drawer";

			if (locate[2] > counter[0])
			{
				location_2 = "in the first floor of the first drawer on the left";
			}

			else if (locate[2]>counter[1])
			{
				location_2 = "in the first floor of the second drawer on the left";

			}
			else
			{
				location_2 = "in the first floor of the third drawer on the left";
			}
		}

		else if (locate[1] >= counter_and_cabinet_level[3])
		{
			if (locate[2] > counter[0])
			{
				location_2 = "in the second floor of the first drawer on the left";
			}

			else if (locate[2]>counter[1])
			{
				location_2 = "in the second floor of the second drawer on the left";

			}

			else
			{
				location_2 = "in the second floor of the third drawer on the left";
			}
		}
	}

	if (in_something(locate, fridge))
	{
		location_1 = "in the fridge";
		destinationString = "fridge";

		if (locate[1] > fridge_level[0])
		{
			location_2 = "in the upper part of the fridge";
		}
		else if (locate[1] > fridge_level[1])
		{
			if (locate[2] > fridge_center_line)
			{
				location_2 = "in the right center drawer";
			}
			else
			{
				location_2 = "in the left center drawer";
			}
		}
		else if (locate[1] >= fridge_level[2])
		{
			location_2 = "in the second drawer from the floor";
		}
		else
		{
			location_2 = "in the bottom drawer";
		}
	}


	if (in_something(locate,paper_box_1))
	{
		location_1 = "in the paper box near the door";
		destinationString = "paper box near the door";
	}

	if (in_something(locate,paper_box_2))
	{
		location_1 = "in the paper box far from the door";
		destinationString = "paper box far from the door";
	}

	if (in_something(locate, bookshelf))
	{
		location_1 = "on the white bookshelf";
		destinationString = "white bookshelf";

		if (locate[1] >= bookshelf_level[0])
		{
			location_2 = "on the first floor of the bookshelf";
		}
		else if (locate[1] >= bookshelf_level[1])
		{
			location_2 = "on the second floor of the bookshelf";
		}
		else if (locate[1] >= bookshelf_level[2])
		{
			location_2 = "on the third floor of the bookshelf";
		}
		else
		{
			location_2 = "on the bottom floor of the bookshelf";
		}
	}

	if (in_something(locate, TVbass))
	{
		location_1 = "on the TV bass";

		destinationString = "TV bass";

		if (locate[1] > TVbass_level[0])
		{
			location_2 = "on the top of the TV bass";
		}

		else
		{
			location_2 = "on the second floor of the TV bass";
		}
	}
}//layout3 end

if (environment == "LayoutHN04")
{
	double fridge[4] = { 2.535,6.849,1.8463,6.14 };
	double fridge_level[3] = { 0.927,0.742,0.38 };//r
	double fridge_center_line = 2.107;

	double counter_and_cabinet[4] = { 1.806, 6.847, -0.662, 6.24 };
	double counter_and_cabinet_level[3] = { 1.3839, 0.8565, 0.35 };//r
	double counter[2] = { 0.98 ,0.175 };
	double cabinet[3] = { 1.171 ,0.565 ,-0.041 };
	double cabinet_line = 1.50785;//r

	double glass_dining_table_with_chair[4] = { -0.309,5.544,-2.076,4.441 };
	double table_level = 0.756;

	double sidetable[4] = { -3.165,5.46,-3.885,3.905 };
	double sidetable_level[2] = {0.493,0.226};//r
	
	double brown_mat[4] = { 2.616,0.161,1.016,-0.851 };

	double blue_cupboard[4] = { 3.973,5.231,3.334,3.701 };
	double blue_cupboard_level[2] = { 1.229,0.72 };
	double cupboard[3] = { 0,0,0 };
	double cupboard_centerline = 2.249;
	double cupboard_leftside_line = 0.268;//高度
	double cupboard_rightside_line = 0.536;//
	double cupboard_rightside_cut = 2.623;

	double trashcan[5] = { 3.727,0,5.473,0.1655,0.437 };//r

	double paper_box[4] = { 3.745,6.626,3.382,6.031 };

	geometry_msgs::Point targetPosition = taskInfo.target_object.position;


	locate[0] = -targetPosition.y;
	locate[1] = targetPosition.z;
	locate[2] = targetPosition.x;

	if (in_something(locate, sidetable))
	{
		if (locate[1] >= sidetable_level[0])
		{
			destinationString = "white sidetable";
			location_1 = "on the white sidetable";
		}
		else
		{
			
				destinationString = "white sidetable";
				location_1 = "on the second layer of the white sidetable";
			
		}
	}

	if (in_something(locate, fridge))
	{
		location_1 = "in the fridge";
		destinationString = "fridge";

		if (locate[1] > fridge_level[0])
		{
			location_2 = "in the upper part of the fridge";
		}
		else if (locate[1] > fridge_level[1])
		{
			if (locate[0] > fridge_center_line)
			{
				location_2 = "in the right center drawer";
			}
			else
			{
				location_2 = "in the left center drawer";
			}
		}
		else if (locate[1] >= fridge_level[2])
		{
			location_2 = "in the second drawer from the floor";
		}
		else
		{
			location_2 = "in the bottom drawer";
		}
	}

	if (in_something(locate, counter_and_cabinet))
	{

		if (locate[1] >= counter_and_cabinet_level[0])
		{
			location_1 = "in the wall cabinet";
			destinationString = "wall cabinet ";

			if (locate[1]>cabinet_line)
			{
				location_3 = "on the upper layer ";
			}
			else
			{
				location_3 = "on the lower layer ";
			}


			if (locate[0]>cabinet[0])
			{
				location_2 = "in the first cabinet on the right";
			}
			else if (locate[0]>cabinet[1])
			{
				location_2 = "in the second cabinet on the right";
			}
			else if (locate[0]>cabinet[2])
			{
				location_2 = "in the third cabniet on the right";
			}

			else
			{
				location_2 = "in the first cabniet on the left";
			}
		}


		else if (locate[1] >= counter_and_cabinet_level[1])
		{
			location_1 = "on the top of the drawer and under the cabinet";
			destinationString = "drawer";
		}

		else if (locate[1] >= counter_and_cabinet_level[2])
		{
			location_1 = "in the drawer";
			destinationString = "drawer";

			if (locate[0] > counter[0])
			{
				location_2 = "in the first floor of the first drawer on the left";
			}

			else if (locate[0]>counter[1])
			{
				location_2 = "in the first floor of the second drawer on the left";

			}
			else
			{
				location_2 = "in the first floor of the third drawer on the left";
			}
		}

		else if (locate[1] >= counter_and_cabinet_level[3])
		{
			if (locate[0] > counter[0])
			{
				location_2 = "in the second floor of the first drawer on the left";
			}

			else if (locate[0]>counter[1])
			{
				location_2 = "in the second floor of the second drawer on the left";

			}

			else
			{
				location_2 = "in the second floor of the third drawer on the left";
			}
		}
	}



	if (in_something(locate, glass_dining_table_with_chair))
	{
		if (locate[1] >= table_level)
		{
			location_1 = "on the glass dining table";
			destinationString = "glass dining table";
		}
		else
		{
			location_1 = "on the chair under the glass table";
			destinationString = "on the chair under the table";
		}
	}

	if (in_something(locate, sidetable))
	{
		location_1 = "on the short sidetable";

		destinationString = "short sidetable";
	}

	if (in_something(locate, blue_cupboard))
	{
		location_1 = "in the blue cupboard";
		destinationString = "blue cupboard";

		if (locate[1] >= blue_cupboard_level[0])
		{
			if (locate[2] > cupboard_centerline)
			{
				location_2 = "in the upper and left part of the cupboard";
			}
			else
			{
				location_2 = "in the upper and right part of the cupboard";
			}
		}

		else if (locate[1] >= blue_cupboard_level[1])
		{
			location_2 = "in the middle part of the cupboard";
		}

		else
		{
			if (locate[2] < cupboard_centerline)
			{
				location_2 = "in the lower and right part of the cupboard";
				if (locate[2] < cupboard_rightside_cut)
				{
					if (locate[1] > cupboard_rightside_line)
					{
						location_3 = "in the right drawer";
					}
					else
					{
						location_3 = "in the right chest";
					}
				}
				else
				{
					if (locate[1] > cupboard_rightside_line)
					{
						location_3 = "in the left drawer";
					}
					else
					{
						location_3 = "in the left chest";
					}
				}
			}

			else
			{
				location_2 = "in the lower and left part of the cupboard";

				if (locate[1] < cupboard_leftside_line)
				{
					location_3 = "in the bottom drawer";
				}
			}
		}
	}

	if (in_something_round(locate, trashcan))
	{
		location_1 = "in the trashcan near the blue cupboard";
		destinationString = "trashcan near the blue cupboard";
	}

	if (in_something(locate, paper_box))
	{
		location_1 = "in the paper box near fridge";
		destinationString = "paper box near fridge";
	}
}
//layout4 end

if (environment == "LayoutHN05")
{
	double bed_1[4] = { -2.338,6.678, -3.823,4.59 };
	double bed_2[4] = { 3.857,6.69,2.37,4.6 };

	double bookshelf_1[4] = { -3.426,4.09,-3.65,3.606 };
	double bookshelf_2[4] = {  3.93,4.55,0.02699965,2.371  };
	double bookshelf_level[3] = { 1.0281,0.6708,0.3512 };//r

	double paper_box_1[4] = { -3.367,2.992,-3.711,2.435 };
	double paper_box_2[4] = { 3.782,2.764,3.407,2.225 };

	double glass_wagon_1[4] = { -1.65,6.764,-2.271,6.192 };
	double glass_wagon_2[4] = { 2.303,6.781,1.726,6.146 };
	double wagon_level[2] = { 0.7286,0.394 };//r

	double wooden_table_and_chair_1[4] = { 1.146,6.769,0.341,6.34 };//hei色的
	double wooden_table[4] = { -1.066,6.782,-0.357,5.532 };
	double chair[4] = { -1.013,6.54,-1.55,6.095 };
	double chair_level = 0.4152;

	//double wooden_table_and_chair_2[4] = { 0,0,0,0 };//黑色的

	double wooden_wagon_1[4] = { -0.383,5.442,-0.94,4.84};
	double wooden_wagon_2[4] = { 0.929,5.445,0.357,4.804 };

	double shelf[4] = { 0.317,6.776,-0.324,5.137 };
	double shelf_level[3] = { 1.0259,0.624,0.221 };//r

	geometry_msgs::Point targetPosition = taskInfo.target_object.position;


	locate[0] = -targetPosition.y;
	locate[1] = targetPosition.z;
	locate[2] = targetPosition.x;

	if (in_something(locate, paper_box_1))
	{
		location_1 = "in the left paper box ";
		destinationString = "paper box on the left";
	}

	if (in_something(locate, paper_box_2))
	{
		location_1 = "in the right paper box";
		destinationString = "paper box on the right";
	}

	if (in_something(locate, bed_1))
	{
		location_1 = "on the left bed";
		destinationString = "bed on the left";
	}

	if (in_something(locate, bed_2))
	{
		location_1 = "on the right bed";
		destinationString = "bed on the right";
	}

	if (in_something(locate, glass_wagon_1))
	{
		location_1 = "on the left glass wagon";
		destinationString = "glass wagon on the left";

		if (locate[1] >= wagon_level[0])
		{
			location_2 = "on the top of the glass wagon";
		}
		else if (locate[1] >= wagon_level[1])
		{
			location_2 = "on the first floor of the glass wagon";
		}
		else
		{
			location_2 = "on the second floor of the glass wagon";
		}
	}

	if (in_something(locate, glass_wagon_2))
	{
		location_1 = "on the right glass wagon";
		destinationString = "glass wagon on the right";

		if (locate[1] >= wagon_level[0])
		{
			location_2 = "on the top of the glass wagon";
		}
		else if (locate[1] >= wagon_level[1])
		{
			location_2 = "on the first floor of the glass wagon";
		}
		else
		{
			location_2 = "on the second floor of the glass wagon";
		}
	}

	if (in_something(locate, bookshelf_1))
	{
		location_1 = "on the left white bookshelf ";
		destinationString = "white bookshelf on the left";

		if (locate[1] >= bookshelf_level[0])
		{
			location_2 = "on the first floor of the bookshelf";
		}
		else if (locate[1] >= bookshelf_level[1])
		{
			location_2 = "on the second floor of the bookshelf";
		}
		else if (locate[1] >= bookshelf_level[2])
		{
			location_2 = "on the third floor of the bookshelf";
		}
		else
		{
			location_2 = "on the bottom floor of the bookshelf";
		}
	}

	if (in_something(locate, bookshelf_2))
	{
		location_1 = "on the right white bookshelf ";
		destinationString = "white bookshelf on the right";

		if (locate[1] >= bookshelf_level[0])
		{
			location_2 = "on the first floor of the bookshelf";
		}
		else if (locate[1] >= bookshelf_level[1])
		{
			location_2 = "on the second floor of the bookshelf";
		}
		else if (locate[1] >= bookshelf_level[2])
		{
			location_2 = "on the third floor of the bookshelf";
		}
		else
		{
			location_2 = "on the bottom floor of the bookshelf";
		}
	}

	if (in_something(locate, wooden_wagon_1))
	{
		location_1 = "on the left wooden wagon";
		destinationString = "wooden wagon on the left";

		if (locate[1] >= wagon_level[0])
		{
			location_2 = "on the top of the wooden  wagon";
		}
		else if (locate[1] >= wagon_level[1])
		{
			location_2 = "on the first floor of the wooden wagon";
		}
		else
		{
			location_2 = "on the second floor of the wooden wagon";
		}
	}

	if (in_something(locate, wooden_wagon_2))
	{
		location_1 = "on the right wooden wagon";
		destinationString = "wooden wagon on the right";

		if (locate[1] >= wagon_level[0])
		{
			location_2 = "on the top of the wooden  wagon";
		}
		else if (locate[1] >= wagon_level[1])
		{
			location_2 = "on the first floor of the wooden wagon";
		}
		else
		{
			location_2 = "on the second floor of the wooden wagon";
		}
	}

	if (in_something(locate, wooden_table_and_chair_1))//白色的椅子和桌子
	{
		if (locate[1] >= chair_level)
		{
			destinationString = "wooden table near the white chair";
			location_1 = "on the right wooden table near the white chair";
		}

		else
		{
			destinationString = "the white chair under the wooden table";
			location_1 = "on the white chair under the wooden table";
		}
	}

	

	if (in_something(locate, wooden_table))
	{
		location_1 = "on the left wooden table near the black chair";
		destinationString = "the wooden table on the left near the black chair";
	}

	if (in_something(locate, chair))
	{
		location_1 = "on the white chair";
		destinationString = "white chair on the left";
	}

	if (in_something(locate, shelf))
	{
		location_1 = "on the wooden shelf";
		destinationString = "wooden shelf";

		if (locate[1] >= shelf_level[0])
		{
			location_2 = "on the first floor of the shelf";
		}
		else if (locate[1] >= shelf_level[1])
		{
			location_2 = "on the second floor of the shelf";
		}
		else if (locate[1] >= shelf_level[2])
		{
			location_2 = "on the third floor of the shelf";
		}
		else
		{
			location_2 = "on the bottom floor of the shelf";
		}
	}

}//layout5 end

if (environment == "LayoutHN06")
{
	double table_1[4] = { -0.598,5.693,-1.829,4.284 };
	double table_2[4] = { 1.369,5.693,0.188,4.274 };

	double glass_wagon[4] = { 0.191,5.666,-0.602,4.274 };
	double wagon_level[2] = { 0.713,0.41 };

	double shelf[4] = { -3.171,6.735,-3.8,3.363 };
	double shelf_level[3] = { 1.413,1.028,0.618 };

	double counter_and_cabinet[4] = { -3.227, 2.186, -3.815, -0.234 };
	double counter_and_cabinet_level[3] = { 1.3839, 0.8565, 0.35 };
	double counter[2] = { 1.401 ,0.592 };
	double cabinet[3] = { 1.6 ,0.997 ,0.393 };
	double cabinet_line = 1.50785;

	double paper_box_1[4] = { 3.859,-0.171,3.324,-0.687 };
	double paper_box_2[4] = { 3.88,0.765,0.324,0.012 };

	double trashcan_1[5] = { 3.774,0,2.327,0.2,0 };
	double trashcan_2[5] = { 3.763,0,1.589,0.2,0 };

	double wooden_dining_table[4] = { 3.88,5.246,2.886,2.959 };

	double bookshelf[4] = { 3.927,6.765,2.955,6.337 };
	double bookshelf_level[3] = { 1.029,0.678,0.361 };

	double paper_box_3[4] = { 2.838,6.691,2.119,6.12 };

	geometry_msgs::Point targetPosition = taskInfo.target_object.position;


	locate[0] = -targetPosition.y;
	locate[1] = targetPosition.z;
	locate[2] = targetPosition.x;

	if (in_something(locate, table_1))
	{
		location_1 = "on the left wooden table near shelf";
		destinationString = "table near shelf";
	}

	if (in_something(locate, glass_wagon))
	{
		location_1 = "on the glass_wagon";
		destinationString = "glass_wagon";

		if (locate[1] >= wagon_level[0])
		{
			location_2 = "on the top of the glass wagon";
		}
		else if (locate[1] >= wagon_level[1])
		{
			location_2 = "on the first floor of the glass wagon";
		}
		else
		{
			location_2 = "on the second floor of the glass wagon";
		}
	}

	if (in_something(locate, table_2))
	{
		location_1 = "on the right wooden table";
		destinationString = "table on the right";
	}

	if (in_something(locate, shelf))
	{
		location_1 = "on the four-in-one shelf";
		destinationString = "four-in-one shelf";
	}

	if (in_something(locate, counter_and_cabinet))
	{

		if (locate[1] >= counter_and_cabinet_level[0])
		{
			location_1 = "in the wall cabinet";
			destinationString = "wall cabinet ";

			if (locate[1]>cabinet_line)
			{
				location_3 = "on the upper layer ";
			}
			else
			{
				location_3 = "on the lower layer ";
			}


			if (locate[2]>cabinet[0])
			{
				location_2 = "in the first cabinet on the right";
			}
			else if (locate[2]>cabinet[1])
			{
				location_2 = "in the second cabinet on the right";
			}
			else if (locate[2]>cabinet[2])
			{
				location_2 = "in the third cabniet on the right";
			}

			else
			{
				location_2 = "in the first cabniet on the left";
			}
		}

		
		else if (locate[1] >= counter_and_cabinet_level[1])
		{
			location_1 = "on the top of the drawer and under the cabinet";
			destinationString = "drawer";
		}

		else if (locate[1] >= counter_and_cabinet_level[2])
		{
			location_1 = "in the drawer";
			destinationString = "drawer";

			if (locate[2] > counter[0])
			{
				location_2 = "in the first floor of the first drawer on the right";
			}

			else if (locate[2]>counter[1])
			{
				location_2 = "in the first floor of the second drawer on the right";

			}
			else
			{
				location_2 = "in the first floor of the third drawer on the right";
			}
		}

		else if (locate[1] >= counter_and_cabinet_level[3])
		{
			if (locate[2] > counter[0])
			{
				location_2 = "in the second floor of the first drawer on the right";
			}

			else if (locate[2]>counter[1])
			{
				location_2 = "in the second floor of the second drawer on the right";

			}

			else
			{
				location_2 = "in the second floor of the third drawer on the right";
			}
		}
	}

	if (in_something(locate, paper_box_1))
	{
		location_1 = "in the paper box near the door";
		destinationString = "paper box near the door";
	}

	if (in_something(locate, paper_box_2))
	{
		location_1 = "in the paper box near the trashcan";
		destinationString = "paper box near the trashcan";
	}

	if (in_something_round(locate, trashcan_1))
	{
		location_1 = " in the trashcan on the left";
		destinationString = "trashcan on the left";
	}

	if (in_something_round(locate, trashcan_2))
	{
		location_1 = " in the trashcan on the right";
		destinationString = "trashcan on the right";
	}

	if (in_something(locate, wooden_dining_table))
	{
		location_1 = "on the wooden dinning table";
		destinationString = "wooden dining table";
	}


	if (in_something(locate, bookshelf))
	{
		location_1 = "on the white bookshelf";
		destinationString = "white bookshelf";

		if (locate[1] >= bookshelf_level[0])
		{
			location_2 = "on the first floor of the bookshelf";
		}
		else if (locate[1] >= bookshelf_level[1])
		{
			location_2 = "on the second floor of the bookshelf";
		}
		else if (locate[1] >= bookshelf_level[2])
		{
			location_2 = "on the third floor of the bookshelf";
		}
		else
		{
			location_2 = "on the bottom floor of the bookshelf";
		}
	}

	if (in_something(locate, paper_box_3))
	{
		location_1 = "in the paper box near the white bookshelf";

		destinationString = "paper box near the white bookshelf";
	}


}

if (environment == "LayoutHN07")
{
	double blue_cupboard[4] = { -0.194,6.669,-1.739,6.196 };
	double blue_cupboard_level[2] = { 1.229,0.72 };
	double cupboard[3] = { 0,0,0 };
	double cupboard_centerline = -0.992;
	double cupboard_leftside_line = 0.268;
	double cupboard_rightside_line = 0.536;
	double cupboard_rightside_cut = -0.596;

	double counter_and_cabinet[4] = { 2.319,6.749,-0.14,6.222 };
	double counter_and_cabinet_level[3] = { 1.3839, 0.8565, 0.35 };
	double counter[2] = { 1.505 ,0.709 };
	double cabinet[3] = { 1.505 ,1.08 ,1.696 };
	double cabinet_line = 1.50785;

	double trashcan_1[5] = { -2.394,0,6.616,0.2,0 };
	double trashcan_2[5] = { -2.806,0,6.611,0.2,0 };
	double trashcan_3[5] = { -3.209,0,6.605,0.2,0 };

	double bed[4] = { -3.861,6.681,-5.381,4.621 };

	double TVbass[4] = { -3.48,3.393,-5.456,2.456 };
	double TVbass_level[2] = { 0.77,0.502 };

	double paper_box[4] = { -4.697,1.757,-5.395,-3.41 };

	double glass_wagon[4] = { -4.832,-0.276,-5.404,-0.849 };
	double wagon_level[2] = { 0.715,0.461 };

	double brown_mat[4] = { 0.988,0.161,-0.561,-0.844 };

	double sidetable[4] = { 2.35,4.265,1.666,2.754 };
	double sidetable_level = 0.471;

	double oven[4] = { -0.341,6.637,-0.83,0.267 };

	double glass_dining_table[4] = { -0.532,5.291,-2.832,4.135 };

	geometry_msgs::Point targetPosition = taskInfo.target_object.position;


	locate[0] = -targetPosition.y;
	locate[1] = targetPosition.z;
	locate[2] = targetPosition.x;

	if (in_something(locate, glass_dining_table))
	{
		//cout<<"11111111111111111"<<endl;
		location_1 = "on the glass dinning table";
		destinationString = "glass dining table";
	}

	if (in_something(locate, brown_mat))
	{
		location_1 = "on the brown mat";

		destinationString = "brown mat";
	}

	if (in_something(locate, sidetable))
	{
		location_1 = "on the short sidetable";

		destinationString = "short sidetable";
		if (locate[1] > sidetable_level)
		{
			location_2 = "on the top of the sidetable";
		}
		else
		{
			location_2 = "on the second floor of the side table";
		}
	}

	if (in_something_round(locate, trashcan_1))
	{
		location_1 = "in the first trashcan on the right";

		destinationString = "first trashcan on the right";
	}

	if (in_something_round(locate, trashcan_2))
	{
		location_1 = "in the second trashcan on the right";

		destinationString = "second trashcan on the right";
	}

	if (in_something_round(locate, trashcan_3))
	{
		location_1 = "in the first trashcan on the left";

		destinationString = "first trashcan on the left";
	}

	if (in_something(locate, TVbass))
	{
		location_1 = "on the TV bass";

		destinationString = "TV bass";

		if (locate[1] > TVbass_level[0])
		{
			location_2 = "on the top of the TV bass";
		}

		else
		{
			location_2 = "on the second floor of the TV bass";
		}
	}

	if (in_something(locate, glass_wagon))
	{
		location_1 = "on the glass_wagon";
		destinationString = "glass_wagon";

		if (locate[1] >= wagon_level[0])
		{
			location_2 = "on the top of the glass wagon";
		}
		else if (locate[1] >= wagon_level[1])
		{
			location_2 = "on the first floor of the glass wagon";
		}
		else
		{
			location_2 = "on the second floor of the glass wagon";
		}
	}

	if (in_something(locate, paper_box))
	{
		location_1 = "in the paper box behind the TV bass";
		destinationString = "paper box behind the TV bass";
	}

	if (in_something(locate, counter_and_cabinet))
	{

		if (locate[1] >= counter_and_cabinet_level[0])
		{
			location_1 = "in the wall cabinet";
			destinationString = "wall cabinet ";

			if (locate[1]>cabinet_line)
			{
				location_3 = "on the upper layer ";
			}
			else
			{
				location_3 = "on the lower layer ";
			}


			if (locate[0]>cabinet[0])
			{
				location_2 = "in the first cabinet on the right";
			}
			else if (locate[0]>cabinet[1])
			{
				location_2 = "in the second cabinet on the right";
			}
			else if (locate[0]>cabinet[2])
			{
				location_2 = "in the third cabniet on the right";
			}

			else
			{
				location_2 = "in the first cabniet on the left";
			}
		}


		else if (locate[1] >= counter_and_cabinet_level[1])
		{
			location_1 = "on the top of the drawer and under the cabinet";
			destinationString = "drawer";
		}

		else if (locate[1] >= counter_and_cabinet_level[2])
		{
			location_1 = "in the drawer";
			destinationString = "drawer";

			if (locate[0] > counter[0])
			{
				location_2 = "in the first floor of the first drawer on the right";
			}

			else if (locate[0]>counter[1])
			{
				location_2 = "in the first floor of the second drawer on the right";

			}
			else
			{
				location_2 = "in the first floor of the third drawer on the right";
			}
		}

		else if (locate[1] >= counter_and_cabinet_level[3])
		{
			if (locate[0] > counter[0])
			{
				location_2 = "in the second floor of the first drawer on the right";
			}

			else if (locate[0]>counter[1])
			{
				location_2 = "in the second floor of the second drawer on the right";

			}

			else
			{
				location_2 = "in the second floor of the third drawer on the right";
			}
		}
	}

	if (in_something(locate, blue_cupboard))
	{
		location_1 = "in the blue cupboard";
		destinationString = "blue cupboard";

		if (locate[1] >= blue_cupboard_level[0])
		{
			if (locate[0] > cupboard_centerline)
			{
				location_2 = "in the upper and right part of the cupboard";
			}
			else
			{
				location_2 = "in the upper and left part of the cupboard";
			}
		}

		else if (locate[1] >= blue_cupboard_level[1])
		{
			location_2 = "in the middle part of the cupboard";
			if (in_something(locate, oven))
			{
				location_1 = "in the oven";
				destinationString = "oven";
			}
		}

		else
		{
			if (locate[0] > cupboard_centerline)
			{
				location_2 = "in the lower and right part of the cupboard";
				if (locate[2] > cupboard_rightside_cut)
				{
					if (locate[1] > cupboard_rightside_line)
					{
						location_3 = "in the right drawer";
					}
					else
					{
						location_3 = "in the right chest";
					}
				}
				else
				{
					if (locate[1] > cupboard_rightside_line)
					{
						location_3 = "in the left drawer";
					}
					else
					{
						location_3 = "in the left chest";
					}
				}
			}

			else
			{
				location_2 = "in the lower and left part of the cupboard";

				if (locate[1] < cupboard_leftside_line)
				{
					location_3 = "in the bottom drawer";
				}
			}
		}
	}
}

if (environment == "LayoutHN08")
{
	double round_table_1[5] = { 3.241,0.767,3.21,0.32,0 };
	double round_table_2[5] = { 3.118,0,0.767,0.32,0 };
	double round_table_3[5] = { 3.26,0,0.767,0.32,0 };
	double round_table_4[5] = { 03.231,0,0.767,0.32,0 };

	double cornersofa_1[4] = { -3.032,0.798,-3.756,-0.039 };
	double cornersofa_2[4] = { -2.973,3.211,-3.824,2.325 };
	
	double glass_wagon[4] = { -3.222,1.887,-3.824,1.273 };
	double wagon_level[2] = { 0.679,0.362 };

	double drawer_alone_1[4] = { 3.96,4.649,1.462,4.03 };
	double drawer_1[2] = { -2.179,-2.993 };
	double drawer_alone_level[2]  = { 0.89,0.373 };

	double drawer_alone_2[4] = { 3.925,4.649,1.498,4.03 };
	double drawer_2[2] = { 3.112,2.319 };

	double shelf[4] = { -1.318,6.549,-3.802,5.919 };
	double shelf_level[3] = { 0,0,0 };

	double fridge_1[4] = { -0.522,6.584,-1.217,5.856 };
	double fridge_level[3] = { 0.927,0.742,0.38 };
	double fridge_1_center_line = -0.951;

	double fridge_2[4] = { 0.215,6.576,-0.474,5.856 };
	double fridge_2_center_line = -0.202;

	double wall_cabinet[4] = { 3.985,6.584,1.507,6.087 };
	double wall_cabinet_level = 1.401;
	double wall_cabinet_line = 0;
	double cabinet[3] = { 3.347,2.76,2.108 };
	double kitchen_drawer[4] = { 3.95,6.576,0.263,5.982 };
	double kitchen_drawer_cut[3] = { 3.012,2.164,1.227 };
	double kitchen_drawer_level[3] = { 0.86,0.608,0.329 };

	geometry_msgs::Point targetPosition = taskInfo.target_object.position;


	locate[0] = -targetPosition.y;
	locate[1] = targetPosition.z;
	locate[2] = targetPosition.x;

	if (in_something(locate, shelf))
	{
		location_1 = "on the wooden shelf";
		destinationString = "wooden shelf";

		if (locate[1] >= shelf_level[0])
		{
			location_2 = "on the first floor of the shelf";
		}
		else if (locate[1] >= shelf_level[1])
		{
			location_2 = "on the second floor of the shelf";
		}
		else if (locate[1] >= shelf_level[2])
		{
			location_2 = "on the third floor of the shelf";
		}
		else
		{
			location_2 = "on the bottom floor of the shelf";
		}
	}

	if (in_something(locate, drawer_alone_1))
	{	
		destinationString = "drawer next to the shelf";
		if (locate[1] > drawer_alone_level[0])
		{
			location_1 = "on the top of the drawer next to the shelf";
			location_2 = "on the top of the drawer";
			destinationString = "drawer next to the shelf";
		}
		else if (locate[1] > drawer_alone_level[1])
		{
			location_1 = "in the drawer next to the shelf";
			if (locate[0] >= drawer_1[0])
			{
				location_2 = "in the first floor and the first drawer on the left";
			}
			else if (locate[0] >= drawer_1[1])
			{
				location_2 = "in the first floor and the second drawer on the left";
			}
			else
			{
				location_2 = "in the first floor and the first drawer on the right";
			}
		}

		else
		{
			location_1 = "in the drawer next to the shelf";
			if (locate[0] >= drawer_1[0])
			{
				location_2 = "in the second floor and the first drawer on the left";
			}
			else if (locate[0] >= drawer_1[1])
			{
				location_2 = "in the second floor and the second drawer on the left";
			}
			else
			{
				location_2 = "in the second floor and the first drawer on the right";
			}
		}
	}

	if (in_something(locate, drawer_alone_2))
	{
		destinationString = "drawer next to the cabinet";
		if (locate[1] > drawer_alone_level[0])
		{
			location_1 = "on the top of the drawer next to the cabniet";
			location_2 = "on the top of the drawer";
			destinationString = "drawer next to the cabinet";
		}
		else if (locate[1] > drawer_alone_level[1])
		{
			location_1 = "in the drawer next to the cabinet";
			if (locate[0] >= drawer_2[0])
			{
				location_2 = "in the first floor and the first drawer on the left";
			}
			else if (locate[0] >= drawer_2[1])
			{
				location_2 = "in the first floor and the second drawer on the left";
			}
			else
			{
				location_2 = "in the first floor and the first drawer on the right";
			}
		}

		else
		{
			location_1 = "in the drawer next to the cabinet";
			if (locate[0] >= drawer_2[0])
			{
				location_2 = "in the second floor and the first drawer on the left";
			}
			else if (locate[0] >= drawer_2[1])
			{
				location_2 = "in the second floor and the second drawer on the left";
			}
			else
			{
				location_2 = "in the second floor and the first drawer on the right";
			}
		}
	}

	if (in_something(locate, fridge_2))
	{
		location_1 = "in the fridge near the oven";
		destinationString = "fridge near the oven";

		if (locate[1] > fridge_level[0])
		{
			location_2 = "in the upper part of the fridge";
		}
		else if (locate[1] > fridge_level[1])
		{
			if (locate[2] > fridge_2_center_line)
			{
				location_2 = "in the right center drawer";
			}
			else
			{
				location_2 = "in the left center drawer";
			}
		}
		else if (locate[1] >= fridge_level[2])
		{
			location_2 = "in the second drawer from the floor";
		}
		else
		{
			location_2 = "in the bottom drawer";
		}
	}

	if (in_something(locate, fridge_1))
	{
		location_1 = "in the fridge near the shelf";
		destinationString = "fridge near the shelf";

		if (locate[1] > fridge_level[0])
		{
			location_2 = "in the upper part of the fridge";
		}
		else if (locate[1] > fridge_level[1])
		{
			if (locate[2] > fridge_1_center_line)
			{
				location_2 = "in the right center drawer";
			}
			else
			{
				location_2 = "in the left center drawer";
			}
		}
		else if (locate[1] >= fridge_level[2])
		{
			location_2 = "in the second drawer from the floor";
		}
		else
		{
			location_2 = "in the bottom drawer";
		}
	}


	if (in_something(locate, kitchen_drawer))
	{
		if (locate[1] > wall_cabinet_level)
		{
			location_1 = "in the wall cabinet";
			destinationString = "wall cabinet";
			if (locate[0] > cabinet[0])
			{
				location_2 = "in the first wall cabinet on the right";
			}
			else if (locate[0] > cabinet[1])
			{
				location_2 = "in the second wall cabinet on the right";
			}
			else if (locate[0] > cabinet[2])
			{
				location_2 = "in the third wall cabinet on the right";
			}
			else
			{
				location_2 = "in the first wall cabinet on the left";
			}
		}

		else if (locate[1] > kitchen_drawer[0])
		{
			location_1 = "on the top of the kitchen drawer";
			destinationString = "kitchen drawer";
			location_2 = "on the top of the drawer and under the cabinet";
		}

		else if (locate[1] > kitchen_drawer[1])
		{
			location_1 = "in the kitchen drawer under the cabinet";
			destinationString = "kitchen drawer";
			if (locate[0] > kitchen_drawer_cut[0])
			{
				location_2 = "in the first floor and the first drawer on the right";
			}
			else if (locate[0] > kitchen_drawer_cut[1])
			{
				location_2 = "in the first floor and the second drawer on the right";
			}
			else if (locate[0] > kitchen_drawer_cut[2])
			{
				location_2 = "in the first floor and the third drawer on the right";
			}
			else
			{
				location_2 = "in the first floor and the first drawer on the left";
			}
		}

		else if (locate[1] > kitchen_drawer[2])
		{
			location_1 = "in the kitchen drawer under the cabinet";
			destinationString = "kitchen drawer";
			if (locate[0] > kitchen_drawer_cut[0])
			{
				location_2 = "in the first floor and the first drawer on the right";
			}
			else if (locate[0] > kitchen_drawer_cut[1])
			{
				location_2 = "in the second floor and the second drawer on the right";
			}
			else if (locate[0] > kitchen_drawer_cut[2])
			{
				location_2 = "in the second floor and the third drawer on the right";
			}
			else
			{
				location_2 = "in the second floor and the first drawer on the left";
			}
		}
		else
		{
			location_1 = "in the kitchen drawer under the cabinet";
			destinationString = "kitchen drawer";
			if (locate[0] > kitchen_drawer_cut[0])
			{
				location_2 = "in the third floor and the first drawer on the right";
			}
			else if (locate[0] > kitchen_drawer_cut[1])
			{
				location_2 = "in the third floor and the second drawer on the right";
			}
			else if (locate[0] > kitchen_drawer_cut[2])
			{
				location_2 = "in the third floor and the third drawer on the right";
			}
			else
			{
				location_2 = "in the third floor and the first drawer on the left";
			}
		}
	}

	if (in_something(locate, glass_wagon))
	{
		location_1 = "on the glass_wagon";
		destinationString = "glass_wagon";

		if (locate[1] >= wagon_level[0])
		{
			location_2 = "on the top of the glass wagon";
		}
		else if (locate[1] >= wagon_level[1])
		{
			location_2 = "on the first floor of the glass wagon";
		}
		else
		{
			location_2 = "on the second floor of the glass wagon";
		}
	}

	if (in_something(locate, cornersofa_1))
	{
		location_1 = "on the corner sofa on the left";

		destinationString = "corner sofa on the left";
	}

	if (in_something(locate, cornersofa_2))
	{
		location_1 = "on the corner sofa on the left";

		destinationString = "corner sofa on the left";
	}

	if (in_something_round(locate, round_table_1))
	{
		location_1 = "on the first round table on the left";
		destinationString = "first round table on the left";
	}

	if (in_something_round(locate, round_table_2))
	{
		location_1 = "on the second round table on the left";
		destinationString = "second round table on the left";
	}

	if (in_something_round(locate, round_table_3))
	{
		location_1 = "on the third round table on the left";
		destinationString = "third round table on the left";
	}

	if (in_something_round(locate, round_table_4))
	{
		location_1 = "on the first round table on the right";
		destinationString = "first round table on the right";
	}
}

if (environment == "LayoutHN09")
{
	double round_table_1[5] = { 0.376,0,6.412,0.32,0 };
	double round_table_2[5] = { 1.63,0,5.27,0.32,0 };
	double round_table_3[5] = { 0.338,0,4.261,0.32,0 };
	double round_table_4[5] = { 0.338,0,4.261,0.32,0 };

	double wooden_table_1[4] = { -4.526,0.724,-5.792,0.004 };
	double wooden_table_2[4] = { -4.526,2.541,-5.792,1.821 };

	double trashcan_1[5] = { -5.522,0,6.455,0.2,0 };
	double trashcan_2[5] = { -5.014,0,6.455,0.2,0 };

	double counter_and_cabinet[4] = { -3.346, 4.176, -5.82, 3.561 };
	double counter_and_cabinet_level[3] = { 1.3839, 0.8565, 0.35 };
	double counter[2] = { -4.178 ,-4.967 };
	double cabinet[3] = { -4.024 ,-4.606 ,-5.16 };
	double cabinet_line = 1.401;

	double wooden_wagon[4] = { -1.642,4.912,-2.249,4.297 };
	double wagon_level[2] = { 0.679,0.362 };

	double fridge[4] = { -4.009,6.789,-4.697,6.086 };
	double fridge_level[3] = { 0.927,0.742,0.38 };
	double fridge_center_line = -4.429;

	double brown_mat[4] = { 0.666,0.116,-0.928,-0.897 };

	double wall_cabinet[4] = { -1.731,4.912,-2.249,4.297 };
	double wall_cabinet_level = 1.401;
	double wall_cabinet_line = 0;
	double cabinetWall[3] = { 6.062,5.459,4.856 };
	double kitchen_drawer[4] = { -1.6758,6.766,-2.225,4.9391 };
	double kitchen_drawer_cut = 5.856;
	double kitchen_drawer_level[3] = { 0.914,0.661,0.417 };

	geometry_msgs::Point targetPosition = taskInfo.target_object.position;


	locate[0] = -targetPosition.y;
	locate[1] = targetPosition.z;
	locate[2] = targetPosition.x;

	if (in_something(locate, fridge))
	{
		location_1 = "in the fridge";
		destinationString = "fridge";

		if (locate[1] > fridge_level[0])
		{
			location_2 = "in the upper part of the fridge";
		}
		else if (locate[1] > fridge_level[1])
		{
			if (locate[0] > fridge_center_line)
			{
				location_2 = "in the right center drawer";
			}
			else
			{
				location_2 = "in the left center drawer";
			}
		}
		else if (locate[1] >= fridge_level[2])
		{
			location_2 = "in the second drawer from the floor";
		}
		else
		{
			location_2 = "in the bottom drawer";
		}
	}

	if (in_something_round(locate, trashcan_1))
	{
		location_1 = "in the trashcan on the left";

		destinationString = "trashcan on the left";
	}

	if (in_something_round(locate, trashcan_2))
	{
		location_1 = "in the trashcan on the right";

		destinationString = "trashcan on the right";
	}

	if (in_something(locate, counter_and_cabinet))
	{

		if (locate[1] >= counter_and_cabinet_level[0])
		{
			location_1 = "in the wall cabinet on the faucet";
			destinationString = "wall cabinet on the faucet";

			if (locate[1]>cabinet_line)
			{
				location_3 = "on the upper layer ";
			}
			else
			{
				location_3 = "on the lower layer ";
			}


			if (locate[0]>cabinet[0])
			{
				location_2 = "in the first cabinet on the left";
			}
			else if (locate[0]>cabinet[1])
			{
				location_2 = "in the second cabinet on the left";
			}
			else if (locate[0]>cabinet[2])
			{
				location_2 = "in the third cabniet on the left";
			}

			else
			{
				location_2 = "in the first cabniet on the right";
			}
		}


		else if (locate[1] >= counter_and_cabinet_level[1])
		{
			location_1 = "on the top of the drawer and under the cabinet";
			destinationString = "drawer under the cabinet";
		}

		else if (locate[1] >= counter_and_cabinet_level[2])
		{
			location_1 = "in the drawer";
			destinationString = "drawer under the cabinet";

			if (locate[0] > counter[0])
			{
				location_2 = "in the first floor of the first drawer on the left";
			}

			else if (locate[0]>counter[1])
			{
				location_2 = "in the first floor of the second drawer on the left";

			}
			else
			{
				location_2 = "in the first floor of the third drawer on the left";
			}
		}

		else if (locate[1] >= counter_and_cabinet_level[3])
		{
			if (locate[0] > counter[0])
			{
				location_2 = "in the second floor of the first drawer on the left";
			}

			else if (locate[0]>counter[1])
			{
				location_2 = "in the second floor of the second drawer on the left";

			}

			else
			{
				location_2 = "in the second floor of the third drawer on the left";
			}
		}
	}

	if (in_something(locate, wooden_table_1))
	{
		location_1 = "on the left wooden table";

		destinationString = "wooden table on the left";
	}

	if (in_something(locate, wooden_table_2))
	{
		location_1 = "on the right wooden table";

		destinationString = "wooden table on the right";
	}

	if (in_something(locate, brown_mat))
	{
		location_1 = "on the brown mat";

		destinationString = "brown mat";
	}


	if (in_something_round(locate, round_table_1))
	{
		location_1 = "on the round table";
		destinationString = "round table";
	}

	if (in_something_round(locate, round_table_2))
	{
		location_1 = "on the round table";
		destinationString = "round table";
	}

	if (in_something_round(locate, round_table_3))
	{
		location_1 = "on the round table";
		destinationString = "round table";
	}

	if (in_something_round(locate, round_table_4))
	{
		location_1 = "on the round table";
		destinationString = "round table";
	}


	if (in_something(locate, wall_cabinet))
	{
		if (locate[1] > wall_cabinet_level)
		{
			location_1 = "in the wall cabinet above the wagon";
			destinationString = "wall cabinet above the wagon";

			if (locate[2] > cabinetWall[0])
			{
				location_2 = "in the first cabinet on the left";
			}
			else if (locate[2] > cabinetWall[1])
			{
				location_2 = "in the second cabinet on the left";
			}
			else if (locate[2] > cabinetWall[2])
			{
				location_2 = "in the third cabinet on the left";
			}
			else
			{
				location_2 = "in the first cabinet on the right";
			}
		}
		else
		{
			if (in_something(locate, kitchen_drawer))
			{
				location_1 = "in the kitchen drawer near the wagon";

				destinationString = "kitchen drawer near the wagon";

				if (locate[1] > kitchen_drawer_level[0])
				{
					location_2 = "on the top of the kitchen drawer";
				}
				else if(locate[1] > kitchen_drawer_level[1])
				{
					if (locate[3] > kitchen_drawer_cut)
					{
						location_2 = "in the first floor and the left drawer";

					}
					else
					{
						location_2 = "in the first floor and the right drawer";
					}

				}
				else if (locate[1] > kitchen_drawer_level[2])
				{
					if (locate[3] > kitchen_drawer_cut)
					{
						location_2 = "in the second floor and the left drawer";

					}
					else
					{
						location_2 = "in the second floor and the right drawer";
					}
				}
				else
				{
					if (locate[3] > kitchen_drawer_cut)
					{
						location_2 = "in the third floor and the left drawer";

					}
					else
					{
						location_2 = "in the third floor and the right drawer";
					}
				}
			}
			if (in_something(locate, wooden_wagon))
			{
				location_1 = "on the wooden wagon";
				destinationString = "wooden wagon";

				if (locate[1] >= wagon_level[0])
				{
					location_2 = "on the top of the wooden wagon";
				}
				else if (locate[1] >= wagon_level[1])
				{
					location_2 = "on the first floor of the wooden wagon";
				}
				else
				{
					location_2 = "on the second floor of the wooden wagon";
				}
			}
		}
	}
}

//ÓÐÉ¾¼õ
if (environment == "LayoutHN10")
{
	double wooden_table_1[4] = { -4.581,2.556,-5.886,1.447 };
	double wooden_table_2[4] = { -4.588,4.341,-5.886,3.261 };
	double wooden_table_3[4] = { -4.588,0.471,-5.886,-0.138 };

	double shelf[4] = { -5.054,6.675,-5.897,1.833 };
	double shelf_level[3] = { 1.424,1.024,0.647 };

	double fridge[4] = { -3.106,6.721,-3.847,5.96 };
	double fridge_level[3] = { 0.927,0.742,0.38 };
	double fridge_center_line = -3.551;

	double paper_box[4] = { -3.847,6.675,-4.689,5.96 };

	double glass_wagon[4] = { -1.838,6.721,-3.106,6.082 };
	double wagon_level[2] = { 0.72,0.41 };

	double wall_cabinet[4] = { 1.838,6.674,-2.777,6.403 };
	double wall_cabinet_level = 1.401;
	double wall_cabinet_line = 0;
	double cabinetWall[7] = { 1.277,0.665,0.073,-0.565,-1.198,-1.795,-2.408 };
	double kitchen_drawer_level[3] = { 0.85,0.558,0.349 };
	double kitchen_drawer_line[3] = { 0.957,0.047,-0.899 };

	double counter_and_cabinet[4] = { 1.866,4.779,-3.052,4.17 };
	double counter_and_cabinet_level[3] = { 1.3839, 0.8565, 0.35 };
	double counter[5] = { 1.034,0.231,-0.601,-1.43,-2.243 };
	double cabinet[7] = { 1.255,0.649,0.056,-0.582,-1.214,-1.803,-2.42 };
	double cabinet_line = 1.50785;

	

	geometry_msgs::Point targetPosition = taskInfo.target_object.position;


	locate[0] = -targetPosition.y;
	locate[1] = targetPosition.z;
	locate[2] = targetPosition.x;

	if (in_something(locate, wooden_table_1))
	{
		location_1 = "on the short wooden table";

		destinationString = "wooden table";
	}

	if (in_something(locate, counter_and_cabinet))
	{

		if (locate[1] >= counter_and_cabinet_level[0])
		{
			location_1 = "in the wall cabinet";
			destinationString = "wall cabinet ";

			if (locate[1]>cabinet_line)
			{
				location_3 = "on the upper layer ";
			}
			else
			{
				location_3 = "on the lower layer ";
			}


			if (locate[0]>cabinet[0])
			{
				location_2 = "in the first cabinet on the left";
			}
			else if (locate[0]>cabinet[1])
			{
				location_2 = "in the second cabinet on the left";
			}
			else if (locate[0]>cabinet[2])
			{
				location_2 = "in the third cabniet on the left";
			}
			else if (locate[0]>cabinet[3])
			{
				location_2 = "in the fourth cabniet on the left";
			}
			else if (locate[0]>cabinet[4])
			{
				location_2 = "in the fifth cabniet on the left";
			}
			else if (locate[0]>cabinet[5])
			{
				location_2 = "in the sixth cabniet on the left";
			}
			else if (locate[0]>cabinet[6])
			{
				location_2 = "in the seventh cabniet on the left";
			}
			else
			{
				location_2 = "in the first cabniet on the right";
			}
		}


		else if (locate[1] >= counter_and_cabinet_level[1])
		{
			location_1 = "on the top of the drawer and under the cabinet";
			destinationString = "drawer";
		}

		else if (locate[1] >= counter_and_cabinet_level[2])
		{
			location_1 = "in the drawer";
			destinationString = "drawer";

			if (locate[0] > counter[0])
			{
				location_2 = "in the first floor of the first drawer on the left";
			}

			else if (locate[0]>counter[1])
			{
				location_2 = "in the first floor of the second drawer on the left";

			}
			else if (locate[0]>counter[2])
			{
				location_2 = "in the first floor of the third drawer on the left";

			}
			else if (locate[0]>counter[3])
			{
				location_2 = "in the first floor of the fourth drawer on the left";

			}
			else if (locate[0]>counter[4])
			{
				location_2 = "in the first floor of the fifth drawer on the left";

			}
			else
			{
				location_2 = "in the first floor of the sixth drawer on the left";
			}
		}

		else if (locate[1] >= counter_and_cabinet_level[3])
		{
			if (locate[0] > counter[0])
			{
				location_2 = "in the second floor of the first drawer on the left";
			}

			else if (locate[0]>counter[1])
			{
				location_2 = "in the second floor of the second drawer on the left";

			}
			else if (locate[0]>counter[2])
			{
				location_2 = "in the second floor of the third drawer on the left";

			}
			else if (locate[0]>counter[3])
			{
				location_2 = "in the second floor of the fourth drawer on the left";

			}
			else if (locate[0]>counter[4])
			{
				location_2 = "in the second floor of the fifth drawer on the left";

			}
			else
			{
				location_2 = "in the second floor of the sixth drawer on the left";
			}
		}
	}

	if (in_something(locate, wooden_table_2))
	{
		location_1 = "on the short wooden table";

		destinationString = "wooden table";
	}

	if (in_something(locate, wooden_table_3))
	{
		location_1 = "on the short wooden table";

		destinationString = "wooden table";
	}

	if (in_something(locate, shelf))
	{
		location_1 = "on the wooden shelf";
		destinationString = "wooden shelf";

		if (locate[1] >= shelf_level[0])
		{
			location_2 = "on the first floor of the shelf";
		}
		else if (locate[1] >= shelf_level[1])
		{
			location_2 = "on the first floor of the shelf";
		}
		else if (locate[1] >= shelf_level[2])
		{
			location_2 = "on the second floor of the shelf";
		}
		else
		{
			location_2 = "on the bottom floor of the shelf";
		}
	}

	if (in_something(locate, fridge))
	{
		location_1 = "in the fridge";
		destinationString = "fridge";

		if (locate[1] > fridge_level[0])
		{
			location_2 = "in the upper part of the fridge";
		}
		else if (locate[1] > fridge_level[1])
		{
			if (locate[0] > fridge_center_line)
			{
				location_2 = "in the right center drawer";
			}
			else
			{
				location_2 = "in the left center drawer";
			}
		}
		else if (locate[1] >= fridge_level[2])
		{
			location_2 = "in the second drawer from the floor";
		}
		else
		{
			location_2 = "in the bottom drawer";
		}
	}

	if (in_something(locate, paper_box))
	{
		location_1 = "in the paper box";
		destinationString = "paper box";
	}

	if (in_something(locate, wall_cabinet))
	{
		if (locate[1] > wall_cabinet_level)
		{
			location_1 = "in the cabinet above the wagon";
			destinationString = "cabinet above the wagon";

			if (locate[0] > cabinetWall[0])
			{
				location_2 = "in the first cabinet on the right";
			}
			else if(locate[0] > cabinetWall[1])
			{
				location_2 = "in the second cabinet on the right";

			}
			else if (locate[0] > cabinetWall[2])
			{
				location_2 = "in the third cabinet on the right";

			}
			else if (locate[0] > cabinetWall[3])
			{
				location_2 = "in the forth cabinet on the right";

			}
			else if (locate[0] > cabinetWall[4])
			{
				location_2 = "in the fifth cabinet on the right";

			}

			else if (locate[0] > cabinetWall[5])
			{
				location_2 = "in the sixth cabinet on the right";

			}

			else if (locate[0] > cabinetWall[6])
			{
				location_2 = "in the seventh cabinet on the right";

			}
			else
			{
				location_2 = "in the first cabinet on the left";

			}
		}
		else
		{
			if (in_something(locate, glass_wagon))
			{
				location_1 = "on the glass_wagon";
				destinationString = "glass_wagon";

				if (locate[1] >= wagon_level[0])
				{
					location_2 = "on the top of the glass wagon";
				}
				else if (locate[1] >= wagon_level[1])
				{
					location_2 = "on the first floor of the glass wagon";
				}
				else
				{
					location_2 = "on the second floor of the glass wagon";
				}
			}
			else
			{
				location_1 = "in the kitchen drawer next to the wagon";
				destinationString = "kitchen drawer next to the wagon";
				if (locate[1] > kitchen_drawer_level[0])
				{
					location_2 = "on the top of the drawer which has a toaster";
				}
				else if (locate[1] > kitchen_drawer_level[1])
				{
					if (locate[0] > kitchen_drawer_line[0])
					{
						location_2 = "in the first floor and the first drawer on the right";
					}
					else if (locate[0] > kitchen_drawer_line[1])
					{
						location_2 = "in the first floor and the second drawer on the right";
					}
					else if (locate[0] > kitchen_drawer_line[2])
					{
						location_2 = "in the first floor and the third drawer on the right";
					}
					else
					{
						location_2 = "in the first floor and the first drawer on the left";
					}
				}
				else if (locate[1] > kitchen_drawer_level[2])
				{
					if (locate[0] > kitchen_drawer_line[0])
					{
						location_2 = "in the second floor and the first drawer on the right";
					}
					else if (locate[0] > kitchen_drawer_line[1])
					{
						location_2 = "in the second floor and the second drawer on the right";
					}
					else if (locate[0] > kitchen_drawer_line[2])
					{
						location_2 = "in the second floor and the third drawer on the right";
					}
					else
					{
						location_2 = "in the second floor and the first drawer on the left";
					}
				}
				else
				{
					if (locate[0] > kitchen_drawer_line[0])
					{
						location_2 = "in the third floor and the first drawer on the right";
					}
					else if (locate[0] > kitchen_drawer_line[1])
					{
						location_2 = "in the third floor and the second drawer on the right";
					}
					else if (locate[0] > kitchen_drawer_line[2])
					{
						location_2 = "in the third floor and the third drawer on the right";
					}
					else
					{
						location_2 = "in the third floor and the first drawer on the left";
					}
				}
			}
		}
	}

}

if (environment == "LayoutHN11")
{
	double wooden_table_1[4] = { -1.1651,5.84,-2.4379,5.1187 };
	double wooden_table_2[4] = { 1.229,5.843,-0.058,5.108 };

	double shelf[4] = { -3.16,6.63,-3.849,5.056 };
	double shelf_level[3] = { 1.328,0.95,0.56 };

	double brown_mat[4] = { 0,0,0,0 };

	double counter_and_cabinet_1[4] = { -3.189, 5.042, -3.812, 2.551 };
	double counter_and_cabinet_level[3] = { 1.3839, 0.8565, 0.35 };
	double counter_1[2] = { 0.567 ,-0.17 };
	double cabinet_1[3] = { 0.751 ,0.195 ,-0.412 };
	double cabinet_line = 1.401;

	double counter_and_cabinet_2[4] = { 0, 3.87101, 3.30582, 1.44405 };
	//double counter_and_cabinet_level[3] = { 1.3839, 0.8565, 0.35 };
	double counter_2[2] = { 4.179 ,3.383 };
	double cabinet_2[3] = { 4.391 ,3.788 ,3.202 };

	double fridge[4] = { -5.173,4.732,-5.852,4.015 };
	double fridge_level[3] = { 0.927,0.742,0.38 };
	double fridge_center_line = 4.262;

	double glass_wagon[4] = { -5.304,3.885,-5.881,2.603 };
	double wagon_level[2] = { 0.589,0.295 };

	double wall_cabinet[4] = { -5.317,3.896,-5.88,-976 };
	double wall_cabinet_level = 1.401;
	double wall_cabinet_line = 0;
	double cabinet[7] = { 3.341,2.692,2.078,1.451,0.855,0.252,-0.36 };
	double kitchen_drawer_level[3] = { 0.822,0.536,0.279 };
	double kitchen_drawer_line[3] = { 1.698,0.771,-0.14 };

	double trashcan[5] = { -5.687,0,5.802,0.2,0 };

	double paper_box[4] = { -5.224,5.611,-5.882,4.783 };

	geometry_msgs::Point targetPosition = taskInfo.target_object.position;


	locate[0] = -targetPosition.y;
	locate[1] = targetPosition.z;
	locate[2] = targetPosition.x;

	if (in_something(locate, wall_cabinet))
	{
		if (locate[1] > wall_cabinet_level)
		{
			location_1 = "in the cabinet above the wagon";
			destinationString = "cabinet above the wagon";

			if (locate[2] > cabinet[0])
			{
				location_2 = "in the first cabinet on the right";
			}
			else if (locate[2] > cabinet[1])
			{
				location_2 = "in the second cabinet on the right";

			}
			else if (locate[2] > cabinet[2])
			{
				location_2 = "in the third cabinet on the right";

			}
			else if (locate[2] > cabinet[3])
			{
				location_2 = "in the forth cabinet on the right";

			}
			else if (locate[2] > cabinet[4])
			{
				location_2 = "in the fifth cabinet on the right";

			}

			else if (locate[2] > cabinet[5])
			{
				location_2 = "in the sixth cabinet on the right";

			}

			else if (locate[2] > cabinet[6])
			{
				location_2 = "in the seventh cabinet on the right";

			}
			else
			{
				location_2 = "in the first cabinet on the left";

			}
		}
		else
		{
			if (in_something(locate, glass_wagon))
			{
				location_1 = "on the glass_wagon";
				destinationString = "glass_wagon";

				if (locate[1] >= wagon_level[0])
				{
					location_2 = "on the top of the glass wagon";
				}
				else if (locate[1] >= wagon_level[1])
				{
					location_2 = "on the first floor of the glass wagon";
				}
				else
				{
					location_2 = "on the second floor of the glass wagon";
				}
			}
			else
			{
				location_1 = "in the kitchen drawer next to the wagon";
				destinationString = "kitchen drawer next to the wagon";
				if (locate[1] > kitchen_drawer_level[0])
				{
					location_2 = "on the top of the drawer which has a toaster";
				}
				else if (locate[1] > kitchen_drawer_level[1])
				{
					if (locate[2] > kitchen_drawer_line[0])
					{
						location_2 = "in the first floor and the first drawer on the right";
					}
					else if (locate[2] > kitchen_drawer_line[1])
					{
						location_2 = "in the first floor and the second drawer on the right";
					}
					else if (locate[2] > kitchen_drawer_line[2])
					{
						location_2 = "in the first floor and the third drawer on the right";
					}
					else
					{
						location_2 = "in the first floor and the first drawer on the left";
					}
				}
				else if (locate[1] > kitchen_drawer_level[2])
				{
					if (locate[2] > kitchen_drawer_line[0])
					{
						location_2 = "in the second floor and the first drawer on the right";
					}
					else if (locate[2] > kitchen_drawer_line[1])
					{
						location_2 = "in the second floor and the second drawer on the right";
					}
					else if (locate[2] > kitchen_drawer_line[2])
					{
						location_2 = "in the second floor and the third drawer on the right";
					}
					else
					{
						location_2 = "in the second floor and the first drawer on the left";
					}
				}
				else
				{
					if (locate[0] > kitchen_drawer_line[0])
					{
						location_2 = "in the third floor and the first drawer on the right";
					}
					else if (locate[2] > kitchen_drawer_line[1])
					{
						location_2 = "in the third floor and the second drawer on the right";
					}
					else if (locate[2] > kitchen_drawer_line[2])
					{
						location_2 = "in the third floor and the third drawer on the right";
					}
					else
					{
						location_2 = "in the third floor and the first drawer on the left";
					}
				}
			}
		}
	}

	if (in_something(locate, counter_and_cabinet_1))
	{

		if (locate[1] >= counter_and_cabinet_level[0])
		{
			location_1 = "in the wall cabinet";
			destinationString = "wall cabinet ";

			if (locate[1]>cabinet_line)
			{
				location_3 = "on the upper layer ";
			}
			else
			{
				location_3 = "on the lower layer ";
			}


			if (locate[2]>cabinet_1[0])
			{
				location_2 = "in the first cabinet on the left";
			}
			else if (locate[2]>cabinet_1[1])
			{
				location_2 = "in the second cabinet on the left";
			}
			else if (locate[2]>cabinet_1[2])
			{
				location_2 = "in the third cabniet on the left";
			}

			else
			{
				location_2 = "in the first cabniet on the right";
			}
		}


		else if (locate[1] >= counter_and_cabinet_level[1])
		{
			location_1 = "on the top of the drawer and under the cabinet";
			destinationString = "drawer";
		}

		else if (locate[1] >= counter_and_cabinet_level[2])
		{
			location_1 = "in the drawer";
			destinationString = "drawer";

			if (locate[2] > counter_1[0])
			{
				location_2 = "in the first floor of the first drawer on the left";
			}

			else if (locate[2]>counter_1[1])
			{
				location_2 = "in the first floor of the second drawer on the left";

			}
			else
			{
				location_2 = "in the first floor of the third drawer on the left";
			}
		}

		else if (locate[1] >= counter_and_cabinet_level[3])
		{
			if (locate[2] > counter_1[0])
			{
				location_2 = "in the second floor of the first drawer on the left";
			}

			else if (locate[2]>counter_1[1])
			{
				location_2 = "in the second floor of the second drawer on the left";

			}

			else
			{
				location_2 = "in the second floor of the third drawer on the left";
			}
		}
	}

	if (in_something(locate, counter_and_cabinet_2))
	{

		if (locate[1] >= counter_and_cabinet_level[0])
		{
			location_1 = "in the wall cabinet";
			destinationString = "wall cabinet ";

			if (locate[1]>cabinet_line)
			{
				location_3 = "on the upper layer ";
			}
			else
			{
				location_3 = "on the lower layer ";
			}


			if (locate[2]>cabinet_2[0])
			{
				location_2 = "in the first cabinet on the left";
			}
			else if (locate[2]>cabinet_2[1])
			{
				location_2 = "in the second cabinet on the left";
			}
			else if (locate[2]>cabinet_2[2])
			{
				location_2 = "in the third cabniet on the left";
			}

			else
			{
				location_2 = "in the first cabniet on the right";
			}
		}


		else if (locate[1] >= counter_and_cabinet_level[1])
		{
			location_1 = "on the top of the drawer and under the cabinet";
			destinationString = "drawer";
		}

		else if (locate[1] >= counter_and_cabinet_level[2])
		{
			location_1 = "in the drawer";
			destinationString = "drawer";

			if (locate[2] > counter_2[0])
			{
				location_2 = "in the first floor of the first drawer on the left";
			}

			else if (locate[2]>counter_2[1])
			{
				location_2 = "in the first floor of the second drawer on the left";

			}
			else
			{
				location_2 = "in the first floor of the third drawer on the left";
			}
		}

		else if (locate[1] >= counter_and_cabinet_level[3])
		{
			if (locate[2] > counter_2[0])
			{
				location_2 = "in the second floor of the first drawer on the left";
			}

			else if (locate[2]>counter_2[1])
			{
				location_2 = "in the second floor of the second drawer on the left";

			}

			else
			{
				location_2 = "in the second floor of the third drawer on the left";
			}
		}
	}

	if (in_something(locate, fridge))
	{
		location_1 = "in the fridge";
		destinationString = "fridge";

		if (locate[1] > fridge_level[0])
		{
			location_2 = "in the upper part of the fridge";
		}
		else if (locate[1] > fridge_level[1])
		{
			if (locate[2] > fridge_center_line)
			{
				location_2 = "in the right center drawer";
			}
			else
			{
				location_2 = "in the left center drawer";
			}
		}
		else if (locate[1] >= fridge_level[2])
		{
			location_2 = "in the second drawer from the floor";
		}
		else
		{
			location_2 = "in the bottom drawer";
		}
	}


	if (in_something(locate, shelf))
	{
		location_1 = "on the wooden shelf";
		destinationString = "wooden shelf";

		if (locate[1] >= shelf_level[0])
		{
			location_2 = "on the first floor of the shelf";
		}
		else if (locate[1] >= shelf_level[1])
		{
			location_2 = "on the second floor of the shelf";
		}
		else if (locate[1] >= shelf_level[2])
		{
			location_2 = "on the third floor of the shelf";
		}
		else
		{
			location_2 = "on the bottom floor of the shelf";
		}
	}

	if (in_something(locate, wooden_table_1))
	{
		location_1 = "on the left wooden table";

		destinationString = "wooden table on the left";
	}

	if (in_something(locate, wooden_table_2))
	{
		location_1 = "on the right wooden table";

		destinationString = "wooden table on the right";
	}
}

if (environment == "LayoutHN12")
{
	double shelf[4] = { 0.964,6.843,-1.558,6.227 };
	double shelf_level[3] = { 1.43,1.04,0.65 };

	double fridge[4] = { 2.143,6.876,1.453,6.167 };
	double fridge_level[3] = { 0.927,0.742,0.38 };
	double fridge_center_line = 1.694;

	double blue_cupboard[4] = { 3.746,6.903,2.191,6.249 };
	double blue_cupboard_level[2] = { 1.229,0.72 };
	double cupboard[3] = { 0,0,0 };
	double cupboard_centerline = 2.952;
	double cupboard_leftside_line = 0.268;
	double cupboard_rightside_line = 0.536;
	double cupboard_rightside_cut = 3.32;

	double counter_and_cabinet[4] = { 6.203, 6.91, 3.746, 6.283 };
	double counter_and_cabinet_level[3] = { 1.3839, 0.8565, 0.35 };
	double counter[2] = { 5.384 ,4.583 };
	double cabinet[3] = { 5.585 ,4.944 ,4.343 };
	double cabinet_line = 1.50785;

	double round_table_1[5] = { 5.199,0,3.128,0.32,0 };
	double round_table_2[5] = { 4.323,0,1.835,0.32,0 };
	double round_table_3[5] = { 5.14,0,0.521,0.32,0 };

	double kitchen_drawer[4] = { 6.171,4.65,0.7,4.078 };
	double kitchen_drawer_level[3] = { 0.878,0.624,0.364 };
	double kitchen_drawer_cut[5] = { 5.241,4.355,3.426,2.521,1.605 };

	double wall_cabinet[7] = { 5.558,4.939,4.351,3.715,3.08,2.486,1.89 };
	double wall_cabinet_line = 1.401;

	geometry_msgs::Point targetPosition = taskInfo.target_object.position;


	locate[0] = -targetPosition.y;
	locate[1] = targetPosition.z;
	locate[2] = targetPosition.x;

	if (in_something(locate, kitchen_drawer))
	{
		if (locate[1] > cabinet_line)
		{
			location_1 = "in the wall cabinet";
			destinationString = "wall cabinet";

			if (locate[0] > wall_cabinet[0])
			{
				location_2 = "in the first wall cabinet on the left";
			}
			else if (locate[0] > wall_cabinet[1])
			{
				location_2 = "in the second wall cabinet on the left";
			}
			else if (locate[0] > wall_cabinet[2])
			{
				location_2 = "in the third wall cabinet on the left";
			}
			else if (locate[0] > wall_cabinet[3])
			{
				location_2 = "in the forth wall cabinet on the left";
			}
			else if (locate[0] > wall_cabinet[4])
			{
				location_2 = "in the fifth wall cabinet on the left";
			}
			else if (locate[0] > wall_cabinet[5])
			{
				location_2 = "in the sixth wall cabinet on the left";
			}
			else if (locate[0] > wall_cabinet[6])
			{
				location_2 = "in the second wall cabinet on the right";
			}
			else
			{
				location_2 = "in the first wall cabinet on the right";
			}
		}
		else
		{
			location_1 = "in the kitchen drawer under the cabinet";
			destinationString = "kitchen drawer under the cabinet";

			if (locate[1] > kitchen_drawer_level[0])
			{
				location_2 = "on the top of the kitchen drawer";
			}
			else if (locate[1] > kitchen_drawer_level[1])
			{
				if (locate[0] > kitchen_drawer_cut[0])
				{
					location_2 = "in the first floor and the first drawer on the left";
				}
				else if (locate[0] > kitchen_drawer_cut[1])
				{
					location_2 = "in the first floor and the second drawer on the left";
				}
				else if (locate[0] > kitchen_drawer_cut[2])
				{
					location_2 = "in the first floor and the third drawer on the left";
				}
				else if (locate[0] > kitchen_drawer_cut[3])
				{
					location_2 = "in the first floor and the forth drawer on the left";
				}
				else if (locate[0] > kitchen_drawer_cut[4])
				{
					location_2 = "in the first floor and the fifth drawer on the left";
				}
				else
				{
					location_2 = "in the first floor and the first drawer on the right";
				}
			}
			else if (locate[1] > kitchen_drawer_level[2])
			{
				if (locate[0] > kitchen_drawer_cut[0])
				{
					location_2 = "in the second floor and the first drawer on the left";
				}
				else if (locate[0] > kitchen_drawer_cut[1])
				{
					location_2 = "in the second floor and the second drawer on the left";
				}
				else if (locate[0] > kitchen_drawer_cut[2])
				{
					location_2 = "in the second floor and the third drawer on the left";
				}
				else if (locate[0] > kitchen_drawer_cut[3])
				{
					location_2 = "in the second floor and the forth drawer on the left";
				}
				else if (locate[0] > kitchen_drawer_cut[4])
				{
					location_2 = "in the second floor and the fifth drawer on the left";
				}
				else
				{
					location_2 = "in the second floor and the first drawer on the right";
				}
			}
			else
			{
				if (locate[0] > kitchen_drawer_cut[0])
				{
					location_2 = "in the third floor and the first drawer on the left";
				}
				else if (locate[0] > kitchen_drawer_cut[1])
				{
					location_2 = "in the third floor and the second drawer on the left";
				}
				else if (locate[0] > kitchen_drawer_cut[2])
				{
					location_2 = "in the third floor and the third drawer on the left";
				}
				else if (locate[0] > kitchen_drawer_cut[3])
				{
					location_2 = "in the third floor and the forth drawer on the left";
				}
				else if (locate[0] > kitchen_drawer_cut[4])
				{
					location_2 = "in the third floor and the fifth drawer on the left";
				}
				else
				{
					location_2 = "in the third floor and the first drawer on the right";
				}
			}
		}
	}

	if (in_something_round(locate, round_table_1))
	{
		location_1 = "on the left round table";
		destinationString = "round table on the left";
	}
	if (in_something_round(locate, round_table_2))
	{
		location_1 = "on the middle round table";
		destinationString = "round table in the middle";
	}

	if (in_something_round(locate, round_table_3))
	{
		location_1 = "on the right round table";
		destinationString = "round table on the right";
	}

	if (in_something(locate, fridge))
	{
		location_1 = "in the fridge";
		destinationString = "fridge";

		if (locate[1] > fridge_level[0])
		{
			location_2 = "in the upper part of the fridge";
		}
		else if (locate[1] > fridge_level[1])
		{
			if (locate[0] > fridge_center_line)
			{
				location_2 = "in the right center drawer";
			}
			else
			{
				location_2 = "in the left center drawer";
			}
		}
		else if (locate[1] >= fridge_level[2])
		{
			location_2 = "in the second drawer from the floor";
		}
		else
		{
			location_2 = "in the bottom drawer";
		}
	}

	if (in_something(locate, blue_cupboard))
	{
		location_1 = "in the blue cupboard";
		destinationString = "blue cupboard";

		if (locate[1] >= blue_cupboard_level[0])
		{
			if (locate[0] > cupboard_centerline)
			{
				location_2 = "in the upper and right part of the cupboard";
			}
			else
			{
				location_2 = "in the upper and left part of the cupboard";
			}
		}

		else if (locate[1] >= blue_cupboard_level[1])
		{
			location_2 = "in the middle part of the cupboard";
		}

		else
		{
			if (locate[0] > cupboard_centerline)
			{
				location_2 = "in the lower and right part of the cupboard";
				if (locate[0] > cupboard_rightside_cut)
				{
					if (locate[1] > cupboard_rightside_line)
					{
						location_3 = "in the right drawer";
					}
					else
					{
						location_3 = "in the right chest";
					}
				}
				else
				{
					if (locate[1] > cupboard_rightside_line)
					{
						location_3 = "in the left drawer";
					}
					else
					{
						location_3 = "in the left chest";
					}
				}
			}

			else
			{
				location_2 = "in the lower and left part of the cupboard";

				if (locate[1] < cupboard_leftside_line)
				{
					location_3 = "in the bottom drawer";
				}
			}
		}
	}

	if (in_something(locate, counter_and_cabinet))
	{

		if (locate[1] >= counter_and_cabinet_level[0])
		{
			location_1 = "in the wall cabinet";
			destinationString = "wall cabinet ";

			if (locate[1]>cabinet_line)
			{
				location_3 = "on the upper layer ";
			}
			else
			{
				location_3 = "on the lower layer ";
			}


			if (locate[0]>cabinet[0])
			{
				location_2 = "in the first cabinet on the right";
			}
			else if (locate[0]>cabinet[1])
			{
				location_2 = "in the second cabinet on the right";
			}
			else if (locate[0]>cabinet[2])
			{
				location_2 = "in the third cabniet on the right";
			}

			else
			{
				location_2 = "in the first cabniet on the left";
			}
		}


		else if (locate[1] >= counter_and_cabinet_level[1])
		{
			location_1 = "on the top of the drawer and under the cabinet";
			destinationString = "drawer";
		}

		else if (locate[1] >= counter_and_cabinet_level[2])
		{
			location_1 = "in the drawer";
			destinationString = "drawer";

			if (locate[0] > counter[0])
			{
				location_2 = "in the first floor of the first drawer on the right";
			}

			else if (locate[0]>counter[1])
			{
				location_2 = "in the first floor of the second drawer on the right";

			}
			else
			{
				location_2 = "in the first floor of the third drawer on the right";
			}
		}

		else if (locate[1] >= counter_and_cabinet_level[3])
		{
			if (locate[0] > counter[0])
			{
				location_2 = "in the second floor of the first drawer on the right";
			}

			else if (locate[0]>counter[1])
			{
				location_2 = "in the second floor of the second drawer on the right";

			}

			else
			{
				location_2 = "in the second floor of the third drawer on the right";
			}
		}
	}

}

if (environment == "LayoutHN13")
{
	double round_table_1[5] = { -4.04,0.92,2.2,0.32,0 };
	double round_table_2[5] = { -4.05,0.92,1.341,0.32,0 };
	double round_table_3[5] = { -5.46,0.92,-0.57,0.32,0 };
	double round_table_4[5] = { -5.437,0.92,1.341,0.32,0 };
	double round_table_5[5] = { -1.351,0.92,5.11,0.32,0 };
	double round_table_6[5] = { 1.602,0.92,5.146,0.32,0 };


	double glass_wagon_1[4] = { -0.294,6.786,-0.897,6.193 };
	double glass_wagon_2[4] = { 0.943,6.79,0.34,6.193 };
	double wooden_wagon[4] = { -1.642,4.912,-2.249,4.297 };
	double wooden_wagon_1[4] = { -3.843,6.752,-4.443,6.149 };
	double wagon_level[2] = { 0.679,0.362 };

	double fridge[4] = { -4.491,6.778,-5.176,6.09 };
	double fridge_level[3] = { 0.927,0.742,0.38 };
	double fridge_center_line = -4.942;

	double counter_and_cabinet[4] = { -3.346,4.176,-5.82,3.561 };
	double counter_and_cabinet_level[3] = { 1.3839, 0.8565, 0.35 };
	double counter[2] = { -4.185 ,-4.987 };
	double cabinet[3] = { -3.979 ,-4.558 ,-5.169 };
	double cabinet_line = 1.673;

	

	double wall_cabinet[4] = { -1.731,4.912,-2.249,4.297 };
	double wall_cabinet_level = 1.401;
	double wall_cabinet_line = 0;
	double cabinetWall[3] = { 6.062,5.459,4.856 };
	double kitchen_drawer[4] = { -1.6758,6.766,-2.225,4.9391 };
	double kitchen_drawer_cut = 5.856;
	double kitchen_drawer_level[3] = { 0.914,0.661,0.417 };

	double trashcan[5] = { -3.612,0,6.577,0.2,0 };

	geometry_msgs::Point targetPosition = taskInfo.target_object.position;


	locate[0] = -targetPosition.y;
	locate[1] = targetPosition.z;
	locate[2] = targetPosition.x;

	if (in_something_round(locate, round_table_1))
	{
		location_1 = "on the round cafe set";
		destinationString = "round cafe set";
	}

	if (in_something_round(locate, round_table_2))
	{
		location_1 = "on the round cafe set";
		destinationString = "round cafe set";
	}

	if (in_something_round(locate, round_table_3))
	{
		location_1 = "on the round cafe set";
		destinationString = "round cafe set";
	}

	if (in_something_round(locate, round_table_4))
	{
		location_1 = "on the round cafe set";
		destinationString = "round cafe set";
	}

	if (in_something_round(locate, round_table_5))
	{
		location_1 = "on the round cafe set";
		destinationString = "round cafe set";
	}

	if (in_something_round(locate, round_table_6))
	{
		location_1 = "on the round cafe set";
		destinationString = "round cafe set";
	}

	if (in_something(locate, glass_wagon_1))
	{
		location_1 = "on the left glass wagon";
		destinationString = "glass wagon on the left";

		if (locate[1] >= wagon_level[0])
		{
			location_2 = "on the top of the glass wagon";
		}
		else if (locate[1] >= wagon_level[1])
		{
			location_2 = "on the first floor of the glass wagon";
		}
		else
		{
			location_2 = "on the second floor of the glass wagon";
		}
	}

	if (in_something(locate, glass_wagon_2))
	{
		location_1 = "on the right glass wagon";
		destinationString = "glass wagon on the right";

		if (locate[1] >= wagon_level[0])
		{
			location_2 = "on the top of the glass wagon";
		}
		else if (locate[1] >= wagon_level[1])
		{
			location_2 = "on the first floor of the glass wagon";
		}
		else
		{
			location_2 = "on the second floor of the glass wagon";
		}
	}

	if (in_something(locate, fridge))
	{
		location_1 = "in the fridge";
		destinationString = "fridge";

		if (locate[1] > fridge_level[0])
		{
			location_2 = "in the upper part of the fridge";
		}
		else if (locate[1] > fridge_level[1])
		{
			if (locate[0] > fridge_center_line)
			{
				location_2 = "in the right center drawer";
			}
			else
			{
				location_2 = "in the left center drawer";
			}
		}
		else if (locate[1] >= fridge_level[2])
		{
			location_2 = "in the second drawer from the floor";
		}
		else
		{
			location_2 = "in the bottom drawer";
		}
	}

	if (in_something(locate, wooden_wagon_1))
	{
		location_1 = "on the wooden wagon near the fridge";
		destinationString = "wooden wagon near the fridge ";

		if (locate[1] >= wagon_level[0])
		{
			location_2 = "on the top of the wooden wagon";
		}
		else if (locate[1] >= wagon_level[1])
		{
			location_2 = "on the first floor of the wooden wagon";
		}
		else
		{
			location_2 = "on the second floor of the wooden wagon";
		}
	}

	if (in_something(locate, wall_cabinet))
	{
		if (locate[1] > wall_cabinet_level)
		{
			location_1 = "in the wall cabinet above the wagon";
			destinationString = "wall cabinet above the wagon";

			if (locate[2] > cabinetWall[0])
			{
				location_2 = "in the first cabinet on the left";
			}
			else if (locate[2] > cabinetWall[1])
			{
				location_2 = "in the second cabinet on the left";
			}
			else if (locate[2] > cabinetWall[2])
			{
				location_2 = "in the third cabinet on the left";
			}
			else
			{
				location_2 = "in the first cabinet on the right";
			}
		}
		else
		{
			if (in_something(locate, kitchen_drawer))
			{
				location_1 = "in the kitchen drawer near the wagon";

				destinationString = "kitchen drawer near the wagon";

				if (locate[1] > kitchen_drawer_level[0])
				{
					location_2 = "on the top of the kitchen drawer";
				}
				else if (locate[1] > kitchen_drawer_level[1])
				{
					if (locate[3] > kitchen_drawer_cut)
					{
						location_2 = "in the first floor and the left drawer";

					}
					else
					{
						location_2 = "in the first floor and the right drawer";
					}

				}
				else if (locate[1] > kitchen_drawer_level[2])
				{
					if (locate[3] > kitchen_drawer_cut)
					{
						location_2 = "in the second floor and the left drawer";

					}
					else
					{
						location_2 = "in the second floor and the right drawer";
					}
				}
				else
				{
					if (locate[3] > kitchen_drawer_cut)
					{
						location_2 = "in the third floor and the left drawer";

					}
					else
					{
						location_2 = "in the third floor and the right drawer";
					}
				}
			}
			if (in_something(locate, wooden_wagon))
			{
				location_1 = "on the wooden wagon";
				destinationString = "wooden wagon";

				if (locate[1] >= wagon_level[0])
				{
					location_2 = "on the top of the wooden wagon";
				}
				else if (locate[1] >= wagon_level[1])
				{
					location_2 = "on the first floor of the wooden wagon";
				}
				else
				{
					location_2 = "on the second floor of the wooden wagon";
				}
			}
		}
	}

	if (in_something(locate, counter_and_cabinet))
	{

		if (locate[1] >= counter_and_cabinet_level[0])
		{
			location_1 = "in the wall cabinet";
			destinationString = "wall cabinet ";

			if (locate[1]>cabinet_line)
			{
				location_3 = "on the upper layer ";
			}
			else
			{
				location_3 = "on the lower layer ";
			}


			if (locate[0]>cabinet[0])
			{
				location_2 = "in the first cabinet on the left";
			}
			else if (locate[0]>cabinet[1])
			{
				location_2 = "in the second cabinet on the left";
			}
			else if (locate[0]>cabinet[2])
			{
				location_2 = "in the third cabniet on the left";
			}

			else
			{
				location_2 = "in the first cabniet on the right";
			}
		}


		else if (locate[1] >= counter_and_cabinet_level[1])
		{
			location_1 = "on the top of the drawer and under the cabinet";
			destinationString = "drawer";
		}

		else if (locate[1] >= counter_and_cabinet_level[2])
		{
			location_1 = "in the drawer";
			destinationString = "drawer";

			if (locate[0] > counter[0])
			{
				location_2 = "in the first floor of the first drawer on the left";
			}

			else if (locate[0]>counter[1])
			{
				location_2 = "in the first floor of the second drawer on the left";

			}
			else
			{
				location_2 = "in the first floor of the third drawer on the left";
			}
		}

		else if (locate[1] >= counter_and_cabinet_level[3])
		{
			if (locate[0] > counter[0])
			{
				location_2 = "in the second floor of the first drawer on the left";
			}

			else if (locate[0]>counter[1])
			{
				location_2 = "in the second floor of the second drawer on the left";

			}

			else
			{
				location_2 = "in the second floor of the third drawer on the left";
			}
		}
	}

	if (in_something_round(locate, trashcan))
	{
		location_1 = "in the trashcan";

		destinationString = "trashcan";
	}
}

					try_ = destinationSentenceOut(location_1, location_2, location_3);

					guideMsg = destinationArrivedOut(location_1 ,location_2 ,location_3);
					geometry_msgs::Point avatarPosition = avatarStatus.head.position;
					
					if(destinationString ==" ")
					{
						destinationString = is_may_near_furni();
					}
					//cout<<std::boolalpha<<(bool)isCatched<<endl;

					//isCatched = (avatarStatus.is_target_object_in_left_hand||avatarStatus.is_target_object_in_right_hand);
/*
					cout<<std::boolalpha<<(bool)isCatched<<endl;
					cout<<std::boolalpha<<(bool)avatarStatus.is_target_object_in_left_hand<<endl;
					cout<<std::boolalpha<<(bool)avatarStatus.is_target_object_in_right_hand<<endl;
					cout<<std::boolalpha<<(bool)(avatarStatus.is_target_object_in_left_hand||avatarStatus.is_target_object_in_right_hand)<<endl;

*/
					double avatarLocation[3];
					avatarLocation[0] = -avatarPosition.y;
					avatarLocation[1] = avatarPosition.z;
					avatarLocation[2] = avatarPosition.x;
					cout<<try_<<endl;
					cout<<guideMsg<<endl;

					cout<<std::boolalpha<<(bool)isNear<<endl;
					isNear = arrived(locate,avatarLocation);


					cout<<std::boolalpha<<(bool)isNear<<endl;
					

					if(isNear == false)						
					{


						//int WaitTime = 15;
						
							//cout<<"0000000000000000000000000000000000000000000000000"<<endl;
							if(speakGuidanceMessage(pubHumanNaviMsg, pubGuidanceMsg, try_))
							{
							//	time = ros::Time::now();
								isMoveDirection = true;
								turn_to_destination();
							}
						
					}

					else
					{
						//int WaitTime = 0;
						//if(time.sec + WaitTime < ros::Time::now().sec)
						{
							if(speakGuidanceMessage(pubHumanNaviMsg, pubGuidanceMsg, guideMsg))
							{
								//time = ros::Time::now();
								isMoveDirection = true;
								step++;
								//cout<<"isMoveDirection: "<<isMoveDirection<<endl;
								
								/*if(isCatched == true)
								{
									step++;
									
								}*/
							}
						}
					}


						
						
					}

					break;
				}
				case WaitTaskFinished:
				{
					if(isFinished)
					{
						ROS_INFO("Task finished");
						step++;
						break;
					}

					if(isRequestReceived)
					{
						bool isSpeaked;
						if(ros::Time::now().sec % 2 > 0)
						{
							isSpeaked = speakGuidanceMessage(pubHumanNaviMsg, pubGuidanceMsg, guideMsg);
						}
						else
						{
							isSpeaked = speakGuidanceMessage(pubHumanNaviMsg, pubGuidanceMsg, "You can find the wall cabinet above the kitchen sink.");
						}

						if(isSpeaked)
						{
							isRequestReceived = false;
						}
					}

					int WaitTime = 25;
					if(time.sec + WaitTime < ros::Time::now().sec)
					{
						if(!isSentGetAvatarStatus && !isSentGetObjectStatus)
						{
							sendMessage(pubHumanNaviMsg, MSG_GET_AVATAR_STATUS);
							sendMessage(pubHumanNaviMsg, MSG_GET_OBJECT_STATUS);
							isSentGetAvatarStatus = true;
							isSentGetObjectStatus = true;
							time = ros::Time::now();
						}
					}

					break;
				}
				case TaskFinished:
				{
					// Wait MSG_GO_TO_NEXT_SESSION or MSG_MISSION_COMPLETE
					//delete []myObject;
					break;
				}
			}
			}

			ros::spinOnce();

			loopRate.sleep();
		}

		return 0;
	}
};

int main(int argc, char **argv)
{
	HumanNavigationSample humanNaviSample;

	humanNaviSample.run(argc, argv);

	return 0;
}

