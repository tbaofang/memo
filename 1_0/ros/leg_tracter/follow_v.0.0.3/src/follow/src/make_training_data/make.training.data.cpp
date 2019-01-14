#include "make.training.data.h"

#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseArray.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>

#include "data.preprocess.h"

MakeTrainingData::MakeTrainingData(){
    ros::NodeHandle nh_("~");

    if(!nh_.getParam("load_bag_file", load_bag_file_))
        ROS_ERROR("couldn't get load_bag_file!");
    if(!nh_.getParam("save_bag_file", save_bag_file_))
        ROS_ERROR("couldn't get save_bag_file!");
    if(!nh_.getParam("scan_topic", scan_topic_))
        ROS_ERROR("couldn't get scan_topic!");
    if(!nh_.getParam("laser_frame", laser_frame_))
        ROS_ERROR("couldn't get laser_frame!");

    if(!nh_.getParam("min_angle", min_angle_))
        ROS_ERROR("couldn't get max_angle!");
    if(!nh_.getParam("max_angle", max_angle_))
        ROS_ERROR("couldn't get max_angle!");
    if(!nh_.getParam("max_dist", max_dist_))
        ROS_ERROR("couldn't get max_dist!");

    cout << "save_bag_file: " << save_bag_file_ << endl;
    cout << "save_bag_file: " << load_bag_file_ << endl;

}
MakeTrainingData::~MakeTrainingData(){

}

void MakeTrainingData::making(){
    rosbag::Bag load_bag;
    rosbag::Bag save_bag;
    load_bag.open(load_bag_file_, rosbag::bagmode::Read);
    save_bag.open(save_bag_file_, rosbag::bagmode::Write);

    vector<string> topics;
    topics.push_back(scan_topic_);
    rosbag::View view(load_bag, rosbag::TopicQuery(topics));

    for(auto v:view){
        sensor_msgs::LaserScan::ConstPtr scan = v.instantiate<sensor_msgs::LaserScan>();
        if(scan == nullptr) continue;

        geometry_msgs::PoseArray leg_pose_arr;

        preprocess::DataPreprocess dp;
        dp.preprocess(scan);
        for(auto spl:dp.spll_){
            tf::Point cluster_position = dp.getPosition(spl);
            double angle = atan2(cluster_position[1], cluster_position[0]) * M_PI;
            double dist_abs = sqrt(pow(cluster_position[0], 2) + pow(cluster_position[1], 2));
            if(angle > min_angle_ and angle < max_angle_ and dist_abs < max_dist_){
                geometry_msgs::Pose leg_pose;
                leg_pose.position.x = cluster_position[0];
                leg_pose.position.y = cluster_position[1];
                leg_pose_arr.header.frame_id = scan->header.frame_id;
                leg_pose_arr.poses.push_back(leg_pose);
            }
        }
        if(!leg_pose_arr.poses.empty()){
            save_bag.write("/training_scan", ros::Time::now(), *scan);
            save_bag.write("/leg_cluster_positions", ros::Time::now(), leg_pose_arr);

            visualization_msgs::MarkerArray ma;
            for(size_t i = 0; i < leg_pose_arr.poses.size(); ++i){
                visualization_msgs::Marker m;
                m.header.frame_id = scan->header.frame_id;
                m.ns = "legs";
                m.id = i;
                m.type = m.SPHERE;
                m.pose.position.x = leg_pose_arr.poses[i].position.x;
                m.pose.position.y = leg_pose_arr.poses[i].position.x;
                m.pose.position.z = 0.1;
                m.scale.x = 0.2;
                m.scale.y = 0.2;
                m.scale.z = 0.2;
                m.color.r = 1.0;
                m.color.g = 0;
                m.color.b = 0;
                m.color.a = 1.0;
                m.lifetime = ros::Duration(0.2);
                ma.markers.push_back(m);
            }
            save_bag.write("/visualization_marker_array", ros::Time::now(), ma);
        }



    }

    load_bag.close();
    save_bag.close();



}