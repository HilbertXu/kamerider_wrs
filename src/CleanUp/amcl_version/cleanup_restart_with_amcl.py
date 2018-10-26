#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os


if __name__ == '__main__':
    os.system('rosclean purge -y')
    os.system('cd /home/kamerider/sim_ws/src/CleanUp/interactive_cleanup/json_files && rm -rf *.xml')
    os.system('cd && cd /home/kamerider/sim_ws/src/CleanUp/interactive_cleanup/openpose_img && rm -rf *.png')
    os.system('sleep 1')
    os.system('gnome-terminal -x bash -c "roslaunch interactive_cleanup amcl_move_base.launch"')
    os.system('gnome-terminal -x bash -c "rosrun interactive_cleanup CamShift_Avatar -Rect 317 93 16 20 -FirstImgPath /home/kamerider/src/CleanUp/interactive_cleanup/FirstImg/Img.png -Kh 0.001 -hTheld 150 && read&& read"')
    