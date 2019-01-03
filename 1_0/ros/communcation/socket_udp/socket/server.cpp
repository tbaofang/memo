#include "socket/server.h"

#include <proto_msgs.LaserScan.h>
#include <proto_msgs.OccpancyGrid.h>
#include <proto_msgs.Odometry.h>
#include <proto_msgs.TFMessage.h>
#include <proto_msgs.TFStaticMessage.h>
#include <proto_msgs.Path.h>
#include <proto_msgs.Twist.h>
#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <nav_msgs/OccupancyGrid.h>
#include <nav_msgs/Odometry.h>
#include <nav_msgs/Path.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/Twist.h>

//static void  Server::printE(char elem)
//{
//    printf("%c", elem);
//}


void Server::mySigintHandler(int sig)
{
    //这里主要进行退出前的数据保存、内存清理、告知其他节点等工作
    ROS_INFO("shutting down!");
    ros::shutdown();
}


void Server::start() {
    recv.clear();


    ros::NodeHandle nh;
//    ros::Rate loop_rate(10.0);

    cout << "server thread" << endl;
    while(1) {
        ros::Subscriber lidar_sub = nh.subscribe("/scan",10,lidarCallBack);
    }
//    ros::Subscriber lidar_sub = nh.subscribe("/scan",10,LidarCallBack);
//    ros::Subscriber map_sub = nh.subscribe("/map",1,MapCallBack);
//    ros::Subscriber odom_sub = nh.subscribe("/odom",10,OdomCallBack);
//    ros::Subscriber tf_sub = nh.subscribe("/tf",10,TFCallBack);
//    ros::Subscriber tf_static_sub = nh.subscribe("/tf_static",10,TFStaticCallBack);
//    ros::Subscriber path_sub = nh.subscribe("/move_base/NavfnROS/plan",10,PathCallBack);
//    ros::Subscriber twist_sub = nh.subscribe("/cmd_vel",10,TwistCallBack);

//    ros::NodeHandle pnh("~");
//    pnh.param<std::string>("server_ip",server_ip,"127.0.0.1");
//    pnh.param("server_port",server_port,40166);
//    std::cout<<"ip:"<<server_ip<<std::endl<<"port:"<<server_port<<std::endl;

//    bzero(&server_addr, sizeof(server_addr));
//    server_addr.sin_family = AF_INET;
//    server_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());
//    server_addr.sin_port = htons(server_port);

//    client_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);/
//    if(client_socket_fd < 0)
//    {
//        perror("Create Socket Failed:");
//        exit(1);
//    }

//    signal(SIGINT, mySigintHandler);

//    while(ros::ok())
//    {
//        ros::spinOnce();
//        loop_rate.sleep();
//    }

//    exit();



    while(1){
        recv1();
        recv2();
    }

    ROS_INFO("Node exit");
    printf("Process exit\n");
    return;
}

void Server::lidarCallBack(const sensor_msgs::LaserScan &msg) {
    cout << "lidarCallBack" << endl;
}


void Server::recv1() {
    char tmp[] = "recv1,";
    mtx_recv.lock();
//    for(int i = 0; i < sizeof(tmp); ++i)
//        recv.push_front(tmp[i]);

    recv.insert(recv.end(), tmp, tmp+sizeof(tmp));


//    recv.push_front('r');
    mtx_recv.unlock();
}

void Server::recv2() {
    char tmp[] = "recv2,";
    mtx_recv.lock();
    recv.insert(recv.end(), tmp, tmp+sizeof(tmp));
    mtx_recv.unlock();
}