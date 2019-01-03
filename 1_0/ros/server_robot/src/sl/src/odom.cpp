#include "odom.h"

OdometryData::OdometryData(const nav_msgs::OdometryConstPtr &odom)
{
  X = odom->pose.pose.position.x;
  Y = odom->pose.pose.position.y;
  Theta = tf::getYaw(odom->pose.pose.orientation);
  rateX = odom->twist.twist.linear.x;
  rateY = odom->twist.twist.linear.y;
  rateYaw=odom->twist.twist.angular.z;
}
