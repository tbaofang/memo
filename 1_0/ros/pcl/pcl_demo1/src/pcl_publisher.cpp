#include "pcl_publisher.h"

CloudPublish::CloudPublish()
{
    com_pub_ = nh_.advertise<sensor_msgs::PointCloud2>("/com_pub", 1);
    com2_pub_ = nh_.advertise<sensor_msgs::PointCloud2>("/com2_pub", 1);
    handle_pub_ = nh_.advertise<sensor_msgs::PointCloud2>("/handle_pub", 1);
//    cloudpub_thread_ = new boost::thread([&]{cloudPub();});
//    cloudfilter_thread_ = new boost::thread([&]{cloudFilter();});
//    pcl_write_thread_ = new boost::thread([&]{pclWrite();});

//    planar_segmentation_thread_ = new boost::thread([&]{planarSegmentation();});
//    octree_change_detection_thread_ = new boost::thread([&]{octreeChangeDetection();});

//        moving_least_squares_thread_ = new boost::thread([&]{movingLeastSquares();});
//        greedy_projection_thread_ = new boost::thread([&]{greedyProjection();});
//        kdtree_thread_ = new boost::thread([&]{kdtree();});
        kdtree_search_thread_ = new boost::thread([&]{kdtree_search();});

}

CloudPublish::~CloudPublish(){

    if(kdtree_search_thread_){
        kdtree_search_thread_->join();
        delete kdtree_search_thread_;
    }
//    if(kdtree_thread_){
//        kdtree_thread_->join();
//        delete kdtree_thread_;
//    }

//    if(greedy_projection_thread_){
//        greedy_projection_thread_->join();
//        delete greedy_projection_thread_;
//    }

//    if(cloudpub_thread_) {
//        cloudpub_thread_->join();
//        delete cloudpub_thread_;
//    }
//
//    if(cloudfilter_thread_){
//        cloudfilter_thread_->join();
//        delete cloudfilter_thread_;
//    }

//    if(planar_segmentation_thread_){
//        planar_segmentation_thread_->join();
//        delete planar_segmentation_thread_;
//    }

//    if(octree_change_detection_thread_){
//        octree_change_detection_thread_->join();
//        delete octree_change_detection_thread_;
//    }

//    if(moving_least_squares_thread_){
//        moving_least_squares_thread_->join();
//        delete moving_least_squares_thread_;
//    }

}


bool CloudPublish::cloudPub(){
	pcl_pub_ = nh_.advertise<sensor_msgs::PointCloud2>("/pcl_out", 1);

	pcl::PointCloud<pcl::PointXYZ> cloud;
	sensor_msgs::PointCloud2 output;

	cloud.width = 50000;
	cloud.height = 2;
	cloud.points.resize(cloud.width * cloud.height);

    for(int i = 0; i < cloud.points.size(); ++i){
        cloud.points[i].x = 512*rand()/(RAND_MAX+1.0f);
        cloud.points[i].y = 512*rand()/(RAND_MAX+1.0f);
        cloud.points[i].z = 512*rand()/(RAND_MAX+1.0f);

    }

    pcl::toROSMsg(cloud, output);
    output.header.frame_id = "point_cloud";
    ros::Rate loop_rate(10.0);
    while(ros::ok()){
        pcl_pub_.publish(output);
        ros::spinOnce();
        loop_rate.sleep();
    }
}

bool CloudPublish::cloudFilter(){
    pcl_sub_ = nh_.subscribe("/pcl_out", 10, &CloudPublish::cloudC, this);
    pcl_filter_pub_ = nh_.advertise<const sensor_msgs::PointCloud2>("/pcl_filtered", 1);
}

void CloudPublish::cloudC(const sensor_msgs::PointCloud2 &input) {
    pcl::PointCloud<pcl::PointXYZ> cloud;
    pcl::PointCloud<pcl::PointXYZ> cloud_filter;
    sensor_msgs::PointCloud2 filter_output;

    pcl::fromROSMsg(input, cloud);

    pcl::VoxelGrid<pcl::PointXYZ> vox_obj;
    vox_obj.setInputCloud(cloud.makeShared());
    vox_obj.setLeafSize(0.2f, 0.2f, 0.2f);
    vox_obj.filter(cloud_filter);

    pcl::toROSMsg(cloud_filter, filter_output);

    filter_output.header.frame_id = "point_cloud";
    pcl_filter_pub_.publish(filter_output);

}

bool CloudPublish::pclWrite(){
    pcl_write_sub_ = nh_.subscribe("/pcl_filtered", 10, &CloudPublish::pclWriteC, this);
    pcl_read_pub_ = nh_.advertise<sensor_msgs::PointCloud2>("/read_output", 1);
}

