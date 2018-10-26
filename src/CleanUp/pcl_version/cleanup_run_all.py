#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os

if __name__ == '__main__':
    os.system('cd ~/.ros/ && rm -rf * && cd')
    os.system('cd /home/kamerider/sim_ws/src/CleanUp/interactive_cleanup/json_files && rm -rf *.xml')
    os.system('cd && cd /home/kamerider/sim_ws/src/CleanUp/interactive_cleanup/openpose_img && rm -rf *.png')
    os.system('sleep 1')
    os.system('gnome-terminal -x bash -c "roslaunch interactive_cleanup Unity-Ros-Navigation.launch && read"')
    os.system('sleep 1')
    os.system('gnome-terminal -x bash -c "rosrun interactive_cleanup CamShift_Avatar -Rect 317 93 16 20 -FirstImgPath /home/kamerider/sim_ws/src/CleanUp/interactive_cleanup/FirstImg/Img.png -Kh 0.001 -hTheld 150 && read&& read"')
    os.system("sleep 1")
    os.system('gnome-terminal -x bash -c "rosrun interactive_cleanup task_control.py && read"')
    os.system('sleep 1')
    os.system('gnome-terminal -x bash -c "rosrun interactive_cleanup node_killer.py && read"')
    os.system('sleep 1')
    os.system('gnome-terminal -x bash -c "rosrun interactive_cleanup node_creator.py && read"')
    os.system('sleep 1')