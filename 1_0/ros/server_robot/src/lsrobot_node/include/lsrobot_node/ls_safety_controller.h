/*******************************************************
@company: Copyright (C) 2018, Leishen Intelligent System
@product: LS_SERVER_ROBOT
@filename:
@brief:
@version:       date:       author:     comments:
@v1.0           18-7-6     fu          new
*******************************************************/
#ifndef LS_SAFETY_CONTROLLER_H
#define LS_SAFETY_CONTROLLER_H
#include "ls.h"
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <kobuki_msgs/SensorState.h>
#include <actionlib_msgs/GoalStatusArray.h>
#include <boost/thread.hpp>
#include "ls_state.h"
#include "ls_topic.h"

typedef enum{
  SONAR_MIN = -1,
  RightFront,
  Center,
  LeftFront,
  UpFront,
  SONAR_MAX
}SonarEnum;

typedef struct{
  int id;
  std::list <int8_t> dist;
  int valid_sum;
}SonarData;

namespace ls {

class LSSafetyController
{
public:
  static LSSafetyController* instance();

private:
  LSSafetyController();
  ~LSSafetyController();
  void sensorCallback(const kobuki_msgs::SensorStateConstPtr &msg);
  void movestatusCallback(const actionlib_msgs::GoalStatusArrayConstPtr &msg);

  bool isAudoDocking();

  ros::NodeHandle n_;
  ros::Subscriber safety_sub_;
  ros::Subscriber status_sub_;
  ros::Publisher safety_pub_;


  LSState *state_;
  LSTopic *topic_;
  geometry_msgs::Pose pre_pose_;
  geometry_msgs::Pose curr_pose_;

  SonarData sonar_[SONAR_MAX];
  double safety_time_;
  double safety_vel_x_;
  double safety_vel_z_;
  bool is_move_;

};

}

#endif // LS_SAFETY_CONTROLLER_H
