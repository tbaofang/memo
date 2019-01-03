
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <nav_msgs/OccupancyGrid.h>
#include <nav_msgs/Path.h>
#include <tf/tf.h>

// #include <geometry_msgs/TransformStamped.h>
// #include <geometry_msgs/PoseStamped.h>
// #include <geometry_msgs/Twist.h>
// #include <move_base_msgs/MoveBaseActionGoal.h>

//#include "server.dataparse.h"
#include "proto_msgs.LaserScan.pb.h"
#include "proto_msgs.OccupancyGrid.pb.h"
#include "proto_msgs.Path.pb.h"

using namespace std;

class Server
{
  public:
    Server();
    ~Server();
    int startServer();

  private:
    ros::NodeHandle node_;
    ros::Publisher scan_pub_;
    ros::Publisher map_pub_;
    ros::Publisher path_pub_;

    std::string server_port_;

    bool dataRecv();
    bool dataParse();
    bool dataSent(int sockdf);

    bool string2Scan();
    bool string2Map();
    bool string2Path();

    int dataRecvBoost();

    void init();

    //    ServerDataParse* sdp_;

    int recv_size_;

    boost::thread *data_recv_thread_;
    boost::thread *data_parse_thread_;
    boost::thread *data_collect_thread_;

    string buff_scan_;
    boost::mutex buff_scan_mtx_;
    string buff_map_;
    boost::mutex buff_map_mtx_;
    string buff_path_;
    boost::mutex buff_path_mtx_;

    proto_msg::LaserScan *proto_Lidar_;
    proto_msg::OccupancyGrid *proto_map_;
    proto_msg::Path *proto_path_;
};

Server::Server()
{
    recv_size_ = 5 * 1024;
    server_port_ = "8001";
    init();

    data_recv_thread_ = NULL;
    data_parse_thread_ = NULL;
    data_collect_thread_ = NULL;
}

void Server::init()
{

    proto_Lidar_ = new proto_msg::LaserScan();
    proto_map_ = new proto_msg::OccupancyGrid();
    proto_path_ = new proto_msg::Path();

    scan_pub_ = node_.advertise<sensor_msgs::LaserScan>("/protobuf_test_lidar", 10);
    map_pub_ = node_.advertise<nav_msgs::OccupancyGrid>("/protobuf_test_map", 1);
    path_pub_ = node_.advertise<nav_msgs::Path>("/protobuf_test_path", 10);

    //    sdp_ = new ServerDataParse();
    //    ROS_ASSERT(sdp_);
}

Server::~Server()
{
    //    if(sdp_) delete sdp_;

    if (data_recv_thread_)
    {
        data_recv_thread_->join();
        delete data_recv_thread_;
    }
    if (data_parse_thread_)
    {
        data_parse_thread_->join();
        delete data_parse_thread_;
    }
    if (data_collect_thread_)
    {
        data_collect_thread_->join();
        delete data_collect_thread_;
    }

    if (proto_Lidar_)
        delete proto_Lidar_;
    if (proto_map_)
        delete proto_map_;
    if (proto_path_)
        delete proto_path_;
}

int Server::startServer()
{
    data_recv_thread_ = new boost::thread([&] { dataRecv(); });
    data_parse_thread_ = new boost::thread([&] { dataParse(); });
}

