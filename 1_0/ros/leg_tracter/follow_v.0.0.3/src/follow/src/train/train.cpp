#include "train.h"

Train::Train(){
    positive_leg_cluster_positions_topic_ = "/leg_cluster_positions";

    undersample_negative_factor_ = 50;
}
Train::~Train(){

}

void Train::loadData(int argc, char** argv){
    for(size_t i = 0; i < argc; ++i){
        if(!strcmp(argv[i],"--pos")){
            string rosbag_file = argv[++i];
            string scan_topic = argv[++i];
            loadPosData(rosbag_file, scan_topic, train_pos_data_);
        }
        if(!strcmp(argv[i], "--neg")){
            string rosbag_file = argv[++i];
            string scan_topic = argv[++i];
            loadNegData(rosbag_file, scan_topic, train_neg_data_);
        }
        else if(!strcmp(argv[i], "--save_file"))
            save_file_ = argv[++i];
    }
    cout << "save_file_: " << save_file_ << endl;
    if (save_file_.empty())
    {
        ROS_ERROR("Save file not specified properly in command line arguments \nExiting");
        exit(1);
    }

//    if (train_pos_data_.empty() or train_neg_data_.empty())
//    {
//        ROS_ERROR("Data not loaded from rosbags properly \nExiting");
//        exit(1);
//    }

    printf("\n  Total positive training samples: %i \t Total negative training samples: %i \n", (int)train_pos_data_.size(), (int)train_neg_data_.size());
    printf("  Total positive test samples: %i \t Total negative test samples: %i \n\n", (int)test_pos_data_.size(), (int)test_neg_data_.size());

}
void Train::loadPosData(const string rosbag_file, const string scan_topic, vector<vector<float> >& train_pos_data){
    rosbag::Bag bag;
    bag.open(rosbag_file, rosbag::bagmode::Read);
//    cout << "bag size: " << bag.getSize() << endl;
//    cout << "bag size: " << bag.getFileName() << endl;
//    cout << "bag size: " << bag.getMajorVersion() << endl;
//    cout << "bag size: " << bag.getMinorVersion() << endl;
//    cout << "bag size: " << bag.getMode() << endl;
    vector<string> topics;
    topics.push_back(scan_topic);
    topics.push_back(positive_leg_cluster_positions_topic_);

    cout << scan_topic << endl;
    cout << positive_leg_cluster_positions_topic_ << endl;

    rosbag::View view(bag, rosbag::TopicQuery(topics));
    geometry_msgs::PoseArray position_clusters;
    size_t message_num = 0;
    size_t initial_pos_data_size = train_pos_data.size();

    cout << endl << endl;
    cout << "view size: " << view.size() << endl;
    for(rosbag::MessageInstance const m: view){
        geometry_msgs::PoseArray::ConstPtr pose_array = m.instantiate<geometry_msgs::PoseArray>();
        if(pose_array != nullptr){
            position_clusters = *pose_array;
//            cout << "position_clusters size: " << position_clusters.poses.size() << endl;
        }
//        cout << "position_clusters size: " << position_clusters.poses.size() << endl;
        sensor_msgs::LaserScan::ConstPtr scan = m.instantiate<sensor_msgs::LaserScan>();
        if(scan != nullptr and position_clusters.poses.size()){
            preprocess::DataPreprocess dpp;
            dpp.preprocess(scan);
            dpp.featureExtract();

            for(auto spl:dpp.spll_){
                tf::Point cluster_position = dpp.getPosition(spl);

//                auto cf_it = dpp.cfl_.begin();
                auto cf = dpp.cfl_.front();
                dpp.cfl_.pop_front();
//                cout << "dpp.cfl_ size: " << dpp.cfl_.size()  << endl;
//                cout << "position_clusters size: " << position_clusters.poses.size() << endl;
                for(auto pose: position_clusters.poses){
                    double dist_x = pose.position.x - cluster_position[0];
                    double dist_y = pose.position.y - cluster_position[1];
                    double dist_abs = sqrt(dist_x * dist_x + dist_y * dist_y);
//                    std::cout << pose.position.x << " " << cluster_position[0] << endl;
//                    std::cout << pose.position.y << " " << cluster_position[1] << endl;
//                    std::cout << dist_x << " " << dist_y << " " << dist_abs << endl<< endl;
//                    cout << "dis_abs: " << dist_abs << endl;
                    vector<float> cf_v;
                    if(dist_abs < 0.001){
                        cf_v.push_back(static_cast<float>(cf.num_points_1));
                        cf_v.push_back(static_cast<float>(cf.std_2));
                        cf_v.push_back(static_cast<float>(cf.avg_median_dev_3));
                        cf_v.push_back(static_cast<float>(cf.width_4));
                        cf_v.push_back(static_cast<float>(cf.linearity_5));
                        cf_v.push_back(static_cast<float>(cf.circularity_6));
                        cf_v.push_back(static_cast<float>(cf.radius_7));
                        cf_v.push_back(static_cast<float>(cf.boundary_length_8));
                        cf_v.push_back(static_cast<float>(cf.boundary_regularity_9));
                        cf_v.push_back(static_cast<float>(cf.mean_curvature_10));
                        cf_v.push_back(static_cast<float>(cf.ang_diff_11));
                        cf_v.push_back(static_cast<float>(cf.iav_12));
                        cf_v.push_back(static_cast<float>(cf.std_iav_13));
                        cf_v.push_back(static_cast<float>(cf.distance_14));
                        cf_v.push_back(static_cast<float>(cf.distance_mean_15));
                        cf_v.push_back(static_cast<float>(cf.occluded_right_16));
                        cf_v.push_back(static_cast<float>(cf.occluded_left_17));
                        train_pos_data.push_back(cf_v);

//                        cout << "feature: " << endl;
//                        for(auto d:cf_v)
//                            cout << d << endl;
                        break;
                    }

                }
            }
            message_num++;
//            break;

        }
//            cout << "train_pos_data size: " << train_pos_data.size() << endl;


    }
    bag.close();
    printf("\t Got %i scan messages, %i samples, from %s  \n",(int)message_num, (int)train_pos_data.size() - (int)initial_pos_data_size, rosbag_file.data());
}

