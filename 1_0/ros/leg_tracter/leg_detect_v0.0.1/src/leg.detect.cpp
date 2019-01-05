#include "leg.detect.h"

#include <visualization_msgs/Marker.h>

LegDetect::LegDetect(){
    cluster_distance_ = 0.03;
    per_cluster_min_points_ = 3;
    per_cluster_max_points_ = 20;
    prev_markers_published_num = 0;

    scan_sub_ = nh_.subscribe("/scan", 10, &LegDetect::scanCb, this);
    markers_pub_ = nh_.advertise<visualization_msgs::Marker>("/markers", 10);
}

LegDetect::~LegDetect(){

}

void LegDetect::scanCb(const sensor_msgs::LaserScan::ConstPtr& msg){

    scanPreprocess(msg);


    ros::Time tf_time = ros::Time(0);
    bool transform_available = tfl_.canTransform("odom", msg->header.frame_id, tf_time);

    int id_num = 1;

    for(auto spl:spll_) {
        double mean_x = 0;
        double mean_y = 0;
        for (auto sp:spl) {
            mean_x += sp.x;
            mean_y += sp.y;
//            cout << sp.index << " " << sp.x << " " << sp.y << " " << mean_x << " " << mean_y << endl;
        }
        mean_x /= spl.size();
        mean_y /= spl.size();

//        cout << "mean_x: " << mean_x << endl;
//        cout << "mean_x: " << mean_y << endl << endl;





        visualization_msgs::Marker m;
        m.header.stamp = msg->header.stamp;
        m.header.frame_id = "laser";
        m.ns = "LEGS";
        m.id = id_num++;
        m.type = m.SPHERE;
        m.pose.position.x = mean_x;
        m.pose.position.y = mean_y;
        m.pose.position.z = 0.2;
        m.scale.x = 0.13;
        m.scale.y = 0.13;
        m.scale.z = 0.13;
        m.color.a = 1;
        m.color.r = 1;
        m.color.g = 0;
        m.color.b = 0;
        markers_pub_.publish(m);


    }
    for(int id_diff = prev_markers_published_num - id_num; id_diff > 0; --id_diff){
        visualization_msgs::Marker m;
        m.header.stamp = msg->header.stamp;
        m.header.frame_id = "laser";
        m.ns = "LEGS";
        m.id = id_diff + id_num;
        m.action = m.DELETE;
        markers_pub_.publish(m);
    }
    prev_markers_published_num = id_num;
}

void LegDetect::scanPreprocess(const sensor_msgs::LaserScan::ConstPtr &msg) {
    ScanProcessed sp;
    list<ScanProcessed> spl;

    for(size_t i = 0; i < msg->ranges.size(); ++i){
        double range = msg->ranges[i];
        if(range >= msg->range_min and range <= msg->range_max){
            sp.index = i;
            sp.x = cos(msg->angle_min + i * msg->angle_increment) * range;
            sp.y = sin(msg->angle_min + i * msg->angle_increment) * range;

            spl.push_back(sp);
        }
    }

    list<ScanProcessed> spl2;
    list<list<ScanProcessed>> spll;

    auto it_spl = spl.begin();
    while(it_spl != spl.end()){
        spl2.clear();
        spl2.push_back(*it_spl);

        auto it_spl2 = it_spl;
        ++it_spl2;
        while(it_spl2 != spl.end()){
            double distance = sqrt(pow(((*it_spl).x - (*it_spl2).x), 2.0) + pow(((*it_spl).x - (*it_spl2).x), 2.0));
            if(distance > cluster_distance_) break;
            spl2.push_back(*it_spl2);
            ++it_spl;
            ++it_spl2;
        }
        spll.push_back(spl2);
        it_spl = it_spl2;
    }

    spll_.clear();
    auto it_spll = spll.begin();
    while(it_spll != spll.end()){
        size_t spl_size = (*it_spll).size();
        if(spl_size >= per_cluster_min_points_)
            spll_.push_back(*it_spll);
        ++it_spll;
    }

}


