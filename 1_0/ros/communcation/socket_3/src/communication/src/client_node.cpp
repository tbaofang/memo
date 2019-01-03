#include "datashare.h"
#include "../include/client.datapackage.h"
#include "client.dataparse.h"
#include "client.udprecv.h"
#include "client.udpsend.h"

#include <iostream>
int main(int argc, char** argv){
    ros::init(argc, argv, "client_node");

    DataShare* ds = DataShare::getInstance();

    ClientDataPackage  data_package;
    ClientUdpSend      udp_send;
    ClientUdpRecv      udp_recv;
    ClientDataParse    data_parse;

    data_package.start(ds);
    udp_send.start(ds);
    udp_recv.start(ds);
    data_parse.start(ds);

//    ros::NodeHandle nh_;
//    ros::Rate loop_rate(10.0);
//    while(ros::ok()){
//        ds->client_data_package_mtx_.lock();
//        cout << "ds size : " << ds->client_data_package_.size() <<  endl;
//        ds->client_data_package_mtx_.unlock();
//        loop_rate.sleep();
//    }

//    ros::spin();


}