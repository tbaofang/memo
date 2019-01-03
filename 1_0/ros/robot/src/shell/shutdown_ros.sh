#!/usr/bin/env bash

sudo pkill roslaunch

echo "waiting for shutting down ROS completely"
while true
do
    sudo pkill roslaunch
    if [ "`ps -e | grep ros`" != "" ] && [ "`rosnode list | grep rosout`" != "" ];
    then
        echo "still has something left"
    else
        echo "ros is gone completely"
        break
    fi
done