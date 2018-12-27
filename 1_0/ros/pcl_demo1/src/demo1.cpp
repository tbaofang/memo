
#include <pcl/ModelCoefficients.h>
#include <pcl/filters/approximate_voxel_grid.h>
#include <pcl/filters/radius_outlier_removal.h>
#include <pcl/filters/conditional_removal.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/project_inliers.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/passthrough.h>
#include <pcl/features/range_image_border_extractor.h>
#include <pcl/features/spin_image.h>
#include <pcl/features/normal_3d.h>
#include <pcl/features/3dsc.h>
#include <pcl/features/impl/3dsc.hpp>
#include <pcl/features/rift.h>
#include <pcl/features/intensity_gradient.h>
#include <pcl/features/normal_3d.h>
#include <pcl/features/principal_curvatures.h>
#include <pcl/features/pfh.h>
#include <pcl/keypoints/narf_keypoint.h>
#include <pcl/point_cloud.h>
#include <pcl/octree/octree_search.h>
#include <pcl/io/pcd_io.h>
#include <pcl/range_image/range_image.h>
#include <pcl/registration/correspondence_estimation_normal_shooting.h>
#include <pcl/registration/ndt.h>
#include <pcl/sample_consensus/ransac.h>
#include <pcl/sample_consensus/sac_model_plane.h>
#include <pcl/sample_consensus/sac_model_sphere.h>
#include <pcl/search/octree.h>
#include <pcl/search/kdtree.h>
#include <pcl/search/organized.h>
#include <pcl/search/impl/search.hpp>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/segmentation/extract_clusters.h>
#include <pcl/segmentation/impl/extract_clusters.hpp>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/visualization/range_image_visualizer.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/console/parse.h>


#include <iostream>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

// #include <ros/ros.h>

using namespace std;
using namespace pcl;
// using namespace ros;

void kdtree_search()
{
    PointCloud<PointXYZ>::Ptr cloud(new PointCloud<PointXYZ>);
    cloud->width = 1000;
    cloud->height = 1;
    cloud->points.resize(cloud->width * cloud->height);
    for (size_t i = 0; i < cloud->points.size(); ++i)
    {
        // cloud->points[i].x = 1024.0f * rand() / (RAND_MAX + 1.0f);
        // cloud->points[i].y = 1024.0f * rand() / (RAND_MAX + 1.0f);
        // cloud->points[i].z = 1024.0f * rand() / (RAND_MAX + 1.0f);
        cloud->points[i].x = static_cast<float>(1024 * rand() / (RAND_MAX + 1.0));
        cloud->points[i].y = static_cast<float>(1024 * rand() / (RAND_MAX + 1.0));
        cloud->points[i].z = static_cast<float>(1024 * rand() / (RAND_MAX + 1.0));
        // cloud->points.push_back(PointXYZ(static_cast<float>(1024 * rand() / (RAND_MAX + 1.0)),
        //                                 static_cast<float>(1024 * rand() / (RAND_MAX + 1.0)),
        //                                 static_cast<float>(1024 * rand() / (RAND_MAX + 1.0))));
    }

    cout << "-----------cloud--------------" << endl;
    for (auto point : cloud->points)
        cout << point.x << " " << point.y << " " << point.z << endl;

    float resolution = 128.0f;
    pcl::octree::OctreePointCloudSearch<pcl::PointXYZ> octree(resolution);

    octree.setInputCloud(cloud);
    octree.addPointsFromInputCloud();

    pcl::PointXYZ search_point{static_cast<float>(1024 * rand() / (RAND_MAX + 1.0)),
                               static_cast<float>(1024 * rand() / (RAND_MAX + 1.0)),
                               static_cast<float>(1024 * rand() / (RAND_MAX + 1.0))};
    // pcl::PointXYZ search_point;
    // search_point.x = 1024.0f * rand() / (RAND_MAX + 1.0f);
    // search_point.y = 1024.0f * rand() / (RAND_MAX + 1.0f);
    // search_point.z = 1024.0f * rand() / (RAND_MAX + 1.0f);

    vector<int> point_idx;
    cout << search_point.x << " " << search_point.x << " " << search_point.x << endl;

    cout << "-----------voxelSearch--------------" << endl;
    if (octree.voxelSearch(search_point, point_idx))
    {
        for (size_t i = 0; i < point_idx.size(); ++i)
        {
            cout << cloud->points[point_idx[i]].x << " "
                 << cloud->points[point_idx[i]].y << " "
                 << cloud->points[point_idx[i]].z << " " << endl;
        }
    }

    vector<int> point_idx_nkn_search;
    vector<float> point_nkn_squared_distance;

    int k = 10;
    cout << "-----------nearestKSearch--------------" << endl;
    if (octree.nearestKSearch(search_point, k, point_idx_nkn_search, point_nkn_squared_distance))
    {
        for (size_t i = 0; i < point_idx_nkn_search.size(); ++i)
        {
            cout << cloud->points[point_idx_nkn_search[i]].x << " "
                 << cloud->points[point_idx_nkn_search[i]].y << " "
                 << cloud->points[point_idx_nkn_search[i]].z << endl;
            cout << "distance: " << point_nkn_squared_distance[i] << endl;
        }
    }

    float radius = static_cast<float>(256 * rand() / (RAND_MAX + 1.0));

    vector<int> point_idx_radius_search;
    vector<float> point_radius_squared_distance;

    cout << "-----------radiusSearch--------------" << endl;
    if (octree.radiusSearch(search_point, radius, point_idx_radius_search, point_radius_squared_distance))
    {
        for (size_t i = 0; i < point_idx_radius_search.size(); ++i)
        {
            cout << cloud->points[point_idx_radius_search.at(i)].x << " "
                 << cloud->points[point_idx_radius_search.at(i)].x << " "
                 << cloud->points[point_idx_radius_search.at(i)].x << endl;
            cout << "radius distance: " << point_radius_squared_distance.at(i) << endl;
        }
    }
}
void viewerOne(pcl::visualization::PCLVisualizer &viewer)
{
    viewer.setBackgroundColor(0.5, 0.5, 1.0);
    pcl::PointXYZ o;
    o.x = 1.0;
    o.y = 0;
    o.z = 0;

    viewer.addSphere(o, 0.25, "sphere", 0);
}
void viewerPsy(pcl::visualization::PCLVisualizer &viewer)
{
    static unsigned count = 0;
    stringstream ss;
    ss << "loop: " << count++;
    viewer.removeShape("text", 0);
    viewer.addText(ss.str(), 200, 300, "text", 0);
}

