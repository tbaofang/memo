#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <vector>
class LaserData
{
public:
  LaserData(const sensor_msgs::LaserScan::ConstPtr& scan);
  ~LaserData();
  int mRangeCount;
  double mRangeMax;
  double (*mRanges)[2];
  std::vector<double> mRange;
  std::vector<int> nRange;
  std::vector<double> mx;
  std::vector<double> my;
  double angle_min;
  double angle_inc;
};
