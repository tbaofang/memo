#include <iostream>

#include <ros/ros.h>

#include "leg.detect.h"

using namespace std;

int main(int argc, char** argv){
    ros::init(argc, argv, "leg_detect");
    LegDetect ld;
    ros::spin();

    return 0;
}