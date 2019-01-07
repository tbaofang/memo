#include "leg.detect.h"

#include <vector>


#include <visualization_msgs/Marker.h>

#include "leg_detcet/Leg.h"
#include "leg_detcet/LegArray.h"

LegDetect::LegDetect(){
    cluster_distance_ = 0.3;
    per_cluster_min_points_ = 3;
    per_cluster_max_points_ = 20;
    prev_markers_published_num = 0;

    fixed_frame_ = "laser";

    scan_sub_ = nh_.subscribe("/scan", 10, &LegDetect::scanCb, this);
    markers_pub_ = nh_.advertise<visualization_msgs::Marker>("/markers", 10);
    detected_leg_clusters_pub_ = nh_.advertise<leg_detcet::LegArray>("detected_leg_clusters", 10);

    forest_file = "./src/leg_detcet/config/trained_leg_detector_res=0.33.yaml";
    max_detect_distance_ = 10.0;
    detection_threshold_ = 0.15;
    num_prev_markers_published_ = 0;
    cout << forest_file << endl;

    forest = cv::ml::RTrees::create();
    forest = cv::ml::StatModel::load<cv::ml::RTrees>(forest_file);
    feature_count_ = forest->getVarCount();
    cout << "feature_count_: " << feature_count_ << endl;
}

LegDetect::~LegDetect(){

}

