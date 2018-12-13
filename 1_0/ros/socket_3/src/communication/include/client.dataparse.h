#pragma once
#include <iostream>
#include <algorithm>
#include <boost/thread.hpp>

#include <ros/ros.h>
#include <move_base_msgs/MoveBaseActionGoal.h>
#include <geometry_msgs/PoseStamped.h>


#include "datashare.h"
#include "proto_msgs.MoveBaseActionGoal.pb.h"
#include "proto_msgs.PoseStamped.pb.h"

using namespace std;

class ClientDataParse{
public:
    ClientDataParse();
    ~ClientDataParse();
    void start(DataShare* datashare);
private:
    ros::NodeHandle nh_;
    ros::Publisher pose_stamped_pub_;

    boost::thread* data_parse_thread_;
    void runThread();

    DataShare* ds_;

    void poseStampedPublish(const string& scan);
    void controlPublish (const string& msg);
};