bool Server::dataRecv()
{
    struct sockaddr_in srvAddr;
    bzero(&srvAddr, sizeof(srvAddr));
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    srvAddr.sin_port = htons(atoi(server_port_.c_str()));
    int srvAddrLen = sizeof(srvAddr);

    int sockdf = socket(AF_INET, SOCK_DGRAM, 0); // udp
    int iRet = bind(sockdf, (struct sockaddr *)&srvAddr, sizeof(srvAddr));

    data_collect_thread_ = new boost::thread([&] { dataSent(sockdf); });

    struct sockaddr_in cliAddr;
    bzero(&cliAddr, sizeof(cliAddr));
    cliAddr.sin_family = AF_INET;
    int cliAddrLen = sizeof(cliAddr);

    char szBuf[recv_size_ + 1] = {0};
    int n = 0;
    string buffer;
    char header[7] = "$START";
    size_t head_n = 0;
    while (ros::ok())
    {

        bzero(&szBuf, sizeof(szBuf));
        // n = recvfrom(sockdf, szBuf, recv_size_, 0, (struct sockaddr *)&cliAddr, (socklen_t *)&cliAddrLen);
        if (n == -1)
            perror("recvfrom error");
        //        cout << "recv_size=" << n << endl;

        if (!strncmp(reinterpret_cast<const char *>(&szBuf), reinterpret_cast<const char *>(&header), 6))
        {
            if (++head_n > 1)
            {
                int name_len = static_cast<unsigned char>(buffer.at(6));
                size_t total_len = static_cast<unsigned char>(buffer.at(10)) |
                                   static_cast<unsigned char>(buffer.at(11)) << 8 |
                                   static_cast<unsigned char>(buffer.at(12)) << 16 |
                                   static_cast<unsigned char>(buffer.at(13)) << 24;
                string name_str = buffer.substr(14, name_len);

                // cout << endl
                //      << "================" << endl;
                // cout << "recv_n = " << head_n << endl;
                // cout << buffer.substr(0, 6) << endl;
                // cout << "head_len = " << name_len << endl;
                // cout << "name = " << name_str << endl;
                // cout << "total_len = " << total_len << endl;
                // cout << buffer.substr(total_len - 4, 4) << endl;
                // cout << "==================" << endl
                //      << endl;

                if (buffer.size() != total_len)
                {
                    buffer.clear();
                }
                else
                {
                    if (!strcmp("proto_msg.LaserScan", name_str.data()))
                    {
                        buff_scan_mtx_.lock();
                        buff_scan_.append(buffer);
                        buff_scan_mtx_.unlock();
                    }
                    if (!strcmp("proto_msg.OccupancyGrid", name_str.data()) && buff_map_.size() < 1000000)
                    {
                        buff_map_mtx_.lock();
                        buff_map_.append(buffer);
                        buff_map_mtx_.unlock();
                        //cout << "[string2Map]: " << "buff_map size = " << buff_map_.size() << endl;
                    }
                    if (!strcmp("proto_msg.Path", name_str.data()) && buff_map_.size() < 100000)
                    {
                        buff_path_mtx_.lock();
                        buff_path_.append(buffer);
                        buff_path_mtx_.unlock();
                        // cout << "[string2Path]: "
                        //      << "buff_path_ size = " << buff_path_.size() << endl;
                    }
                }
            }
            buffer.assign(szBuf, n);
            //            cout << "head_n = " << head_n << endl;
            //            cout << "buffer size = " <<  buffer.size() << endl;
        }
        else
        {
            buffer.append(szBuf, n);
        }
    }

    close(sockdf);
    return 0;


}

bool Server::dataSent(int sockdf)
{
    struct sockaddr_in targetAddr;
    bzero(&targetAddr, sizeof(targetAddr));
    targetAddr.sin_family = AF_INET;
    targetAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    targetAddr.sin_port = htons(8002);
    int targetAddrLen = sizeof(targetAddr);

    int n = 0;
    int send_size;
    string buf = "hello i am full duplex";
    while (ros::ok())
    {
        send_size = std::min(static_cast<int>(buf.size()), 100);
        n = sendto(sockdf, buf.data(), send_size, 0, (struct sockaddr *)&targetAddr, sizeof(targetAddr));
        cout << "send_size = " << n << endl;

        // if (cdc_->data_collection_.size() > 0)
        // {
        //     cdc_->data_collection_mtx_.lock();
        //     buf.assign(cdc_->data_collection_);
        //     cdc_->data_collection_.clear();
        //     cdc_->data_collection_mtx_.unlock();
        //     while (!buf.empty())
        //     {
        //         send_size = std::min(static_cast<int>(buf.size()), send_size_);
        //         n = sendto(sockdf, buf.data(), send_size, 0, (struct sockaddr *)&srvAddr, sizeof(srvAddr));
        //         if (n == -1)
        //             perror("sendto error");
        //         //                cout << "send size: " <<  n << endl;
        //         buf.erase(0, n);
        //     }
        // }
    }

    close(sockdf);


    
}

