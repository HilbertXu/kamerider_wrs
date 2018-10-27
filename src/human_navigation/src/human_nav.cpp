#include <ros/ros.h>
#include <human_navigation/HumanNaviObjectInfo.h>
#include <human_navigation/HumanNaviDestination.h>
#include <human_navigation/HumanNaviTaskInfo.h>
#include <human_navigation/HumanNaviMsg.h>
#include <human_navigation/HumanNaviGuidanceMsg.h>
#include <human_navigation/HumanNaviAvatarStatus.h>
#include <human_navigation/HumanNaviObjectStatus.h>


using namespace std;

class HumanNavigationSample
{
private:
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

	human_navigation::HumanNaviTaskInfo taskInfo;
	std::string guideMsg;

	human_navigation::HumanNaviAvatarStatus avatarStatus;
	human_navigation::HumanNaviObjectStatus objectStatus;

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
	}

	// send humanNaviMsg to the moderator (Unity)
	void sendMessage(ros::Publisher &publisher, const std::string &message)
	{
		human_navigation::HumanNaviMsg human_navi_msg;
		human_navi_msg.message = message;
		publisher.publish(human_navi_msg);

		ROS_INFO("Send message:%s", message.c_str());
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
	}

	void avatarStatusMessageCallback(const human_navigation::HumanNaviAvatarStatus::ConstPtr& message)
	{
		avatarStatus = *message;

		ROS_INFO_STREAM(
			"Subscribe avatar status message:" << std::endl <<
			"Head: " << std::endl << avatarStatus.head << 
			"LeftHand: " << std::endl << avatarStatus.left_hand << 
			"rightHand: " << std::endl << avatarStatus.right_hand << std::endl
			//"objctInLeftHand: " << avatarStatus.object_in_left_hand << std::endl <<
			//"objectInRightHand: " << avatarStatus.object_in_right_hand << std::endl <<
			//"isTargetObjectInLeftHand: " << std::boolalpha << (bool)avatarStatus.is_target_object_in_left_hand << std::endl <<
			//"isTargetObjectInRightHand: " << std::boolalpha << (bool)avatarStatus.is_target_object_in_right_hand << std::endl
		);
		isSentGetAvatarStatus = false;
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
			if(timePrevSpeechStateConfirmed.sec + interval < ros::Time::now().sec)
			{
				sendMessage(pubHumanNaviMsg, MSG_GET_SPEECH_STATE);
				timePrevSpeechStateConfirmed = ros::Time::now();
				speechState = SpeechState::WaitingState;
			}
		}

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

		cout<<"半径："<<r<<endl;

		if(distance < r)
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
			describe = "66 cup";
		}
		if(target.name.find("empty_plastic_bottle")!=std::string::npos)
		{
			describe = "an empty plastic bottle";
		}
		if(target.name.find("pepper_01")!=std::string::npos)
		{
			describe = "a pepper box";
		}
		if(target.name.find("coffee_press_01")!=std::string::npos)
		{
			describe = "a coffee press";
		}
		if(target.name.find("plastic_bottle_01")!=std::string::npos)
		{
			describe = "a full plastic bottle";
		}


		string outstc = "Our target object is " + describe + ".";
		if(a == " ")
		{
			outstc = outstc + "Please go to find where the target furniture is. ";
		}
		else
		{
			outstc = outstc + "It is " + a + ". " + "Please go to find where the target furniture is. ";
		}
		return outstc;
	}

	string location_sentence_out(string a, string b,string c)
	{
		string output;

		output = "Now you have found the target furniture. ";

		if(a == " "&& b == " "&& c == " ")
		{
			cout<<"I can't find the specific location of the target. ";
			output = output + "I can't find the specific location of the target. ";
		}
		else if(b == " "&& c == " ")
		{
			cout<<"The target is " + a + ". ";
			output = output + "The target is " + a + ". ";
		}
		else if(c == " ")
		{
			cout << "The target is  " << a << ", " << b << endl;
			output = output +  "The target is " + a + "," + b;
		}
		else
		{
			cout << "The target is  " << a << ", " << b << " and " << c << endl;
			output = output + "The target is " + a + ", " + b + "and " + c;
		}


		output = output + "Please go to find it. ";

		return output;
	}

	string destinationSentenceOut(string a , string b , string c)
	{
		string output;

		output = "Now you have got the target object. ";
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
			switch(step)
			{
				case Initialize:
				{
					reset();

					ROS_INFO("##### Initialized ######");

					step++;
					break;
				}
				case Ready:
				{
					if(isStarted)
					{
						step++;

						sendMessage(pubHumanNaviMsg, MSG_I_AM_READY);

						ROS_INFO("Task start");
					}
					break;
				}
				case WaitTaskInfo:
				{
					if(isTaskInfoReceived){ step++; }
					break;
				}
				case GuideForTakingObject:
				{
					if(isRequestReceived)
					{
						isRequestReceived = false;
					}

					std::string targetObjectName;

					string environment = taskInfo.environment_id;


					string location_1 = " ";
					string location_2 = " ";
					string location_3 = " ";

					string try_ = " ";


					if(environment == "LayoutX")
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
						double counter[2] = {2.6913 ,3.06622};
						double cabinet[3] = {2.04622 ,2.6556 ,3.2664};
						double cabinet_line = 1.50785;


						

						geometry_msgs::Point targetPosition = taskInfo.target_object.position;

						double locate[3] ;

					

						locate[0] = -targetPosition.y;
						locate[1] = targetPosition.z;
						locate[2] = targetPosition.x;

						if(in_something(locate,wagon))
						{
							location_1 = "on the wagon";

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
							}

							else if(locate[1]>=counter_and_cabinet_level[2])
							{
								location_1 = "in the counter";
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
						}


						else
						{
							cout<<"Nothing.";
						}
					}

					try_ = location_sentence_out(location_1, location_2 , location_3);

					guideMsg = try_;

					if(speakGuidanceMessage(pubHumanNaviMsg, pubGuidanceMsg, guideMsg))
					{
						time = ros::Time::now();
						step++;
					}
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

					int WaitTime = 5;
					if(time.sec + WaitTime < ros::Time::now().sec)
					{
						geometry_msgs::Point destinationPosition = taskInfo.destination.position;


						string location_1 = " ";
						string location_2 = " ";
						string location_3 = " ";


						string try_ = " ";


						string environment = taskInfo.environment_id;




						if(environment == "LayoutX")
						{
							double locate[3];


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
									location_1 = "in the counter";
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
							}


							if(in_something_round(locate,trashcan_02))
							{
								location_1 = "in the second trashcan on the right. ";
							}



							else 
							{
								cout<<"Nothing.";
							}

							try_ = destinationSentenceOut(location_1, location_2, location_3);

							guideMsg = try_;
						}


						if(speakGuidanceMessage(pubHumanNaviMsg, pubGuidanceMsg, guideMsg))
						{
							time = ros::Time::now();
							step++;
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

					int WaitTime = 5;
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
};

