#include "local.mapping.h"

#include <tf/transform_listener.h>
//#include <tf/transform_broadcaster.h>

#define ALPHA 0.2
#define BETA 0.1
#define OBSTACLE 0.7
#define FREE_SPACE 0.4
#define UNKNOWN 0.5
#define MIN_PROB 0.001
#define MAX_PROB 1-MIN_PROB



LocalMapping::LocalMapping():
        scan_sub_(nh_, "scan", 100),
//        non_leg_clusters_sub_(nh_, "non_leg_clusters", 100),
        non_leg_clusters_sub_(nh_, "detected_leg_clusters", 100),
        sync_(scan_sub_, non_leg_clusters_sub_, 100)
{
    scan_topic_ = "/scan";
    fixed_frame_ = "/laser";
    base_frame_ = "/base_link";

    grid_centre_pos_found_ = false;
    shift_threshold_ = 1.0;
    width_ = 400;
    resolution_ = 0.05;
    reliable_inf_range_ = 5.0;
    invalid_measurements_are_free_space_ = false;
    unseen_is_freespace_ = true;

    l0_ = logit(UNKNOWN);
    l_min_ = logit(MIN_PROB);
    l_max_ = logit(MAX_PROB);
    l_.resize(width_*width_);
    for (int i = 0; i < width_; i++)
    {
        for (int j = 0; j < width_; j++)
        {
            if (unseen_is_freespace_)
                l_[i + width_*j] = l_min_;
            else
                l_[i + width_*j] = l0_;
        }
    }

    sync_.registerCallback(std::bind(&LocalMapping::laserAndLegCb, this, std::placeholders::_1, std::placeholders::_2));
    map_pub_ = nh_.advertise<nav_msgs::OccupancyGrid>("/local_map", 10);

    ros::spin();
}

LocalMapping::~LocalMapping(){

}

