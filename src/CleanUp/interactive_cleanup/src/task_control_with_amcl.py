#!/usr/bin/env python
# -*- coding: utf-8 -*-
import roslib
import rospy
from std_msgs.msg import String
from std_msgs.msg import Int8
from interactive_cleanup.msg import InteractiveCleanupMsg
import os

class battle_control:
    def __init__(self):
        rospy.Subscriber('/interactive_cleanup/message/to_robot', InteractiveCleanupMsg, self.AvatarCallback)
        rospy.Subscriber('/task_control', String, self.taskCallback)
        self.control_pub = rospy.Publisher('/task_control',String,queue_size = 1)
        self.avatar_pub  = rospy.Publisher('/interactive_cleanup/message/to_moderator', InteractiveCleanupMsg,queue_size=1)

    def AvatarCallback(self,msg):
        if msg.message == 'Are_you_ready?':
            print '-------Operation Urd starts-------'
            print '---------EL PSY CONGROO----------'
            self.control_pub.publish('operation_start')
            flag = InteractiveCleanupMsg()
            flag.message = 'I_am_ready'
            self.avatar_pub.publish(flag)

        if msg.message == 'Pick_it_up!':
            print '------start openpose-------'
            self.control_pub.publish('openpose_start')

        if msg.message == 'Task_failed':
            print '------Battle Control terminated------'
            print '------Maybe we mess up this time-----'
            print '-but failure is the mother of success-'
            print '------------EL PSY CONGROO-----------'
            self.control_pub.publish('operation_end')

    def taskCallback(self,msg):
        if msg.data == 'OPAros_end':
            print '--------Operation Skuld starts--------'
            print '-----------EL PSY CONGROO-------------'
            self.control_pub.publish('grasp_start')
            os.system("rosnode kill /ListenerKingNew_Xml")
            

if __name__ == '__main__':
    rospy.init_node('task_control')
    print '--------Battle Control Online--------'
    print '------Waiting For Your Command-------'
    battle_control()
    rospy.spin()
