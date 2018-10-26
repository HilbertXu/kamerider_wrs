#!/usr/bin/env python
# -*- coding: utf-8 -*-
import roslib
import rospy
from std_msgs.msg import String
from std_msgs.msg import Int8
import os

class NodeCreator:
    def __init__(self):
        rospy.Subscriber('/task_control', String, self.taskCallback)

    def taskCallback(self,msg):
        if msg.data == 'operation_start':
            print '-------Operation Urd starts-------'
            print '---------EL PSY CONGROO----------'
            os.system('gnome-terminal -x bash -c "rosrun interactive_cleanup OPArosNew_Xml"')
            os.system('gnome-terminal -x bash -c "rosrun interactive_cleanup GCPresPanTilt"')            
            os.system('gnome-terminal -x bash -c "cd && rosrun interactive_cleanup GetGCPinDarknet -base_link_Name base_link -rgbdFrame_Name head_rgbd_sensor_depth_frame -ResEtheld 1 -ResSetHeadT 4 -DarknetShouldNum 3 -xmlPath /home/kamerider/sim_ws/src/CleanUp/interactive_cleanup/MPoseNew.xml && read"')
        
        if msg.data == 'openpose_start':
            os.system('gnome-terminal -x bash -c "rosrun openpose_ros openpose_ros.py"')
            os.system('gnome-terminal -x bash -c "roslaunch darknet_ros darknet_ros.launch"')

        if msg.data == 'grasp_start':
            print '--------Operation Skuld starts--------'
            print '-----------EL PSY CONGROO-------------'
            os.system('gnome-terminal -x bash -c "rosrun interactive_cleanup DOFmove_AutoGcp -externNavCmd \'rosrun interactive_cleanup FromONavPts -Kvx 1 -Kvy 1 -eTheld 0.005 -FocusRow 72 -FocusCol 320 -dT 0.1 -headT 5\' true && read"')
            os.system("sleep 1")

if __name__ == '__main__':

    rospy.init_node('node_creator')
    print '--------------------init--------------------'
    print '-----------THE CREATOR OF ALL NODES---------'
    NodeCreator()
    rospy.spin()