void LegDetect::scanCb(const sensor_msgs::LaserScan::ConstPtr& scan){

    scanPreprocess(scan);
    clusterFeatureExtract(*scan);

    CvMat* tmp_mat = cvCreateMat(1, feature_count_, CV_32FC1);

    leg_detcet::LegArray detected_leg_clusters;
    detected_leg_clusters.header.frame_id = scan->header.frame_id;
    detected_leg_clusters.header.stamp = scan->header.stamp;

//    list<leg_detcet::Leg> leg_list;

    cout << endl;
    for(auto spl:spll_){

        double x_mean = 0.0;
        double y_mean = 0.0;
        for(auto sp:spl){
            x_mean += sp.x;
            y_mean += sp.y;
        }
        x_mean /= spl.size();
        y_mean /= spl.size();
        tf::Point point{x_mean, y_mean, 0.0};
//        cout << point.x() << endl;

        tf::Stamped<tf::Point> position(point, ros::Time(0), scan->header.frame_id);
        double rel_dist = sqrt(pow(position[0], 2) + pow(position[1], 2));

        auto cf = cfl_.front();
        cfl_.pop_front();
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
                leg_detcet::Leg new_leg;
                new_leg.position.x = position[0];
                new_leg.position.y = position[1];
                new_leg.confidence = probability_of_leg;
//                leg_list.push_back(new_leg);
                detected_leg_clusters.legs.push_back(new_leg);
            }
        }
    }

    cout << "leg_list size: " << detected_leg_clusters.legs.size() << endl;

    size_t  id_num = 0;
    for(auto leg:detected_leg_clusters.legs){
//        cout << leg.position.x << endl;
        visualization_msgs::Marker m;
        m.header.stamp = scan->header.stamp;
        m.header.frame_id = fixed_frame_;
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

    for (int id_num_diff = num_prev_markers_published_-id_num; id_num_diff > 0; id_num_diff--)
    {
        visualization_msgs::Marker m;
        m.header.stamp = scan->header.stamp;
        m.header.frame_id = fixed_frame_;
        m.ns = "legs";
        m.id = id_num_diff + id_num;
        m.action = m.DELETE;
        markers_pub_.publish(m);
    }
    num_prev_markers_published_ = id_num; // For the next callback

    detected_leg_clusters_pub_.publish(detected_leg_clusters);
    cvReleaseMat(&tmp_mat);



//    ros::Time tf_time = ros::Time(0);
//    bool transform_available = tfl_.canTransform(fixed_frame_, msg->header.frame_id, tf_time);
//    if(transform_available){
//        cout << "Not publishing detected leg clusters because no tf was available" << endl;
//    }
//
//    for(auto spl:spll_){
//        double mean_x = 0;
//        double mean_y = 0;
//        for (auto sp:spl) {
//            mean_x += sp.x;
//            mean_y += sp.y;
////            cout << sp.index << " " << sp.x << " " << sp.y << " " << mean_x << " " << mean_y << endl;
//        }
//        mean_x /= spl.size();
//        mean_y /= spl.size();
//
//        tf::Point point(mean_x, mean_y, 0);
//        tf::Stamped<tf::Point> position(point, ros::Time(0), msg->header.frame_id);
//        tfl_.transformPoint(fixed_frame_, position, position);
//    }

//    tf::Point point;



//    int id_num = 1;
//
//    for(auto spl:spll_) {
//        double mean_x = 0;
//        double mean_y = 0;
//        for (auto sp:spl) {
//            mean_x += sp.x;
//            mean_y += sp.y;
//            cout << sp.index << " " << sp.x << " " << sp.y << " " << mean_x << " " << mean_y << endl;
//        }
//        mean_x /= spl.size();
//        mean_y /= spl.size();

//        cout << "mean_x: " << mean_x << endl;
//        cout << "mean_x: " << mean_y << endl << endl;







//        visualization_msgs::Marker m;
//        m.header.stamp = msg->header.stamp;
//        m.header.frame_id = fixed_frame_;
//        m.ns = "LEGS";
//        m.id = id_num++;
//        m.type = m.SPHERE;
//        m.pose.position.x = mean_x;
//        m.pose.position.y = mean_y;
//        m.pose.position.z = 0.2;
//        m.scale.x = 0.13;
//        m.scale.y = 0.13;
//        m.scale.z = 0.13;
//        m.color.a = 1;
//        m.color.r = 1;
//        m.color.g = 0;
//        m.color.b = 0;
//        markers_pub_.publish(m);




//    }
//    for(int id_diff = prev_markers_published_num - id_num; id_diff > 0; --id_diff){
//        visualization_msgs::Marker m;
//        m.header.stamp = msg->header.stamp;
//        m.header.frame_id = fixed_frame_;
//        m.ns = "LEGS";
//        m.id = id_diff + id_num;
//        m.action = m.DELETE;
//        markers_pub_.publish(m);
//    }
//    prev_markers_published_num = id_num;
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
            sp.range = range;

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

void LegDetect::printMat(CvMat* A)
{
    for(int i=0; i<A->rows; i++)
    {
        for(int j=0; j<A->cols; j++)
            cout << cvmGet(A, i, j) << ' ';
        cout << endl;
    }
    cout << endl ;
}


void LegDetect::clusterFeatureExtract(const sensor_msgs::LaserScan& scan) {
    cfl_.clear();

    auto spl = spll_.begin();
    while(spl != spll_.end()){
//    for(auto spl:spll_){
        ClusterFeature cf;

        cf.num_points_1 = (*spl).size();

        double x_mean = 0.0;
        double y_mean = 0.0;
        std::vector<double> x_median_set;
        std::vector<double> y_median_set;
        for(auto sp:*spl){
            x_mean += sp.x;
            y_mean += sp.y;
            x_median_set.push_back(sp.x);
            y_median_set.push_back(sp.y);
        }
        x_mean /= cf.num_points_1;
        y_mean /= cf.num_points_1;
//        cout << "x_mean: " << x_mean << endl;
//        cout << "y_mean: " << y_mean << endl;

        sort(x_median_set.begin(), x_median_set.end());
        sort(y_median_set.begin(), y_median_set.end());
        double x_median = 0.5 * (*(x_median_set.begin() + (cf.num_points_1 -1) / 2) + *(x_median_set.begin() + cf.num_points_1 / 2));
        double y_median = 0.5 * (*(y_median_set.begin() + (cf.num_points_1 -1) / 2) + *(y_median_set.begin() + cf.num_points_1 / 2));
//        cout << x_median << endl;
//        cout << y_median << endl;

        cf.distance_14 = sqrt(pow(x_median, 2) + pow(y_median, 2));
        cf.distance_mean_15 = cf.distance_14 / cf.num_points_1;


        double std_diff = 0.0;
        double med_diff = 0.0;
        for(auto sp:*spl){
            std_diff += pow(sp.x - x_mean, 2) + pow(sp.y - y_mean, 2);
            med_diff += pow(sp.x - x_median, 2) + pow(sp.y - y_median, 2);
        }

        cf.std_2 = sqrt(std_diff / (cf.num_points_1 - 1));
        cf.avg_median_dev_3 = sqrt(med_diff) / cf.num_points_1;

        auto first = (*spl).begin();
        auto last = (*spl).end();
        size_t prev_ind = (*first).index - 1;
        size_t next_ind = (*(--last)).index + 1;
        if (prev_ind >= 0) {
            double range = scan.ranges[prev_ind];
            if (range >= scan.range_min and range <= scan.range_max) {
                if ((*first).range < range or range < 0.01)
                    cf.occluded_left_17 = 0;
            }
        }
        if(next_ind < scan.ranges.size()){
            double range = scan.ranges[next_ind];
            if (range >= scan.range_min and range <= scan.range_max) {
                if ((*last).range < range or range < 0.01)
                    cf.occluded_right_16 = 0;
            }
        }
        cf.width_4 = sqrt(pow( (*first).x - (*last).x, 2) + pow((*first).y - (*last).y, 2));

        CvMat* points = cvCreateMat(cf.num_points_1, 2, CV_64FC1);
        {
            size_t i = 0;
            for (auto sp:*spl) {
                cvmSet(points, i, 0, sp.x - x_mean);
                cvmSet(points, i, 1, sp.y - y_mean);
                ++i;
            }
        }

        CvMat* W = cvCreateMat(2, 2, CV_64FC1);
        CvMat* U = cvCreateMat(cf.num_points_1, 2, CV_64FC1);
        CvMat* V = cvCreateMat(2, 2, CV_64FC1);
        cvSVD(points, W, U, V);


        CvMat* rot_points = cvCreateMat(cf.num_points_1, 2, CV_64FC1);
        cvMatMul(U, W, rot_points);

//        printMat(points);
//        cout << "W: " << endl;
//        printMat(W);
//        cout << "U: " << endl;
//        printMat(U);
//        cout << "V: " << endl;
//        printMat(V);
//        cout << "rot_points: " << endl;
//        printMat(rot_points);

        for (int i = 0; i < cf.num_points_1; i++)
        {
            cf.linearity_5 += pow(cvmGet(rot_points, i, 1), 2);
        }

        cvReleaseMat(&points);
        cvReleaseMat(&W);
        cvReleaseMat(&U);
        cvReleaseMat(&V);
        cvReleaseMat(&rot_points);

        CvMat* A = cvCreateMat(cf.num_points_1, 3, CV_64FC1);
        CvMat* B = cvCreateMat(cf.num_points_1, 1, CV_64FC1);
        {
            size_t i = 0;
            for (auto sp:*spl) {
                cvmSet(A, i, 0, -2.0 * sp.x);
                cvmSet(A, i, 1, -2.0 * sp.y);
                cvmSet(A, i, 2, 1);
                cvmSet(B, i, 0, -pow(sp.x, 2) - pow(sp.y, 2));
                ++i;
            }
        }
        CvMat* sol = cvCreateMat(3, 1, CV_64FC1);
        cvSolve(A, B, sol, CV_SVD);

        double xc = cvmGet(sol, 0, 0);
        double yc = cvmGet(sol, 1, 0);
        cf.radius_7 = sqrt(pow(xc, 2) + pow(yc, 2) - cvmGet(sol, 2, 0));

        cvReleaseMat(&A);
        cvReleaseMat(&B);
        cvReleaseMat(&sol);

        for (auto sp:*spl)
        {
            cf.circularity_6 += pow(xc - sqrt(pow(xc - sp.x, 2) + pow(yc - sp.y, 2)), 2);
        }

        double sum_boundary_reg_sq = 0.0;
        double last_boundary_seg = 0.0;

        auto left = spl->begin();
        left++;
        left++;
        auto mid = spl->begin();
        mid++;
        auto right = spl->begin();
        while(left != spl->end()){
            double mlx = (*left).x - (*mid).x;
            double mly = (*left).y - (*mid).y;
            double L_ml = sqrt(mlx * mlx + mly * mly);

            double mrx = (*right).x - (*mid).x;
            double mry = (*right).y - (*mid).y;
            double L_mr = sqrt(mrx * mrx + mry * mry);

            double lrx = (*left).x - (*right).x;
            double lry = (*left).y - (*right).y;
            double L_lr = sqrt(lrx * lrx + lry * lry);

            cf.boundary_length_8 += L_mr;
            sum_boundary_reg_sq += L_mr * L_mr;
            last_boundary_seg += L_ml;

            double A = (mlx * mrx + mly * mry) / pow(L_mr, 2);
            double B = (mlx * mrx - mly * mry) / pow(L_mr, 2);
            double th = atan2(B, A);

            if (th < 0)
                th += 2 * M_PI;

            cf.ang_diff_11 += th /cf.num_points_1;

            double s = 0.5 * (L_ml + L_mr + L_lr);
            double area = sqrt(s * (s - L_ml) * (s - L_mr) * (s - L_lr));

            if (th > 0)
                cf.mean_curvature_10 += 4 * (area) / (L_ml * L_mr * L_lr * cf.num_points_1);
            else
                cf.mean_curvature_10 -= 4 * (area) / (L_ml * L_mr * L_lr * cf.num_points_1);

            left++;
            mid++;
            right++;
        }

        cf.boundary_length_8 += last_boundary_seg;
        sum_boundary_reg_sq += last_boundary_seg * last_boundary_seg;
        cf.boundary_regularity_9 = sqrt((sum_boundary_reg_sq - pow(cf.boundary_length_8, 2) / cf.boundary_length_8) / (cf.boundary_length_8 - 1));



        double sum_iav = 0.0;
        double sum_iav_sq = 0.0;
        first = spl->begin();
        mid = spl->begin();
        mid++;
        last = spl->end();
        last--;
        while (mid != last){
            double mlx = (*first).x - (*mid).x;
            double mly = (*first).y - (*mid).y;

            double mrx = (*last).x - (*mid).x;
            double mry = (*last).y - (*mid).y;
            double L_mr = sqrt(mrx * mrx + mry * mry);


            double A = (mlx * mrx + mly * mry) / pow(L_mr, 2);
            double B = (mlx * mry - mly * mrx) / pow(L_mr, 2);

            double th = atan2(B, A);
            if (th < 0)
                th += 2 * M_PI;
            sum_iav += th;
            sum_iav_sq += th * th;
            ++mid;
        }
        cf.iav_12 = sum_iav / cf.num_points_1;
        cf.std_iav_13 = sqrt((sum_iav_sq - pow(sum_iav, 2) / cf.num_points_1) / (cf.num_points_1 - 1));


//        cout << "======== cf: ===========" << endl;
//        cout << "num_points_1: " << cf.num_points_1 << endl;
//        cout << "std_2:" << cf.std_2 << endl;
//        cout << "avg_median_dev_3: " << cf.avg_median_dev_3 << endl;
//        cout << "width_4: " << cf.width_4 << endl;
//        cout << "linearity_5: " << cf.linearity_5 << endl;
//        cout << "circularity_6: " << cf.circularity_6 << endl;
//        cout << "radius_7: " << cf.radius_7 << endl;
//        cout << "boundary_length_8: " << cf.boundary_length_8 << endl;
//        cout << "boundary_regularity_9: " << cf.boundary_regularity_9 << endl;
//        cout << "mean_curvature_10: " << cf.mean_curvature_10 << endl;
//        cout << "ang_diff_11: " << cf.ang_diff_11 << endl;
//        cout << "iav_12: " << cf.iav_12 << endl;
//        cout << "std_iav_13: " << cf.std_iav_13 << endl;
//        cout << "distance_14: " << cf.distance_14 << endl;
//        cout << "distance_mean_15: " << cf.distance_mean_15 << endl;
//        cout << "occluded_right_16: " << cf.occluded_right_16 << endl;
//        cout << "occluded_left_17: " << cf.occluded_left_17 << endl;
//        cout << endl;
        cfl_.push_back(cf);
        ++spl;
    }
}