void cloudViewer()
{
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::io::loadPCDFile("./data/bun02.pcd", *cloud);

    pcl::visualization::CloudViewer viewer("bun02");
    viewer.showCloud(cloud);

    viewer.runOnVisualizationThreadOnce(viewerOne);
    viewer.runOnVisualizationThread(viewerPsy);

    while (!viewer.wasStopped())
    {
    }
}

void passThrough()
{
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>(100, 1));
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filter(new pcl::PointCloud<pcl::PointXYZ>);
    for (size_t i = 0; i < cloud->points.size(); ++i)
    {
        cloud->points[i].x = static_cast<float>(1024 * rand() / (RAND_MAX + 1.0));
        cloud->points[i].y = static_cast<float>(1024 * rand() / (RAND_MAX + 1.0));
        cloud->points[i].z = static_cast<float>(1024 * rand() / (RAND_MAX + 1.0));
    }

    pcl::PassThrough<pcl::PointXYZ> pass;
    pass.setInputCloud(cloud);
    pass.setFilterFieldName("x");
    pass.setFilterLimits(0.0, 0.6);
    pass.setFilterLimitsNegative(true);
    pass.filter(*cloud_filter);

    cout << "[cloud_filter->points.size():] " << cloud_filter->points.size() << endl;
}

void voxelGrid()
{
    pcl::PCLPointCloud2::Ptr cloud(new pcl::PCLPointCloud2());
    pcl::PCLPointCloud2::Ptr cloud2(new pcl::PCLPointCloud2());
    // pcl::io::loadPCDFile("data/table_scene_lms400.pcd", *cloud);

    pcl::PCDReader reader;
    reader.read("data/table_scene_lms400.pcd", *cloud2);
    // cout << "[cloud->points.size():] " << cloud->points.size() << endl;
    cout << "[cloud->width:] " << cloud2->width << endl;
    cout << "[pcl::getFieldsList(*cloud):] " << pcl::getFieldsList(*cloud2) << endl;

    pcl::VoxelGrid<pcl::PCLPointCloud2> vox;
    vox.setInputCloud(cloud2);
    vox.setLeafSize(0.02, 0.02, 0.02);
    vox.filter(*cloud);
    cout << "[cloud->width:] " << cloud->width << endl;

    cout << "[Eigen::Vector4f::Zero():] " << Eigen::Vector4f::Zero() << endl;
    // cout << "[Eigen::Quaternionf::Identity():] " << Eigen::Quaternionf::Identity() << endl;
    pcl::PCDWriter writer;
    writer.write("data/table_scene_lms400_downsampled.pcd", *cloud,
                 Eigen::Vector4f::Zero(), Eigen::Quaternionf::Identity(), false);

    pcl::StatisticalOutlierRemoval<pcl::PCLPointCloud2> sor;
    sor.setInputCloud(cloud2);
    sor.setMeanK(100);           //用于计算距离用的邻域点数量
    sor.setStddevMulThresh(2.0); //标准差倍数阈值
    sor.filter(*cloud);

    cout << "[cloud->width:] " << cloud->width << endl;

    writer.write("data/table_scene_lms400_inlier.pcd", *cloud,
                 Eigen::Vector4f::Zero(), Eigen::Quaternionf::Identity(), false);

    sor.setNegative(true);
    sor.filter(*cloud);
    cout << "[cloud->width:] " << cloud->width << endl;
    writer.write("data/table_scene_lms400_outlier.pcd", *cloud,
                 Eigen::Vector4f::Zero(), Eigen::Quaternionf::Identity(), false);
}

