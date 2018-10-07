#!/usr/bin/env python
# -*- coding: utf-8 -*-
import roslib
import rospy
from std_msgs.msg import String
from std_msgs.msg import Int8
import os

class NodeCreator:
    def __init__(self):
        rospy.Subscriber('/killer_pub', String, self.killerCallback)
        rospy.Subscriber('/OPAros2nav', String, self.navCallback)
        self.creator = rospy.Publisher('/creator_pub', String, queue_size=1)

    def killerCallback(self,msg):
        if msg.data == 'restart':
            print 'restart all nodes after 7 seconds'
            os.system("sleep 7")
            os.system('gnome-terminal -x bash -c "cd ~/sim_ws/src/CleanUp/CamShift_Avatar/build && ./Run.sh && read"')
            os.system("sleep 2")
            os.system('gnome-terminal -x bash -c "roslaunch interactive_cleanup interactive_clean_up.launch && read"')

    def navCallback(self,msg):
        if msg.data == 'start':
            os.system("rosrun interactive_cleanup find_graspable_object")
            os.system("sleep 2")
            os.system("rosrun interactive_cleanup DOFmove_AutoGcpDcp -isNav false -Kvx 1 -Kvy 1 -eTheld 0.001 -dT 0.05 -Hg 0.15 -JointDurT 1")
            os.system("sleep 2")

if __name__ == '__main__':
    rospy.init_node('node_creator')
    print '--------------------init--------------------'
    print '-----------THE CREATOR OF ALL NODES---------'
    NodeCreator()
    rospy.spin()