bool Server::dataParse()
{
    while (ros::ok())
    {
        if (buff_scan_.size() > 0)
            string2Scan();
        if (buff_map_.size() > 0)
            string2Map();
        if (buff_path_.size() > 0)
            string2Path();
    }
}

bool Server::string2Scan()
{
    //    cout << "[string2Scan]: " << "buff_scan size = " << buff_scan_.size() << endl;
    size_t start = buff_scan_.find("$START", 0);
    size_t last = buff_scan_.find("$END", 1);
    //    cout << "[string2Scan]: " << "start = " << start << endl;
    //    cout << "[string2Scan]: " << "last= " << last<< endl;
    if (start == string::npos || last == string::npos)
    {
        cout << "not find start and end !!!!!!!!!!!!";
        buff_scan_.clear();
        return false;
    }
    if (start)
    {
        buff_scan_mtx_.lock();
        buff_scan_.erase(0, start);
        buff_scan_mtx_.unlock();
        cout << "start != 0 !!!!!!!!!!" << endl;
    }

    size_t total_size = static_cast<unsigned char>(buff_scan_.at(10)) |
                        static_cast<unsigned char>(buff_scan_.at(11)) << 8;
    if (total_size != (last - start + 4))
    {
        cout << "loss flame!!!!!!!!!!" << endl;
    }

    size_t content_start_pos = 6 + 4 + 4 + 19;
    size_t content_length = total_size - content_start_pos - 4;
    //    cout << "[string2Scan]: " << "content_length=" <<content_length<< endl;
    //    printf("test..............%c \n", buff_scan_.at(total_size-1));
    string content = buff_scan_.substr(content_start_pos, content_length);
    buff_scan_mtx_.lock();
    buff_scan_.erase(0, last + 4);
    buff_scan_mtx_.unlock();

    sensor_msgs::LaserScan ls;
    bool pfs = proto_Lidar_->ParseFromString(content);
    if (!pfs)
    {
        cout << "parse false!!!!!!!!!!!!!!!" << endl;
        return false;
    }

    ls.header.frame_id = proto_Lidar_->frame_id();
    ls.header.stamp = static_cast<ros::Time>(proto_Lidar_->publish_stamp());
    ls.range_min = proto_Lidar_->range_min();
    ls.range_max = proto_Lidar_->range_max();
    ls.angle_min = proto_Lidar_->angle_min();
    ls.angle_max = proto_Lidar_->angle_max();
    ls.scan_time = proto_Lidar_->scan_time();
    ls.time_increment = proto_Lidar_->time_increment();
    ls.angle_increment = proto_Lidar_->angle_increment();
    for (int i = 0; i < proto_Lidar_->ranges_size(); ++i)
        ls.ranges.push_back(proto_Lidar_->ranges(i));
    for (int i = 0; i < proto_Lidar_->intensitys_size(); ++i)
        ls.intensities.push_back(proto_Lidar_->intensitys(i));

    scan_pub_.publish(ls);
    cout << "publish lidar succeed!" << endl;
    return true;
}