void projectInlier()
{
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>(5, 1));
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filter(new pcl::PointCloud<pcl::PointXYZ>);
    for (size_t i = 0; i < cloud->points.size(); ++i)
    {
        cloud->points[i].x = static_cast<float>(1024 * rand() / (RAND_MAX + 1.0));
        cloud->points[i].y = static_cast<float>(1024 * rand() / (RAND_MAX + 1.0));
        cloud->points[i].z = static_cast<float>(1024 * rand() / (RAND_MAX + 1.0));
    }

    pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients());
    coefficients->values.resize(4);
    coefficients->values.at(3) = coefficients->values.at(1) = coefficients->values.at(3) = 1.0;
    coefficients->values.at(0) = 1.0;

    pcl::ProjectInliers<pcl::PointXYZ> proj;
    proj.setInputCloud(cloud);
    proj.setModelType(pcl::SACMODEL_PLANE);
    proj.setModelCoefficients(coefficients);
    proj.filter(*cloud_filter);

    cout << "[cloud_filter->width:] " << cloud_filter->width << endl;

    for (auto point : cloud_filter->points)
        cerr << point.x << " " << point.y << " " << point.z << endl;
}
void extractIndiers()
{
    pcl::PCLPointCloud2::Ptr cloud_blob(new pcl::PCLPointCloud2);
    pcl::PCLPointCloud2::Ptr cloud_filtered_blob(new pcl::PCLPointCloud2);
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZ>),
        cloud_p(new pcl::PointCloud<pcl::PointXYZ>),
        cloud_f(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PCDReader reader;
    reader.read("data/table_scene_lms400.pcd", *cloud_blob);

    pcl::VoxelGrid<pcl::PCLPointCloud2> vox;
    vox.setInputCloud(cloud_blob);
    vox.setLeafSize(0.01, 0.01, 0.01);
    vox.filter(*cloud_filtered_blob);
    cout << "[cloud->width:] " << cloud_filtered_blob->width << endl;

    pcl::fromPCLPointCloud2(*cloud_filtered_blob, *cloud_filtered);
    cout << "[cloud_filtered.points.size():] " << cloud_filtered->points.size() << endl;

    pcl::PCDWriter writer;
    writer.write<pcl::PointXYZ>("table_scene_lms400_downsampled.pcd", *cloud_filtered, false);

    pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients());
    pcl::PointIndices::Ptr inliers(new pcl::PointIndices());
    pcl::SACSegmentation<pcl::PointXYZ> seg;
    pcl::ExtractIndices<pcl::PointXYZ> extract;

    int i = 0;
    int nr_points = static_cast<int>(cloud_filtered->points.size());
    cout << "[typeid(cloud_filtered->points.size()).name():] " << typeid(cloud_filtered->points.size()).name() << endl;

    seg.setOptimizeCoefficients(true);
    seg.setModelType(pcl::SACMODEL_PLANE);
    seg.setMethodType(pcl::SAC_RANSAC);
    seg.setMaxIterations(1000);
    seg.setDistanceThreshold(0.01);

    while (cloud_filtered->points.size() > 0.3 * nr_points)
    {
        seg.setInputCloud(cloud_filtered);
        seg.segment(*inliers, *coefficients);
        cout << "[inliers->indices.size():] " << inliers->indices.size() << endl;
        if (inliers->indices.size() == 0)
        {
            std::cerr << "Could not estimate a planar model for the given dataset." << std::endl;
            break;
        }

        extract.setInputCloud(cloud_filtered);
        extract.setIndices(inliers);
        extract.setNegative(false);
        extract.filter(*cloud_p);
        cout << "[inliers->indeces.size():] " << inliers->indices.size() << endl;
        cout << "[cloud_p->points.size():] " << cloud_p->points.size() << endl;

        std::stringstream ss;
        ss << "table_scene_lms400_plane_" << i << ".pcd";
        writer.write<pcl::PointXYZ>(ss.str(), *cloud_p, false);

        extract.setNegative(true);
        extract.filter(*cloud_f);
        cloud_filtered.swap(cloud_f);
        ++i;
    }
}
auto assigned = [](auto &point) mutable {
    point.x = static_cast<float>(1024 * rand() / (RAND_MAX + 1.0));
    point.y = static_cast<float>(1024 * rand() / (RAND_MAX + 1.0));
    point.z = static_cast<float>(1024 * rand() / (RAND_MAX + 1.0));
};

