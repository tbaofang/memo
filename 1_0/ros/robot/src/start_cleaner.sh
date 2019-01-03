#!/bin/bash

### BEGIN INIT INFO
# Provides: $(basename ${FILE_NAME})
# Required-Start: \$local_fs \$network \$remote_fs
# Required-Stop: \$local_fs \$network \$remote_fs
# Default-Start: 2 3 4 5
# Default-Stop: 0 1 6
# Short-Description: Start or stop the navigation.
### END INIT INFO

source /opt/ros/indigo/setup.bash
source /home/fa/projects/ban_ka/install/setup.bash

source /home/fa/ros_ip.bash

echo "following are ROS environment variables"
echo $ROS_IP
echo $ROS_HOSTNAME
echo $ROS_MASTER_URI

roslaunch robotic robot.launch > /home/fa/log/setup.log 2>&1 & 
echo "bring up the master launch for basic need"

roslaunch --wait ls01d ls01d.launch  >> /home/fa/log/setup.log 2>&1 &
roslaunch --wait rosbridge_server rosbridge_tcp.launch > /home/fa/log/web.log 2>&1 &

sleep 5
echo "begin to bring up lsrobot related code"

while true
do
if [ "`ps -e | grep ros`" != "" ] && [ "`rosnode list | grep rosbridge_tcp`" != "" ];
then
	echo "rosbridge is launched! It is safe to launch lsrobot!"
	break
fi
echo "wait for rosbridge ready"
done

echo "begin to bring up lsrobot"


sleep 0.2

roslaunch lsrobot lsrobot.launch  > /home/fa/log/setup.log 2>&1 &
