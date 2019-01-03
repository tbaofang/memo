#include "lsrobot_node/ls_safety_controller.h"

namespace ls {
const int max_sonar_num = 10;

LSSafetyController *LSSafetyController::instance()
{
    static LSSafetyController obj;
    return &obj;
}

LSSafetyController::LSSafetyController()
{

  safety_sub_ = n_.subscribe("/mobile_base/sensors/core", 10, &LSSafetyController::sensorCallback, this);
  status_sub_ = n_.subscribe("/move_base/status", 1, &LSSafetyController::movestatusCallback, this);
  safety_pub_ = n_.advertise<geometry_msgs::Twist>("/cmd_vel_mux/input/safety_controller", 1);

  ros::NodeHandle nh("~");
  nh.param("safety_time", safety_time_, 1.0);
  nh.param("safety_vel_x", safety_vel_x_, 0.1);
  nh.param("safety_vel_z", safety_vel_z_, 0.5);
  pre_pose_ = geometry_msgs::Pose();
  curr_pose_ = geometry_msgs::Pose();
  is_move_ = false;
  state_ = LSState::instance();
  topic_ = LSTopic::instance();
}

LSSafetyController::~LSSafetyController()
{

}

void LSSafetyController::sensorCallback(const kobuki_msgs::SensorStateConstPtr &msg)
{
  // get state
  LS_STATE_ENUM state;
  state = state_->getState();
  if (state != LS_STATE_MAPPING && state != LS_STATE_NAVIGATION)
  {
    return;
  }

  if (!is_move_)
  {
    return;
  }

  if (isAudoDocking())
  {
    return;
  }

  // get sonar state
  int8_t data[SONAR_MAX] = {0};

  for(int i = 0; i < SONAR_MAX; i++)
  {
    data[i] = (msg->bumper >> i)&0x01;
  }

  // update sonar data
  for (int i = 0; i < SONAR_MAX; i++)
  {
    if (sonar_[i].dist.size() >= max_sonar_num)
    {
      int temp = sonar_[i].dist.front();
      sonar_[i].dist.pop_front();
      sonar_[i].valid_sum -= temp;

      sonar_[i].dist.push_back(data[i]);
      sonar_[i].valid_sum += data[i];
    }
    else
    {
      sonar_[i].dist.push_back(data[i]);
      sonar_[i].valid_sum += data[i];
    }

  }

  // calc the highest valid sum
  int max_sum = 0;
  SonarEnum max_index = SONAR_MIN;
  for (int i = 0; i < SONAR_MAX; i++)
  {
    if (max_sum < sonar_[i].valid_sum)
    {
      max_sum = sonar_[i].valid_sum;
      max_index = (SonarEnum)i;
    }
  }

  // max valid sonar
  geometry_msgs::Twist cmd_vel;
  bool is_bumper = false;
  switch(max_index)
  {
  case RightFront:
  {
    cmd_vel.linear.x = -safety_vel_x_;
    cmd_vel.angular.z = -safety_vel_z_;
    is_bumper = true;
    break;
  }
  case Center:
  {
    cmd_vel.linear.x = -safety_vel_x_;
    cmd_vel.angular.z = -safety_vel_z_;
    is_bumper = true;
    break;
  }
  case LeftFront:
  {
    cmd_vel.linear.x = -safety_vel_x_;
    cmd_vel.angular.z = safety_vel_z_;
    is_bumper = true;
    break;
  }
  case UpFront:
  {
    cmd_vel.linear.x = -safety_vel_x_;
    cmd_vel.angular.z = 0.0;
    is_bumper = true;
    break;
  }
  default :
    is_bumper = false;
    break;
  }

  // publish vel
  ros::Time start = ros::Time::now();
  ros::Time end = ros::Time::now();

  ros::Rate r(20);
  while (ros::ok() && ((end-start) < ros::Duration(safety_time_)) && is_bumper) {
    safety_pub_.publish(cmd_vel);
    end = ros::Time::now();
    ROS_INFO("time = %f", (end-start).toSec());
    r.sleep();
  }

  for (int i = 0; i < SONAR_MAX; i++)
  {
     sonar_[i].valid_sum = 0;
  }
}

void LSSafetyController::movestatusCallback(const actionlib_msgs::GoalStatusArrayConstPtr &msg)
{
  for (int i = 0; i < msg->status_list.size(); i ++)
  {
    if (msg->status_list[i].status == msg->status_list[i].ACTIVE)
    {
      is_move_ = true;
      break;
    }
    else
    {
      is_move_ = false;
    }
  }
}

bool LSSafetyController::isAudoDocking()
{
  return false;
}

}