void revomeOutlier()
{
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>(5, 1));
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZ>);

    for_each(cloud->points.begin(), cloud->points.end(), assigned);

    cout << "filter before:" << endl;
    for (auto point : cloud->points)
        cout << point.x << "    " << point.y << "   " << point.z << endl;

    pcl::RadiusOutlierRemoval<pcl::PointXYZ> outrem;
    outrem.setInputCloud(cloud);
    outrem.setRadiusSearch(0.8);
    outrem.setMinNeighborsInRadius(1);
    outrem.filter(*cloud_filtered);

    cout << "outlierremoval:" << endl;
    for (auto point : cloud_filtered->points)
        cout << point.x << "    " << point.y << "   " << point.z << endl;

    pcl::ConditionAnd<pcl::PointXYZ>::Ptr range_cond(new pcl::ConditionAnd<pcl::PointXYZ>());
    range_cond->addComparison(pcl::FieldComparison<pcl::PointXYZ>::ConstPtr(new pcl::FieldComparison<pcl::PointXYZ>("z", pcl::ComparisonOps::GT, 0.0)));
    range_cond->addComparison(pcl::FieldComparison<pcl::PointXYZ>::ConstPtr(new pcl::FieldComparison<pcl::PointXYZ>("z", pcl::ComparisonOps::LT, 1.0)));

    pcl::ConditionalRemoval<pcl::PointXYZ> condrem;
    condrem.setInputCloud(cloud);
    condrem.setCondition(range_cond);
    condrem.setKeepOrganized(true);
    condrem.filter(*cloud_filtered);

    cout << "conditionremoval:" << endl;
    for (auto point : cloud_filtered->points)
        cout << point.x << "    " << point.y << "   " << point.z << endl;
}

void rangeImageCreate()
{
    pcl::PointCloud<pcl::PointXYZ> cloud;
    float resolution = 0.01;
    for (float x = -0.5; x < 0.5; x += resolution)
        for (float y = -0.5; y < 0.5; y += resolution)
        {
            pcl::PointXYZ point;
            point.x = x;
            point.y = y;
            point.z = 2.0 - y;
            cloud.points.push_back(point);
        }
    cloud.width = static_cast<uint32_t>(cloud.points.size());
    cloud.height = 1;

    float angular_resolution = static_cast<float>(1.0f * (M_PI / 180.0f)); //   1.0 degree in radians
    float max_angle_width = static_cast<float>(360.0f * (M_PI / 180.0f));   // 360.0 degree in radians
    float max_angle_height = static_cast<float>(180.0f * (M_PI / 180.0f));  // 180.0 degree in radians
    Eigen::Affine3f sensor_pose = (Eigen::Affine3f)Eigen::Translation3f(0.0f, 0.0f, 0.0f);
    pcl::RangeImage::CoordinateFrame coordinate_frame = pcl::RangeImage::CAMERA_FRAME;
    float noise_level = 0.00;
    float min_range = 0.0f;
    int border_size = 1;

    pcl::RangeImage range_image;
    range_image.createFromPointCloud(cloud, angular_resolution, max_angle_width, max_angle_height,sensor_pose, 
                                    coordinate_frame, noise_level, min_range, border_size);
    cout << "[range_image:] " << range_image << endl;
}

