FILE_NAME=/etc/init.d/robot_auto_start.sh
# FILE_NAME=robot_auto_start.sh

if [ $# -lt 1 ];
then
	echo "本脚本用于配置开机自动启动导航，设置前要先创建地图"
	echo "用法： 设置开机启动"
	echo "       sudo $0 -f"
	echo "       取消开机启动"
	echo "       sudo $0 -r"
	exit	# 退出
fi

echo $(basename ${FILE_NAME})
case $1 in
-r)
	update-rc.d $(basename ${FILE_NAME}) remove
	;;
-f)
	if [ $# -ne 1 ];
	then
		echo "错误！需要传递一个参数"
		exit
	fi

	echo "设置自动启动中..."

	# 将导航命令写入文件，EOF之间的是文件内容
	cat > ${FILE_NAME}<<EOF
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

roslaunch robotic robot.launch > /home/fa/log/setup.log 2>&1 &

roslaunch --wait rosbridge_server rosbridge_websocket.launch > /home/fa/log/web.log 2>&1 &

roslaunch --wait ls01d ls01d.launch >/home/fa/log/nav.log 2>&1 &
EOF

	chmod +x ${FILE_NAME}
	update-rc.d $(basename ${FILE_NAME}) defaults 99
	
	echo "完成"
	;;
*)
	echo "未知参数"
	;;
esac
