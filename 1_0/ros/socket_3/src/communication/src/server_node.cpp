#include "datashare.h"
#include "server.datapackage.h"
#include "server.dataparse.h"
#include "server.udpsend.h"
#include "server.udprecv.h"


#include <iostream>

int main(int argc, char** argv){
    ros::init(argc, argv, "server_node");

    DataShare* ds = DataShare::getInstance();

    ServerDataPackage  data_package;
    ServerUdpSend      udp_send;
    ServerUdpRecv      udp_recv;
    ServerDataParse    data_parse;

    data_package.start(ds);
    udp_send.start(ds);
    udp_recv.start(ds);
    data_parse.start(ds);


//    ros::NodeHandle nh;
//    ros::Rate loop_rate(10.0);
//    while(ros::ok()){
//        ds->server_data_package_mtx_.lock();
//        cout << "ds size : " << ds->server_data_package_.size() <<  endl;
//        ds->server_data_package_mtx_.unlock();
//        loop_rate.sleep();
//    }
//    ros::spin();

}