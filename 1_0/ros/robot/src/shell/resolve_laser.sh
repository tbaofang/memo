#!/usr/bin/env bash

frame=`cat /home/fa/laser_config.yaml | shyaml get-value Laser.frame_id laser_link`

type=`cat /home/fa/laser_config.yaml | shyaml get-value Laser.laser_type 1`

topic=`cat /home/fa/laser_config.yaml | shyaml get-value Laser.scan_topic scan`

angle_min=`cat /home/fa/laser_config.yaml | shyaml get-value Laser.truncated_angle_min -1`

angle_max=`cat /home/fa/laser_config.yaml | shyaml get-value Laser.truncated_angle_max -1`

deviceIP=`cat /home/fa/laser_config.yaml | shyaml get-value Laser.device_ip 192.168.1.222`

devicePort=`cat /home/fa/laser_config.yaml | shyaml get-value Laser.device_port 2368`