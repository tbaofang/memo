#pragma once
#include <iostream>
#include <string>
#include <boost/thread.hpp>
#include <thread>

#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <nav_msgs/OccupancyGrid.h>
#include <nav_msgs/Path.h>
#include <std_msgs/String.h>
#include <tf/tf.h>

#include "datashare.h"
#include "proto_msgs.LaserScan.pb.h"
#include "proto_msgs.OccupancyGrid.pb.h"
#include "proto_msgs.Path.pb.h"

using namespace std;

class ServerDataParse{
public:
    ServerDataParse();
    ~ServerDataParse();
    void start(DataShare* datashare);
private:
    ros::NodeHandle nh_;
    ros::Publisher scan_pub_;
    ros::Publisher map_pub_;
    ros::Publisher path_pub_;

    boost::thread* data_parse_thread_;
    void runThread();

    DataShare* ds_;

    void scanPublish(const string& scan);

};

ServerDataParse::ServerDataParse():data_parse_thread_(NULL) {
    scan_pub_ = nh_.advertise<sensor_msgs::LaserScan>("/protobuf_test_lidar", 10);
    map_pub_ = nh_.advertise<nav_msgs::OccupancyGrid>("/protobuf_test_map", 1);
    path_pub_ = nh_.advertise<nav_msgs::Path>("/protobuf_test_path", 10);
}
ServerDataParse::~ServerDataParse() {
    if(data_parse_thread_){
        data_parse_thread_->join();
        delete data_parse_thread_;
    }
}

void ServerDataParse::start(DataShare *datashare) {
    ds_ = datashare;
    data_parse_thread_ = new boost::thread([&]{runThread();});
}

void ServerDataParse::runThread() {
    string laser{"proto_msg.LaserScan"};
    string map{"proto_msg.OccupancyGrid"};
    string path{"proto_msg.Path"};
    string buffer;
    string content;
    while(ros::ok()) {
        if (ds_->server_data_parse_.size() > 0) {
            size_t total_len = stoi(ds_->server_data_parse_.substr(8, 8));
            buffer.assign(ds_->server_data_parse_.substr(0, total_len));

            ds_->server_data_parse_mtx_.lock();
            ds_->server_data_parse_.erase(0, total_len);
            ds_->server_data_parse_mtx_.unlock();

            size_t name_len = stoi(buffer.substr(6, 2));
            string name_str = buffer.substr(16, name_len);
            size_t content_start_pos = 6 + 2 + 8 + name_len;
            size_t content_length = total_len - content_start_pos - 4;

            content.assign(buffer.substr(content_start_pos, content_length));

//            cout << endl << "================" << endl;
//            cout << buffer.substr(0, 6) << endl;
//            cout << "head_len = " << name_len << endl;
//            cout << "name = " << name_str << endl;
//            cout << "total_len = " << total_len << endl;
//            cout << buffer.substr(total_len - 4, 4) << endl;
//            cout << "server_data_parse_ size = " << ds_->server_data_parse_.size() << endl;
//            cout << "==================" << endl << endl;

        if(!name_str.compare(laser))
            scanPublish(content);



        }
    }

}

void ServerDataParse::scanPublish(const string &scan) {
    sensor_msgs::LaserScan ls;
    proto_msg::LaserScan pls;

    bool re = pls.ParseFromString(scan);
    if (!re)
    {
        cout << "parse false!!!!!!!!!!!!!!!" << endl;
        return;
    }
    ls.header.frame_id = pls.frame_id();
    ls.header.stamp = static_cast<ros::Time>(pls.publish_stamp());
    ls.range_min = pls.range_min();
    ls.range_max = pls.range_max();
    ls.angle_min = pls.angle_min();
    ls.angle_max = pls.angle_max();
    ls.scan_time = pls.scan_time();
    ls.time_increment = pls.time_increment();
    ls.angle_increment = pls.angle_increment();
    for (int i = 0; i < pls.ranges_size(); ++i)
        ls.ranges.push_back(pls.ranges(i));
    for (int i = 0; i < pls.intensitys_size(); ++i)
        ls.intensities.push_back(pls.intensitys(i));

    scan_pub_.publish(ls);
    cout << "publish lidar succeed!" << endl;
}