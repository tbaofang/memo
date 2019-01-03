#include "client.h"

#include <iostream>
#include <thread>

#include <ros/ros.h>

using namespace std;

int main(int argc, char** argv) {
    ros::init(argc, argv, "client_node");
    Client cli;

    cli.startClient();

//    thread c([&]{cli.startClient();});
//    if(c.joinable()) c.join();



//    ROS_INFO("clientt +++++++++++");

//    ros::spin();


//    cout << "client node" << endl;
}