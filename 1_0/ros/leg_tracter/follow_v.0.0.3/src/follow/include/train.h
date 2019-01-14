#pragma once
#include <iostream>
#include <string>
#include <vector>

#include <ros/ros.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/PoseArray.h>

#include "data.preprocess.h"

using namespace std;

class Train{
public:
    Train();
    ~Train();

    void loadData(int argc, char** argv);
    void training();
    void testing();

private:
    vector<vector<float> > train_pos_data_;
    vector<vector<float> > train_neg_data_;
    vector<vector<float> > test_pos_data_;
    vector<vector<float> > test_neg_data_;

    string save_file_;
    string positive_leg_cluster_positions_topic_;

    void loadPosData(const string rosbag_file, const string scan_topic, vector<vector<float> >& train_pos_data);
    void loadNegData(const string rosbag_file, const string scan_topic, vector<vector<float> >& train_pos_data);

    size_t feature_count_;
    cv::Ptr<cv::ml::RTrees> forest_ = cv::ml::RTrees::create();

    int undersample_negative_factor_;


protected:
};