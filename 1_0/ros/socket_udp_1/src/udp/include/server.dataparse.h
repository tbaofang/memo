#include <iostream>
#include <boost/thread.hpp>


#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>

//#include <server.h>

using namespace std;


class ServerDataParse {
public:
    ServerDataParse();
    string buf_scan_;
    boost::mutex buf_scan_mtx_;

    string buf_map_;
    boost::mutex buf_map_mtx_;
    int dataParse();

private:
    ros::NodeHandle node_;
    ros::Publisher scan_pub_;

    void string2Scan();

//    Server* s_;
};

ServerDataParse::ServerDataParse() {
    buf_scan_ = {0};

//    s_ = new Server();
}

int ServerDataParse::dataParse() {
//    scan_pub_ = node_.advertise<>
//    cout << "hello -----------" << endl;

    while(ros::ok()) {
//        if(buf_scan_->size() > 0)  string2Scan();
//        cout<< "[ServerDataParse::string2Scan]: " << s_->buff_scan_.size() << endl;

    }

}

void ServerDataParse::string2Scan() {
    cout<< "[ServerDataParse::string2Scan]: " << endl;

//    cout << "[ServerDataParse::string2Scan]: " << buf_scan_->size() << endl;
//    buf_scan_mtx_.lock();
//    buf_scan_.clear();
//    buf_scan_mtx_.unlock();
}