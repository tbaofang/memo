#/bin/bash
if [ $# -lt "1" ];
then
	echo "-----------missing argument! Usage:----------"
	echo "config --dummy  -> modify dummy.yaml"
	echo "config --param  -> modify param.yaml"
	echo "config --tf     -> modify base2laser.launch.xml"
	echo "config --smooth -> modify smoother.yaml"
	echo "config --laser  -> modify laser_config.yaml"
	echo "-----------Editor: nano ---------------------"
else
	case $1 in 
	"--dummy")
		nano $(rospack find turtlebot_navigation)/param/dummy.yaml
		;;
	"--param")
		nano $(rospack find kobuki_node)/param/param.yaml
		;;
	"--tf")
		nano $(rospack find robotic)/launch/includes/base2laser.launch.xml
		;;
	"--smooth")
		nano $(rospack find turtlebot_bringup)/param/defaults/smoother.yaml
		;;
	"--laser")
		nano /home/fa/laser_config.yaml
		;;
	*)
		echo "unrecognized argument"
	esac
fi
