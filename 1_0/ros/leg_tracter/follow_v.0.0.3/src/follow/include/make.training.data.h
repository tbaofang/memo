#pragma once

#include <iostream>

#include <ros/ros.h>

using namespace std;

class MakeTrainingData{
public:
    MakeTrainingData();
    ~MakeTrainingData();

    void making();

private:
    string load_bag_file_;
    string save_bag_file_;
    string scan_topic_;
    string laser_frame_;
    double min_angle_;
    double max_angle_;
    double max_dist_;


protected:
};