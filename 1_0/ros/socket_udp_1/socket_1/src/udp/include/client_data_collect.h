#include <stdlib.h>

#include <vector>
#include <iostream>
#include <algorithm>
#include <boost/thread.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>


#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>


#include "proto_msgs.LaserScan.pb.h"


using namespace std;


namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

using namespace logging::trivial;



class ClientDataCollect{
public:
    ClientDataCollect();
    ~ClientDataCollect();

    std::vector<int> data_collect_ = {1, 2, 3};
    boost::mutex data_collect_mutex_;


    std::string data_collection_;
    boost::mutex data_collection_mtx_;

    int dataCollect();
private:
    ros::NodeHandle node_;
    ros::Subscriber lidar_sub_;

    std::string content_str;
    proto_msg::LaserScan* proto_Lidar_;



    void lidarCallBack(const sensor_msgs::LaserScan::ConstPtr& scan);
//    vector<size_t> data_collect_ = {0};

    void init();

    src::severity_logger<logging::trivial::severity_level> lg;


};

ClientDataCollect::ClientDataCollect() {
    init();
    logging::add_common_attributes();
}

void ClientDataCollect::init() {
    logging::add_file_log(keywords::file_name = "sample_%N.log",                                        /*< file name pattern >*/
                    keywords::rotation_size = 10 * 1024 * 1024,                                   /*< rotate files every 10 MiB... >*/
                    keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0), /*< ...or at midnight >*/
                    keywords::format = "[%TimeStamp%][%Severity%][%Channel%]: %Message%"                                 /*< log record format >*/
    );

    logging::core::get()->set_filter(logging::trivial::severity>=logging::trivial::info);

}

ClientDataCollect::~ClientDataCollect() {
    if(proto_Lidar_)
        delete proto_Lidar_;
}


int ClientDataCollect::dataCollect() {
//    cout << "data collect " << endl;

    ros::Subscriber lidar_sub_ = node_.subscribe("/scan",10,&ClientDataCollect::lidarCallBack, this);
    ros::spin();
//    ros::Rate r(1.0);
//    size_t i = 0;
//    while(ros::ok()){
//        data_collect_mutex_.lock();
//        data_collect_.push_back(i);
//        data_collect_mutex_.unlock();
//        i += 1;
//        ROS_INFO("this is data collect, i = %d", data_collect_.back());
//        for_each(data_collect_.begin(), data_collect_.end(), [](size_t i){cout << i << endl;});

//        publishTransform();
//        r.sleep();
//    }
}

void ClientDataCollect::lidarCallBack(const sensor_msgs::LaserScan::ConstPtr& scan) {
//    GOOGLE_PROTOBUF_VERIFY_VERSION;
    std::string name = "proto_msg.LaserScan";

    proto_Lidar_ = new proto_msg::LaserScan();
    proto_Lidar_->set_protocol_type(name);
    proto_Lidar_->set_publish_stamp(scan->header.stamp.toSec());
    proto_Lidar_->set_frame_id(scan->header.frame_id);
    proto_Lidar_->set_angle_min(scan->angle_min);
    proto_Lidar_->set_angle_max(scan->angle_max);
    proto_Lidar_->set_angle_increment(scan->angle_increment);
    proto_Lidar_->set_time_increment(scan->time_increment);
    proto_Lidar_->set_scan_time(scan->scan_time);
    proto_Lidar_->set_range_min(scan->range_min);
    proto_Lidar_->set_range_max(scan->range_max);
    for(double x:scan->ranges) proto_Lidar_->add_ranges(x);
    for(double x:scan->intensities) proto_Lidar_->add_intensitys(x);
    proto_Lidar_->SerializeToString(&content_str);
    proto_Lidar_->Clear();
//    google::protobuf::ShutdownProtobufLibrary();

    std::string buffer;
    buffer.append("$START");
    int name_len = static_cast<int>(name.size());
    buffer.append(reinterpret_cast<const char*>(&name_len), sizeof(int));
    int total_len = static_cast<int>(6+name_len+4+4+content_str.size());
    buffer.append(reinterpret_cast<const char*>(&total_len),sizeof(int));
    buffer.append(name);
    buffer.append(content_str);

//    cout << buffer.size() << " " << total_len << endl;
//
//     string head_str;
//    head_str = buffer.substr(6, 8);
//
//
//    std::string str_dec = "2001, A Space Odyssey";
//    std::string::size_type sz;
//    printf("%c \n", head_str.at(0));
//    printf("%x \n", head_str.at(1));
//    printf("%x \n", head_str.at(2));
//    printf("%x \n", head_str.at(3));
//    printf("%x \n", head_str.at(4));
//    printf("%x \n", head_str.at(5));
//    printf("%x \n", head_str.at(6));
//    printf("%x \n", head_str.at(7));

//    cout << typeid(head_str) << endl;
//    printf("%x \n", head_str.at(10));
//    printf("%x \n", head_str.at(11));
//    printf("%x \n", head_str.at(12));
//    printf("%x \n", head_str.at(13));
//    cout << std::stoi(str_dec, &sz) << endl;

//    unsigned char he;
//    he = reinterpret_cast<char>(&head_len);

//
//    cout << buffer.substr(0, 6) << endl;
//    cout << buffer.substr(14, 19) << endl;

    if(data_collection_.size() < 2000000) {
        data_collection_mtx_.lock();
        data_collection_.append(buffer);
        data_collection_mtx_.unlock();
    }

    cout << "[ClientDataCollect::lidarCallBack] " << "data_collection_.size()=" << data_collection_.size() << endl;
//    BOOST_LOG_TRIVIAL(info) << "boost log" ;
//    BOOST_LOG_SEV(lg, trace) << "A trace severity message";
//    BOOST_LOG_SEV(lg, debug) << "A debug severity message";
//    BOOST_LOG_SEV(lg, info) << "An informational severity message";
//    BOOST_LOG_SEV(lg, warning) << "A warning severity message";
//    BOOST_LOG_SEV(lg, error) << "An error severity message";
//    BOOST_LOG_SEV(lg, fatal) << "A fatal severity message";


}