bool Server::string2Map()
{
    cout << endl
         << "================" << endl;
    cout << buff_map_.substr(0, 6) << endl;
    cout << (static_cast<unsigned char>(buff_map_.at(6)) |
             static_cast<unsigned char>(buff_map_.at(7)) << 8 |
             static_cast<unsigned char>(buff_map_.at(8)) << 16 |
             static_cast<unsigned char>(buff_map_.at(9)) << 24)
         << endl;
    cout << (static_cast<unsigned char>(buff_map_.at(10)) |
             static_cast<unsigned char>(buff_map_.at(11)) << 8 |
             static_cast<unsigned char>(buff_map_.at(12)) << 16 |
             static_cast<unsigned char>(buff_map_.at(13)) << 24)
         << endl;
    cout << buff_map_.substr(14, 23) << endl;
    cout << "==================" << endl
         << endl;
    //
    //    size_t start = buff_map_.find("$START", 0);
    //    size_t last = buff_map_.find("$END", 1);
    //    cout << "[string2Scan]: " << "start = " << start << endl;
    //    cout << "[string2Scan]: " << "last= " << last<< endl;
    //    if(start == string::npos || last == string::npos) {
    //        cout << "not find start and end !!!!!!!!!!!!";
    //        buff_map_.clear();
    //        return false;
    //    }
    //    if(start) {
    //        buff_map_mtx_.lock();
    //        buff_map_.erase(0,start);
    //        buff_map_mtx_.unlock();
    //        cout << "start != 0 !!!!!!!!!!" << endl;
    //    }

    size_t name_size = static_cast<unsigned char>(buff_map_.at(6)) |
                       static_cast<unsigned char>(buff_map_.at(7)) << 8 |
                       static_cast<unsigned char>(buff_map_.at(8)) << 16 |
                       static_cast<unsigned char>(buff_map_.at(9)) << 24;

    size_t total_size = static_cast<unsigned char>(buff_map_.at(10)) |
                        static_cast<unsigned char>(buff_map_.at(11)) << 8 |
                        static_cast<unsigned char>(buff_map_.at(12)) << 16 |
                        static_cast<unsigned char>(buff_map_.at(13)) << 24;
    //    if(total_size != (last-start+4)) {
    //        cout << "loss flame!!!!!!!!!!" << endl;
    //    }

    size_t content_start_pos = 6 + 4 + 4 + name_size;
    size_t content_length = total_size - content_start_pos - 4;

    cout << "end: " << buff_map_.substr(total_size - 4, 4) << endl;

    string content;
    try
    {
        content = buff_map_.substr(content_start_pos, content_length);
    }
    catch (std::exception &e)
    {
        cout << e.what();
        return false;
    }

    buff_map_mtx_.lock();
    buff_map_.erase(0, total_size);
    buff_map_mtx_.unlock();

    // cout << "buff_map_ size" << buff_map_.size() <<  endl;

    nav_msgs::OccupancyGrid og;
    bool pfs = proto_map_->ParseFromString(content);
    if (!pfs)
    {
        cout << "parse false!!!!!!!!!!!!!!!" << endl;
        return false;
    }

    og.header.frame_id = proto_map_->frame_id();
    og.header.stamp = static_cast<ros::Time>(proto_map_->publish_stamp());
    og.info.map_load_time = static_cast<ros::Time>(proto_map_->load_time());
    og.info.resolution = proto_map_->resolution();
    og.info.width = proto_map_->width();
    og.info.height = proto_map_->height();
    og.info.origin.position.x = proto_map_->x();
    og.info.origin.position.y = proto_map_->y();
    og.info.origin.position.z = proto_map_->z();
    og.info.origin.orientation.x = proto_map_->q_x();
    og.info.origin.orientation.y = proto_map_->q_y();
    og.info.origin.orientation.z = proto_map_->q_z();
    og.info.origin.orientation.w = proto_map_->q_w();
    std::string m = proto_map_->map_data();
    og.data.assign(m.begin(), m.end());

    map_pub_.publish(og);
    cout << "publish map succeed!" << endl;
    return true;
}

