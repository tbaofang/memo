#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>
#include <algorithm>
#include <boost/thread.hpp>

#include "datashare.h"

using namespace std;

class ClientUdpSend{
public:
    ClientUdpSend();
    ~ClientUdpSend();

    void start(DataShare* datashare);

private:
    DataShare* ds_;

    boost::thread* data_send_thread_;
    void runThread();

    size_t send_size_;

};

ClientUdpSend::ClientUdpSend():send_size_(5*1024),
                               data_send_thread_(NULL)
{

}

ClientUdpSend::~ClientUdpSend() {
    if(data_send_thread_){
        data_send_thread_->join();
        delete data_send_thread_;
    }
}

void ClientUdpSend::start(DataShare *datashare) {
    ds_ = datashare;
    data_send_thread_ = new boost::thread([&]{runThread();});
}

void ClientUdpSend::runThread() {
    struct sockaddr_in targetAddr;
    bzero(&targetAddr, sizeof(targetAddr));
    targetAddr.sin_family = AF_INET;
    targetAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    targetAddr.sin_port = htons(8001);
    int targetAddrLen = sizeof(targetAddr);

    int sockdf = socket(AF_INET, SOCK_DGRAM, 0);

    int n = 0;
    int send_size;
    string buf;
    while (ros::ok())
    {
        if (ds_->client_data_package_.size() > 0)
        {
            ds_->client_data_package_mtx_.lock();
            buf.assign(ds_->client_data_package_);
            ds_->client_data_package_.clear();
            ds_->client_data_package_mtx_.unlock();
            while (!buf.empty())
            {
                send_size = std::min(static_cast<int>(buf.size()), static_cast<int>(send_size_));
                n = sendto(sockdf, buf.data(), send_size, 0, (struct sockaddr *)&targetAddr, sizeof(targetAddr));
                if (n == -1)
                    perror("sendto error");
                cout << "send size: " <<  n << endl;
                buf.erase(0, n);
            }

            cout << "----------------" << endl;
        }
    }

    close(sockdf);
}