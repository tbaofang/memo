#ifndef GMAPPING_SERVER_H
#define GMAPPING_SERVER_H

#include "slam_gmapping.h"
#include "std_srvs/Empty.h"
#include "lsrobot/cmd.h"

class GmappingServer
{
public:
  GmappingServer();
  ~GmappingServer();

private:
  bool cmdCallback(lsrobot::cmd::Request &req,
                     lsrobot::cmd::Response &res);

private:
  ros::NodeHandle n;
  SlamGMapping *slam_gmapping;

  ros::ServiceServer service;


};

#endif // GMAPPING_SERVER_H
