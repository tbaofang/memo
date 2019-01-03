/*******************************************************
@company: Copyright (C) 2018, Leishen Intelligent System
@product: LS_SERVER_ROBOT
@filename:
@brief:
@version:       date:       author:     comments:
@v1.0           18-5-26     fu          new
*******************************************************/
#ifndef LS_SERVER_H
#define LS_SERVER_H

#include <ros/ros.h>

#include "lsrobot/cmd.h"
#include "lsrobot_node/ls.h"
#include "lsrobot_node/ls_state.h"
#include "lsrobot_node/ls_topic.h"

namespace ls {
class LSServer
{
public:
  LSServer();
  ~LSServer();
  
private:
  bool cmdCallback(lsrobot::cmd::Request &req,
                   lsrobot::cmd::Response &res);
  

  ros::NodeHandle n_;
  ros::ServiceServer ls_service;
  ros::ServiceClient gmapping_client;
  ros::ServiceClient amcl_client;
  ros::ServiceClient map_client;

  LSState *state_;
  LSTopic *topic_;
  ros::Subscriber map_sub_;

  // pub map
  ros::Publisher map_pub_;
};

}

#endif // LS_SERVER_H
