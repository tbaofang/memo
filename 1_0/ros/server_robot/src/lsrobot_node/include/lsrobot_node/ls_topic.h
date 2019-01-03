/*******************************************************
@company: Copyright (C) 2018, Leishen Intelligent System
@product: LS_SERVER_ROBOT
@filename:
@brief:
@version:       date:       author:     comments:
@v1.0           18-5-26     fu          new
*******************************************************/
#ifndef LS_TOPIC_H
#define LS_TOPIC_H

#include <ros/ros.h>
#include <kobuki_msgs/SensorState.h>
#include <nav_msgs/Odometry.h>
#include <nav_msgs/Path.h>
#include <tf/transform_listener.h>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Pose.h>
#include <std_msgs/Int8.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <fstream>
#include <yaml-cpp/yaml.h>
#include "opencv2/opencv.hpp"

#include "nav_msgs/GetMap.h"
#include "lsrobot_node/ls.h"
#include "lsrobot_node/ls_state.h"
#include "lsrobot/LSVirtualWall.h"
#include "lsrobot/LSVirtualWalls.h"


/** Map mode
 *  Default: TRINARY -
 *      value >= occ_th - Occupied (100)
 *      value <= free_th - Free (0)
 *      otherwise - Unknown
 *  SCALE -
 *      alpha < 1.0 - Unknown
 *      value >= occ_th - Occupied (100)
 *      value <= free_th - Free (0)
 *      otherwise - f( (free_th, occ_th) ) = (0, 100)
 *          (linearly map in between values to (0,100)
 *  RAW -
 *      value = value
 */
enum MapMode {TRINARY, SCALE, RAW};
#define MAP_IDX(sx, i, j) ((sx) * (j) + (i))

namespace ls {
class LSTopic{
public:
  static LSTopic * instance();
  int saveCVMap(std::string map_name);
  int loadMap(std::string fname);
  geometry_msgs::Pose getPose();
  void subscribe(std::string topic);
  void unsubscribe(std::string topic);

private:
  LSTopic();
  ~LSTopic();
  void initParam();
  void odomCallback(const nav_msgs::OdometryConstPtr &odom);
  void coreSensorCallback(const kobuki_msgs::SensorStateConstPtr &sensor);
  void scanCallback(const sensor_msgs::LaserScanConstPtr &scan);
  void pathCallback(const nav_msgs::PathConstPtr &path);
  void mapCallback(const nav_msgs::OccupancyGridPtr &msg);
  void vwallCallback(const lsrobot::LSVirtualWallsConstPtr &vwalls);
  void costmapCallback(const nav_msgs::OccupancyGridPtr &msg);

  cv::Mat calcCostMat(std::vector<int8_t> data);
  void generate4Neibor(cv::Point pt_curr, std::vector <cv::Point> &pt_v);
  void stateTimer(const ros::TimerEvent&);
  bool getTfPose(geometry_msgs::Pose &pose);
  double getDistance(double x1, double y1, double x2, double y2);

  cv::Point world2Map(double x, double y);
  void Display(void);

  ros::NodeHandle n_;
  ros::NodeHandle private_nh_;
  ros::Subscriber scan_sub_, odom_sub_, coresensor_sub_;
  ros::Subscriber path_sub_;
  ros::Subscriber map_sub_;
  ros::Subscriber costmap_sub_;

  ros::Publisher pose_pub_;
  ros::Publisher scan_pub_;
  ros::Publisher path_pub_;
  ros::Publisher state_pub_;
  ros::Publisher quality_pub_;
  ros::Publisher cost_map_pub_;
  tf::TransformListener tf_;

  LSState *state_;
  ros::Timer timer_;

  int beam_space_;
  double dist_space_;
  geometry_msgs::Pose lspose_;

  nav_msgs::OccupancyGridPtr map_;
  nav_msgs::OccupancyGridPtr costmap_;
  std::vector<int8_t> map_data_;
  int map_width_;
  int map_height_;
  std::string map_name_;
  bool get_costmap_;

  cv::Mat cost_mat_;
};
}
#endif // LS_TOPIC_H
