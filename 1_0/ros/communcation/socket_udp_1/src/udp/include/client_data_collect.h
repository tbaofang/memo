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
#include <nav_msgs/OccupancyGrid.h>
#include <nav_msgs/Path.h>
#include <tf/tf.h>

#include "proto_msgs.LaserScan.pb.h"
#include "proto_msgs.OccupancyGrid.pb.h"
#include "proto_msgs.Path.pb.h"

using namespace std;

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

using namespace logging::trivial;

class ClientDataCollect
{
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
    // ros::Subscriber lidar_sub_;
    // ros::Subscriber map_sub_;

    std::string content_str;
    proto_msg::LaserScan *proto_Lidar_;
    proto_msg::OccupancyGrid *proto_map_;
    proto_msg::Path *proto_path_;

    void lidarCallBack(const sensor_msgs::LaserScan::ConstPtr &scan);
    void mapCallBack(const nav_msgs::OccupancyGrid::ConstPtr &map);
    void pathCallback(const nav_msgs::Path::ConstPtr &path);
    //    vector<size_t> data_collect_ = {0};

    void init();

    src::severity_logger<logging::trivial::severity_level> lg;
};

ClientDataCollect::ClientDataCollect()
{
    init();
    logging::add_common_attributes();

    proto_Lidar_ = new proto_msg::LaserScan();
    proto_map_ = new proto_msg::OccupancyGrid();
    proto_path_ = new proto_msg::Path();
}

void ClientDataCollect::init()
{
    logging::add_file_log(keywords::file_name = "sample_%N.log",                                        /*< file name pattern >*/
                          keywords::rotation_size = 10 * 1024 * 1024,                                   /*< rotate files every 10 MiB... >*/
                          keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0), /*< ...or at midnight >*/
                          keywords::format = "[%TimeStamp%][%Severity%][%Channel%]: %Message%"          /*< log record format >*/
    );

    logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::info);
}

ClientDataCollect::~ClientDataCollect()
{
    if (proto_Lidar_)
        delete proto_Lidar_;
    if (proto_map_)
        delete proto_map_;
    if (proto_path_)
        delete proto_path_;
}

int ClientDataCollect::dataCollect()
{
    ros::Subscriber lidar_sub = node_.subscribe("/scan", 10, &ClientDataCollect::lidarCallBack, this);
    ros::Subscriber map_sub = node_.subscribe("/map", 1, &ClientDataCollect::mapCallBack, this);
    ros::Subscriber path_sub = node_.subscribe("/move_base/NavfnROS/plan", 10, &ClientDataCollect::pathCallback, this);

    ros::spin();
}

void ClientDataCollect::lidarCallBack(const sensor_msgs::LaserScan::ConstPtr &scan)
{
    //    GOOGLE_PROTOBUF_VERIFY_VERSION;
    std::string name = "proto_msg.LaserScan";
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
    for (double x : scan->ranges)
        proto_Lidar_->add_ranges(x);
    for (double x : scan->intensities)
        proto_Lidar_->add_intensitys(x);
    proto_Lidar_->SerializeToString(&content_str);
    proto_Lidar_->Clear();
    //    google::protobuf::ShutdownProtobufLibrary();

    std::string buffer;
    buffer.append("$START");
    int name_len = static_cast<int>(name.size());
    buffer.append(reinterpret_cast<const char *>(&name_len), sizeof(int));
    int total_len = static_cast<int>(6 + name_len + 4 + 4 + content_str.size() + 4);
    buffer.append(reinterpret_cast<const char *>(&total_len), sizeof(int));
    buffer.append(name);
    buffer.append(content_str);
    buffer.append("$END");

    //    cout << buffer.size() << " " << total_len << endl;

    if (data_collection_.size() < 2000000)
    {
        data_collection_mtx_.lock();
        data_collection_.append(buffer);
        data_collection_mtx_.unlock();
    }

    cout << endl
         << "================" << endl;
    cout << buffer.substr(0, 6) << endl;
    cout << (static_cast<unsigned char>(buffer.at(6)) |
             static_cast<unsigned char>(buffer.at(7)) << 8 |
             static_cast<unsigned char>(buffer.at(8)) << 16 |
             static_cast<unsigned char>(buffer.at(9)) << 24)
         << endl;
    cout << (static_cast<unsigned char>(buffer.at(10)) |
             static_cast<unsigned char>(buffer.at(11)) << 8 |
             static_cast<unsigned char>(buffer.at(12)) << 16 |
             static_cast<unsigned char>(buffer.at(13)) << 24)
         << endl;
    cout << buffer.substr(14, name_len) << endl;
    cout << buffer.substr(total_len - 4, 4) << endl;
    cout << "==================" << endl
         << endl;

    //    cout << "[ClientDataCollect::lidarCallBack] " << "data_collection_.size()=" << data_collection_.size() << endl;
    //    BOOST_LOG_TRIVIAL(info) << "boost log" ;
    //    BOOST_LOG_SEV(lg, trace) << "A trace severity message";
    //    BOOST_LOG_SEV(lg, debug) << "A debug severity message";
    //    BOOST_LOG_SEV(lg, info) << "An informational severity message";
    //    BOOST_LOG_SEV(lg, warning) << "A warning severity message";
    //    BOOST_LOG_SEV(lg, error) << "An error severity message";
    //    BOOST_LOG_SEV(lg, fatal) << "A fatal severity message";
}

