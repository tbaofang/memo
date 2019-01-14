#include "leg.detect.h"

#include <iostream>

#include <ros/ros.h>

int main(int argc, char** argv){
    ros::init(argc, argv, "leg_detect");
    LegDetect ld;
    ros::spin();
    return 0;
}