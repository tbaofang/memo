#include "laser.h"
LaserData::LaserData(const sensor_msgs::LaserScan::ConstPtr& scan)
{
  mRangeCount = scan->ranges.size();
  mRanges =  new double[mRangeCount][2];
  mRangeMax = scan->range_max;
  double x=0.0,y=0.0;
  angle_min=scan->angle_min;
  angle_inc=scan->angle_increment;
  angle_inc=fmod(angle_inc + 5*M_PI,2*M_PI)-M_PI;
  for(int i=0;i<mRangeCount;i++)
  {
    if(scan->ranges[i]<=scan->range_min)
    {
      mRanges[i][0]=scan->range_max;
    }
    else 
    {
      mRanges[i][0]=scan->ranges[i];
    }
   
    mRanges[i][1] = angle_min + (i*angle_inc);
    x=mRanges[i][0]*sin(mRanges[i][1]);
    y=mRanges[i][0]*cos(mRanges[i][1]);
    mx.push_back(x);
    my.push_back(y);
    mRange.push_back(mRanges[i][0]);
    nRange.push_back(i);
    
  }
  
}
LaserData::~LaserData()
{
  delete[] mRanges;
}
