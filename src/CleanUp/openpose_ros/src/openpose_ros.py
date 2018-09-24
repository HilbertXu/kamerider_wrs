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

class openpose_ros:
	def __init__(self):
		self.openpose_dir     = '/home/kamerider/openpose'
		self.openpose_pub     = rospy.Publisher('/finish_openpose',String,queue_size=1)
		rospy.Subscriber         ('/start_openpose',String,self.openpose)


	def openpose(self,msg):
		msg.data = msg.data.lower()
		if msg.data == 'start':
			os.getcwd()
			os.chdir(self.openpose_dir)
			os.system("./build/examples/openpose/openpose.bin --image_dir /home/kamerider/sim_ws/src/CleanUp/interactive_cleanup/openpose_img --hand true --write_keypoint /home/kamerider/sim_ws/src/CleanUp/interactive_cleanup/json_files --write_keypoint_format xml")
			self.openpose_pub.publish('finish')


if __name__ == '__main__':
	rospy.init_node('openpose_ros')
	print '--------------------init--------------------'
	print '--------------WAITING FOR IMAGE-------------'
	openpose_ros()
	rospy.spin()