void ClientDataCollect::mapCallBack(const nav_msgs::OccupancyGrid::ConstPtr &map)
{
    string name = "proto_msg.OccupancyGrid";
    proto_map_->set_protocol_type(name);
    proto_map_->set_publish_stamp(map->header.stamp.toSec());
    proto_map_->set_frame_id(map->header.frame_id);
    proto_map_->set_load_time(map->info.map_load_time.toSec());
    proto_map_->set_resolution(map->info.resolution);
    proto_map_->set_width(map->info.width);
    proto_map_->set_height(map->info.height);
    proto_map_->set_x(map->info.origin.position.x);
    proto_map_->set_y(map->info.origin.position.y);
    proto_map_->set_z(map->info.origin.position.z);
    proto_map_->set_q_x(map->info.origin.orientation.x);
    proto_map_->set_q_y(map->info.origin.orientation.y);
    proto_map_->set_q_z(map->info.origin.orientation.z);
    proto_map_->set_q_w(map->info.origin.orientation.w);
    //    for(auto x:map->data) proto_map_->set_map_data(reinterpret_cast<const char*>(&x));

    std::string m;
    for (int i = 0; i < map->data.size(); i++)
    {
        m.push_back(map->data.at(i));
    }
    proto_map_->set_map_data(m);

    //    google::protobuf::Message req;
    string buf_map;
    bool to_str = proto_map_->SerializeToString(&buf_map);
    proto_map_->Clear();
    if (!to_str)
    {
        cout << "fail!!!!!" << endl;
    }
    else
    {
        int name_len = static_cast<int>(name.size());
        int content_size = buf_map.size();
        int total_len = static_cast<int>(6 + name_len + 4 + 4 + content_size + 4);
        buf_map.insert(0, name);
        buf_map.insert(0, reinterpret_cast<const char *>(&total_len), sizeof(int));
        buf_map.insert(0, reinterpret_cast<const char *>(&name_len), sizeof(int));

        buf_map.insert(0, "$START");
        buf_map.append("$END");

        cout << buf_map.size() << " " << total_len << endl;

        if (data_collection_.size() < 3000000)
        {
            data_collection_mtx_.lock();
            data_collection_.append(buf_map);
            data_collection_mtx_.unlock();
        }

        cout << endl
             << "================" << endl;
        cout << buf_map.substr(0, 6) << endl;
        cout << (static_cast<unsigned char>(buf_map.at(6)) |
                 static_cast<unsigned char>(buf_map.at(7)) << 8 |
                 static_cast<unsigned char>(buf_map.at(8)) << 16 |
                 static_cast<unsigned char>(buf_map.at(9)) << 24)
             << endl;
        cout << (static_cast<unsigned char>(buf_map.at(10)) |
                 static_cast<unsigned char>(buf_map.at(11)) << 8 |
                 static_cast<unsigned char>(buf_map.at(12)) << 16 |
                 static_cast<unsigned char>(buf_map.at(13)) << 24)
             << endl;
        cout << buf_map.substr(14, name_len) << endl;
        cout << buf_map.substr(total_len - 4, 4) << endl;
        cout << "==================" << endl
             << endl;

        //        cout << buf_map.substr(0, 6) << endl;
        //        cout << buf_map.size() << endl;
    }
}

void ClientDataCollect::pathCallback(const nav_msgs::Path::ConstPtr &path)
{
    string name = "proto_msg.Path";
    proto_path_->set_protocol_type(name);
    proto_path_->set_publish_stamp(path->header.stamp.toSec());
    proto_path_->set_frame_id(path->header.frame_id);
    for (auto pose : path->poses)
    {
        proto_msg::PathPoint *pp;
        pp = proto_path_->add_poses();
        pp->set_publish_stamp(pose.header.stamp.toSec());
        pp->set_frame_id(pose.header.frame_id);
        pp->set_x(pose.pose.position.x);
        pp->set_y(pose.pose.position.y);
        pp->set_z(pose.pose.position.z);
        double roll, pitch, yaw;
        tf::Quaternion quat;
        tf::quaternionMsgToTF(pose.pose.orientation, quat);
        tf::Matrix3x3(quat).getRPY(roll, pitch, yaw);
        pp->set_roll(roll);
        pp->set_pitch(pitch);
        pp->set_yaw(yaw);
    }

    string buffer;
    bool to_str = proto_path_->SerializeToString(&buffer);
    proto_path_->Clear();
    if (to_str)
    {
        int name_len = static_cast<int>(name.size());
        int content_size = buffer.size();
        int total_len = static_cast<int>(6 + name_len + 4 + 4 + content_size + 4);
        buffer.insert(0, name);
        buffer.insert(0, reinterpret_cast<const char *>(&total_len), sizeof(int));
        buffer.insert(0, reinterpret_cast<const char *>(&name_len), sizeof(int));
        buffer.insert(0, "$START");
        buffer.append("$END");
        if (data_collection_.size() < 3000000)
        {
            data_collection_mtx_.lock();
            data_collection_.append(buffer);
            data_collection_mtx_.unlock();
        }

        cout << endl
             << "================" << endl;
        cout << buffer.substr(0, 6) << endl;
        cout << (static_cast<unsigned char>(buffer.at(6)) |
                 static_cast<unsigned char>(buffer.at(7)) << 8 |
                 static_cast<unsigned char>(buffer.at(8)) << 16 |
                 static_cast<unsigned char>(buffer.at(9)) << 24)
             << endl;
        cout << (static_cast<unsigned char>(buffer.at(10)) |
                 static_cast<unsigned char>(buffer.at(11)) << 8 |
                 static_cast<unsigned char>(buffer.at(12)) << 16 |
                 static_cast<unsigned char>(buffer.at(13)) << 24)
             << endl;
        cout << buffer.substr(14, name_len) << endl;
        cout << buffer.substr(total_len - 4, 4) << endl;
        cout << "==================" << endl
             << endl;
    }
    else
    {
        cout << "path: SerializeToString fail!!!!!" << endl;
    }
}