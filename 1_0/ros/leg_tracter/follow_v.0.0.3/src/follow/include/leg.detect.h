#pragma once

#include <iostream>
#include <list>

#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <tf/transform_listener.h>

#include "follow/Leg.h"
#include "follow/LegArray.h"

#include "data.preprocess.h"

using namespace std;

class LegDetect{
public:
    LegDetect();
    ~LegDetect();

private:
    preprocess::DataPreprocess dpp;

    void scanCb(const sensor_msgs::LaserScan::ConstPtr& scan);

    void legDistinguish();

    cv::Ptr<cv::ml::RTrees> forest;

    string forest_file;
    size_t feature_count_;
    size_t num_prev_markers_published_;
    double max_detect_distance_;
    double detection_threshold_;

    void showMarkers(follow::LegArray& leg_array);
protected:
    ros::NodeHandle nh_;
    ros::Subscriber scan_sub_;
    ros::Publisher markers_pub_;
    ros::Publisher leg_detected_pub_;
};