void Train::loadNegData(const string rosbag_file, const string scan_topic, vector<vector<float> >& train_pos_data) {
    rosbag::Bag bag;
    bag.open(rosbag_file, rosbag::bagmode::Read);

    vector<string> topics;
    topics.push_back(scan_topic);
    rosbag::View view(bag, rosbag::TopicQuery(topics));

    size_t message_num = 0;
    size_t initial_neg_data_size = 0;
    for(rosbag::MessageInstance const m:view){
        sensor_msgs::LaserScan::ConstPtr scan = m.instantiate<sensor_msgs::LaserScan>();
        if(scan != nullptr){
            preprocess::DataPreprocess dpp;
            dpp.preprocess(scan);
            dpp.featureExtract();
            for(auto spl:dpp.spll_){
//                tf::Point cluster_position = dpp.getPosition(spl);
                auto cf = dpp.cfl_.front();
                dpp.cfl_.pop_front();
                vector<float> cf_v;
                if(rand() % undersample_negative_factor_ == 0){
                    cf_v.push_back(static_cast<float>(cf.num_points_1));
                    cf_v.push_back(static_cast<float>(cf.std_2));
                    cf_v.push_back(static_cast<float>(cf.avg_median_dev_3));
                    cf_v.push_back(static_cast<float>(cf.width_4));
                    cf_v.push_back(static_cast<float>(cf.linearity_5));
                    cf_v.push_back(static_cast<float>(cf.circularity_6));
                    cf_v.push_back(static_cast<float>(cf.radius_7));
                    cf_v.push_back(static_cast<float>(cf.boundary_length_8));
                    cf_v.push_back(static_cast<float>(cf.boundary_regularity_9));
                    cf_v.push_back(static_cast<float>(cf.mean_curvature_10));
                    cf_v.push_back(static_cast<float>(cf.ang_diff_11));
                    cf_v.push_back(static_cast<float>(cf.iav_12));
                    cf_v.push_back(static_cast<float>(cf.std_iav_13));
                    cf_v.push_back(static_cast<float>(cf.distance_14));
                    cf_v.push_back(static_cast<float>(cf.distance_mean_15));
                    cf_v.push_back(static_cast<float>(cf.occluded_right_16));
                    cf_v.push_back(static_cast<float>(cf.occluded_left_17));
                    train_pos_data.push_back(cf_v);
                    break;
                }
            }
            message_num++;
        }
    }
    bag.close();
    printf("\t Got %i scan messages, %i samples, from %s  \n",(int)message_num, (int)train_pos_data.size() - (int)initial_neg_data_size, rosbag_file.data());
}

