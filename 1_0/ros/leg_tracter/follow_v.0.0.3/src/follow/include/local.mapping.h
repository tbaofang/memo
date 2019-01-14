#include <iostream>
#include <vector>
#include <string>

#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <nav_msgs/OccupancyGrid.h>

#include "follow/Leg.h"
#include "follow/LegArray.h"

#include "data.preprocess.h"
using namespace std;

class LocalMapping{
public:
    LocalMapping();
    ~LocalMapping();

private:
    string scan_topic_;
    string fixed_frame_;
    string base_frame_;

    void laserAndLegCb(const sensor_msgs::LaserScan::ConstPtr& scan, const follow::LegArray::ConstPtr& non_leg);

    bool grid_centre_pos_found_;
    double grid_centre_pos_x_;
    double grid_centre_pos_y_;
    double shift_threshold_;

    size_t width_;
    double betweenPIandNegPI(double angle_in);

    double resolution_;
    double reliable_inf_range_;
    bool invalid_measurements_are_free_space_;

    double l0_;
    std::vector<double> l_;
    double l_min_;
    double l_max_;
    double logit(double p);
    double inverseLogit(double p);
    bool unseen_is_freespace_;


protected:
    ros::NodeHandle nh_;

    message_filters::Subscriber<sensor_msgs::LaserScan> scan_sub_;
    message_filters::Subscriber<follow::LegArray> non_leg_clusters_sub_;
    message_filters::TimeSynchronizer<sensor_msgs::LaserScan, follow::LegArray> sync_;
    ros::Publisher map_pub_;
};