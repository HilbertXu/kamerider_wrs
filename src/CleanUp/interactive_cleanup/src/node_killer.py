#!/usr/bin/env python
# -*- coding: utf-8 -*-
import roslib
import rospy
from std_msgs.msg import String
from std_msgs.msg import Int8
from interactive_cleanup.msg import InteractiveCleanupMsg
import os

class NodeKiller:
    def __init__(self):
        rospy.Subscriber('/task_control', String, self.taskCallback)
        self.killer_pub = rospy.Publisher('/task_control',String,queue_size = 1)
        rospy.Subscriber('/interactive_cleanup/message/to_moderator',InteractiveCleanupMsg,self.robotCallback)
        
    def taskCallback(self, msg):
        if msg.data == 'camshift_end':
            print '------------operation Verdandi starts------------'
            os.system("rosnode kill /CamShift_Avatar")
            os.system("sleep 1")

        if msg.data == 'OPAros_end':
            print '----------------OPAros & openpose_ros has finished thier jobs we dont need them any more---------------'

        if msg.data == 'operation_end':
            print '--------task will be restarted right now---------'
            os.system('gnome-terminal -x bash -c "cd ~/pcl_version && python cleanup_restart.py"')
            os.system("rosnode kill /darknet_ros")
            os.system("pkill DOFmove_AutoGcp")
            os.system("rosnode kill /DOFmove_AutoGcp")
            os.system("rosnode kill /GCPresPanTilt")
            os.system("rosnode kill /GetGCPinDarknet")
            os.system("rosnode kill /FromONavPts")

    def robotCallback(self,msg):
        if msg.message == 'Object_grasped':
            os.system("rosnode kill /DOFmove_AutoGcp")
            os.system("rosnode kill /FromONavPts")

    
if __name__ == '__main__':
    rospy.init_node('node_killer')
    print '--------------------init--------------------'
    print '-------I AM A KILLER AS COLD AS MARBLE-------'
    NodeKiller()
    rospy.spin()