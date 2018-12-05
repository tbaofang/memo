
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

#include "server.dataparse.h"

using namespace std;


class Server {
public:
    Server(string server_port);
    ~Server();
    int startServer();    


private:
    ros::NodeHandle node_;
    std::string server_port_;

    int dataRecv();
    int dataRecvBoost();

    void init();

    ServerDataParse* sdp_;

    int recv_size_;

    boost::thread* data_recv_thread_;
    boost::thread* data_parse_thread_;



};

Server::Server(string server_port):server_port_(server_port) {
    recv_size_ = 5*1024;
    init();

    data_recv_thread_ = NULL;
    data_parse_thread_ =NULL;

}

void Server::init() {
    sdp_ = new ServerDataParse();
    ROS_ASSERT(sdp_);
}

Server::~Server() {
    if(sdp_) delete sdp_;

    if(data_recv_thread_){
        data_recv_thread_->join();
        delete data_recv_thread_;
    }

    if(data_parse_thread_) {
        data_parse_thread_->join();
        delete data_parse_thread_;
    }
}

int Server::startServer() {
    data_recv_thread_ = new boost::thread([&]{dataRecv();});
    data_parse_thread_ = new boost::thread([&]{sdp_->dataParse();});
}

int Server::dataRecv() {
    struct sockaddr_in srvAddr;
    bzero(&srvAddr,sizeof(srvAddr));
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    srvAddr.sin_port = htons(atoi(server_port_.c_str()));
    int srvAddrLen = sizeof(srvAddr);

    int iSock = socket(AF_INET, SOCK_DGRAM, 0);  // udp
    int iRet = bind(iSock, (struct sockaddr *)&srvAddr, sizeof(srvAddr));

    struct sockaddr_in cliAddr;
    bzero(&cliAddr,sizeof(cliAddr));
    cliAddr.sin_family = AF_INET;
    int cliAddrLen = sizeof(cliAddr);
    char szBuf[recv_size_+1] = {0};
    int n = 0;

    string buffer;
    char header[7] = "$START";
    char header_scan[30] = "proto_msg.LaserScan";

    size_t head_n = 0;

    while(ros::ok())
    {

        bzero(&szBuf, sizeof(szBuf));
//        cout << "dfdf" << sizeof(szBuf) << endl;

        n = recvfrom(iSock, szBuf, recv_size_, 0, (struct sockaddr *)&cliAddr, (socklen_t*)&cliAddrLen);
        if (n == -1) perror("recvfrom error");
        cout << "recv_size=" << n << endl;

        cout << header << endl;

        if(!strncmp(reinterpret_cast<const char*>(&szBuf), reinterpret_cast<const char*>(&header), 6)) {
            if(++head_n > 2) {
                int name_len = static_cast<unsigned char>(buffer.at(6));
                string name_str = buffer.substr(14, name_len);
                cout << "head_len = " << name_len << endl;
                cout << "name = " << name_str << endl;

                if(!strcmp("proto_msg.LaserScan",name_str.data()) && sdp_->buf_scan_.size() < 3000000) {
                    sdp_->buf_scan_mtx_.lock();
                    sdp_->buf_scan_.append(buffer);
                    cout << "buf_scan_.size = " << sdp_->buf_scan_.size() << endl;
                    sdp_->buf_scan_mtx_.unlock();
                }
            }
            buffer.assign(szBuf, n);
            cout << "buffer size = " <<  buffer.size() << endl;
            cout << buffer.substr(0, 6) << endl;


        }else{
            buffer.append(szBuf, n);
            cout << "buffer size = " <<  buffer.size() << endl;

            cout << buffer.substr(12, 21) << endl;
        }



//        printf("%s\n", szBuf);
    }

    close(iSock);
    return 0;
}

int Server::dataRecvBoost() {
    boost::asio::io_service io_service;
    boost::asio::ip::udp::socket udp_socket(io_service);

    boost::asio::ip::udp::endpoint local_add(boost::asio::ip::address::from_string("127.0.0.1"), 9000);

    udp_socket.open(local_add.protocol());
    udp_socket.bind(local_add);

    char receive_str[1024] = { 0 };
    while (ros::ok())
    {
        boost::asio::ip::udp::endpoint  sendpoint;
        udp_socket.receive_from(boost::asio::buffer(receive_str, 1024), sendpoint);
        cout << "收到" << sendpoint.address().to_string() <<":"<< receive_str << endl;
        udp_socket.send_to(boost::asio::buffer("服务端返回success"), sendpoint);
        memset(receive_str, 0, 1024);

    }
}