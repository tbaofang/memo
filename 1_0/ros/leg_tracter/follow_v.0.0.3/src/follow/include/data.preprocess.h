#pragma once

#include <iostream>
#include <list>

#include <opencv2/core/core.hpp>
#include <opencv2/ml.hpp>

#include <sensor_msgs/LaserScan.h>
#include <tf/transform_datatypes.h>

using namespace std;

namespace preprocess{
class DataPreprocess{
public:
    DataPreprocess();
    ~DataPreprocess();

    struct ScanProcessed{
        size_t index{0};
        double x{0};
        double y{0};
        double range{0};
    };

    struct ClusterFeature{
        size_t num_points_1{0};
        double std_2{0};
        double avg_median_dev_3{0};
        double width_4{0};
        double linearity_5{0};
        double circularity_6{0};
        double radius_7{0};
        double boundary_length_8{0};
        double boundary_regularity_9{0};
        double mean_curvature_10{0};
        double ang_diff_11{0};
        double iav_12{0};
        double std_iav_13{0};
        double distance_14{0};
        double distance_mean_15{0};
        double occluded_right_16{1};
        double occluded_left_17{1};
    };

    list<list<ScanProcessed>> spll_;
    list<ClusterFeature> cfl_;
    sensor_msgs::LaserScan scan_;


    void preprocess(const sensor_msgs::LaserScan::ConstPtr& scan);
    void featureExtract();
    void printMat(CvMat* A);

    tf::Point getPosition(list<ScanProcessed>& spl);



private:
    double cluster_distance_;
    size_t  per_cluster_min_points_;
    size_t  per_cluster_max_points_;





protected:
};
};