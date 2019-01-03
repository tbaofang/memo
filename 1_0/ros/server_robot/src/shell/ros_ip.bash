#!/usr/bin/env bash

# mode value 1: fix ip address.
# mode value 2: dynamic ip address resolved by hostname
# mode value 3: hostname
# mode value 4: dynamic ip address from specific network device
# mode value 5: empty ROS_IP

mode=4

# network device name
networkdev="wlan0"
fix_ip=192.168.1.104
#fix_ip=127.0.0.1

#echo $app

case $mode in

# fix ip approach
    1) export ROS_IP=$fix_ip
    ;;
#     read hostname and resolve it to ip address
    2) export ROS_IP=`hostname -I | sed 's/[ \t]*$//g'`
    ;;

    3)
    # using hostname
    export ROS_IP=tongsky-desktop.local
    ;;

    4)
    # using specific network device (eth or wlan)
    export ROS_IP=`ifconfig $networkdev | grep 'inet' | cut -d : -f 2 | cut -d ' ' -f 1`
    ;;
    
    *)
    # default empty ROS_IP
    export ROS_IP=
    ;;

esac

unset mode

#echo $ROS_IP

export ROS_HOSTNAME=$ROS_IP
export ROS_MASTER_URI=http://$ROS_IP:11311
