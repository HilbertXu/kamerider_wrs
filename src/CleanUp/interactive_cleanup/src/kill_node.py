#!/usr/bin/env python
# -*- coding: utf-8 -*-
import roslib
import rospy
import numpy as np
import cv2
import matplotlib.pyplot as plt
from std_msgs.msg import String
from std_msgs.msg import Int8
import os

class NodeKiller:
    def __init__(self):
        rospy.Subscriber('/task_control', String, self.taskCallback)
        rospy.Subscriber('/interactive_cleanup/message/to_robot', String, self.AvatarCallback)

    def taskCallback(self, msg):
        if msg.data == 'OPAros_end':
            os.system("rosnode kill /ListenerKingNew_Xml")
            os.system("sleep 3")
            os.system("rosnode kill /FollowAvatar")
            os.system("sleep 3")
            os.system("pkill CamShift_Avatar")
            os.system("sleep 3")


    def AvatarCallback(self, msg):
        if msg.data == ''
