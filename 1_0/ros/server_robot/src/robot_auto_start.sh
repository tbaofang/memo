#!/bin/bash

### BEGIN INIT INFO
# Provides: robot_auto_start.sh
# Required-Start: $local_fs $network $remote_fs
# Required-Stop: $local_fs $network $remote_fs
# Default-Start: 2 3 4 5
# Default-Stop: 0 1 6
# Short-Description: Start or stop the navigation.
### END INIT INFO

cnt=0
username=fa
true > /home/$username/ros_status.log
echo "start" > /home/$username/ros_status.log

while true
do
IP_address=`ifconfig wlan0 | grep 'inet' | cut -d : -f 2 | cut -d ' ' -f 1`
echo $IP_address
if [ "$IP_address" != "" ];
then
	echo "Get wlan0 IP address, it is safe to go to next phase!"
	echo "Success with IP: $IP_address" >> /home/$username/ros_status.log
	break
fi

if [ $cnt -gt 300 ];
then
	echo "Failed to bring up ROS in 30s, exit"
	echo "Failed" >> /home/$username/ros_status.log
	break
fi

cnt=$((cnt+1))
echo "wait for wlan0 IP bring up"
echo "pending with count: $cnt" >> /home/$username/ros_status.log
sleep 0.1
done

echo "begin to bring up cleaner robot related code"

source /opt/ros/indigo/setup.bash
source /home/fa/projects/ban_ka/install/setup.bash

source /home/fa/ros_ip.bash

roslaunch robotic robot.launch > /home/fa/log/setup.log 2>&1 &

roslaunch --wait rosbridge_server rosbridge_websocket.launch > /home/fa/log/web.log 2>&1 &

roslaunch --wait ls01d ls01d.launch >/home/fa/log/nav.log 2>&1 &

roslaunch --wait lsrobot lsrobot.launch >/home/fa/log/nav.log 2>&1 &

roslaunch --wait lsrobot lsrobot_serial.launch >> /home/fa/log/nav.log 2>&1 &

rosrun sl sl >> /home/fa/log/nav.log 2>&1 &
