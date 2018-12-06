#include "server.h"

//#include

#include <iostream>
#include <thread>

#include <ros/ros.h>

using namespace std;

int main(int argc, char** argv) {
    ros::init(argc, argv, "server_node");
    // ros::NodeHandle nh;
   Server serv;

   serv.startServer();
//
//    thread s([]{serv.server();});
//    if(s.joinable()) s.join();
    // ros::Rate loop_rate(10.0);

    // while(ros::ok()) {
    //     ROS_INFO("hello_server----------");
    //     ros::spinOnce();
    //     loop_rate.sleep();
    // }



//    serv.hello();
}