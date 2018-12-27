#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <boost/thread.hpp>
#include <ctime>
#include <vector>

#include <pcl/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/ModelCoefficients.h>
#include <pcl/point_types.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
//#include <pcl/octree/octree_pointcloud_changedetector.h>
#include <pcl/octree/octree.h>
#include <pcl/octree/octree_search.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/surface/mls.h>
#include <pcl/surface/gp3.h>
#include <pcl/features/normal_3d.h>
#include <pcl/visualization/pcl_visualizer.h>


#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <std_msgs/String.h>

using namespace std;
using namespace ros;
using namespace pcl;

class CloudPublish{
public:
    CloudPublish();
    ~CloudPublish();

private:

//    boost::thread* cloudpub_thread_;
//    boost::thread* cloudfilter_thread_;
//    boost::thread* pcl_write_thread_;
//
//    boost::thread* planar_segmentation_thread_;
//    boost::thread* octree_change_detection_thread_;

//    boost::thread* moving_least_squares_thread_;
//    boost::thread* greedy_projection_thread_;
//    boost::thread* kdtree_thread_;
    boost::thread* kdtree_search_thread_;

    bool kdtree_search();
    bool kdtree();
    bool greedyProjection();


    bool cloudPub();
    bool cloudFilter();
    void cloudC(const sensor_msgs::PointCloud2& input);
    bool pclWrite();
    void pclWriteC(const sensor_msgs::PointCloud2& input);

    bool createPointCloud(pcl::PointCloud<pcl::PointXYZ>::Ptr cloudPtr, size_t width, size_t height);
    bool planarSegmentation();
    bool octreeChangeDetection();

    bool movingLeastSquares();



protected:
    ros::NodeHandle nh_;
    ros::Publisher pcl_pub_;
    ros::Publisher pcl_filter_pub_;
    ros::Subscriber pcl_sub_;
    ros::Subscriber pcl_write_sub_;
    ros::Publisher pcl_read_pub_;

    ros::Publisher com_pub_;
    ros::Publisher com2_pub_;
    ros::Publisher handle_pub_;

};