bool Server::string2Path()
{
    cout << buff_path_.size() << endl;

    size_t start = buff_path_.find("$START", 0);
    size_t last = buff_path_.find("$END", 1);
    if (start == string::npos || last == string::npos)
    {
        cout << "not find start and end !!!!!!!!!!!!";
        buff_path_.clear();
        return false;
    }
    if (start)
    {
        buff_path_mtx_.lock();
        buff_path_.erase(0, start);
        buff_path_mtx_.unlock();
        cout << "start != 0 !!!!!!!!!!" << endl;
    }
    size_t name_size = static_cast<unsigned char>(buff_path_.at(6)) |
                       static_cast<unsigned char>(buff_path_.at(7)) << 8;
    size_t total_size = static_cast<unsigned char>(buff_path_.at(10)) |
                        static_cast<unsigned char>(buff_path_.at(11)) << 8;
    string buffer = buff_path_.substr(start, total_size);

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
    cout << buffer.substr(14, name_size) << endl;
    cout << buffer.substr(total_size - 4, 4) << endl;
    cout << "==================" << endl
         << endl;

    buff_path_mtx_.lock();
    buff_path_.erase(0, total_size);
    buff_path_mtx_.unlock();

    try
    {
        size_t content_start_pos = 6 + 4 + 4 + name_size;
        buffer.erase(total_size - 4, 4);
        buffer.erase(0, content_start_pos);
    }
    catch (exception &e)
    {
        cout << e.what() << endl;
    }

    bool pfs = proto_path_->ParseFromString(buffer);
    if (!pfs)
    {
        cout << "parse false!!!!!!!!!!!!!!!" << endl;
        return false;
    }

    nav_msgs::Path p;
    geometry_msgs::PoseStamped ps;
    for (int i = 0; i < proto_path_->poses_size(); ++i)
    {
        ps.header.stamp = static_cast<ros::Time>(proto_path_->poses(i).publish_stamp());
        ps.header.frame_id = proto_path_->poses(i).frame_id();
        ps.pose.position.x = proto_path_->poses(i).x();
        ps.pose.position.y = proto_path_->poses(i).y();
        ps.pose.position.z = proto_path_->poses(i).z();
        tf::Quaternion quat;
        quat.setRPY(proto_path_->poses(i).roll(), proto_path_->poses(i).pitch(), proto_path_->poses(i).yaw());
        ps.pose.orientation.x = quat.x();
        ps.pose.orientation.y = quat.y();
        ps.pose.orientation.z = quat.z();
        ps.pose.orientation.w = quat.w();

        p.poses.push_back(ps);
    }

    // ros::Time time(t.proto_path_.poses(i).publish_stamp());
    //     ps.header.stamp = time;

    //     ps.header.frame_id = t.proto_path_.poses(i).frame_id();
    //     ps.pose.position.x = t.proto_path_.poses(i).x();
    //     ps.pose.position.y = t.proto_path_.poses(i).y();
    //     ps.pose.position.z = t.proto_path_.poses(i).z();
    //     tf::Quaternion qua;
    //     qua.setRPY(t.proto_path_.poses(i).roll(),t.proto_path_.poses(i).pitch(),t.proto_path_.poses(i).yaw());
    //     ps.pose.orientation.x = qua.x();
    //     ps.pose.orientation.y = qua.y();
    //     ps.pose.orientation.z = qua.z();
    //     ps.pose.orientation.w = qua.w();

    //     p.poses.push_back(ps);
    // }

    path_pub_.publish(p);
    cout << "public path succeed!!!" << endl;
}

int Server::dataRecvBoost()
{
    boost::asio::io_service io_service;
    boost::asio::ip::udp::socket udp_socket(io_service);

    boost::asio::ip::udp::endpoint local_add(boost::asio::ip::address::from_string("127.0.0.1"), 9000);

    udp_socket.open(local_add.protocol());
    udp_socket.bind(local_add);

    char receive_str[1024] = {0};
    while (ros::ok())
    {
        boost::asio::ip::udp::endpoint sendpoint;
        udp_socket.receive_from(boost::asio::buffer(receive_str, 1024), sendpoint);
        cout << "收到" << sendpoint.address().to_string() << ":" << receive_str << endl;
        udp_socket.send_to(boost::asio::buffer("服务端返回success"), sendpoint);
        memset(receive_str, 0, 1024);
    }
}