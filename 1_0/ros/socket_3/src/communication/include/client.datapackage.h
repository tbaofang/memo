#pragma once
#include <iostream>
#include <algorithm>
#include <boost/thread.hpp>

#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <nav_msgs/OccupancyGrid.h>
#include <nav_msgs/Path.h>
#include <tf/tf.h>

#include "datashare.h"
#include "proto_msgs.LaserScan.pb.h"
#include "proto_msgs.OccupancyGrid.pb.h"
#include "proto_msgs.Path.pb.h"


using namespace std;

class ClientDataPackage{
public:
    ClientDataPackage();
    ~ClientDataPackage();
    void start(DataShare* datashare);
private:
    boost::thread* data_package_thread_;
    void runThread();

    ros::NodeHandle nh_;

    void lidarCallback(const sensor_msgs::LaserScan::ConstPtr &scan);

    struct SensorData
    {
        string start{"$START"};
        string name_len;
        string total_len;
        string name;
        string content;
        string terminator{"$END"};
        string all;
    };

    DataShare *ds_;
};

ClientDataPackage::ClientDataPackage():data_package_thread_(NULL){

}

ClientDataPackage::~ClientDataPackage() {
    if (data_package_thread_)
    {
        data_package_thread_->join();
        delete data_package_thread_;
    }
}

void ClientDataPackage::start(DataShare *datashare){
//    cout << "enter start" << endl;
    ds_ = datashare;
    data_package_thread_ = new boost::thread([&]{runThread();});
}

void ClientDataPackage::runThread(){
//    cout << "enter run thread" << endl;
    ros::Subscriber lidar_sub = nh_.subscribe("/scan", 10, &ClientDataPackage::lidarCallback, this);
    ros::spin();
}

void ClientDataPackage::lidarCallback(const sensor_msgs::LaserScan::ConstPtr &scan) {
//    cout << " enter lidar callback" << endl;
    if(ds_->client_control_command_.compare("start")) return;
    SensorData buff;
    proto_msg::LaserScan proto_Lidar;
    std::string name = "proto_msg.LaserScan";
    proto_Lidar.set_protocol_type(name);
    proto_Lidar.set_publish_stamp(scan->header.stamp.toSec());
    proto_Lidar.set_frame_id(scan->header.frame_id);
    proto_Lidar.set_angle_min(scan->angle_min);
    proto_Lidar.set_angle_max(scan->angle_max);
    proto_Lidar.set_angle_increment(scan->angle_increment);
    proto_Lidar.set_time_increment(scan->time_increment);
    proto_Lidar.set_scan_time(scan->scan_time);
    proto_Lidar.set_range_min(scan->range_min);
    proto_Lidar.set_range_max(scan->range_max);
    for (double x : scan->ranges)
        proto_Lidar.add_ranges(x);
    for (double x : scan->intensities)
        proto_Lidar.add_intensitys(x);
    proto_Lidar.SerializeToString(&buff.content);
    proto_Lidar.Clear();

    buff.total_len.resize(8);
    buff.name = name;
    buff.name_len = to_string(buff.name.size());
    buff.total_len = to_string(buff.start.size() + buff.name_len.size() + buff.total_len.size() + buff.name.size() + buff.content.size() + buff.terminator.size());
    buff.total_len.resize(8);
    buff.all = buff.start + buff.name_len + buff.total_len + buff.name + buff.content + buff.terminator;

    ds_->client_data_package_mtx_.lock();
    ds_->client_data_package_.append(buff.all);
    ds_->client_data_package_mtx_.unlock();

//    cout << endl << "------------------------" << endl;
//    cout << buff.start << endl;
//    cout << buff.name << endl;
//    cout << buff.total_len << " " << buff.all.size() << endl;
//    cout << buff.terminator << endl;
//    cout << "client_data_package_ size : " << ds_->client_data_package_.size() << endl;
//    cout << "------------------------" << endl << endl;


}