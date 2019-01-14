#include "train.h"

#include <iostream>

#include <ros/ros.h>

using namespace std;

int main(int argc, char** argv){
    ros::init(argc, argv, "train");

    Train t;
    t.loadData(argc, argv);
    t.training();
    t.testing();

    return 0;
}