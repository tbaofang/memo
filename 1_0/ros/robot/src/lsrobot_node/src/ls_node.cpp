/*******************************************************
@company: Copyright (C) 2018, Leishen Intelligent System
@product: LS_SERVER_ROBOT
@filename:
@brief:
@version:       date:       author:     comments:
@v1.0           18-5-26     fu          new
*******************************************************/

#include <ros/ros.h>
#include "lsrobot_node/ls_server.h"
#include "lsrobot_node/ls_topic.h"
#include "lsrobot_node/ls_safety_controller.h"
int main(int argc, char **argv)
{
  ros::init(argc, argv, "ls_node");
  ros::NodeHandle nh;

  ls::LSServer ls_server;
  ls::LSTopic *ls_topic = ls::LSTopic::instance();
  ls::LSSafetyController *ls_satety = ls::LSSafetyController::instance();
  ros::spin();
}

