#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "client_data_collect.h"

#include <iostream>
#include <algorithm>
#include <boost/thread.hpp>
#include <boost/asio.hpp>

#include <ros/ros.h>


using namespace std;

class Client{
public:
    Client(string server_port);
    ~Client();
    int startClient();

    void init();


private:
    ros::NodeHandle node_;
    string server_port_;

    boost::thread* data_send_thread_;
    boost::thread* data_collect_thread_;

    ClientDataCollect* cdc_;

    boost::mutex data_collect_mutex_;
    vector<size_t> data_collect_ = {1, 2, 3};

    int send_size_;

    int dataSend();
    int dataSendBoost();
    // int dataCollect();

};

//Client::Client(string server_port): server_port_(server_port){}

Client::Client(string server_port):send_size_(4*1024),
        server_port_(server_port),data_send_thread_(NULL),data_collect_thread_(NULL) {
    init();
}


//Client::~Client() {}
Client::~Client() {
    if(data_send_thread_){
        data_send_thread_->join();
        delete data_send_thread_;
    }
    if(data_collect_thread_){
        data_collect_thread_->join();
        delete data_collect_thread_;
    }

    if(cdc_) delete cdc_;
}

void Client::init() {
    cdc_ = new ClientDataCollect();
    ROS_ASSERT(cdc_);
}

int Client::startClient() {
//    sentData();
//    data_send_thread_ = new boost::thread(boost::bind(&Client::dataCollect, this));
    // data_collect_thread_ = new boost::thread([&]{cdc_->dataCollect();});
    data_send_thread_= new boost::thread([&]{dataSend();});
    data_collect_thread_ = new boost::thread([&]{cdc_->dataCollect();});



}

int Client::dataSend() {
    struct sockaddr_in srvAddr;
    bzero(&srvAddr, sizeof(srvAddr));
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    srvAddr.sin_port = htons(atoi(server_port_.c_str()));

    int iSock = socket(AF_INET, SOCK_DGRAM, 0); // udp
    int n = 0;
    int send_size;
    string buf;
    while(ros::ok())
    {

        if(cdc_->data_collection_.size() > 0) {
            cdc_->data_collection_mtx_.lock();
            buf.assign(cdc_->data_collection_);
            cdc_->data_collection_.clear();
            cdc_->data_collection_mtx_.unlock();
            while(!buf.empty()) {
                send_size = std::min(static_cast<int>(buf.size()), send_size_);
                n = sendto(iSock, buf.data(), send_size, 0, (struct sockaddr *) &srvAddr, sizeof(srvAddr));
                if (n == -1)
                    perror("sendto error");
                cout << "send size: " <<  n << endl;
                buf.erase(0, n);
            }


        }

    }

    close(iSock);
    return 0;
}

int Client::dataSendBoost() {
    boost::asio::io_service io_service;
    boost::asio::ip::udp::socket udp_socket(io_service);
    boost::asio::ip::udp::endpoint local_add(boost::asio::ip::address::from_string("127.0.0.1"), 9000);

    udp_socket.open(local_add.protocol());

    char receive_str[1024] = { 0 };//字符串
    while (ros::ok())
    {
        boost::asio::ip::udp::endpoint  sendpoint;//请求的IP以及端口
        string s;
        cin >> s;
        udp_socket.send_to(boost::asio::buffer(s.c_str(),s.size()), local_add);
        udp_socket.receive_from(boost::asio::buffer(receive_str, 1024), local_add);//收取
        cout << "收到" << receive_str << endl;
        memset(receive_str, 0, 1024);//清空字符串
    }

}














// int Client::dataCollect() {

// //    ros::Rate r(1.0);
//     size_t i = 0;
//     while(ros::ok()){
//         data_collect_mutex_.lock();
//         data_collect_.push_back(i);
//         data_collect_mutex_.unlock();
//         i += 1;
//         ROS_INFO("this is data collect, i = %d", data_collect_.back());
// //        for_each(data_collect_.begin(), data_collect_.end(), [](size_t i){cout << i << endl;});

// //        publishTransform();
// //        r.sleep();
//     }
// }