void narfKeyPointExtract(){
    float angular_resolution = 0.5f;
    float support_size = 0.1f;
    pcl::RangeImage::CoordinateFrame coordinate_frame = pcl::RangeImage::CAMERA_FRAME;
    bool setUnseenToMaxRange = true;

    int tmp_coordinate_frame;
    angular_resolution = pcl::deg2rad (angular_resolution);

    cout << "angular_resolution" << angular_resolution << endl;
    pcl::PointCloud<PointXYZ>::Ptr point_cloud_ptr (new pcl::PointCloud<PointXYZ>);
    pcl::PointCloud<PointXYZ>& point_cloud = *point_cloud_ptr;

    pcl::PointCloud<pcl::PointWithViewpoint> far_ranges;
    Eigen::Affine3f scene_sensor_pose (Eigen::Affine3f::Identity ());

    pcl::io::loadPCDFile("data/table_scene_lms400.pcd", point_cloud);
    cout << "pont_cloud size() : " << point_cloud.points.size() << endl;

    float noise_level = 0.0;
    float min_range = 0.0f;
    int border_size = 1;
    boost::shared_ptr<pcl::RangeImage> range_image_ptr (new pcl::RangeImage);
    pcl::RangeImage& range_image = *range_image_ptr;
    scene_sensor_pose = Eigen::Affine3f (Eigen::Translation3f (point_cloud.sensor_origin_[0],
                                                               point_cloud.sensor_origin_[1],
                                                               point_cloud.sensor_origin_[2])) *
                        Eigen::Affine3f (point_cloud.sensor_orientation_);
    range_image.createFromPointCloud (point_cloud, angular_resolution, pcl::deg2rad (360.0f), pcl::deg2rad (180.0f),
                                      scene_sensor_pose, coordinate_frame, noise_level, min_range, border_size);
    range_image.integrateFarRanges (far_ranges);
    if (setUnseenToMaxRange)
        range_image.setUnseenToMaxRange ();

    pcl::visualization::PCLVisualizer viewer ("3D Viewer");
    viewer.setBackgroundColor (0.5, 0.5, .5);
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointWithRange> range_image_color_handler (range_image_ptr, 0, 0, 0);
    viewer.addPointCloud (range_image_ptr, range_image_color_handler, "range image");
    viewer.setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "range image");
    viewer.initCameraParameters ();

    pcl::RangeImageBorderExtractor range_image_border_extractor;
    pcl::NarfKeypoint narf_keypoint_detector (&range_image_border_extractor);
    narf_keypoint_detector.setRangeImage (&range_image);
    narf_keypoint_detector.getParameters ().support_size = support_size;

    pcl::PointCloud<int> keypoint_indices;
    narf_keypoint_detector.compute (keypoint_indices);
    std::cout << "Found "<<keypoint_indices.points.size ()<<" key points.\n";

    pcl::PointCloud<pcl::PointXYZ>::Ptr keypoints_ptr (new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PointCloud<pcl::PointXYZ>& keypoints = *keypoints_ptr;
    keypoints.points.resize (keypoint_indices.points.size ());
    for (size_t i=0; i<keypoint_indices.points.size (); ++i)
        keypoints.points[i].getVector3fMap () = range_image.points[keypoint_indices.points[i]].getVector3fMap ();

    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> keypoints_color_handler (keypoints_ptr, 0, 255, 0);
    viewer.addPointCloud<pcl::PointXYZ> (keypoints_ptr, keypoints_color_handler, "keypoints");
    viewer.setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 7, "keypoints");

    while (!viewer.wasStopped ())
    {
        viewer.spinOnce ();
        pcl_sleep(0.01);
    }

}

