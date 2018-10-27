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

	double nearTargetDistance = 0.1;
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
		putWrong
	};


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
		 directionString = "";

		 stopMove = false;

		 LeftHandCatch = false;
		 RightHandCatch = false;

		 isGoDestination = false;
		 targetInLeftHand = false;
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


		output = output + "  When you put your hand on the right object we will remiand you. ";

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
				output = "rightS";
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
		//cout<<"moveStep"<<moveStep<<endl;
		switch(moveStep)
			{
				case goRight:
				{
					//ROS_INFO_STREAM("step:goright"<<std::endl);
					//ROS_INFO_STREAM("You are on the right direction."<<std::endl);
					//output = "You are on the right direction.";
					output = "The "+furnitureString +" is at your "+LRdirection+", "+directionString+ "o'clock direction. You are " + stringDistance +" meters away from the "+t_object+".";
					break;
					//is_on_the_way();
				}
				case goWrong:
				{
					//ROS_INFO_STREAM("You are on the wrong direction."<<std::endl);
					//ROS_INFO_STREAM("The "+targetObjectName+" is ");
					//ROS_INFO_STREAM("Please go ");
					output = "The "+furnitureString +" is at your "+LRdirection+", "+directionString+ "o'clock direction. You are " + stringDistance +" meters away from the "+t_object+".";
					
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
					output = output + "to catch the "+t_object+".";
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
					cout<<"000002---------------speak catchWrong--------------"<<endl;
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
					if(Avatar.object_in_left_hand!=""||Avatar.object_in_right_hand!="")//抓了东西
					{	
						cout<<"000000---------------ininini--------------"<<endl;
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
							cout<<"000001---------------catchWrong--------------"<<endl;
							moveStep = catchWrong;	
						}
					}	
					else
					{
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
					//is_on_the_way();
					//ROS_INFO_STREAM("goRight");
					break;
				}
				case goWrong:
				{
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
						
							left_or_right();
						
						}
						else
                    				{
                    					isMoveDirection = false;	
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
						if(is_near()&&!is_stop_move_hand())
						{
							//cout<<"isGoDestination: "<<isGoDestination<<endl;
							move_hand();
						}
						//判断是否抓住
						if(Avatar.object_in_left_hand!=""||Avatar.object_in_right_hand!="")//抓了东西
						{	
							cout<<"000000---------------ininini--------------"<<endl;
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
								cout<<"000001---------------catchWrong--------------"<<endl;
								moveStep = catchWrong;	
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
								step = GuideForPlacement;
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
					cout<<"000004---------------runmoveCatchWrong--------------"<<endl;
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
				int wait = 0;
				if(output !="nothing_default.")
				{
					speakGuidanceMessage(pubHumanNaviMsg, pubGuidanceMsg, output);
					//cout<<output<<endl;
					//time = ros::Time::now();
					//cout<<"step："<<step<<endl;

					//cout<<"家具"<<furnitureString<<endl;
				}
				//ROS_INFO_STREAM(output);
				

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


					if(environment == "ExampleLayoutHN01")
					{


						//cout<<"000000000000000000"<<endl;
						double drawer_unit_white[4] = { 0,0,0,0 };
						double drawer_unit_brown[4] = { 0,0,0,0 };
						double drawer_unit_brown_wide[4] = { 0,0,0,0 };
						double drawer_unit_white_wide[4] = { 0,0,0,0 };
						double drawer_level[4] = { 0,0,0,0 };
						double kitchen_drawer[4] = { 0,0,0,0 };
						double kitchen_drawer_line = 0;
						double kitchen_drawer_level[4] = { 0,0,0,0 };
						double counter_and_cabinet[4] = { 3.89651, 3.87101, 3.30582, 1.44405 };
						double counter_and_cabinet_level[3] = { 1.3839, 0.8565, 0.35 };
						double counter[2] = { 3.06622 ,2.212 };
						double cabinet[3] = { 2.04622 ,2.6556 ,3.2664 };
						double cabinet_line = 1.50785;
						double basketball_board[5] = { 0,0,0,0,0 };
						double bed[4] = { 0,0,0,0 };
						double bookshelf[4] = { 0,0,0,0 };
						double bookshelf_level[] = { 0,0,0 };
						double round_table[5] = { 0,0,0,0,0 };
						double paper_box[4] = { 0,0,0,0 };
						double CDplayer[4] = { 0,0,0,0 };
						double big_chair_white[4] = { 0,0,0,0 };
						double big_chair_black[4] = { 0,0,0,0 };
						double small_chair_white[4] = { 0,0,0,0 };
						double small_chair_wooden[4] = { 0,0,0,0 };
						double cornersofa[4] = { 0,0,0,0 };
						double sofa[4] = { 0,0,0,0 };
						double blue_cupboard[4] = { 2.995,8.158,1.4819,7.5725 };
						double blue_cupboard_level[2] = { 1.229,0.72 };
						double cupboard[3] = { 0,0,0 };
						double cupboard_centerline = 2.249;
						double cupboard_leftside_line = 0.268;
						double cupboard_rightside_line = 0.536;
						double cupboard_rightside_cut = 2.623;
						double glass_dining_table[4] = { -0.44,5.669,-2.644,4.616 };
						double wooden_dining_table[4] = { 0,0,0,0 };
						double fridge[4] = { 4.138,8.186,3.461,7.513 };
						double fridge_level[3] = { 0.927,0.742,0.38 };
						double fridge_center_line = 3.716;
						double brown_mat[4] = { 0,0,0,0 };
						double blue_mat[4] = { 0,0,0,0 };
						double shelf[4] = { 0,0,0,0 };
						double shelf_level[3] = { 0,0,0 };
						double sidetable[4] = { 0,0,0,0 };
						double oven[4] = { 0,0,0,0 };
						double trashcan[5] = { 0,0,0,0,0 };
						double TVbass[4] = { 0,0,0,0 };
						double TVbass_level[2] = { 0,0 };
						double glass_wagon[4] = { 0,0,0,0 };
						double wooden_wagon[4] = { 0,0,0,0 };
						double wagon_level[2] = { 0,0 };
						double wooden_table[4] = { 0,0,0,0 };

						geometry_msgs::Point targetPosition = taskInfo.target_object.position;


						locate[0] = -targetPosition.y;
						locate[1] = targetPosition.z;
						locate[2] = targetPosition.x;


						cout<<locate[0]<<" "<<"3333333333"<<endl;
						cout<<locate[1]<<" "<<"4444444444"<<endl;
						cout<<locate[2]<<" "<<"5555555555"<<endl;

						if (in_something(locate, drawer_unit_white_wide))
						{
							location_1 = "in the larger white drawer";

							furnitureString = "larger white drawer";

							if (locate[1] >= drawer_level[0])
							{
								location_2 = "on the top of the larger white drawer";
							}
							else if (locate[1] >= drawer_level[1])
							{
								location_2 = "in the first floor of the larger white drawer";
							}
							else if (locate[1] >= drawer_level[2])
							{
								location_2 = "in the second floor of the larger white drawer";
							}
							else if (locate[1] >= drawer_level[3])
							{
								location_2 = "in the third floor of the larger white drawer";
							}
							else
							{
								location_2 = "in the bottom floor of the larger white drawer";
							}
						}

						if (in_something(locate, drawer_unit_brown_wide))
						{
							location_1 = "in the larger brown drawer";

							furnitureString = "larger brown drawer";

							if (locate[1] >= drawer_level[0])
							{
								location_2 = "in the top of the larger brown drawer";
							}
							else if (locate[1] >= drawer_level[1])
							{
								location_2 = "in the first floor of the larger brown drawer";
							}
							else if (locate[1] >= drawer_level[2])
							{
								location_2 = "in the second floor of the larger brown drawer";
							}
							else if (locate[1] >= drawer_level[3])
							{
								location_2 = "in the third floor of the larger brown drawer";
							}
							else
							{
								location_2 = "in the bottom floor of the larger brown drawer";
							}
						}

						if (in_something(locate, drawer_unit_white))
						{
							location_1 = "in the smaller white drawer";

							furnitureString = "smaller white drawer";

							if (locate[1] >= drawer_level[0])
							{
								location_2 = "in the top of the smaller white drawer";
							}
							else if (locate[1] >= drawer_level[1])
							{
								location_2 = "in the first floor of the smaller white drawer";
							}
							else if (locate[1] >= drawer_level[2])
							{
								location_2 = "in the second floor of the smaller white drawer";
							}
							else if (locate[1] >= drawer_level[3])
							{
								location_2 = "in the third floor of the smaller white drawer";
							}
							else
							{
								location_2 = "in the bottom floor of the smaller white drawer";
							}
						}


						if (in_something(locate, drawer_unit_brown))
						{
							location_1 = "in the smaller brown drawer";

							furnitureString = "smaller brown drawer";

							if (locate[1] >= drawer_level[0])
							{
								location_2 = "in the top of the smaller brown drawer";
							}
							else if (locate[1] >= drawer_level[1])
							{
								location_2 = "in the first floor of the smaller brown drawer";
							}
							else if (locate[1] >= drawer_level[2])
							{
								location_2 = "in the second floor of the smaller brown drawer";
							}
							else if (locate[1] >= drawer_level[3])
							{
								location_2 = "in the third floor of the smaller brown drawer";
							}
							else
							{
								location_2 = "in the bottom floor of the smaller brown drawer";
							}
						}

						//此处有修改项
						if (in_something(locate, kitchen_drawer))
						{
							location_1 = "in the kitchen drawer";

							furnitureString = "kitchen drawer";


							if (locate[2] >= kitchen_drawer_line)
							{
								if (locate[1] > kitchen_drawer_level[0])
								{
									if (in_something(locate, oven))
									{
										location_2 = "in the oven";
									}
									else
									{
										location_2 = "on the top of the kitchen drawer";
									}
								}

								else if (locate[1] >= kitchen_drawer_level[1])
								{
									location_2 = "in the first floor of the right drawer";
								}
								else if (locate[1] >= kitchen_drawer_level[2])
								{
									location_2 = "in the second floor of the right drawer";
								}
								else
								{
									location_2 = "in the bottom floor of the right drawer";
								}
							}
							else
							{
								if (locate[1] > kitchen_drawer_level[0])
								{
									if (in_something(locate, oven))
									{
										location_2 = "in the oven";
									}
									else
									{
										location_2 = "on the top of the kitchen drawer";
									}
								}

								else if (locate[1] >= kitchen_drawer_level[1])
								{
									location_2 = "in the first floor of the left drawer";
								}
								else if (locate[1] >= kitchen_drawer_level[2])
								{
									location_2 = "in the second floor of the left drawer";
								}
								else
								{
									location_2 = "in the bottom floor of the left drawer";
								}
							}
						}




						//此处有修改项
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


						if (in_something(locate, bed))
						{
							location_1 = "on the bed";
							furnitureString = "bed";
						}

						if (in_something_round(locate, round_table))
						{
							location_1 = "on the round cafe set";
							furnitureString = "cafe set";
						}

						if (in_something(locate, paper_box))
						{
							location_1 = "in the paper box";
							furnitureString = "paper box";
						}

						if (in_something(locate, big_chair_black))
						{
							location_1 = "on the big black chair";
							furnitureString = "big black chair";
						}

						if (in_something(locate, big_chair_white))
						{
							location_1 = "on the big white chair";
							furnitureString = "big white chair";
						}

						if (in_something(locate, small_chair_white))
						{
							location_1 = "on the small white chair";
							furnitureString = "small white chair";
						}


						if (in_something(locate, small_chair_wooden))
						{
							location_1 = "on the small wooden chair";
							furnitureString = "small wooden chair";
						}



						//有修改项
						if (in_something(locate, blue_cupboard))
						{
							location_1 = "in the blue cupboard";
							furnitureString = "blue cupboard";

							if (locate[1] >= blue_cupboard_level[0])
							{
								if (locate[2] > cupboard_centerline)
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
								if (locate[2] > cupboard_centerline)
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

						if (in_something(locate, glass_dining_table))
						{
							cout<<"11111111111111111"<<endl;
							location_1 = "on the glass dinning table";
							furnitureString = "glass dining table";
						}

						if (in_something(locate, wooden_dining_table))
						{
							location_1 = "on the wooden dinning table";
							furnitureString = "wooden dining table";
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


						if (in_something(locate, brown_mat))
						{
							location_1 = "on the brown mat";

							furnitureString = "brown mat";
						}

						if (in_something(locate, blue_mat))
						{
							location_1 = "on the blue mat";

							furnitureString = "blue mat";
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

						if (in_something(locate, sidetable))
						{
							location_1 = "on the short sidetable";

							furnitureString = "short sidetable";
						}

						if (in_something(locate, sofa))
						{
							location_1 = "on the sofa";

							furnitureString = "sofa";
						}

						if (in_something(locate, cornersofa))
						{
							location_1 = "on the corner sofa";

							furnitureString = "corner sofa";
						}
							
						if (in_something_round(locate, trashcan))
						{
							location_1 = "in the trashcan";

							furnitureString = "trashcan";
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


						if (in_something(locate, wooden_wagon))
						{
							location_1 = "on the wooden wagon";
							furnitureString = "wooden wagon";

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

						if (in_something(locate, wooden_table))
						{
							location_1 = "on the short wooden table";

							furnitureString = "wooden table";
						}

						else
							cout<<"Nothing!!!!!!"<<endl;
					}

						



					


					if(environment == "ExampleLayoutHN02")
					{
						double wagon[4] = {-2.60412, 6.18329, -3.23151, 5.6598};
						double wagon_level[2] = {0.725514, 0.3923};
						double wood_table[4] = {2.2469, 6.1205, 0.995, 5.469};
						double shelf[4] = {3.9178, 0.73622, 3.296, -0.8959};
						double shelf_level[3] = {1.4012, 1.01263, 0.619365};
						double trashcan_01[5] = {-1.473, 0, 5.917, 0.2, 0};
						double trashcan_02[5] = {-0.823, 0, 5.917, 0.2, 0};
						double trashcan_03[5] = {-0.173, 0, 5.917, 0.2, 0};
						double counter_and_cabinet[4] = {3.89651, 3.87101, 3.30582, 1.44405};
						double counter_and_cabinet_level[3] = {1.3839, 0.8565, 0.35};
						double counter[2] = {3.06622 ,2.212};
						double cabinet[3] = {2.04622 ,2.6556 ,3.2664};
						double cabinet_line = 1.50785;

						

						geometry_msgs::Point targetPosition = taskInfo.target_object.position;

						
						//furnitureString = "5";
						//cout<<"F:"<<furnitureString<<endl;;
					

						locate[0] = -targetPosition.y;
						locate[1] = targetPosition.z;
						locate[2] = targetPosition.x;

						if(in_something(locate,wagon))
						{
							location_1 = "on the wagon";
							furnitureString = "wagon";

							if(locate[1] >= wagon_level[0])
							{
								location_2 = "on the top of the wagon";
							}
							else if(locate[1]>=wagon_level[1])
							{
								location_2 = "on the first floor of the wagon";
							}
							else
							{
								location_2 = "on the second floor of the wagon";
							}
						}

						

						if(in_something(locate,shelf))
						{
							location_1 = "on the shelf";
							furnitureString = "shelf";

							if(locate[1] >= shelf_level[0])
							{
								location_2 = "on the first floor of the shelf";
							}
							else if(locate[1]>=shelf_level[1])
							{
								location_2 = "on the second floor of the shelf";
							}
							else if(locate[1]>=shelf_level[2])
							{
								location_2 = "on the third floor of the shelf";
							}
							else
							{
								location_2 = "on the bottom floor of the shelf";
							}
						}


						if(in_something(locate,counter_and_cabinet))
						{
							
							if(locate[1]>=counter_and_cabinet_level[0])
							{
								location_1 = "in the cabinet";
								furnitureString = "cabinet ";

								if(locate[1]>cabinet_line)
								{
									location_3 = "on the upper layer ";
								}
								else
								{
									location_3 = "on the lower layer ";
								}


								if(locate[2]>cabinet[0])
								{
									location_2 = "in the first cabinet on the right";
								}
								else if(locate[2]>cabinet[1])
								{
									location_2 = "in the second cabinet on the right";
								}
								else if(locate[2]>cabinet[2])
								{
									location_2 = "in the third cabniet on the right";
								}

								else
								{
									location_2 = "in the first cabniet on the left";
								}
							}


							else if (locate[1]>= counter_and_cabinet_level[1])
							{
								location_1 = "on the top of the counter and under the cabinet";	
								furnitureString = "drawer";
							}

							else if(locate[1]>=counter_and_cabinet_level[2])
							{
								location_1 = "in the drawer";
								furnitureString = "drawer";

								if(locate[0] > counter[0])
								{
									location_2 = "in the first floor and the first drawer on the left";
								}

								else if(locate[0]>counter[1])
								{
									location_2 = "in the first floor and the second drawer on the left";

								}
								else
								{
									location_2 = "in the first floor and the third drawer on the left";
								}
							}

							else if(locate[1] >= counter_and_cabinet_level[3])
							{
								if(locate[0] > counter[0])
								{
									location_2 = "in the second floor and the first drawer on the left";
								}

								else if(locate[0]>counter[1])
								{
									location_2 = "in the second floor and the second drawer on the left";

								}

								else
								{
									location_2 = "in the second floor and the third drawer on the left";
								}
							}
						}

						if(in_something(locate,wood_table))
						{
							location_1 = "on the wooden table";
							furnitureString = "wooden table";
						}


						
					}

					try_ = furniture_sentence_out(location_1, taskInfo.target_object);

					guideMsg = location_sentence_out(location_1 , location_2 , location_3);

					

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


						if (environment == "ExampleLayoutHN01")
						{
							double drawer_unit_white[4] = { 0,0,0,0 };
							double drawer_unit_brown[4] = { 0,0,0,0 };
							double drawer_unit_brown_wide[4] = { 0,0,0,0 };
							double drawer_unit_white_wide[4] = { 0,0,0,0 };
							double drawer_level[4] = { 0,0,0,0 };
							double kitchen_drawer[4] = { 0,0,0,0 };
							double kitchen_drawer_line = 0;
							double kitchen_drawer_level[4] = { 0,0,0,0 };
							double counter_and_cabinet[4] = { 3.89651, 3.87101, 3.30582, 1.44405 };
							double counter_and_cabinet_level[3] = { 1.3839, 0.8565, 0.35 };
							double counter[2] = { 3.06622 ,2.212 };
							double cabinet[3] = { 2.04622 ,2.6556 ,3.2664 };
							double cabinet_line = 1.50785;
							double basketball_board[5] = { 0,0,0,0,0 };
							double bed[4] = { 0,0,0,0 };
							double bookshelf[4] = { 0,0,0,0 };
							double bookshelf_level[] = { 0,0,0 };
							double round_table[5] = { 0,0,0,0,0 };
							double paper_box[4] = { 0,0,0,0 };
							double CDplayer[4] = { 0,0,0,0 };
							double big_chair_white[4] = { 0,0,0,0 };
							double big_chair_black[4] = { 0,0,0,0 };
							double small_chair_white[4] = { 0,0,0,0 };
							double small_chair_wooden[4] = { 0,0,0,0 };
							double cornersofa[4] = { 0,0,0,0 };
							double sofa[4] = { 0,0,0,0 };
							double blue_cupboard[4] = { 2.995,8.158,1.4819,7.5725 };
							double blue_cupboard_level[2] = { 1.229,0.72 };
							double cupboard[3] = { 0,0,0 };
							double cupboard_centerline = 2.249;
							double cupboard_leftside_line = 0.268;
							double cupboard_rightside_line = 0.536;
							double cupboard_rightside_cut = 2.623;
							double glass_dining_table[4] = { 0,0,0,0 };
							double wooden_dining_table[4] = { 0,0,0,0 };
							double fridge[4] = { 4.138,8.186,3.461,7.513 };
							double fridge_level[3] = { 0.927,0.742,0.38 };
							double fridge_center_line = 3.716;
							double brown_mat[4] = { 0,0,0,0 };
							double blue_mat[4] = { 0,0,0,0 };
							double shelf[4] = { 0,0,0,0 };
							double shelf_level[3] = { 0,0,0 };
							double sidetable[4] = { 0,0,0,0 };
							double oven[4] = { 0,0,0,0 };
							double trashcan[5] = { 0,0,0,0,0 };
							double TVbass[4] = { 0,0,0,0 };
							double TVbass_level[2] = { 0,0 };
							double glass_wagon[4] = { 0,0,0,0 };
							double wooden_wagon[4] = { 0,0,0,0 };
							double wagon_level[2] = { 0,0 };
							double wooden_table[4] = { 0,0,0,0 };

							geometry_msgs::Point targetPosition = taskInfo.target_object.position;


							locate[0] = -destinationPosition.y;
							locate[1] = destinationPosition.z;
							locate[2] = destinationPosition.x;



							if (in_something(locate, drawer_unit_white_wide))
							{
								location_1 = "in the larger white drawer";

								destinationString = "larger white drawer";

								if (locate[1] >= drawer_level[0])
								{
									location_2 = "on the top of the larger white drawer";
								}
								else if (locate[1] >= drawer_level[1])
								{
									location_2 = "in the first floor of the larger white drawer";
								}
								else if (locate[1] >= drawer_level[2])
								{
									location_2 = "in the second floor of the larger white drawer";
								}
								else if (locate[1] >= drawer_level[3])
								{
									location_2 = "in the third floor of the larger white drawer";
								}
								else
								{
									location_2 = "in the bottom floor of the larger white drawer";
								}
							}

							if (in_something(locate, drawer_unit_brown_wide))
							{
								location_1 = "in the larger brown drawer";

								destinationString = "larger brown drawer";

								if (locate[1] >= drawer_level[0])
								{
									location_2 = "in the top of the larger brown drawer";
								}
								else if (locate[1] >= drawer_level[1])
								{
									location_2 = "in the first floor of the larger brown drawer";
								}
								else if (locate[1] >= drawer_level[2])
								{
									location_2 = "in the second floor of the larger brown drawer";
								}
								else if (locate[1] >= drawer_level[3])
								{
									location_2 = "in the third floor of the larger brown drawer";
								}
								else
								{
									location_2 = "in the bottom floor of the larger brown drawer";
								}
							}

							if (in_something(locate, drawer_unit_white))
							{
								location_1 = "in the smaller white drawer";

								destinationString = "smaller white drawer";

								if (locate[1] >= drawer_level[0])
								{
									location_2 = "in the top of the smaller white drawer";
								}
								else if (locate[1] >= drawer_level[1])
								{
									location_2 = "in the first floor of the smaller white drawer";
								}
								else if (locate[1] >= drawer_level[2])
								{
									location_2 = "in the second floor of the smaller white drawer";
								}
								else if (locate[1] >= drawer_level[3])
								{
									location_2 = "in the third floor of the smaller white drawer";
								}
								else
								{
									location_2 = "in the bottom floor of the smaller white drawer";
								}
							}


							if (in_something(locate, drawer_unit_brown))
							{
								location_1 = "in the smaller brown drawer";

								destinationString = "smaller brown drawer";

								if (locate[1] >= drawer_level[0])
								{
									location_2 = "in the top of the smaller brown drawer";
								}
								else if (locate[1] >= drawer_level[1])
								{
									location_2 = "in the first floor of the smaller brown drawer";
								}
								else if (locate[1] >= drawer_level[2])
								{
									location_2 = "in the second floor of the smaller brown drawer";
								}
								else if (locate[1] >= drawer_level[3])
								{
									location_2 = "in the third floor of the smaller brown drawer";
								}
								else
								{
									location_2 = "in the bottom floor of the smaller brown drawer";
								}
							}

							//此处有修改项
							if (in_something(locate, kitchen_drawer))
							{
								location_1 = "in the kitchen drawer";

								destinationString = "kitchen drawer";


								if (locate[2] >= kitchen_drawer_line)
								{
									if (locate[1] > kitchen_drawer_level[0])
									{
										if (in_something(locate, oven))
										{
											location_2 = "in the oven";
										}
										else
										{
											location_2 = "on the top of the kitchen drawer";
										}
									}

									else if (locate[1] >= kitchen_drawer_level[1])
									{
										location_2 = "in the first floor of the right drawer";
									}
									else if (locate[1] >= kitchen_drawer_level[2])
									{
										location_2 = "in the second floor of the right drawer";
									}
									else
									{
										location_2 = "in the bottom floor of the right drawer";
									}
								}
								else
								{
									if (locate[1] > kitchen_drawer_level[0])
									{
										if (in_something(locate, oven))
										{
											location_2 = "in the oven";
										}
										else
										{
											location_2 = "on the top of the kitchen drawer";
										}
									}

									else if (locate[1] >= kitchen_drawer_level[1])
									{
										location_2 = "in the first floor of the left drawer";
									}
									else if (locate[1] >= kitchen_drawer_level[2])
									{
										location_2 = "in the second floor of the left drawer";
									}
									else
									{
										location_2 = "in the bottom floor of the left drawer";
									}
								}
							}




							//此处有修改项
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


							if (in_something(locate, bed))
							{
								location_1 = "on the bed";
								destinationString = "bed";
							}

							if (in_something_round(locate, round_table))
							{
								location_1 = "on the round cafe set";
								destinationString = "cafe set";
							}

							if (in_something(locate, paper_box))
							{
								location_1 = "in the paper box";
								destinationString = "paper box";
							}

							if (in_something(locate, big_chair_black))
							{
								location_1 = "on the big black chair";
								destinationString = "big black chair";
							}

							if (in_something(locate, big_chair_white))
							{
								location_1 = "on the big white chair";
								destinationString = "big white chair";
							}

							if (in_something(locate, small_chair_white))
							{
								location_1 = "on the small white chair";
								destinationString = "small white chair";
							}


							if (in_something(locate, small_chair_wooden))
							{
								location_1 = "on the small wooden chair";
								destinationString = "small wooden chair";
							}



							//有修改项
							if (in_something(locate, blue_cupboard))
							{
								location_1 = "in the blue cupboard";
								destinationString = "blue cupboard";

								if (locate[1] >= blue_cupboard_level[0])
								{
									if (locate[2] > cupboard_centerline)
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
									if (locate[2] > cupboard_centerline)
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

							if (in_something(locate, glass_dining_table))
							{
								location_1 = "on the glass dinning table";
								destinationString = "glass dining table";
							}

							if (in_something(locate, wooden_dining_table))
							{
								location_1 = "on the wooden dinning table";
								destinationString = "wooden dining table";
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


							if (in_something(locate, brown_mat))
							{
								location_1 = "on the brown mat";

								destinationString = "brown mat";
							}

							if (in_something(locate, blue_mat))
							{
								location_1 = "on the blue mat";

								destinationString = "blue mat";
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

							if (in_something(locate, sidetable))
							{
								location_1 = "on the short sidetable";

								destinationString = "short sidetable";
							}

							if (in_something(locate, sofa))
							{
								location_1 = "on the sofa";

								destinationString = "sofa";
							}

							if (in_something(locate, cornersofa))
							{
								location_1 = "on the corner sofa";

								destinationString = "corner sofa";
							}

							if (in_something_round(locate, trashcan))
							{
								location_1 = "in the trashcan";

								destinationString = "trashcan";
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


							if (in_something(locate, wooden_wagon))
							{
								location_1 = "on the wooden wagon";
								destinationString = "wooden wagon";

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

							if (in_something(locate, wooden_table))
							{
								location_1 = "on the short wooden table";

								destinationString = "wooden table";
							}



							try_ = destinationSentenceOut(location_1, location_2, location_3);

							guideMsg = destinationArrivedOut(location_1 ,location_2 ,location_3);
						}



						if(environment == "ExampleLayoutHN02")
						{
							


							locate[0] = -destinationPosition.y;
							locate[1] = destinationPosition.z;
							locate[2] = destinationPosition.x;


							double wagon[4] = {-2.60412, 6.18329, -3.23151, 5.6598};
							double wagon_level[2] = {0.725514, 0.3923};
							double wood_table[4] = {2.2469, 6.1205, 0.995, 5.469};
							double shelf[4] = {3.9178, 0.73622, 3.296, -0.8959};
							double shelf_level[3] = {1.4012, 1.01263, 0.619365};
							double trashcan_01[5] = {-1.473, 0, 5.917, 0.2, 0};
							double trashcan_02[5] = {-0.823, 0, 5.917, 0.2, 0};
							double trashcan_03[5] = {-0.173, 0, 5.917, 0.2, 0};
							double counter_and_cabinet[4] = {3.89651, 3.87101, 3.30582, 1.44405};
							double counter_and_cabinet_level[3] = {1.3839, 0.8565, 0.35};
							double counter[2] = {2.6913 ,3.06622};
							double cabinet[3] = {2.04622 ,2.6556 ,3.2664};
							double cabinet_line = 1.50785;


							if(in_something(locate,wagon))
							{
								location_1 = "on the wagon";
								destinationString = "wagon";

								if(locate[1] >= wagon_level[0])
								{
									location_2 = "on the top of the wagon";
								}
								else if(locate[1]>=wagon_level[1])
								{
									location_2 = "on the first floor of the wagon";
								}
								else
								{
									location_2 = "on the second floor of the wagon";
								}
							}

							if(in_something(locate,shelf))
							{
								location_1 = "on the shelf";
								destinationString = "shelf";

								if(locate[1] >= shelf_level[0])
								{
									location_2 = "on the first floor of the shelf";
								}
								else if(locate[1]>=shelf_level[1])
								{
									location_2 = "on the second floor of the shelf";
								}
								else if(locate[1]>=shelf_level[2])
								{
									location_2 = "on the third floor of the shelf";
								}
								else
								{
									location_2 = "on the bottom floor of the shelf";
								}
							}


							if(in_something(locate,counter_and_cabinet))
							{
								if(locate[1]>=counter_and_cabinet_level[0])
								{
									location_1 = "in the cabinet";
									destinationString = "cabinet";

									if(locate[1]>cabinet_line)
									{
										location_3 = "on the upper layer ";
									}
									else
									{
										location_3 = "on the lower layer ";
									}


									if(locate[2]<cabinet[0])
									{
										location_2 = "in the first cabinet on the right";
									}
									else if(locate[2]<cabinet[1])
									{
										location_2 = "in the second cabinet on the right";
									}
									else if(locate[2]<cabinet[2])
									{
										location_2 = "in the third cabniet on the right";
									}

									else
									{
										location_2 = "in the first cabniet on the left";
									}
								}


								else if (locate[1]>= counter_and_cabinet_level[1])
								{
									location_1 = "on the top of the counter and under the cabinet";
								}

								else if(locate[1]>=counter_and_cabinet_level[2])
								{
									location_1 = "in the drawer";
									destinationString = "drawer";
									if(locate[0] < counter[0])
									{
										location_2 = "in the first floor and the first drawer on the left";
									}

									else if(locate[0]<counter[1])
									{
										location_2 = "in the first floor and the second drawer on the left";

									}
									else
									{
										location_2 = "in the first floor and the third drawer on the left";
									}
								}

								else if(locate[1] >= counter_and_cabinet_level[3])
								{
									if(locate[0] < counter[0])
									{
										location_2 = "in the second floor and the first drawer on the left";
									}

									else if(locate[0]<counter[1])
									{
										location_2 = "in the second floor and the second drawer on the left";

									}

									else
									{
										location_2 = "in the second floor and the third drawer on the left";
									}
								}
							}


							if(in_something(locate,wood_table))
							{
								location_1 = "on the wooden table";
								destinationString = "wooden table";
							}


							if(in_something_round(locate,trashcan_02))
							{
								location_1 = "in the second trashcan on the right. ";
							}



							

							try_ = destinationSentenceOut(location_1, location_2, location_3);

							guideMsg = destinationArrivedOut(location_1 ,location_2 ,location_3);
						}

					try_ = destinationSentenceOut(location_1, location_2, location_3);

					guideMsg = destinationArrivedOut(location_1 ,location_2 ,location_3);
					geometry_msgs::Point avatarPosition = avatarStatus.head.position;

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

