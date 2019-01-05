#pragma once

#include <iostream>
#include <list>

#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <tf/transform_listener.h>

using namespace std;

class LegDetect{
public:
    LegDetect();
    ~LegDetect();
private:
    struct ScanProcessed{
        size_t index;
        double x;
        double y;
    };

    list<list<ScanProcessed>> spll_;


    void scanCb(const sensor_msgs::LaserScan::ConstPtr& msg);
    void scanPreprocess(const sensor_msgs::LaserScan::ConstPtr& msg);


    double cluster_distance_;
    size_t  per_cluster_min_points_;
    size_t  per_cluster_max_points_;

    int  prev_markers_published_num;






protected:
    ros::NodeHandle nh_;

    ros::Subscriber scan_sub_;

    ros::Publisher markers_pub_;

    tf::TransformListener tfl_;


};
