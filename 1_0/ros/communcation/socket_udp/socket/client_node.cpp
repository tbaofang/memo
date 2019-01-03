#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>

#include <thread>
#include <mutex>
#include <chrono>
#include <iostream>
#include <algorithm>

// #include <boost/thread.hpp>

#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <nav_msgs/OccupancyGrid.h>
#include <nav_msgs/Odometry.h>
#include <nav_msgs/Path.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Twist.h>
#include <move_base_msgs/MoveBaseActionGoal.h>

#include <proto_msgs.LaserScan.h>
#include <proto_msgs.OccpancyGrid.h>
#include <proto_msgs.Odometry.h>
#include <proto_msgs.TFMessage.h>
#include <proto_msgs.TFStaticMessage.h>
#include <proto_msgs.Path.h>
#include <proto_msgs.MoveBaseActionGoal.h>
#include <proto_msgs.Twist.h>

using namespace std;



int serv_recv_port = 8000;
int serv_send_port = 8001;
const char serv_ip[] = "127.0.0.1";
std::string clie_ip = "127.0.0.1";


char test[BUFSIZ] = "hello";
std::mutex mtx;

string collection, distribution;
mutex mtx_dc, mtx_dd;





void clientSend() {

   struct sockaddr_in servaddr;
   int sockfd, n;
   const int SENDSIZE = 1400;
   char send[SENDSIZE];

   sockfd = socket(AF_INET, SOCK_DGRAM, 0);

   bzero(&servaddr, sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   inet_pton(AF_INET, serv_ip, &servaddr.sin_addr);
   servaddr.sin_port = htons(serv_recv_port);


   while(1) {
        if(collection.length() > 0) {
            int send_size = std::min(static_cast<int>(collection.size()), SENDSIZE);
            n = sendto(sockfd, collection.data(), send_size, 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
            if (n == -1)
                perror("sendto error");
            cout << "send size: " << n << endl;
            
            mtx_dc.lock();
            cout << "collection before size : " << collection.size() << endl;
            collection.erase(0, n);
            cout << "collection after size: " << collection.size() << endl;
            mtx_dc.unlock();
           
        }
   }



}

void clientRecv() {


    struct sockaddr_in serv_addr, clie_addr;
    socklen_t clie_addr_len;
    int sockfd;
    char buf[BUFSIZ] = "server test";
    char recv[BUFSIZ];
    char str[INET_ADDRSTRLEN];
    int i, n;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(serv_send_port);

    bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    while (1) {
        bzero(&recv, sizeof(recv));
        clie_addr_len = sizeof(clie_addr);
        n = recvfrom(sockfd, recv, BUFSIZ, 0, (struct sockaddr *) &clie_addr, &clie_addr_len);
        if (n == -1)
            perror("recvfrom error");
        else{
            string s(recv, n);

            // std::lock_guard<std::mutex> lk(mtx_dd);
            // distribution.append(s, 0, n);
            mtx_dd.lock();
            distribution.append(s, 0, n);
            mtx_dd.unlock();
            printf("s size: %d", s.size());
            printf("recv size = %d, distriburtion size = %d \n", n, static_cast<int>(distribution.size()));
            // if(distribution.size() > 1500000) {
            //     mtx_dd.lock();
            //     distribution.erase(0, 4000);
            //     mtx_dd.unlock();
            // }
                
        }
    }
    close(sockfd);
}

void dataProduce() {
    cout << "data Produce" << endl << endl;

    // unsigned char command[] = {0x1, 0x1};
    // collection.assign(command, command+sizeof(command));

    
    while(1) {
        string command = {0x1, 0x1};
        mtx_dc.lock();
        collection.append(command);
        mtx_dc.unlock();

        std::this_thread::sleep_for(chrono::seconds(5));
        command = {0x0, 0x0};
        mtx_dc.lock();
        collection.append(command);
        mtx_dc.unlock();
        
        std::this_thread::sleep_for(chrono::seconds(5));

        // for_each(collection.begin(), collection.end(), [](char c){printf("%x ", c);}); cout << endl;

    }
    

}

void string2LaserScan(std::string& content, ros::Publisher& lidar_pub) {
    sensor_msgs::LaserScan l;
    LS_SLAM::LSLaserScan t(l);
    t.str = content;
    t.str2proto();

    sensor_msgs::LaserScan ll;
    ros::Time time(t.proto_Lidar_.publish_stamp());
    ll.header.stamp = time;
    ll.header.frame_id = t.proto_Lidar_.frame_id();
    ll.range_min = t.proto_Lidar_.range_min();
    ll.range_max = t.proto_Lidar_.range_max();
    ll.angle_min = t.proto_Lidar_.angle_min();
    ll.angle_max = t.proto_Lidar_.angle_max();
    ll.scan_time = t.proto_Lidar_.scan_time();
    ll.time_increment = t.proto_Lidar_.time_increment();
    ll.angle_increment = t.proto_Lidar_.angle_increment();

    std::vector<double> ttttt;
    for(int i = 0;i < t.proto_Lidar_.ranges_size();i ++)
    {
        ttttt.push_back(t.proto_Lidar_.ranges(i));
    }
    ll.ranges.assign(ttttt.begin(),ttttt.end());
    ttttt.clear();

    for(int i = 0;i < t.proto_Lidar_.intensitys_size();i ++)
    {
        ttttt.push_back(t.proto_Lidar_.intensitys(i));
    }
    ll.intensities.assign(ttttt.begin(),ttttt.end());
    ttttt.clear();

    lidar_pub.publish(ll);
    cout << "public lidar" << endl;
}

void string2OccupancyGrid(std::string& content, ros::Publisher& map_pub_) {
    nav_msgs::OccupancyGrid l;
    LS_SLAM::LSOccupancyGrid t(l);
    t.str = content;
    t.str2proto();

    nav_msgs::OccupancyGrid map;
    ros::Time time1(t.proto_map_.publish_stamp());
    map.header.stamp = time1;
    map.header.frame_id = t.proto_map_.frame_id();

    ros::Time time2(t.proto_map_.load_time());
    map.info.map_load_time = time2;
    map.info.resolution = t.proto_map_.resolution();
    map.info.width = t.proto_map_.width();
    map.info.height = t.proto_map_.height();

    map.info.origin.position.x = t.proto_map_.x();
    map.info.origin.position.y = t.proto_map_.y();
    map.info.origin.position.z = t.proto_map_.z();
    tf::Quaternion qua;
    qua.setRPY(t.proto_map_.roll(),t.proto_map_.pitch(),t.proto_map_.yaw());
    map.info.origin.orientation.x = qua.x();
    map.info.origin.orientation.y = qua.y();
    map.info.origin.orientation.z = qua.z();
    map.info.origin.orientation.w = qua.w();

    std::string m = t.proto_map_.map_data();
    map.data.assign(m.begin(),m.end());

    map_pub_.publish(map);
    cout << "public map" << endl;
}

void string2Odometry(std::string& content, ros::Publisher& odom_pub) {
    nav_msgs::Odometry l;
    LS_SLAM::LSOdometry t(l);
    t.str = content;
    t.str2proto();

    nav_msgs::Odometry odom;
    ros::Time time(t.proto_odom_.publish_stamp());
    odom.header.stamp = time;
    odom.child_frame_id = t.proto_odom_.child_frame_id();
    odom.header.frame_id = t.proto_odom_.frame_id();
    odom.pose.pose.position.x = t.proto_odom_.pose_x();
    odom.pose.pose.position.y = t.proto_odom_.pose_y();
    odom.pose.pose.position.z = t.proto_odom_.pose_z();
    tf::Quaternion qua;
    qua.setRPY(t.proto_odom_.pose_roll(),t.proto_odom_.pose_pitch(),t.proto_odom_.pose_yaw());
    odom.pose.pose.orientation.x = qua.x();
    odom.pose.pose.orientation.y = qua.y();
    odom.pose.pose.orientation.z = qua.z();
    odom.pose.pose.orientation.w = qua.w();
    for(int i = 0;i < t.proto_odom_.pose_cova_size();i ++)
    {
        odom.pose.covariance.assign(t.proto_odom_.pose_cova(i));
    }

    odom.twist.twist.linear.x = t.proto_odom_.twist_x();
    odom.twist.twist.linear.y = t.proto_odom_.twist_y();
    odom.twist.twist.linear.z = t.proto_odom_.twist_z();
    odom.twist.twist.angular.x = t.proto_odom_.twist_roll();
    odom.twist.twist.angular.y = t.proto_odom_.twist_pitch();
    odom.twist.twist.angular.z = t.proto_odom_.twist_yaw();
    for(int i = 0;i < t.proto_odom_.twist_cova_size();i ++)
    {
        odom.twist.covariance.assign(t.proto_odom_.twist_cova(i));
    }

    odom_pub.publish(odom);
    cout << "public odom" << endl;
}

void string2TFMessage(std::string& content, ros::Publisher& tf_pub) {
    tf2_msgs::TFMessage o;
    LS_SLAM::TFMessage t(o);
    t.str = content;
    t.str2proto();

    tf2_msgs::TFMessage tf;
    geometry_msgs::TransformStamped tt;
    for(int i = 0;i < t.proto_tf_.tfs_size();i ++)
    {
        ros::Time time(t.proto_tf_.tfs(i).publish_stamp());
        tt.header.stamp = time;
        tt.header.frame_id = t.proto_tf_.tfs(i).frame_id();
        tt.child_frame_id = t.proto_tf_.tfs(i).child_frame_id();
        tt.transform.translation.x = t.proto_tf_.tfs(i).x();
        tt.transform.translation.y = t.proto_tf_.tfs(i).y();
        tt.transform.translation.z = t.proto_tf_.tfs(i).z();
        tf::Quaternion qua;
        qua.setRPY(t.proto_tf_.tfs(i).roll(),t.proto_tf_.tfs(i).pitch(),t.proto_tf_.tfs(i).yaw());
        tt.transform.rotation.x = qua.x();
        tt.transform.rotation.y = qua.y();
        tt.transform.rotation.z = qua.z();
        tt.transform.rotation.w = qua.w();

        tf.transforms.push_back(tt);
    }

    tf_pub.publish(tf);
    cout << "public tf" << endl;
}

void string2TFStaticMessage(std::string& content, ros::Publisher& tf_static_pub) {
    tf2_msgs::TFMessage o;
    LS_SLAM::TFStaticMessage t(o);
    t.str = content;
    t.str2proto();

    tf2_msgs::TFMessage tf;
    geometry_msgs::TransformStamped tt;
    for(int i = 0;i < t.proto_tf_.tfs_size();i ++)
    {
        ros::Time time(t.proto_tf_.tfs(i).publish_stamp());
        tt.header.stamp = time;
        tt.header.frame_id = t.proto_tf_.tfs(i).frame_id();
        tt.child_frame_id = t.proto_tf_.tfs(i).child_frame_id();
        tt.transform.translation.x = t.proto_tf_.tfs(i).x();
        tt.transform.translation.y = t.proto_tf_.tfs(i).y();
        tt.transform.translation.z = t.proto_tf_.tfs(i).z();
        tf::Quaternion qua;
        qua.setRPY(t.proto_tf_.tfs(i).roll(),t.proto_tf_.tfs(i).pitch(),t.proto_tf_.tfs(i).yaw());
        tt.transform.rotation.x = qua.x();
        tt.transform.rotation.y = qua.y();
        tt.transform.rotation.z = qua.z();
        tt.transform.rotation.w = qua.w();

        tf.transforms.push_back(tt);
    }

    tf_static_pub.publish(tf);
    cout << "public static tf" << endl;
}

void string2Path(std::string& content, ros::Publisher& path_pub) {
    nav_msgs::Path pp;
    LS_SLAM::LSPath t(pp);
    t.str = content;
    t.str2proto();

    nav_msgs::Path p;
    geometry_msgs::PoseStamped ps;
    ros::Time time(t.proto_path_.publish_stamp());
    p.header.stamp = time;
    p.header.frame_id = t.proto_path_.frame_id();

    for(int i = 0;i < t.proto_path_.poses_size();i ++)
    {
        ros::Time time(t.proto_path_.poses(i).publish_stamp());
        ps.header.stamp = time;
        ps.header.frame_id = t.proto_path_.poses(i).frame_id();
        ps.pose.position.x = t.proto_path_.poses(i).x();
        ps.pose.position.y = t.proto_path_.poses(i).y();
        ps.pose.position.z = t.proto_path_.poses(i).z();
        tf::Quaternion qua;
        qua.setRPY(t.proto_path_.poses(i).roll(),t.proto_path_.poses(i).pitch(),t.proto_path_.poses(i).yaw());
        ps.pose.orientation.x = qua.x();
        ps.pose.orientation.y = qua.y();
        ps.pose.orientation.z = qua.z();
        ps.pose.orientation.w = qua.w();

        p.poses.push_back(ps);
    }

    path_pub.publish(p);
    cout << "public path" << endl;
}

void string2Twist(std::string& content, ros::Publisher& twist_pub) {
    geometry_msgs::Twist o;
    LS_SLAM::LSTwist t(o);
    t.str = content;
    t.str2proto();

    geometry_msgs::Twist cmd_vel;
    cmd_vel.linear.x = t.proto_vel_.x();
    cmd_vel.linear.y = t.proto_vel_.y();
    cmd_vel.linear.z = t.proto_vel_.z();
    cmd_vel.angular.x = t.proto_vel_.roll();
    cmd_vel.angular.y = t.proto_vel_.pitch();
    cmd_vel.angular.z = t.proto_vel_.yaw();
    twist_pub.publish(cmd_vel);
    cout << "public twist" << endl;
}

void dataConsume() {
    ros::NodeHandle nh;
    ros::Publisher lidar_pub = nh.advertise<sensor_msgs::LaserScan>("/protobuf_test_lidar",10);
    ros::Publisher map_pub_ = nh.advertise<nav_msgs::OccupancyGrid>("/protobuf_test_map",1);
    ros::Publisher odom_pub = nh.advertise<nav_msgs::Odometry>("/protobuf_test_odom",10);
    ros::Publisher tf_pub = nh.advertise<tf2_msgs::TFMessage>("/protobuf_test_tf",10);
    ros::Publisher tf_static_pub = nh.advertise<tf2_msgs::TFMessage>("/protobuf_test_tf_static",10);
    ros::Publisher path_pub = nh.advertise<nav_msgs::Path>("/protobuf_test_path",10);
    ros::Publisher twist_pub = nh.advertise<geometry_msgs::Twist>("/protobuf_test_twist",10);
//    ros::Rate loop_rate(1000.0);

    while(ros::ok()) {
        size_t dist_size = distribution.size();
        if(dist_size > 1000 ) {

            cout << "distribute size: " << distribution.size() << endl;
            string::iterator it = distribution.begin();


            size_t start = distribution.find("$START", 0);
            if(start != string::npos) {

                cout <<"start: " <<  start << endl << endl;
                int name_len = (static_cast<unsigned char>(distribution.at(start+6)) << 24) |(static_cast<unsigned char>(distribution.at(start+7)) << 16) |(static_cast<unsigned char>(distribution.at(start+8)) << 8) | (static_cast<unsigned char>(distribution.at(start+9)));
                int content_len = (static_cast<unsigned char>(distribution.at(start+10)) << 24) |(static_cast<unsigned char>(distribution.at(start+11)) << 16) |(static_cast<unsigned char>(distribution.at(start+12)) << 8) | (static_cast<unsigned char>(distribution.at(start+13)));
                cout << "name_len : " << name_len << endl;
                cout << "content len : " << content_len << endl;
                
                // for_each(it+14, it+14+name_len, [](char c){printf("%c", c);}); cout << endl;

                size_t end = distribution.find("$START", start+1);
               

                if(dist_size < content_len+start) {
                    // if(end != string::npos) {
                    //     mtx_dd.lock();
                    //     distribution.erase(0,end);
                    //     mtx_dd.unlock();
                    // }
                    std::this_thread::sleep_for(chrono::milliseconds(10));
                    continue;
                }else {

                    // size_t end = distribution.find("$START", start+1);

                    string name_str = distribution.substr(start+14, name_len);
                    string content = distribution.substr(start+name_len+14, content_len-name_len-14);
                    for_each(name_str.begin(), name_str.end(), [](char c){printf("%c", c);}); cout << endl;

                    if(!strcmp("proto_msg.LaserScan",name_str.data()))
                        string2LaserScan(content, lidar_pub);
                    if(!strcmp("proto_msg.OccupancyGrid",name_str.data()))
                        string2OccupancyGrid(content, map_pub_);
                    // if(!strcmp("proto_msg.Odometry",name_str.data()))
                    //     string2Odometry(content, odom_pub);
                    // if(!strcmp("proto_msg.TFMessage",name_str.data()))
                    //     string2TFMessage(content, tf_pub);
                    // if(!strcmp("proto_msg.TFStaticMessage",name_str.data()))
                    //     string2TFStaticMessage(content, tf_static_pub);
                    // if(!strcmp("proto_msg.Path",name_str.data()))
                    //     string2Path(content, path_pub); 
                    // if(!strcmp("proto_msg.Twist",name_str.data()))
                    //     string2Twist(content, twist_pub);

                    // if(!strcmp("proto_msg.LaserScan",name_str.data())) {

                    //     cout << "proto_msg.laserscan start ..............." << endl;
                    //         sensor_msgs::LaserScan l;
                    //         LS_SLAM::LSLaserScan t(l);
                    //         t.str = content;
                    //         t.str2proto();

                    //         sensor_msgs::LaserScan ll;
                    //         ros::Time time(t.proto_Lidar_.publish_stamp());
                    //         ll.header.stamp = time;
                    //         ll.header.frame_id = t.proto_Lidar_.frame_id();
                    //         ll.range_min = t.proto_Lidar_.range_min();
                    //         ll.range_max = t.proto_Lidar_.range_max();
                    //         ll.angle_min = t.proto_Lidar_.angle_min();
                    //         ll.angle_max = t.proto_Lidar_.angle_max();
                    //         ll.scan_time = t.proto_Lidar_.scan_time();
                    //         ll.time_increment = t.proto_Lidar_.time_increment();
                    //         ll.angle_increment = t.proto_Lidar_.angle_increment();

                    //         std::vector<double> ttttt;
                    //         for(int i = 0;i < t.proto_Lidar_.ranges_size();i ++)
                    //         {
                    //             ttttt.push_back(t.proto_Lidar_.ranges(i));
                    //         }
                    //         ll.ranges.assign(ttttt.begin(),ttttt.end());
                    //         ttttt.clear();

                    //         for(int i = 0;i < t.proto_Lidar_.intensitys_size();i ++)
                    //         {
                    //             ttttt.push_back(t.proto_Lidar_.intensitys(i));
                    //         }
                    //         ll.intensities.assign(ttttt.begin(),ttttt.end());
                    //         ttttt.clear();

                    //         lidar_pub.publish(ll);
                    //         cout << "public lidar" << endl;
                    // }


                    // if(!strcmp("proto_msg.OccupancyGrid",name_str.data())) {
                    //     cout << "proto_msg.OccupancyGrid start................" << endl;
                    //     nav_msgs::OccupancyGrid l;
                    //     LS_SLAM::LSOccupancyGrid t(l);
                    //     t.str = content;
                    //     t.str2proto();

                    //     nav_msgs::OccupancyGrid map;
                    //     ros::Time time1(t.proto_map_.publish_stamp());
                    //     map.header.stamp = time1;
                    //     map.header.frame_id = t.proto_map_.frame_id();

                    //     ros::Time time2(t.proto_map_.load_time());
                    //     map.info.map_load_time = time2;
                    //     map.info.resolution = t.proto_map_.resolution();
                    //     map.info.width = t.proto_map_.width();
                    //     map.info.height = t.proto_map_.height();

                    //     map.info.origin.position.x = t.proto_map_.x();
                    //     map.info.origin.position.y = t.proto_map_.y();
                    //     map.info.origin.position.z = t.proto_map_.z();
                    //     tf::Quaternion qua;
                    //     qua.setRPY(t.proto_map_.roll(),t.proto_map_.pitch(),t.proto_map_.yaw());
                    //     map.info.origin.orientation.x = qua.x();
                    //     map.info.origin.orientation.y = qua.y();
                    //     map.info.origin.orientation.z = qua.z();
                    //     map.info.origin.orientation.w = qua.w();

                    //     std::string m = t.proto_map_.map_data();
                    //     map.data.assign(m.begin(),m.end());

                    //     map_pub_.publish(map);
                    //     cout << "public map" << endl;
                    // }

                    mtx_dd.lock();
                    distribution.erase(0,end);
                    mtx_dd.unlock();

                }
            }

            cout << "=========== consume ===========" << endl;

            ros::spinOnce();
            // loop_rate.sleep();

        }
    }
}




int main(int argc, char** argv) {
    ros::init(argc, argv, "client");
//    ros::NodeHandle nh;

    thread recv(clientRecv);
    thread datac([&]{dataConsume();});

    thread datap([&]{dataProduce();});
    thread send(clientSend);
    
    recv.join();
    datac.join();
    send.join();
    datap.join();



    return 0;

}