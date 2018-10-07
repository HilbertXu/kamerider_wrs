#!/usr/bin/env python
# -*- coding: utf-8 -*-
import roslib
import rospy
from std_msgs.msg import String
from std_msgs.msg import Int8
import os

class NodeKiller:
    def __init__(self):
        rospy.Subscriber('/task_control', String, self.taskCallback)
        rospy.Subscriber('/interactive_cleanup/message/to_robot', String, self.AvatarCallback)
        self.killer_pub = rospy.Publisher('/killer_pub',String, queue_size=1)


    def taskCallback(self, msg):
        if msg.data == 'OPAros_end':
            print 'OPAros and FollowAvatar finished thier jobs'
            os.system("rosnode kill /OPArosNew_Xml")
            os.system("sleep 2")
            os.system("rosnode kill /FollowAvatar")
            os.system("sleep 2")
            os.system("pkill CamShift_Avatar")
            os.system("sleep 2")
            os.system("rosnode kill /openpose_ros")
            os.system("sleep 2")

    def AvatarCallback(self, msg):
        if msg.data == 'Task_failed':
            print 'We messed up one attempt but we will try again'
            os.system("rosnode kill /cleanup_navigation")
            os.system("sleep 2")
            os.system("rosnode kill /find_graspable_object")
            os.system("sleep 2")
            os.system("rosnode kill /DOFmove_AutoGcpDcp")
            os.system("sleep 2")
            os.system("rosnode kill /amcl")
            os.system("sleep 2")
            os.system("rosnode kill /move_base")
            os.system("sleep 2")
            os.system("rosnode kill /map_server")
            os.system("sleep 2")
            os.system("rosnode kill /rosbridge_websocket")
            os.system("sleep 2")
            os.system("rosnode kill /teleop_key_interactive_cleanup")
            os.system("sleep 2")
            os.system("rosnode kill /rviz")
            os.system("sleep 2")
            self.killer_pub.publish('restart')

        if msg.data == 'Task_succeeded':
            print 'Ohhhhh we did it !'
            os.system("rosnode kill /cleanup_navigation")
            os.system("sleep 2")
            os.system("rosnode kill /find_graspable_object")
            os.system("sleep 2")
            os.system("rosnode kill /DOFmove_AutoGcpDcp")
            os.system("sleep 2")
            os.system("rosnode kill /amcl")
            os.system("sleep 2")
            os.system("rosnode kill /move_base")
            os.system("sleep 2")
            os.system("rosnode kill /map_server")
            os.system("sleep 2")
            os.system("rosnode kill /rosbridge_websocket")
            os.system("sleep 2")
            os.system("rosnode kill /teleop_key_interactive_cleanup")
            os.system("sleep 2")
            os.system("rosnode kill /rviz")
            os.system("sleep 2")

        if msg.data == 'Mission_complete':
            print 'We run out all attempts'
            os.system("rosnode kill /cleanup_navigation")
            os.system("sleep 2")
            os.system("rosnode kill /find_graspable_object")
            os.system("sleep 2")
            os.system("rosnode kill /DOFmove_AutoGcpDcp")
            os.system("sleep 2")
            os.system("rosnode kill /amcl")
            os.system("sleep 2")
            os.system("rosnode kill /move_base")
            os.system("sleep 2")
            os.system("rosnode kill /map_server")
            os.system("sleep 2")
            os.system("rosnode kill /rosbridge_websocket")
            os.system("sleep 2")
            os.system("rosnode kill /teleop_key_interactive_cleanup")
            os.system("sleep 2")
            os.system("rosnode kill /rviz")
            os.system("sleep 2")

if __name__ == '__main__':
    rospy.init_node('node_killer')
    print '--------------------init--------------------'
    print '-------I AM A KILLER AS COLD AS MARBLE-------'
    NodeKiller()
    rospy.spin()
