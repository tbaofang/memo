#ifndef MAP_SERVER_H
#define MAP_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <fstream>

#include "ros/ros.h"
#include "ros/console.h"
#include "map_server/image_loader.h"
#include "nav_msgs/MapMetaData.h"
#include "yaml-cpp/yaml.h"
#include <lsrobot/cmd.h>
#include "lsrobot/LSVirtualWall.h"
#include "lsrobot/LSVirtualWalls.h"
#include <std_msgs/Int8.h>
#include <opencv2/opencv.hpp>
class MapServer
{
public:
  MapServer(const std::string& file_name, double resolution);
  int loadMap();
private:
  /** Callback invoked when someone requests our service */
  bool mapCallback(nav_msgs::GetMap::Request  &req,
                   nav_msgs::GetMap::Response &res );


  void vwallCallback(const lsrobot::LSVirtualWallsConstPtr &vwalls);

  cv::Point world2Map(double x, double y);

  /** The map data is cached here, to be sent out to service callers
   */
  ros::NodeHandle n;
  ros::Publisher map_pub;
  ros::Publisher metadata_pub;
  ros::ServiceServer service;
  bool deprecated;

  std::string fname;
  double res;

  nav_msgs::MapMetaData meta_data_message_;
  nav_msgs::GetMap::Response map_resp_;
  ros::Subscriber vwall_sub_;
  ros::Subscriber state_sub_;
};

class MapServerControl
{
public:
  MapServerControl();
  ~MapServerControl();

private:
  bool cmdCallback(lsrobot::cmd::Request &req,
                  lsrobot::cmd::Response &res);
private:
  ros::NodeHandle n;
  MapServer *map_server;
  ros::ServiceServer service;



};


#endif // MAP_SERVER_H
