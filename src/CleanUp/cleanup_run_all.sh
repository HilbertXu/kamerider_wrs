#!/bin/sh

echo "CamShift Start"
gnome-terminal -x bash -c "cd /home/kamerider/sim_ws/src/CleanUp/CamShift_Avatar/build"
sleep 1

gnome-terminal -x bash -c "./Run.sh"

sleep 1

echo "CleanUp Project start"
gnome-terminal -x bash -c "roslaunch interactive_cleanup interactive_clean_up.launch"

sleep 1

exit 0

