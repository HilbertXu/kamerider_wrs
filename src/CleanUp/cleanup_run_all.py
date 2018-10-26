#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os


if __name__ == '__main__':
    os.system('gnome-terminal -x bash -c "roscore && read"')
    os.system('sleep 1')
    os.system('gnome-terminal -x bash -c "roslaunch interactive_cleanup node_killer.py && read"')
    os.system('sleep 1')
    os.system('gnome-terminal -x bash -c "roslaunch interactive_cleanup node_creator.py && read"')
    os.system('sleep 1')
    os.system('gnome-terminal -x bash -c "cd ~/sim_ws/src/CleanUp/CamShift_Avatar/build && ./Run.sh && read"')
    os.system('sleep 1')
    os.system('gnome-terminal -x bash -c "roslaunch interactive_cleanup interactive_clean_up.launch && read"')