void LocalMapping::laserAndLegCb(const sensor_msgs::LaserScan::ConstPtr& scan, const follow::LegArray::ConstPtr& non_leg){
    tf::TransformListener tfl;


//    try
//    {
//        tfl.waitForTransform(fixed_frame_, scan->header.frame_id, scan->header.stamp, ros::Duration(1.0));
//    }
//    catch(tf::TransformException ex)
//    {
//        ROS_INFO("Local map: No tf available");
//    }

    ros::Time tf_time = ros::Time(0);

    bool transform_available = tfl.canTransform(fixed_frame_, scan->header.frame_id, tf_time);
    if (!transform_available) {
        ROS_INFO("transform fail!!");
        return;
    }

    vector<tf::Point> non_legs;
//        cout << non_leg->legs.size() << endl;
//        cout << endl;
    for (int i = 0; i < non_leg->legs.size(); i++)
    {
        follow::Leg leg = non_leg->legs[i];
        tf::Point p;
        p[0] = leg.position.x;
        p[1] = leg.position.y;

        tf::Stamped<tf::Point> ps(p, tf_time, fixed_frame_);
        try
        {
            tfl.transformPoint(scan->header.frame_id, ps, ps);
            non_legs.push_back(tf::Point(ps[0],ps[1],0));
        }
        catch (tf::TransformException ex)
        {
            ROS_ERROR("Local map tf error: %s", ex.what());
        }

    }
    std::vector<bool> is_sample_human;
    is_sample_human.resize(scan->ranges.size(), false);
    sensor_msgs::LaserScan scan_msg = *scan;
    preprocess::DataPreprocess dp;
    dp.preprocess(scan);

    for(auto spl:dp.spll_){
        bool is_cluster_human = true;
        tf::Point c_pos = dp.getPosition(spl);
        for(auto non_leg = non_legs.begin(); non_leg != non_legs.end(); ++non_leg){
            double dist = sqrt(pow(c_pos[0] - (*non_leg)[0], 2) + pow(c_pos[1] - (*non_leg)[1], 2));
            if(dist < 0.1){
                non_legs.erase(non_leg);
                is_cluster_human = false;
                break;
            }
        }

        for(auto sp:spl){
    //            cout << sp.index << endl;
            is_sample_human[sp.index] = is_cluster_human;
        }
    }
    bool transform_succesful;
    geometry_msgs::PoseStamped init_pose;
    geometry_msgs::PoseStamped laser_pose_fixed_frame;
    init_pose.header.frame_id = scan->header.frame_id;
    init_pose.pose.orientation = tf::createQuaternionMsgFromYaw(0.0);
    init_pose.header.stamp = tf_time;
    try
    {
        tfl.transformPose(fixed_frame_, init_pose, laser_pose_fixed_frame);
        transform_succesful = true;
    }
    catch (tf::TransformException ex)
    {
        ROS_ERROR("Local map tf error: %s", ex.what());
        transform_succesful = false;
    }


    if (!transform_succesful){
        ROS_INFO("transfore fail!!!!!!!!!!!!!");
    }
    double laser_x = laser_pose_fixed_frame.pose.position.x;
    double laser_y = laser_pose_fixed_frame.pose.position.y;
    double laser_yaw = tf::getYaw(laser_pose_fixed_frame.pose.orientation);

    if(!grid_centre_pos_found_){
        grid_centre_pos_found_ = true;
        grid_centre_pos_x_ = laser_x;
        grid_centre_pos_y_ = laser_y;

    }

    if(sqrt(pow(grid_centre_pos_x_ - laser_x, 2) + pow(grid_centre_pos_y_ - laser_y, 2)) > shift_threshold_){
        cout << "distance super shift_threshold !!" << endl;
    }

    for(size_t i = 0; i < width_; ++i){
        for(size_t j = 0; j < width_; ++j) {
            double m_update;
            double dist = sqrt(pow(i * resolution_ + grid_centre_pos_x_ - (width_ / 2.0) * resolution_ - laser_x, 2.0) +
                               pow(j * resolution_ + grid_centre_pos_y_ - (width_ / 2.0) * resolution_ - laser_y, 2.0));
            double angle = betweenPIandNegPI(
                    atan2(j * resolution_ + grid_centre_pos_y_ - (width_ / 2.0) * resolution_ - laser_y,
                          i * resolution_ + grid_centre_pos_x_ - (width_ / 2.0) * resolution_ - laser_x) - laser_yaw);
            bool is_human;

//            cout << "dist: " << dist << endl;
//            cout << "angel: " << angle << endl;
            if (angle > scan->angle_min - scan->angle_increment / 2.0 and
                angle < scan->angle_max + scan->angle_increment / 2.0){

                double closest_beam_angle = round(angle / scan->angle_increment) * scan->angle_increment;
                int closest_beam_idx = (int)round(angle / scan->angle_increment) + scan->ranges.size()/2;
                is_human = is_sample_human[closest_beam_idx];

//                cout << "closest_beam_angle: " << closest_beam_angle << endl;
//                cout << "closest_beam_idx: " << closest_beam_idx << endl;
//                cout << "is_human: " << is_human << endl;
//                cout << "is_human: " << is_human << endl;

                bool valid_measurement;
                if(scan->range_min <= scan->ranges[closest_beam_idx] && scan->ranges[closest_beam_idx] <= scan->range_max)
                {
                    valid_measurement = true;
                }
                else if( !std::isfinite(scan->ranges[closest_beam_idx]) && scan->ranges[closest_beam_idx] < 0)
                {
                    valid_measurement = false;
                }
                else if( !std::isfinite(scan->ranges[closest_beam_idx] ) && scan->ranges[closest_beam_idx] > 0)
                {
                    valid_measurement = true;
                }
                else if( std::isnan(scan->ranges[closest_beam_idx]) )
                {
                    valid_measurement = false;
                }
                else
                {
                    valid_measurement = false;
                }

//                cout << "valid_measurement :"  << valid_measurement << endl;
                if (valid_measurement)
                {
                    double dist_rel = dist - scan->ranges[closest_beam_idx];
                    double angle_rel = angle - closest_beam_angle;
                    if (dist > scan->range_max
                        or dist > scan->ranges[closest_beam_idx] + ALPHA/2.0
                        or fabs(angle_rel) > BETA/2
                        or (!std::isfinite(scan->ranges[closest_beam_idx]) and dist > reliable_inf_range_))
                        m_update = UNKNOWN;
                    else if (scan->ranges[closest_beam_idx] < scan->range_max and fabs(dist_rel) < ALPHA / 2 and !is_human)
                        m_update = OBSTACLE;
                    else
                        m_update = FREE_SPACE;
                }
                else
                {
                    if (invalid_measurements_are_free_space_)
                        m_update = FREE_SPACE;
                    else
                        m_update = UNKNOWN;
                }
            }
            else
            {
                m_update = UNKNOWN;
            }

            l_[i + width_ * j] = (l_[i + width_ * j] + logit(m_update) - l0_);
            if (l_[i + width_ * j] < l_min_)
                l_[i + width_ * j] = l_min_;
            else if (l_[i + width_ * j] > l_max_)
                l_[i + width_ * j] = l_max_;



        }
    }
    nav_msgs::OccupancyGrid m_msg;
    m_msg.header.stamp = scan->header.stamp;
    m_msg.header.frame_id = fixed_frame_;
    m_msg.info.resolution = resolution_;
    m_msg.info.width = width_;
    m_msg.info.height = width_;
    m_msg.info.origin.position.x = grid_centre_pos_x_ - (width_/2.0)*resolution_;
    m_msg.info.origin.position.y = grid_centre_pos_y_ - (width_/2.0)*resolution_;
    for (int i = 0; i < width_; i++)
        for (int j = 0; j < width_; j++) {
            m_msg.data.push_back((int) (inverseLogit(l_[width_ * i + j]) * 100));
            cout << "l_[width_*i + j]: " << l_[width_*i + j] << endl;
        }

    map_pub_.publish(m_msg);

}

double LocalMapping::betweenPIandNegPI(double angle_in) {
    double between_0_and_2PI = fmod(angle_in, 2 * M_PI);
    if (between_0_and_2PI < M_PI)
        return between_0_and_2PI;
    else
        return between_0_and_2PI - 2 * M_PI;
}

double LocalMapping::logit(double p)
{
    return log(p / (1-p));
}

double LocalMapping::inverseLogit(double p)
{
    return exp(p) / (1+exp(p));
}