boost::shared_ptr<pcl::visualization::PCLVisualizer> simpleVis (pcl::PointCloud<pcl::PointXYZ>::ConstPtr cloud){
    boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer (new pcl::visualization::PCLVisualizer("3D Viewer"));
    viewer->setBackgroundColor(0, 0, 0);
    viewer->addPointCloud<pcl::PointXYZ> (cloud , "sample cloud");
    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "sample cloud");
    viewer->initCameraParameters();
    return viewer;
}
void randomSampleConsensus(){
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>(500, 1));
    pcl::PointCloud<pcl::PointXYZ>::Ptr final (new pcl::PointCloud<pcl::PointXYZ>);

    vector<int> inliers;

    bool tag = true;
    cloud->is_dense =false;
    for (size_t i = 0; i < cloud->points.size (); ++i)
    {

            cloud->points[i].x = 1024 * rand () / (RAND_MAX + 1.0);
            cloud->points[i].y = 1024 * rand () / (RAND_MAX + 1.0);
            if (i % 5 == 0)
                cloud->points[i].z = 1024 * rand () / (RAND_MAX + 1.0);
            else if(i % 2 == 0)
                cloud->points[i].z =  sqrt( 1 - (cloud->points[i].x * cloud->points[i].x) - (cloud->points[i].y * cloud->points[i].y));
            else
                cloud->points[i].z =  - sqrt( 1 - (cloud->points[i].x * cloud->points[i].x) - (cloud->points[i].y * cloud->points[i].y));

//            cloud->points[i].x = 1024 * rand () / (RAND_MAX + 1.0);
//            cloud->points[i].y = 1024 * rand () / (RAND_MAX + 1.0);
//            if( i % 2 == 0)
//                cloud->points[i].z = 1024 * rand () / (RAND_MAX + 1.0);
//            else
//                cloud->points[i].z = -1 * (cloud->points[i].x + cloud->points[i].y);

    }

    pcl::SampleConsensusModelSphere<pcl::PointXYZ>::Ptr model_s (new pcl::SampleConsensusModelSphere<pcl::PointXYZ> (cloud));
    pcl::SampleConsensusModelPlane<pcl::PointXYZ>::Ptr model_p (new pcl::SampleConsensusModelPlane<pcl::PointXYZ> (cloud));

    pcl::RandomSampleConsensus<pcl::PointXYZ> ransac (model_s);
    ransac.setDistanceThreshold (.01);
    ransac.computeModel();
    ransac.getInliers(inliers);

    pcl::copyPointCloud<pcl::PointXYZ>(*cloud, inliers, *final);

    boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer;
    viewer = simpleVis(final);

    while(!viewer->wasStopped()){
        viewer->spinOnce();
        boost::this_thread::sleep(boost::posix_time::microseconds(10000));

    }
}

void exampleSpinImages(char** argv){
    string filename = argv[1];
    cout << "filename" << filename << endl;
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
    if(pcl::io::loadPCDFile(filename, *cloud) == -1){
        PCL_ERROR("");
        return;
    }
    cout <<"cloud size: " << cloud->points.size() << endl;

    pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> normal_estimation;
    pcl::search::KdTree<pcl::PointXYZ>::Ptr kdtree(new pcl::search::KdTree<pcl::PointXYZ>);
    pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);
    pcl::SpinImageEstimation<pcl::PointXYZ, pcl::Normal, pcl::Histogram<153> > spin_image_descriptor(8, 0.5, 16);
    pcl::PointCloud<pcl::Histogram<153> >::Ptr spin_image (new pcl::PointCloud<pcl::Histogram<153> >);


    normal_estimation.setInputCloud(cloud);
    normal_estimation.setSearchMethod(kdtree);
    normal_estimation.setRadiusSearch(0.03);
    normal_estimation.compute(*normals);

    cout << "spin_image size : " << normals->points.size() << endl;
    cout << "spin_image 0: " << normals->points.at(0) << endl;

    spin_image_descriptor.setInputCloud(cloud);
    spin_image_descriptor.setInputNormals(normals);
    spin_image_descriptor.setSearchMethod(kdtree);
    spin_image_descriptor.setRadiusSearch(0.2);
    spin_image_descriptor.compute(*spin_image);

    cout << "spin_image size : " << spin_image->points.size() << endl;
    cout << "spin_image 0: " << spin_image->points.at(0) << endl;
}

