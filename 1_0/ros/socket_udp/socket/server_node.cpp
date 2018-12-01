//#include "socket/server.h"

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>

#include <thread>
#include <mutex>
#include <chrono>
#include <iostream>
#include <string>
#include <algorithm>

#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <nav_msgs/OccupancyGrid.h>
#include <nav_msgs/Odometry.h>
#include <nav_msgs/Path.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/Twist.h>

#include <proto_msgs.LaserScan.h>
#include <proto_msgs.OccpancyGrid.h>
#include <proto_msgs.Odometry.h>
#include <proto_msgs.TFMessage.h>
#include <proto_msgs.TFStaticMessage.h>
#include <proto_msgs.Path.h>
#include <proto_msgs.Twist.h>

using namespace std;

//Server s;

const char serv_ip[] = "127.0.0.1";
int serv_recv_port = 8000;
int serv_send_port = 8001;

string collection, distribution;
mutex mtx_dc, mtx_dd;
unsigned short state_laser = 0;



void serverRecv() {
    cout << "server_recv_test" << endl;
    struct sockaddr_in serv_addr, clie_addr;
    socklen_t clie_addr_len;
    int sockfd;
    char recv[BUFSIZ];
    char str[INET_ADDRSTRLEN];
    int i, n;
 
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
 
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(serv_recv_port);
 
    bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    while (1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        bzero(&recv, sizeof(recv));
        clie_addr_len = sizeof(clie_addr);
        n = recvfrom(sockfd, recv, BUFSIZ, 0, (struct sockaddr *) &clie_addr, &clie_addr_len);
        if (n == -1)
            perror("recvfrom error");
        else{
            string s(recv, n);
            mtx_dd.lock();
            distribution.append(s, 0, n);
            mtx_dd.unlock();
            printf("recv size = %d, distriburtion size = %d \n", static_cast<int>(s.size()), static_cast<int>(distribution.size()));
        }

   }
   close(sockfd);
}

void serverSend(string& coll, std::mutex& mtx_dc) {

    cout << "serv_send_test" << endl;
    const int SENDSIZE = 1400;
    struct sockaddr_in servaddr;
    int sockfd, n;
    char send[BUFSIZ];
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, serv_ip, &servaddr.sin_addr);
    servaddr.sin_port = htons(serv_send_port);

    while(1) {
        if(collection.length() > 0) {
            int send_size = std::min(static_cast<int>(collection.size()), SENDSIZE);
            n = sendto(sockfd, collection.data(), send_size, 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
            if (n == -1)
                perror("sendto error");

            printf("send size: %d, collection size: %d \n", n, static_cast<int>(collection.size()));

            // std::lock_guard<std::mutex> lk(mtx_dc);
            mtx_dc.lock();
            collection.erase(0, n);
            mtx_dc.unlock();

            cout << "============================" << endl;
        }
    }
    close(sockfd);
}

void package(string& type_name, string& content) {
    std::string buf;
//    cout << "package type size: " << type.size() << endl;
//    cout << "package content size: " << content.size() << endl;
    unsigned head,end,str_data_size;
    int name_len,total_len,be32;
    head = 0x55aa;
    str_data_size = content.size();
    name_len = static_cast<int>(type_name.size() + 1);
    end = 0xaa55;
    total_len = str_data_size + name_len + 14;

    be32 = htonl(head);
    buf.append("$START");
    be32 = htonl(name_len);
    buf.append(reinterpret_cast<char *>(&be32),sizeof(be32));
    be32 = htonl(total_len);
    buf.append(reinterpret_cast<char *>(&be32),sizeof(be32));
    buf.append(type_name.c_str(),name_len);
    buf.append(content);



    mtx_dc.lock();
    collection.append(buf);
    mtx_dc.unlock();

    printf("total_len = %d; collection size = %d \n",total_len, static_cast<int>(collection.size()));

}


void lidarCallBack(const sensor_msgs::LaserScan &msg)
{
    // if(!state_laser)
    //     return;

    cout << "state_laser :"  << state_laser << endl;
    if(state_laser){
        cout << "lidar callback" << endl;
        LS_SLAM::LSLaserScan t(msg);
        t.proto2str();
        string type_name = t.proto_Lidar_.GetTypeName();
        string content = t.str;

        package(type_name, content);
    }
   
}

void mapCallBack(const nav_msgs::OccupancyGrid &msg)
{
    LS_SLAM::LSOccupancyGrid t(msg);
    t.proto2str();

    string type_name = t.proto_map_.GetTypeName();
    string content = t.str;
    package(type_name, content);

}

void odomCallBack(const nav_msgs::Odometry &msg)
{
    LS_SLAM::LSOdometry t(msg);
    t.proto2str();

    string type_name = t.proto_odom_.GetTypeName();
    string content = t.str;
    package(type_name, content);
}

void tfCallBack(const tf2_msgs::TFMessage &msg)
{
    LS_SLAM::TFMessage t(msg);
    t.proto2str();

    string type_name = t.proto_tf_.GetTypeName();
    string content = t.str;
    package(type_name, content);
}

void tfStaticCallBack(const tf2_msgs::TFMessage &msg)
{
    LS_SLAM::TFStaticMessage t(msg);
    t.proto2str();

    string type_name = t.proto_tf_.GetTypeName();
    string content = t.str;
    package(type_name, content);

}

void pathCallBack(const nav_msgs::Path &msg)
{
    LS_SLAM::LSPath t(msg);
    t.proto2str();

    string type_name = t.proto_path_.GetTypeName();
    string content = t.str;
    package(type_name, content);

}

void twistCallBack(const geometry_msgs::Twist &msg)
{
    LS_SLAM::LSTwist t(msg);
    t.proto2str();

    string type_name = t.proto_vel_.GetTypeName();
    string content = t.str;
    package(type_name, content);

}

void dataProduce() {

    ros::NodeHandle nh;
    cout << "data prodece" << endl << endl;

    
    // ros::Subscriber lidar_sub = nh.subscribe("/scan",10,lidarCallBack);
    ros::Subscriber map_sub = nh.subscribe("/map",1,mapCallBack);
    // ros::Subscriber odom_sub = nh.subscribe("/odom",10,odomCallBack);
    // ros::Subscriber tf_sub = nh.subscribe("/tf",10,tfCallBack);
    // ros::Subscriber tf_static_sub = nh.subscribe("/tf_static",10,tfStaticCallBack);
    // ros::Subscriber path_sub = nh.subscribe("/move_base/NavfnROS/plan",10,pathCallBack);
    // ros::Subscriber twist_sub = nh.subscribe("/cmd_vel",10,twistCallBack);

    ros::spin();
}

void dataConsume() {
     while(1) {
        if(distribution.size() > 0) {
            state_laser = distribution.at(0);
            printf("state_laser = %d  \n", static_cast<int>(state_laser));
            mtx_dd.lock();
            cout << "begin size: " << distribution.size() << endl;
            distribution.erase(distribution.begin());
            cout << "after size: " << distribution.size() << endl;
            mtx_dd.unlock();
        }
     }
}


int main(int argc, char** argv){
    ros::init(argc, argv, "server");
    ros::NodeHandle nh;

    thread datap([&]{dataProduce();});
    thread send([&]{serverSend(collection, mtx_dc);});
    

    thread recv([&]{serverRecv();});
    thread datac([&]{dataConsume();});
 
    recv.join();
    datac.join();

    datap.join();
    send.join();

    return 0;
}

