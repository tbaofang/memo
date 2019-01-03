/*******************************************************
@company: Copyright (C) 2018, Leishen Intelligent System
@product: LS_SERVER_ROBOT
@filename:
@brief:
@version:       date:       author:     comments:
@v1.0           18-5-26     fu          new
*******************************************************/

#include "lsrobot_node/ls_topic.h"
#include "lsrobot_node/ls_es.h"
#include "lsrobot/LSPoint.h"
#include "lsrobot/LSPoints.h"

#ifdef HAVE_NEW_YAMLCPP
// The >> operator disappeared in yaml-cpp 0.5, so this function is
// added to provide support for code written under the yaml-cpp 0.3 API.
template<typename T>
void operator >> (const YAML::Node& node, T& i)
{
  i = node.as<T>();
}
#endif

namespace ls {

LSTopic::LSTopic()
{
  initParam();
  scan_sub_ = n_.subscribe("/scan", 1, &LSTopic::scanCallback, this);
  odom_sub_ = n_.subscribe("/odom", 1, &LSTopic::odomCallback, this);
  coresensor_sub_ = n_.subscribe("/mobile_base/sensors/core", 1, &LSTopic::coreSensorCallback, this);
  path_sub_ = n_.subscribe("/move_base/NavfnROS/plan", 1, &LSTopic::pathCallback, this);
  map_sub_ = n_.subscribe("map", 1, &LSTopic::mapCallback, this);


  pose_pub_ = n_.advertise<geometry_msgs::Pose>("lspose", 1);
  scan_pub_ = n_.advertise<lsrobot::LSPoints>("lsscan", 1);
  path_pub_ = n_.advertise<lsrobot::LSPoints>("lspath", 1);
  state_pub_ = n_.advertise<std_msgs::Int8>("lsstate", 1);
  quality_pub_ = n_.advertise<std_msgs::Int8>("lsquality", 1);
  cost_map_pub_ = n_.advertise<nav_msgs::OccupancyGrid>("/cost_map",10);
  timer_ = n_.createTimer(ros::Duration(1.0), &LSTopic::stateTimer, this);
  state_ = LSState::instance();
}

LSTopic::~LSTopic()
{

}

void LSTopic::initParam()
{
  ros::NodeHandle private_nh_("~");
  private_nh_.param("degree_space", beam_space_, 6);
  private_nh_.param("dist_space", dist_space_, 0.1);
  ROS_INFO("Get parameters: degree space: %d", beam_space_);
  get_costmap_ = false;
}

void LSTopic::odomCallback(const nav_msgs::OdometryConstPtr &odom)
{
  geometry_msgs::Pose pose;

  LS_STATE_ENUM state = state_->getState();
  if (state == LS_STATE_ERROR)
  {
    ROS_ERROR_ONCE("ROBOT state = %s", LSES::enum2str(state));
    return;
  }
  else if (state == LS_STATE_IDLE)
  {
    // use odom pose
//    ROS_INFO("use odom pose");
    pose.position = odom->pose.pose.position;
    pose.orientation = odom->pose.pose.orientation;
  }
  else
  {
    // use tf pose
//    ROS_INFO("use tf pose");
    bool rtn = true;
    rtn = getTfPose(pose);
    if (!rtn)
    {
//      ROS_INFO("cannot get tf pose, use odom pose");
      pose.position = odom->pose.pose.position;
      pose.orientation = odom->pose.pose.orientation;
    }
  }

  lspose_ = pose;
  pose_pub_.publish(pose);
}

void LSTopic::coreSensorCallback(const kobuki_msgs::SensorStateConstPtr &sensor)
{

}

void LSTopic::scanCallback(const sensor_msgs::LaserScanConstPtr &scan)
{
  LS_STATE_ENUM state;
  state = state_->getState();
  if (state != LS_STATE_MAPPING && state != LS_STATE_NAVIGATION)
    return;

  ROS_INFO_ONCE("SCAN callback");
  lsrobot::LSPoints lsscan;
  sensor_msgs::PointCloud pc, pct;
  pc.header.frame_id = scan->header.frame_id;
  pc.header.stamp = ros::Time(0);

  std::vector<double> scan_range;
  scan_range.assign(scan->ranges.begin(), scan->ranges.end());

  for (int i = 0; i < scan_range.size(); i++)
  {
    if (scan_range[i] > scan->angle_max || scan_range[i] < scan->range_min)
    {
      continue;
    }
    double theta = scan->angle_min + i*scan->angle_increment;

    geometry_msgs::Point32 pt;
    pt.x = scan_range[i] * std::cos(theta);
    pt.y = scan_range[i] * std::sin(theta);
    pt.z = 0;
    pc.points.push_back(pt);
  } // end for

  try
  {
    std::string laser_frame = scan->header.frame_id.c_str();
    tf_.waitForTransform("map", laser_frame, ros::Time::now(), ros::Duration(0.2));
    tf_.transformPointCloud("map", pc, pct);
  }
  catch(tf::TransformException &e)
  {
    ROS_WARN_THROTTLE(5,"can not find transformation between laser frame and map frame");
  }

//  ROS_INFO("pct.points.size() = %d", pct.points.size());
  if (pct.points.size() > 0)
  {
    double costmap_sum = 0.0;
    for (int i = 0; i < pct.points.size(); i++)
    {
      if (i%beam_space_ == 0)
      {
        lsrobot::LSPoint lspt;
        lspt.x = pct.points[i].x;
        lspt.y = pct.points[i].y;
        lspt.z = 0;

        lsscan.points.push_back(lspt);
      }

//      // calc points quality
      if (get_costmap_)
      {
        cv::Point pt_world;
        pt_world = world2Map(pct.points[i].x, pct.points[i].y);
        pt_world.y = map_height_-pt_world.y;
        int cost;
        cost = cost_mat_.at<uchar>(pt_world);
        costmap_sum += cost;
      }
    }

    if (get_costmap_)
    {
      double q = costmap_sum / (pct.points.size()*100) * 100;
//      ROS_INFO("quality = %f", q);
      std_msgs::Int8 quality;
      quality.data = (int)q;
      quality_pub_.publish(quality);
    }
    lsscan.stamp = ros::Time::now();
    scan_pub_.publish(lsscan);
  }
}

void LSTopic::pathCallback(const nav_msgs::PathConstPtr &path)
{
//  ROS_INFO("PATH callback");
  if(path->poses.size() <= 0)
  {
    return;
  }
  lsrobot::LSPoint lspose;
  lsrobot::LSPoints lspath;
  geometry_msgs::Point pre_p;
  pre_p = path->poses[0].pose.position;

  geometry_msgs::Point curr_p;
  curr_p = pre_p;

  geometry_msgs::Quaternion q;

  for (int i = 1; i < path->poses.size(); i++)
  {
    curr_p = path->poses[i].pose.position;
    double distance = getDistance(curr_p.x, curr_p.y, pre_p.x, pre_p.y);
    if (distance > dist_space_)
    {
      pre_p = curr_p;

      lspose.x = curr_p.x;
      lspose.y = curr_p.y;

      lspath.stamp = ros::Time::now();
      lspath.points.push_back(lspose);
    }
  }

  path_pub_.publish(lspath);
}

void LSTopic::mapCallback(const nav_msgs::OccupancyGridPtr &msg)
{
  ROS_INFO("mapCallback");
  map_ = msg;
  map_width_  = msg->info.width;
  map_height_ = msg->info.height;

  cost_mat_ = calcCostMat(msg->data);
//  Display();


  get_costmap_ = true;
  ROS_INFO("width = %d, height = %d", map_width_, map_height_);


}

void LSTopic::costmapCallback(const nav_msgs::OccupancyGridPtr &msg)
{
  ROS_INFO("costmapCallback");
//  GenerateGlobalMapCostMatrix(msg);
  costmap_ = msg;
  std::cout << costmap_->header.frame_id << " "
            << costmap_->info.height << " "
            << costmap_->info.width << " "
            << costmap_->data.size() << " "
            << std::endl;

}

cv::Mat LSTopic::calcCostMat(std::vector<int8_t> data)
{
  std::vector <cv::Point> pts;
  cv::Point pt;
  cv::Mat cv_map(map_->info.height, map_->info.width, CV_8U);
  for(unsigned int y = 0; y < map_->info.height; y++) {
    for(unsigned int x = 0; x < map_->info.width; x++) {
      unsigned int i = x + (map_->info.height - y - 1) * map_->info.width;
      unsigned int j = x + y * map_->info.width;
      if (data[i] == 100) { //occ [0,0.1)
        pt.x = x;
        pt.y = y;
        pts.push_back(pt);
        cv_map.at<uchar>(j) = 100;
      }else{
        cv_map.at<uchar>(j) = 0;
      }
    }
  }
//  cv::imwrite("raw.jpg", cv_map);


  for (int value = 100; value > 0; value = value-10)
  {
    std::vector <cv::Point> pt_tmp;
    pt_tmp.resize(pts.size());
    pt_tmp.assign(pts.begin(), pts.end());

    pts.clear();
    for(int i = 0; i < pt_tmp.size(); i++)
    {
      std::vector<cv::Point> pt_arr;
      generate4Neibor(pt_tmp[i], pt_arr);

      for(int j = 0; j < pt_arr.size(); j++)
      {
        if (cv_map.at<uchar>(pt_arr[j]) == 0)
        {
          cv_map.at<uchar>(pt_arr[j]) = value;
          pts.push_back(pt_arr[j]);
        }
      }

    }
  }

//  cv::imwrite("costmap.jpg", cv_map);
  return cv_map;
//  cv::waitKey(0);
}

void LSTopic::generate4Neibor(cv::Point pt_curr, std::vector<cv::Point> &pt_v)
{
  // up down left righ
  cv::Point up;
  cv::Point down;
  cv::Point left;
  cv::Point right;

  up.x = pt_curr.x;
  up.y = pt_curr.y-1;
  if (up.y > 0 && up.y < map_height_)
    pt_v.push_back(up);

  down.x = pt_curr.x;
  down.y = pt_curr.y+1;
  if (down.y > 0 && down.y < map_height_)
    pt_v.push_back(down);

  left.x = pt_curr.x-1;
  left.y = pt_curr.y;
  if (left.y > 0 && left.y < map_width_)
    pt_v.push_back(left);

  right.x = pt_curr.x+1;
  right.y = pt_curr.y;
  if (right.y > 0 && right.y < map_width_)
    pt_v.push_back(right);

}

LSTopic *LSTopic::instance()
{
  static LSTopic obj;
  return &obj;
}

int LSTopic::saveCVMap(std::string map_name)
{
  int error_code = 0;
  ROS_INFO("saveCVMap a %d X %d map @ %.3f m/pix",
           map_->info.width,
           map_->info.height,
           map_->info.resolution);

  cv::Mat cv_map(map_->info.height, map_->info.width, CV_8U);
  for(unsigned int y = 0; y < map_->info.height; y++) {
    for(unsigned int x = 0; x < map_->info.width; x++) {
      unsigned int i = x + (map_->info.height - y - 1) * map_->info.width;
      unsigned int j = x + y * map_->info.width;
      if (map_->data[i] == 0) { //occ [0,0.1)
        cv_map.at<uchar>(j) = 254;
      } else if (map_->data[i] == +100) { //occ (0.65,1]
        cv_map.at<uchar>(j) = 000;
      } else { //occ [0.1,0.65]
        cv_map.at<uchar>(j) = 205;
      }
    }
  }

  if (map_name == "")
  {
    error_code = LSSV_SAVE_MAP_ERROR;
    return error_code;
  }
  std::string mapdatafile = map_name + ".png";
  bool rtn = cv::imwrite(mapdatafile, cv_map);
  if (!rtn)
  {
    error_code = LSSV_SAVE_MAP_ERROR;
    ROS_ERROR("save map error");
    return error_code;
  }

  std::string mapmetadatafile = map_name + ".yaml";
  ROS_INFO("Writing map occupancy data to %s", mapmetadatafile.c_str());
  FILE* yaml = fopen(mapmetadatafile.c_str(), "w");

  geometry_msgs::Quaternion orientation = map_->info.origin.orientation;
  tf::Matrix3x3 mat(tf::Quaternion(orientation.x, orientation.y, orientation.z, orientation.w));
  double yaw, pitch, roll;
  mat.getEulerYPR(yaw, pitch, roll);

  fprintf(yaml, "image: %s\nresolution: %f\norigin: [%f, %f, %f]\nnegate: 0\noccupied_thresh: 0.65\nfree_thresh: 0.196\n\n",
          mapdatafile.c_str(), map_->info.resolution, map_->info.origin.position.x, map_->info.origin.position.y, yaw);

  fclose(yaml);

  return error_code;
}

geometry_msgs::Pose LSTopic::getPose()
{
  return lspose_;
}

void LSTopic::subscribe(std::string topic)
{
  costmap_sub_ = n_.subscribe(topic, 10, &LSTopic::costmapCallback, this);
}

void LSTopic::unsubscribe(std::string topic)
{
  costmap_sub_.shutdown();
  get_costmap_ = false;
  costmap_->data.clear();
}



void LSTopic::stateTimer(const ros::TimerEvent &)
{
  std_msgs::Int8 state;
  state.data = (int8_t)state_->getState();
  state_pub_.publish(state);
}

bool LSTopic::getTfPose(geometry_msgs::Pose& pose)
{
  tf::Stamped<tf::Pose> global_pose;
  global_pose.setIdentity();
  tf::Stamped<tf::Pose> robot_pose;
  robot_pose.setIdentity();
  robot_pose.frame_id_="base_footprint";
  robot_pose.stamp_=ros::Time();
  try
  {
    tf_.waitForTransform("map", "base_footprint", ros::Time(0), ros::Duration(1.0));
    tf_.transformPose("map", robot_pose, global_pose);
    pose.position.x = global_pose.getOrigin().x();
    pose.position.y = global_pose.getOrigin().y();
    pose.position.z = global_pose.getOrigin().z();

    pose.orientation.x = global_pose.getRotation().getX();
    pose.orientation.y = global_pose.getRotation().getY();
    pose.orientation.z = global_pose.getRotation().getZ();
    pose.orientation.w = global_pose.getRotation().getW();
  }
  catch (tf::LookupException& ex)
  {
    ROS_ERROR_THROTTLE(1.0, "No Transform available Error looking up robot pose: %s\n", ex.what());
    return false;
  } catch (tf::ConnectivityException& ex)
  {
    ROS_ERROR_THROTTLE(1.0, "Connectivity Error looking up robot pose: %s\n", ex.what());
    return false;
  } catch (tf::ExtrapolationException& ex)
  {
    ROS_ERROR_THROTTLE(1.0, "Extrapolation Error looking up robot pose: %s\n", ex.what());
    return false;
  }
  return true;
}

double LSTopic::getDistance(double x1, double y1, double x2, double y2)
{
  return std::sqrt((y2-y1)*(y2-y1) + (x2-x1)*(x2-x1));
}

cv::Point LSTopic::world2Map(double x, double y)
{
  cv::Point pt(0,0);
  if (map_->info.resolution <= 0)
    return pt;

  pt.x = (x - map_->info.origin.position.x)/ map_->info.resolution;
  pt.y = (y - map_->info.origin.position.y)/ map_->info.resolution;
  return pt;
}


void LSTopic::Display(void)
{
  nav_msgs::OccupancyGrid costmap_;
  costmap_.header.frame_id = "/map";
  costmap_.info.height = 1000;
  costmap_.info.width = 1000;
  costmap_.info.resolution = this->map_->info.resolution;
  costmap_.info.origin.position.x = this->map_->info.origin.position.x;
  costmap_.info.origin.position.y = this->map_->info.origin.position.y;
  costmap_.info.origin.orientation.x = this->map_->info.origin.orientation.x;
  costmap_.info.origin.orientation.y = this->map_->info.origin.orientation.y;
  costmap_.info.origin.orientation.z = this->map_->info.origin.orientation.z;
  costmap_.info.origin.orientation.w = this->map_->info.origin.orientation.w;

  for(int i = 0;i < map_height_;i ++)
  {
    for(int j = 0;j < map_width_;j ++)
    {
      cv::Point pt;
      pt.x = j;
      pt.y = map_height_-i;
      costmap_.data.push_back(cost_mat_.at<char>(pt));
    }
  }
  this->cost_map_pub_.publish(costmap_);
}

}