void exampleShapeContexts(){
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);

    pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> normal_estimation;
    pcl::search::KdTree<pcl::PointXYZ>::Ptr kdtree(new pcl::search::KdTree<pcl::PointXYZ>);
    pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);

    pcl::ShapeContext3DEstimation<pcl::PointXYZ, pcl::Normal, pcl::ShapeContext1980> shape_context;
    pcl::PointCloud<pcl::ShapeContext1980>::Ptr shape_context_features (new pcl::PointCloud<pcl::ShapeContext1980>);


    if(pcl::io::loadPCDFile("data/table_scene_lms400.pcd", *cloud)){
        PCL_ERROR("");
        return;
    }

    normal_estimation.setInputCloud(cloud);
    normal_estimation.setSearchMethod(kdtree);
    normal_estimation.setRadiusSearch(0.03);
    normal_estimation.compute(*normals);

    shape_context.setInputCloud(cloud);
    shape_context.setInputNormals(normals);
    shape_context.setSearchMethod(kdtree);
    shape_context.setRadiusSearch(0.2);
    shape_context.setPointDensityRadius(0.04);
    shape_context.setMinimalRadius(0.02);
    shape_context.compute(*shape_context_features);

    cout << "shape context features size: " << shape_context_features->points.size() << endl;
    cout << "shape context features 0: " << shape_context_features->points.at(0) << endl;
}
void correspondingEstiomation(){
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud1 (new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud2 (new pcl::PointCloud<pcl::PointXYZ>);
    pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> ne;
    pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ>);
    pcl::PointCloud<pcl::Normal>::Ptr cloud1_normals (new pcl::PointCloud<pcl::Normal>);
    pcl::CorrespondencesPtr corr (new pcl::Correspondences);
    pcl::registration::CorrespondenceEstimationNormalShooting<pcl::PointXYZ, pcl::PointXYZ, pcl::Normal> ce;


    for(float x = 0.0; x < 10.0; x += 0.2f){
        for(float z = 0.0; z < 5.0f; z += 0.2f) {
            cloud1->points.push_back(pcl::PointXYZ(x, 0, z));
            cloud2->points.push_back(pcl::PointXYZ(x, 2, z));
        }
    }

    ne.setInputCloud(cloud1);
    ne.setSearchMethod(tree);
    ne.setKSearch(5);
    ne.compute(*cloud1_normals);

    ce.setInputSource(cloud1);
    ce.setKSearch(10);
    ce.setSourceNormals(cloud1_normals);
    ce.setInputTarget(cloud2);
    ce.determineCorrespondences(*corr);

    cout << "corr size: " << corr->size() << endl;
    cout << "cloud1 size: " << cloud2->size() << endl;

    for(size_t i = 0; i < corr->size(); ++i){
//        cout << corr->at(i) << endl;
    }



}
void extractClusterNormal(){
//    shared_ptr<pcl::PointCloud<pcl::PointXYZ>> cloud_ptr (new pcl::PointCloud<pcl::PointXYZ>);
//    shared_ptr<pcl::PointCloud<pcl::Normal>> cloud_normals (new pcl::PointCloud<pcl::Normal>);
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_ptr (new pcl::PointCloud<pcl::PointXYZ>());
    pcl::PointCloud<pcl::Normal>::Ptr cloud_normals (new pcl::PointCloud<pcl::Normal>());


    pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> ne;
    pcl::search::KdTree<pcl::PointXYZ>::Ptr tree_n (new pcl::search::KdTree<pcl::PointXYZ>());

    shared_ptr<pcl::KdTree<pcl::PointXYZ> > tree_ec (new pcl::KdTreeFLANN<pcl::PointXYZ> );
    vector<int> indices;
    vector<pcl::PointIndices> cluster_indices;

    const float tolerance = 0.5f;
    const double eps_angle = 5 * (M_PI / 180.0);
    const unsigned int min_cluster_size = 50;


    if(pcl::io::loadPCDFile<pcl::PointXYZ> ("data/bun01.pcd", *cloud_ptr) == -1){
        PCL_ERROR("");
        return;
    }

    cout << "cloud_ptr size() : " << cloud_ptr->points.size() << endl;
    cout << "shared count: " << cloud_ptr.use_count() << endl;

    ne.setInputCloud(cloud_ptr);
    ne.setSearchMethod(tree_n);
    ne.setRadiusSearch(0.03);
    ne.compute(*cloud_normals);

    tree_ec->setInputCloud(cloud_ptr);
//    pcl::extractEuclideanClusters(*cloud_ptr, *cloud_normals, tolerance, tree_ec, cluster_indices, eps_angle, min_cluster_size);


    cout << "cluster_indices size(): " << cluster_indices.size() << endl;





}

