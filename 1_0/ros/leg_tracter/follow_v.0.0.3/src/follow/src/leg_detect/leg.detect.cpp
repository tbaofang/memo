#include "leg.detect.h"

#include <visualization_msgs/Marker.h>
//#include <visualization_msgs/Marker.h>


LegDetect::LegDetect(){
    scan_sub_ = nh_.subscribe("/scan", 10, &LegDetect::scanCb, this);
    leg_detected_pub_ = nh_.advertise<follow::LegArray>("/detected_leg_clusters", 10);
    markers_pub_ = nh_.advertise<visualization_msgs::Marker>("/markers", 10);

//    forest_file = "./src/follow/config/trained_leg_detector_res=0.33.yaml";
//    forest_file = "./src/follow/config/training_0108.yaml";
    if(!nh_.getParam("leg_detect_node/forest_file", forest_file)){
        ROS_ERROR("not get forest_file!!!");
    }
    cout << "forest_file : " << forest_file << endl;
    max_detect_distance_ = 10.0;
    detection_threshold_ = 0.5;
    num_prev_markers_published_ = 0;

    forest = cv::ml::RTrees::create();
    forest = cv::ml::StatModel::load<cv::ml::RTrees>(forest_file);
//
    feature_count_ = forest->getVarCount();

}
LegDetect::~LegDetect(){

}

void LegDetect::scanCb(const sensor_msgs::LaserScan::ConstPtr& scan){
    dpp.preprocess(scan);
    dpp.featureExtract();

    legDistinguish();

}

void LegDetect::legDistinguish(){
    CvMat* tmp_mat = cvCreateMat(1, feature_count_, CV_32FC1);

    follow::LegArray detected_leg_clusters;
    detected_leg_clusters.header.frame_id = dpp.scan_.header.frame_id;
    detected_leg_clusters.header.stamp = dpp.scan_.header.stamp;

    for(auto spl:dpp.spll_){
        double x_mean = 0.0;
        double y_mean = 0.0;
        for(auto sp:spl){
            x_mean += sp.x;
            y_mean += sp.y;
        }
        x_mean /= spl.size();
        y_mean /= spl.size();
        tf::Point point{x_mean, y_mean, 0.0};
        tf::Stamped<tf::Point> position(point, ros::Time(0), dpp.scan_.header.frame_id);
        double rel_dist = sqrt(pow(position[0], 2) + pow(position[1], 2));

        auto cf = dpp.cfl_.front();
        dpp.cfl_.pop_front();
        if(rel_dist < max_detect_distance_){
            tmp_mat->data.fl[0] = static_cast<float>(cf.num_points_1);
            tmp_mat->data.fl[1] = static_cast<float>(cf.std_2);
            tmp_mat->data.fl[2] = static_cast<float>(cf.avg_median_dev_3);
            tmp_mat->data.fl[3] = static_cast<float>(cf.width_4);
            tmp_mat->data.fl[4] = static_cast<float>(cf.linearity_5);
            tmp_mat->data.fl[5] = static_cast<float>(cf.circularity_6);
            tmp_mat->data.fl[6] = static_cast<float>(cf.radius_7);
            tmp_mat->data.fl[7] = static_cast<float>(cf.boundary_length_8);
            tmp_mat->data.fl[8] = static_cast<float>(cf.boundary_regularity_9);
            tmp_mat->data.fl[9] = static_cast<float>(cf.mean_curvature_10);
            tmp_mat->data.fl[10] = static_cast<float>(cf.ang_diff_11);
            tmp_mat->data.fl[11] = static_cast<float>(cf.iav_12);
            tmp_mat->data.fl[12] = static_cast<float>(cf.std_iav_13);
            tmp_mat->data.fl[13] = static_cast<float>(cf.distance_14);
            tmp_mat->data.fl[14] = static_cast<float>(cf.distance_mean_15);
            tmp_mat->data.fl[15] = static_cast<float>(cf.occluded_right_16);
            tmp_mat->data.fl[16] = static_cast<float>(cf.occluded_left_17);

            cv::Mat result;
            forest->getVotes(cv::cvarrToMat(tmp_mat), result, 0);
            int positive_votes = result.at<int>(1, 1);
            int negative_votes = result.at<int>(1, 0);
            double probability_of_leg = positive_votes / static_cast<double>(positive_votes + negative_votes);
            cout << "probability_of_leg: " << probability_of_leg << endl;
            if (probability_of_leg > detection_threshold_){
                follow::Leg new_leg;
                new_leg.position.x = position[0];
                new_leg.position.y = position[1];
                new_leg.confidence = probability_of_leg;
//                leg_list.push_back(new_leg);
                detected_leg_clusters.legs.push_back(new_leg);
            }
        }
    }

    cout << "leg_list size: " << detected_leg_clusters.legs.size() << endl;
    leg_detected_pub_.publish(detected_leg_clusters);
    cout << detected_leg_clusters.header.stamp << endl;
    cvReleaseMat(&tmp_mat);

    showMarkers(detected_leg_clusters);
}

void LegDetect::showMarkers(follow::LegArray& leg_array){
    cout << leg_array.legs.size() << endl;

    static size_t num_prev_markers_published = 0;
    size_t  id_num = 0;
    for(auto leg:leg_array.legs){
//        cout << leg.position.x << endl;
        visualization_msgs::Marker m;
        m.header.stamp = dpp.scan_.header.stamp;
        m.header.frame_id = "laser";
        m.ns = "legs";
        m.id = id_num++;
        m.type = m.SPHERE;
        m.pose.position.x = leg.position.x ;
        m.pose.position.y = leg.position.y;
        m.pose.position.z = 0.2;
        m.scale.x = 0.13;
        m.scale.y = 0.13;
        m.scale.z = 0.13;
        m.color.a = 1;
        m.color.r = 0;
        m.color.g = 1;
        m.color.b = 0;
//        m.color.g = leg.confidence;
//        m.color.b = leg.confidence;
        markers_pub_.publish(m);
    }

    for (int id_num_diff = num_prev_markers_published - id_num; id_num_diff > 0; --id_num_diff)
    {
        visualization_msgs::Marker m;
        m.header.stamp = dpp.scan_.header.stamp;
        m.header.frame_id = "laser";
        m.ns = "legs";
        m.id = id_num_diff + id_num;
        m.action = m.DELETE;
        markers_pub_.publish(m);
    }
    num_prev_markers_published = id_num;
}