void Train::training(){
    size_t sample_size = train_pos_data_.size() + train_neg_data_.size();
    feature_count_ = train_pos_data_[0].size();
//    cout << "sample_size: " << sample_size << endl;
//    cout << "feature_count: " << feature_count_ << endl;

    CvMat* cv_data = cvCreateMat(sample_size, feature_count_, CV_32FC1);
    CvMat* cv_resp = cvCreateMat(sample_size, 1, CV_32S);

    size_t j = 0;
    for (auto i = train_pos_data_.begin(); i != train_pos_data_.end(); ++i){
        float* data_row = (float*)(cv_data->data.ptr + cv_data->step*j);
        for (int k = 0; k < feature_count_; k++)
            data_row[k] = (*i)[k];
        cv_resp->data.i[j] = 1;
        ++j;
    }

    for(auto i = train_neg_data_.begin(); i != train_neg_data_.end(); ++i){
        float* data_row = (float*)(cv_data->data.ptr + cv_data->step*j);
        for (int k = 0; k < feature_count_; k++)
            data_row[k] = (*i)[k];
        cv_resp->data.i[j] = 1;
        cv_resp->data.i[j] = -1;
        ++j;
    }

    CvMat* var_type = cvCreateMat(1, feature_count_ + 1, CV_8U);
    cvSet(var_type, cvScalarAll(cv::ml::VAR_ORDERED));
    cvSetReal1D(var_type, feature_count_, cv::ml::VAR_CATEGORICAL);

    float priors[] = {1.0, 1.0};
    cv::Mat priors_mat = cv::Mat(1, 2, CV_32F, priors);

    forest_->setMaxDepth(20);
    forest_->setMinSampleCount(2);
    forest_->setRegressionAccuracy(0);
    forest_->setUseSurrogates(false);
    forest_->setMaxCategories(1000);
    forest_->setPriors(priors_mat);
    forest_->setCalculateVarImportance(false);
    forest_->setActiveVarCount(2);
    int nTrees = 100;
    forest_->setRegressionAccuracy(0.001f);
    forest_->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, nTrees, 1e-6));

    preprocess::DataPreprocess dpp;
    cout <<  endl << "cv_resp: " << endl;
//    dpp.printMat(cv_resp);
    cout << "width: " << cv_data->width << endl;
    cout << "height: " << cv_data->height << endl;

    cout << "width: " << cv_resp->width << endl;
    cout << "height: " << cv_resp->height << endl;


    forest_->train(cv::ml::TrainData::create(
            cv::cvarrToMat(cv_data),
            cv::ml::ROW_SAMPLE,
            cv::cvarrToMat(cv_resp)
    ));

    cout << "save_file: " << save_file_ << endl;
    forest_->save(save_file_.c_str());

//    cout <<  endl << "cv_data: " << endl;
//    dpp.printMat(cv_data);



    cvReleaseMat(&cv_data);
    cvReleaseMat(&cv_resp);
    cvReleaseMat(&var_type);
}

void Train::testing(){
    test_pos_data_ = train_pos_data_;
    test_neg_data_ = train_neg_data_;

    int correct_pos = 0;
    int correct_neg = 0;

    CvMat* tmp_mat = cvCreateMat(1, feature_count_, CV_32FC1);
    vector<vector<float> >::const_iterator it = test_pos_data_.begin();
    for(; it != test_pos_data_.end(); ++it){
        for(size_t i = 0; i < feature_count_; ++i){
//            cout << (*it)[i] << endl;
            tmp_mat->data.fl[i] = static_cast<float>((*it)[i]);

//            for(size_t i = 0; i < tmp_mat->rows; ++i){
//                for(size_t j = 0; j < tmp_mat->cols; ++i){
//                    cout << cvmGet(tmp_mat, i, j) << " ";
//                }
//                cout << endl;
//            }
//            cout << endl;
        }
//        cout << "forest_->predict(cv::cvarrToMat(tmp_mat)) : " << forest_->predict(cv::cvarrToMat(tmp_mat)) << endl;
        if(forest_->predict(cv::cvarrToMat(tmp_mat)) > 0)
            correct_pos++;
    }
    vector<vector<float> >::const_iterator i = test_neg_data_.begin();
    for(; i != test_neg_data_.end(); ++i){
        for(size_t j = 0; j < feature_count_; ++j){
            tmp_mat->data.fl[j] = static_cast<float>((*i)[j]);
        }
        if(forest_->predict(cv::cvarrToMat(tmp_mat)) < 0)
            correct_neg++;
    }
    cout << "feature_count_: " << feature_count_ << endl;
    cout << "correct_ pos: " << correct_pos << endl;
    cout << "correct_ neg: " << correct_neg << endl;

    printf("   Positive: %d/%d \t\t Error: %g%%\n", correct_pos, (int)test_pos_data_.size(), 100.0 - 100.0*(float)(correct_pos)/(int)test_pos_data_.size());
    printf("   Negative: %d/%d \t\t Error: %g%%\n", correct_neg, (int)test_neg_data_.size(), 100.0 - 100.0*(float)(correct_neg)/(int)test_neg_data_.size());
    printf("   Combined: %d/%d \t\t Error: %g%%\n\n",
           correct_pos + correct_neg,
           (int)test_pos_data_.size() + (int)test_neg_data_.size(),
           100.0 - 100.0*(float)(correct_pos + correct_neg)/((int)test_pos_data_.size() + (int)test_neg_data_.size()));

    cvReleaseMat(&tmp_mat);

}