void normalDistanceTransform() {
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud1 (new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud2 (new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PointCloud<pcl::PointXYZ>::Ptr filter_cloud (new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PointCloud<pcl::PointXYZ>::Ptr output_cloud (new pcl::PointCloud<pcl::PointXYZ>);

    pcl::ApproximateVoxelGrid<pcl::PointXYZ> approximate_voxel_grid;
    pcl::NormalDistributionsTransform<pcl::PointXYZ, pcl::PointXYZ> ndt;

    shared_ptr<pcl::visualization::PCLVisualizer> viewer (new pcl::visualization::PCLVisualizer ("3D viewer"));


//    Eigen::AngleAxisf init_rotation (0.5, Eigen::Vector3f::UnitZ());
//    Eigen::Translation3f init_translation (1.5, 0.5, 0);
//    Eigen::Matrix4f init_guest = (init_rotation * init_translation).matrix();
    Eigen::AngleAxisf init_rotation (0.6931, Eigen::Vector3f::UnitZ ());
    Eigen::Translation3f init_translation (1.79387, 0.720047, 0);
    Eigen::Matrix4f init_guess = (init_translation * init_rotation).matrix ();

    cout << "sss" << pcl::io::loadPCDFile("data/room_scan1.pcd", *cloud1) << endl;


    if(!pcl::io::loadPCDFile("data/room_scan1.pcd", *cloud1) &&
       !pcl::io::loadPCDFile("data/room_scan2.pcd", *cloud2)){
        cout << "cloud1 size = " << cloud1->size() << endl;
        cout << "cloud2 size = " << cloud2->size() << endl;
    }
    else{
        PCL_ERROR("");
        return;
    }

    approximate_voxel_grid.setLeafSize(0.2, 0.2, 0.2);
    approximate_voxel_grid.setInputCloud(cloud2);
    approximate_voxel_grid.filter(*filter_cloud);

    cout << "filter_cloud size = " << filter_cloud->size() << endl;

    ndt.setTransformationEpsilon(0.01);
    ndt.setStepSize(0.1);
    ndt.setResolution(1.0);
    ndt.setMaximumIterations(20);
    ndt.setInputSource(filter_cloud);
    ndt.setInputTarget(cloud1);
    ndt.align(*output_cloud, init_guess);
    std::cout << "Normal Distributions Transform has converged:" << ndt.hasConverged ()
              << " score: " << ndt.getFitnessScore () << std::endl;

    pcl::transformPointCloud (*cloud2, *output_cloud, ndt.getFinalTransformation ());

    // Saving transformed input cloud.
    pcl::io::savePCDFileASCII ("data/room_scan2_transformed.pcd", *output_cloud);


    viewer->setBackgroundColor(0, 0, 0);
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> cloud1_color (cloud1, 255, 0, 0);
    viewer->addPointCloud<pcl::PointXYZ>(cloud1, cloud1_color, "cloud1 color");
    viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "cloud1 cloud");

    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> output_color (output_cloud, 0, 255, 0);
    viewer->addPointCloud<pcl::PointXYZ>(output_cloud, output_color, "output color");
    viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "output cloud");

    viewer->initCameraParameters();




    while(!viewer->wasStopped()){
        viewer->spinOnce();
        boost::this_thread::sleep(boost::posix_time::microseconds(100000));
    }



}



int main(int argc, char **argv)
{
    // kdtree_search();
    // cloudViewer();
    // passThrough();
    // voxelGrid();
    // projectInlier();
    // extractIndiers();
    // revomeOutlier();
//    rangeImageCreate();
//    narfKeyPointExtract();
//    randomSampleConsensus();
//    exampleSpinImages(argv);
//    exampleShapeContexts();
//    correspondingEstiomation();
//    extractClusterNormal();
    normalDistanceTransform();

}
