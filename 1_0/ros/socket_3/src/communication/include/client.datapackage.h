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

    void lidarCallback(const sensor_msgs::LaserScan::ConstPtr &msg);
    void mapCallback(const nav_msgs::OccupancyGrid::ConstPtr &msg);
    void pathCallback(const nav_msgs::Path::ConstPtr &msg);

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

    void pack(const string &name, const string &content);
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
    ros::Subscriber map_sub = nh_.subscribe("/map", 1, &ClientDataPackage::mapCallback, this);
    ros::Subscriber path_sub = nh_.subscribe("/move_base/NavfnROS/plan", 10, &ClientDataPackage::pathCallback, this);

    ros::spin();
}

void ClientDataPackage::pack(const string &name, const string &content) {
    SensorData buff;
    buff.name = name;
    buff.content = content;
    buff.total_len.resize(8);
    buff.name_len = to_string(buff.name.size());
    buff.total_len = to_string(buff.start.size() + buff.name_len.size() + buff.total_len.size() + buff.name.size() + buff.content.size() + buff.terminator.size());
    buff.total_len.resize(8);
    buff.all = buff.start + buff.name_len + buff.total_len + buff.name + buff.content + buff.terminator;

    ds_->client_data_package_mtx_.lock();
    ds_->client_data_package_.append(buff.all);
    ds_->client_data_package_mtx_.unlock();

    cout << endl << "------------------------" << endl;
    cout << buff.start << endl;
    cout << buff.name << endl;
    cout << buff.total_len << " " << buff.all.size() << endl;
    cout << buff.terminator << endl;
    cout << "client_data_package_ size : " << ds_->client_data_package_.size() << endl;
    cout << "------------------------" << endl << endl;
}

void ClientDataPackage::lidarCallback(const sensor_msgs::LaserScan::ConstPtr &scan) {
//    cout << " enter lidar callback" << endl;
    if(ds_->client_control_command_.compare("start")) return;
    string name = "proto_msg.LaserScan";
    string content;
    proto_msg::LaserScan proto_Lidar;

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
    proto_Lidar.SerializeToString(&content);
    proto_Lidar.Clear();

    pack(name, content);

//    buff.total_len.resize(8);
//    buff.name = name;
//    buff.name_len = to_string(buff.name.size());
//    buff.total_len = to_string(buff.start.size() + buff.name_len.size() + buff.total_len.size() + buff.name.size() + buff.content.size() + buff.terminator.size());
//    buff.total_len.resize(8);
//    buff.all = buff.start + buff.name_len + buff.total_len + buff.name + buff.content + buff.terminator;
//
//    ds_->client_data_package_mtx_.lock();
//    ds_->client_data_package_.append(buff.all);
//    ds_->client_data_package_mtx_.unlock();

//    cout << endl << "------------------------" << endl;
//    cout << buff.start << endl;
//    cout << buff.name << endl;
//    cout << buff.total_len << " " << buff.all.size() << endl;
//    cout << buff.terminator << endl;
//    cout << "client_data_package_ size : " << ds_->client_data_package_.size() << endl;
//    cout << "------------------------" << endl << endl;
}

void ClientDataPackage::mapCallback(const nav_msgs::OccupancyGrid::ConstPtr &msg) {
    if(ds_->client_control_command_.compare("start")) return;
    string name = "proto_msg.OccupancyGrid";
    string content;
    proto_msg::OccupancyGrid proto;

    proto.set_protocol_type(name);
    proto.set_publish_stamp(msg->header.stamp.toSec());
    proto.set_frame_id(msg->header.frame_id);
    proto.set_load_time(msg->info.map_load_time.toSec());
    proto.set_resolution(msg->info.resolution);
    proto.set_width(msg->info.width);
    proto.set_height(msg->info.height);
    proto.set_x(msg->info.origin.position.x);
    proto.set_y(msg->info.origin.position.y);
    proto.set_z(msg->info.origin.position.z);
    proto.set_q_x(msg->info.origin.orientation.x);
    proto.set_q_y(msg->info.origin.orientation.y);
    proto.set_q_z(msg->info.origin.orientation.z);
    proto.set_q_w(msg->info.origin.orientation.w);
    std::string m;
    for (int i = 0; i < msg->data.size(); i++)
        m.push_back(msg->data.at(i));
    proto.set_map_data(m);
    bool re = proto.SerializeToString(&content);
    proto.Clear();
    if (!re)
    {
        cout << "fail!!!!!" << endl;
        return;
    }

    pack(name, content);
}

void ClientDataPackage::pathCallback(const nav_msgs::Path::ConstPtr &msg) {

}