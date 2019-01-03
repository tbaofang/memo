#ifndef MAP_SAVER_H
#define MAP_SAVER_H

#include <cstdio>
#include "ros/ros.h"
#include "ros/console.h"
#include "nav_msgs/GetMap.h"
#include "tf/LinearMath/Matrix3x3.h"
#include "geometry_msgs/Quaternion.h"
#include <lsrobot/cmd.h>

namespace map_server
{
class MapSaver
{
public:
  MapSaver(const std::string& mapname);
  void mapCallback(const nav_msgs::OccupancyGridConstPtr& map);

  std::string mapname_;
  ros::Subscriber map_sub_;
  bool saved_map_;
};

class MapSaverServer
{
public:
  MapSaverServer();
  ~MapSaverServer();

private:
  bool cmdCallback(lsrobot::cmd::Request &req,
                  lsrobot::cmd::Response &res);
private:
  ros::NodeHandle n;
  MapSaver *map_saver;
  ros::ServiceServer service;

};
}

#endif // MAP_SAVER_H
