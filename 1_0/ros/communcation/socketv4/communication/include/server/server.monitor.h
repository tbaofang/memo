#pragma once
#include <iostream>
#include <boost/thread.hpp>

#include <ros/ros.h>
#include <nav_msgs/OccupancyGrid.h>
#include <tf2_msgs/TFMessage.h>


#include "utils/datashare.h"
#include "utils/common.h"

using namespace std;

class ServerMonitor
{
  public:
    ServerMonitor();
    ~ServerMonitor();
    void start(DataShare *datashare);
  private:
    boost::thread *monitor_thread_;
    void runThread();

    DataShare *ds_;

    void mapCb(const nav_msgs::OccupancyGrid::ConstPtr &msg);
    void tfStaticCb(const tf2_msgs::TFMessage::ConstPtr &msg);


    bool map_state_;
    bool tf_static_state_;

    Common common;


  protected:
    ros::NodeHandle nh_;
    string mode_;
};