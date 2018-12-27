#include "client.udpsend.h"


ClientUdpSend::ClientUdpSend():data_send_thread_(NULL)
{
    if(!nh_.getParam("/client_node/targetIP", targetIP_))
        targetIP_ = "127.0.0.1";
    if(!nh_.getParam("/client_node/targetPort", targetPort_))
        targetPort_ = 8002;
    if(!nh_.getParam("/client_node/sendSize", sendSize_))
        sendSize_ = 5;
    send_size_ = sendSize_*1024;
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
    targetAddr.sin_addr.s_addr = inet_addr(targetIP_.c_str());
    targetAddr.sin_port = htons(targetPort_);
    int targetAddrLen = sizeof(targetAddr);

    int sockdf = socket(AF_INET, SOCK_DGRAM, 0);

    int n = 0;
    int send_size;
    string buf;
    while (ros::ok())
    {
        if (ds_->client_data_package_.size() > 0 )
        {
            size_t total_len = stoi(ds_->client_data_package_.substr(8, 8));
            ROS_ERROR("package size: %d; total_len: %d",ds_->client_data_package_.size(), total_len);
            ds_->client_data_package_mtx_.lock();
            buf.append(ds_->client_data_package_, 0, total_len);
            ds_->client_data_package_.erase(0, total_len);
            ds_->client_data_package_mtx_.unlock();
            size_t i = 0;
            while (!buf.empty())
            { 
                buf.insert(0,"LS");
                send_size = std::min(static_cast<int>(buf.size()), static_cast<int>(send_size_));
                n = sendto(sockdf, buf.data(), send_size, 0, (struct sockaddr *)&targetAddr, sizeof(targetAddr));
                if (n == -1)
                    perror("sendto error");
                // cout << "send size: " <<  n << endl;
                buf.erase(0, n);
            }

            // cout << "----------------" << endl;
        }
    }

    close(sockdf);
}