#ifndef AMCL_SERVER_H
#define AMCL_SERVER_H

#include <ros/ros.h>
#include "amcl/amcl_node.h"
#include <std_srvs/Empty.h>
#include <lsrobot/cmd.h>

class AmclServer{
public:
  AmclServer();
  ~AmclServer();

  AmclNode *amcl_node_ptr;
private:
  bool cmdCallback(lsrobot::cmd::Request &req,
                     lsrobot::cmd::Response &res);

private:
  ros::NodeHandle n;
  // ros::ServiceClient sl;
  ros::ServiceServer service;

};

#endif // AMCL_SERVER_H
