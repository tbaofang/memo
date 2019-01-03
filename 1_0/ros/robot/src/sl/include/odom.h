#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_listener.h>
class OdometryData
{
public:
  OdometryData(const nav_msgs::OdometryConstPtr &odom);
  double X;
  double Y;
  double Theta;
  double rateX;
  double rateY;
  double rateYaw;
};