void CloudPublish::pclWriteC(const sensor_msgs::PointCloud2& input) {
    pcl::PointCloud<pcl::PointXYZ> cloud;
    pcl::fromROSMsg(input, cloud);
    pcl::io::savePCDFileASCII("data/test.pcd", cloud);
    cout << "save succeed!" << endl;


    sensor_msgs::PointCloud2 read_outpub;
    pcl::io::loadPCDFile("data/test.pcd", cloud);
    pcl::toROSMsg(cloud, read_outpub);
    read_outpub.header.frame_id = "point_cloud";

    pcl_read_pub_.publish(read_outpub);
}

bool CloudPublish::planarSegmentation() {
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloudPtr(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloudPtr2(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PointCloud<pcl::PointXYZ> cloud1, cloud2;
    sensor_msgs::PointCloud2 output1, output2;

    cloudPtr->width = 15;
    cloudPtr->height = 1;
    cloudPtr->points.resize(cloudPtr->width*cloudPtr->height);
    auto assignment = [](auto& point){
        point.x = 1024*rand()/(RAND_MAX+1.0f);
        point.y = 1024*rand()/(RAND_MAX+1.0f);
        point.z = 1;
    };
    for_each(cloudPtr->points.begin(), cloudPtr->points.end(), assignment);

    cloudPtr->points[0].z = 2.0;
    cloudPtr->points[3].z = -2.0;
    cloudPtr->points[6].z = 4.0;

//    for(auto point:cloudPtr->points)
//        cout << point.x << " " << point.y << " " << point.z << endl;

    pcl::ModelCoefficients::Ptr coefficients (new pcl::ModelCoefficients);
    pcl::PointIndices::Ptr inliers(new pcl::PointIndices);

    pcl::SACSegmentation<pcl::PointXYZ> seg;
    seg.setOptimizeCoefficients(true);
    seg.setModelType(pcl::SACMODEL_PLANE);
    seg.setMethodType(pcl::SAC_RANSAC);
    seg.setDistanceThreshold(0.01);
    seg.setInputCloud(cloudPtr);
    seg.segment(*inliers, *coefficients);

    cout << "coefficients: " << endl;
    for(auto coefficient:coefficients->values)
        cout << coefficient << endl;

    cloudPtr2->width = inliers->indices.size();
    cloudPtr2->height = 1;
    cloudPtr2->resize(cloudPtr2->width*cloudPtr2->height);
    for(size_t i = 0; i < inliers->indices.size(); ++i){
        cloudPtr2->points[i].x = cloudPtr->points[inliers->indices[i]].x;
        cloudPtr2->points[i].y = cloudPtr->points[inliers->indices[i]].y;
        cloudPtr2->points[i].z = cloudPtr->points[inliers->indices[i]].z;
    }

    cloud1 = *cloudPtr;
    cloud2 = *cloudPtr2;
    pcl::toROSMsg(cloud1, output1);
    pcl::toROSMsg(cloud2, output2);
    output1.header.frame_id = "map";
    output2.header.frame_id = "map";

    ros::Rate loop_rate(10.0);
    while(ros::ok()){
        com_pub_.publish(output1);
        handle_pub_.publish(output2);
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}

bool CloudPublish::createPointCloud(pcl::PointCloud<pcl::PointXYZ>::Ptr cloudPtr, size_t width, size_t height) {
//    srand((unsigned int) time(NULL));
//    cout << "time: " << time(NULL) << endl;
    cloudPtr->points.resize(width * height);
    auto assignment = [](auto& point){
        point.x = 64.0f*rand()/(RAND_MAX+1.0f);
        point.y = 64.0f*rand()/(RAND_MAX+1.0f);
        point.z = 64.0f*rand()/(RAND_MAX+1.0f);
    };
    for_each(cloudPtr->points.begin(), cloudPtr->points.end(), assignment);
}

bool CloudPublish::octreeChangeDetection() {
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloudPtr(new pcl::PointCloud<pcl::PointXYZ>);
    cloudPtr->width = 128;
    cloudPtr->height = 1;
    createPointCloud(cloudPtr, cloudPtr->width, cloudPtr->height);
    for(auto point:cloudPtr->points)
        cout << "cloudPtr1: " << point.x << " " << point.y << " " << point.z << endl;


    float resolution = 16.0f;
    pcl::octree::OctreePointCloudChangeDetector<pcl::PointXYZ> octree(resolution);
    octree.setInputCloud(cloudPtr);
    octree.addPointsFromInputCloud();
    octree.switchBuffers();


    pcl::PointCloud<pcl::PointXYZ>::Ptr cloudPtr2(new pcl::PointCloud<pcl::PointXYZ>);
    cloudPtr2->width = 128;
    cloudPtr2->height = 1;
    createPointCloud(cloudPtr2, cloudPtr2->width, cloudPtr2->height);
    for(auto point:cloudPtr2->points)
        cout << "cloudPtr2: " << point.x << " " << point.y << " " << point.z << endl;

    octree.setInputCloud(cloudPtr2);
    octree.addPointsFromInputCloud();

    std::vector<int> new_point_idx;
    octree.getPointIndicesFromNewVoxels(new_point_idx);

    cout << "new_point_idx size: " << new_point_idx.size() << endl;

    pcl::PointCloud<pcl::PointXYZ> cloud, cloud2, cloud3;

    cloud3.width = new_point_idx.size();
    cloud3.height = 1;
    cloud3.resize(cloud3.width*cloud3.height);

    for(size_t i = 0; i < new_point_idx.size(); ++i){
        cloud3.points[i].x = cloudPtr2->points[i].x;
        cloud3.points[i].y = cloudPtr2->points[i].y;
        cloud3.points[i].z = cloudPtr2->points[i].z;
    }
    sensor_msgs::PointCloud2 output, output2, output3;
    cloud = *cloudPtr;
    cloud2 = *cloudPtr2;

    pcl::toROSMsg(cloud, output);
    pcl::toROSMsg(cloud2, output2);
    pcl::toROSMsg(cloud3, output3);
    output.header.frame_id = "map";
    output2.header.frame_id = "map";
    output3.header.frame_id = "map";

    ros::Rate loop_rate(1.0);
    while(ros::ok()){
        com_pub_.publish(output);
        com2_pub_.publish(output2);
        handle_pub_.publish(output3);
        ros::spinOnce();
        loop_rate.sleep();
    }
}

bool CloudPublish::movingLeastSquares() {
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloudPtr(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::io::loadPCDFile("data/bun0.pcd", *cloudPtr);

    pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ>);
    pcl::PointCloud<pcl::PointNormal> mls_points;
    pcl::MovingLeastSquares<pcl::PointXYZ, pcl::PointNormal> mls;
    mls.setComputeNormals(true);
    mls.setInputCloud(cloudPtr);
    mls.setPolynomialOrder(2);
    mls.setSearchMethod(tree);
    mls.setSearchRadius(0.03);
    mls.process(mls_points);

    pcl::io::savePCDFileASCII("bun0-mls.pcd", mls_points);


    pcl::PointCloud<pcl::PointXYZ> cloud, cloud2, cloud3;
    sensor_msgs::PointCloud2 output, output2, output3;
    cloud = *cloudPtr;
    pcl::toROSMsg(cloud, output);
    pcl::toROSMsg(mls_points, output2);
    output.header.frame_id = "map";
    output2.header.frame_id = "map";
    ros::Rate loop_rate(1.0);
    while(ros::ok()){
        com_pub_.publish(output);
        com2_pub_.publish(output2);
        handle_pub_.publish(output3);
        ros::spinOnce();
        loop_rate.sleep();
    }
}

bool CloudPublish::greedyProjection() {
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_p (new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PCLPointCloud2 cloud_blob;
    pcl::io::loadPCDFile("data/bun0.pcd", cloud_blob);
    pcl::fromPCLPointCloud2(cloud_blob, *cloud_p);

    pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> n;
    pcl::PointCloud<pcl::Normal>::Ptr normals_p (new pcl::PointCloud<pcl::Normal>);
    pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ>);
    tree->setInputCloud(cloud_p);
    n.setInputCloud(cloud_p);
    n.setSearchMethod(tree);
    n.setKSearch(20);
    n.compute(*normals_p);

    pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals (new pcl::PointCloud<pcl::PointNormal>);
    pcl::concatenateFields(*cloud_p, *normals_p, *cloud_with_normals);

    pcl::search::KdTree<pcl::PointNormal>::Ptr tree2 (new pcl::search::KdTree<pcl::PointNormal>);
    tree2->setInputCloud(cloud_with_normals);

    pcl::GreedyProjectionTriangulation<pcl::PointNormal> gp3;
    pcl::PolygonMesh triangles;

    gp3.setSearchRadius(0.025);
    gp3.setMu(2.5);
    gp3.setMaximumNearestNeighbors(100);
    gp3.setMaximumSurfaceAngle(M_PI/4);
    gp3.setMinimumAngle(M_PI/18);
    gp3.setMaximumAngle(2*M_PI/3);
    gp3.setNormalConsistency(false);

    gp3.setInputCloud(cloud_with_normals);
    gp3.setSearchMethod(tree2);
    gp3.reconstruct(triangles);

    vector<int> parts = gp3.getPartIDs();
    vector<int> states = gp3.getPointStates();



//    pcl::io::savePCDFileASCII("data/cloud_with_normals.pcd", triangles);
    pcl::io::savePLYFile("result.ply", triangles);

    boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer("3d viewer"));
    viewer->setBackgroundColor(0, 0, 0);
    viewer->addPolygonMesh(triangles, "my");
//    viewer->addCoordinateSystem(1.0);
//    viewer->initCameraParameters();
    while(!viewer->wasStopped()){
        viewer->spinOnce(100);
        boost::this_thread::sleep(boost::posix_time::microseconds(100000));
    }


    sensor_msgs::PointCloud2 output;
    pcl::toROSMsg(*cloud_with_normals, output);
    output.header.frame_id = "map";
    ros::Rate loop_rate(1.0);
    while(ros::ok()){
        com_pub_.publish(output);
        ros::spinOnce();
        loop_rate.sleep();
    }
}

bool CloudPublish::kdtree() {
    struct MyPoint:public PointXYZ{
        MyPoint(float x, float y, float z){
            this->x = x;
            this->y = y;
            this->z = z;
        }
    };

//    PointCloud<MyPoint> cloud, cloud_big;
    PointCloud<PointXYZ> cloud, cloud_big;


    float resoluttion = 0.1f;
    for(float z = -0.5f; z < 0.5f; z += resoluttion)
        for(float y = -0.5f; y < 0.5f; y += resoluttion)
            for(float x = -0.5f; x < 0.5f; x += resoluttion)
                cloud.points.push_back(PointXYZ(x, y, z));

    cloud.width = static_cast<uint32_t>(cloud.points.size());
    cloud.height = 1;

    cloud_big.width = 640;
    cloud_big.height = 480;
    srand(static_cast<unsigned int>(time(NULL)));
    for(size_t i =0; i < cloud_big.width * cloud_big.height; ++i)
        cloud_big.points.push_back(PointXYZ(static_cast<float>(1024 * rand() / (RAND_MAX + 1.0)),
                                   static_cast<float>(1024 * rand() / (RAND_MAX + 1.0)),
                                   static_cast<float>(1024 * rand() / (RAND_MAX + 1.0))));

    KdTreeFLANN<PointXYZ> kdtree;
    kdtree.setInputCloud(cloud.makeShared());

    PointXYZ search_point{0, 0, 0};

    int k =10;
    vector<int> point_idx_nkn_search(k);
    vector<float> point_nkn_squared_distance(k);

    if(kdtree.nearestKSearch(search_point, k, point_idx_nkn_search, point_nkn_squared_distance)){
        cout << point_idx_nkn_search.size() << endl;
        cout << cloud.points[point_idx_nkn_search[9]].x << endl;
        cout << point_nkn_squared_distance[9] << endl;
    }

    sensor_msgs::PointCloud2 output;
    toROSMsg(cloud_big, output);
    output.header.frame_id = "map";
    ros::Rate loop_rate(1.0);
    while(ros::ok()){
        com_pub_.publish(output);
        ros::spinOnce();
        loop_rate.sleep();
    }

}

bool CloudPublish::kdtree_search() {
    PointCloud<PointXYZ> cloud;
    cloud.width = 1000;
    cloud.height = 1;
    cloud.resize(cloud.width * cloud.height);
    for(size_t i = 0; i < cloud.points.size(); ++i)
        cloud.points.push_back(PointXYZ(static_cast<float>(1024 * rand() / (RAND_MAX + 1.0)),
                                        static_cast<float>(1024 * rand() / (RAND_MAX + 1.0)),
                                        static_cast<float>(1024 * rand() / (RAND_MAX + 1.0))));

    float resolution = 128.0f;
    pcl::octree::OctreePointCloudSearch<pcl::PointXYZ> octree (resolution);

    octree.setInputCloud(cloud.makeShared());
    octree.addPointsFromInputCloud();

    pcl::PointXYZ searchPoint{static_cast<float>(1024 * rand() / (RAND_MAX + 1.0)),
                              static_cast<float>(1024 * rand() / (RAND_MAX + 1.0)),
                              static_cast<float>(1024 * rand() / (RAND_MAX + 1.0))};
    vector<int> point_idx;

    if(octree.voxelSearch(searchPoint, point_idx)){
        for(size_t i = 0; i < point_idx.size(); ++i){
            cout << cloud.points[point_idx[i]].x << " "
                    << cloud.points[point_idx[i]].y << " "
                    << cloud.points[point_idx[i]].z << " " << endl;
        }
    }
    

    
    
}








