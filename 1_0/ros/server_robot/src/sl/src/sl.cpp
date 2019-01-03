/*
程序的任务是协助amcl定位
1计算出当前能扫描到地图的中心点。
2驱动到该点。
3旋转360度返回1直到粒子完全收敛。

*/
/*
探寻
1 记录A节点的定位
2 记录B节电的定位
3 用坐标系匹配两个定位
4 如果成功则发布定位
5 如果失败则删除A返回2 



*/
#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <nav_msgs/Odometry.h>
#include "laser.h"
#include "odom.h"

#include <tf/transform_listener.h>

//call /global_location
#include <std_srvs/Empty.h>
#include <std_msgs/Empty.h>
#include <std_msgs/Bool.h>
#include <nav_msgs/GetMap.h>
#include <std_srvs/Trigger.h>
#include <vector>
#include <deque>
#include <cmath>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include "lsrobot/cmd.h"
#define PI 3.14159265


bool isNaN(double a)
{
  return boost::math::isnan(a);

}
static double normalize(double z)
{
  return atan2(sin(z),cos(z));

}
static double angle_diff(double a,double b)
{
  double d1,d2;
  a=normalize(a);
  b=normalize(b);
  d1=a-b;
  d2=2*M_PI-fabs(d1);
  if(d1>0)
    d2*=-1.0;
  if(fabs(d1)<fabs(d2))
    return(d1);
  else
    return(d2);
}

class Area
{
public:
  Area(const LaserData* scan);
  bool dccarea;
  bool stoparea;
private:
  double r;
  double a_up;
  double b_down;
  void free(const LaserData* scan);
};
Area::Area(const LaserData* scan)
{
  r=0.0;
  a_up=0.0;
  b_down=0.0;
  
  free(scan);

}
void Area::free(const LaserData* scan)
{
  
  std::vector<double> mmx;
  std::vector<double> mmy;
  for(int i=270;i!=360;i++)
  {
    mmx.push_back(scan->mx[i]);
    mmy.push_back(scan->my[i]);
  }
  for(int i=0;i<90;i++)
  {
    mmx.push_back(scan->mx[i]);
    mmy.push_back(scan->my[i]);
  }
  for(int i=0;i<mmx.size();i++)
  {

    if(mmy[i]<r&&mmy[i]>-1*r)
    {
      if(mmx[i]<a_up&&mmx[i]>b_down)
      {
        dccarea=true;
        stoparea=false;
      }
      else if(mmx[i]<b_down)
      {
        dccarea=false;
        stoparea=true;
      }
      
    }
    else
    {
      stoparea=false;
      dccarea=false;
    }
    
  }
}
class mpose
{
public:
  double mposex;
  double mposey;
  double mposetheta;

};

class SL
{
public:
  SL();
  ~SL();
  //get command stop
 // bool receiveStop(std_srvs::Trigger::Request &req,std_srvs::Trigger::Response &res);
  //获得暂停命令
 // bool receivePause(std_srvs::Trigger::Request &req,std_srvs::Trigger::Response &res);
 
  //计算面积中心
 
  void calculationAreaCenter();
  //send vel angle to finddoor,target to areaCenter
  
  void loop();
 
private:
  ros::NodeHandle nh_;
  int best_dir;
  double tolerance;
  double k_line;
  double min_vel;
  double max_vel;
  int timec1,timec2;
  int tim;
  double error_dis;

  double px;
  double py;
  double ptheta;
  double cov;
  double a;
  double vel_x;  
  bool onceorloop;
  bool dccarea;
  bool stoparea;
  std::vector<double> laser_data;
  LaserData *laserdata;
  OdometryData *odomdata;
  ros::Subscriber laserSubscriber;
  ros::Subscriber odomSubscriber;
  ros::Subscriber amclposeSubscriber;
  ros::Publisher pub_vel;
  ros::Publisher sl_done;
  ros::Timer timer;
  ros::ServiceClient gl;
  ros::Subscriber amcl_service;
  int find_dir();
  bool maching(const mpose& pose1,const mpose& pose2,const double& in);
  void odomCallback(const nav_msgs::OdometryConstPtr &odom);  
  void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan);
  void timeCallback(const ros::TimerEvent& event);
  void amclposeCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& pose);
  void amclServiceCallback(const std_msgs::Empty::ConstPtr& msg);
  bool moveforward(const LaserData* scan);
  void turn(const int& ix);
}; 
SL::SL()
{

  ros::NodeHandle node("~");
  laserdata=NULL;
  odomdata=NULL;
  for(int i=0;i<180;i++)
  {
    laser_data.push_back(0.0);
  }
  node.param("tolerance",tolerance,0.1);
  node.param("ang",a,0.2);
  node.param("vel",vel_x,0.1);
  node.param("k_line",k_line,0.3);
  node.param("min_vel",min_vel,0.3);
  node.param("max_vel",max_vel,0.5);
  node.param("timec1",timec1,10);
  node.param("timec2",timec2,30);
  node.param("error_dis",error_dis,0.3);
  node.param("onceorloop",onceorloop,false);
  laserSubscriber=nh_.subscribe("/scan",1000,&SL::scanCallback,this);
  odomSubscriber=nh_.subscribe("/odom",1,&SL::odomCallback,this);
  pub_vel=nh_.advertise<geometry_msgs::Twist>("/cmd_vel_mux/input/navi",1);
  timer=nh_.createTimer(ros::Duration(1),&SL::timeCallback,this);
  gl= nh_.serviceClient<std_srvs::Empty>("global_localization");
  amcl_service = nh_.subscribe("/self_localization", 1, &SL::amclServiceCallback, this);
  sl_done = nh_.advertise<std_msgs::Bool>("/self_localization/done",1, true);
  amclposeSubscriber=nh_.subscribe("/amcl_pose",10,&SL::amclposeCallback,this);

}
SL::~SL()
{

}

void SL::amclServiceCallback(const std_msgs::Empty::ConstPtr& msg)
{
  std_msgs::Bool bool_msg;
  bool_msg.data = false;
  sl_done.publish(bool_msg);
  ROS_INFO("self localization while amcl brings up!");
  loop();
  bool_msg.data = true;
  sl_done.publish(bool_msg);
}


void SL::amclposeCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& pose)
{
  px=pose->pose.pose.position.x;
  py=pose->pose.pose.position.y;
  tf::Quaternion quat;
  tf::quaternionMsgToTF(pose->pose.pose.orientation,quat);
  double roll,pitch,yaw;
  tf::Matrix3x3(quat).getRPY(roll,pitch,yaw);
  ptheta=yaw;
  //cov=pose->pose.covariance;
  //ROS_ERROR("px:%f,py:%f,ptheta:%f,cov:%f",px,py,ptheta,cov);
}

void SL::scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
  LaserData data(scan);

  laserdata = &data;
  for(int i=270;i!=360;i++)
  {
    laser_data[i-270]=laserdata->mRange[i];
//    ROS_ERROR("%f",laser_data[i-270]);
//    ROS_ERROR("%d",i-270);
  }
  for(int i=0;i<90;i++)
  {
    laser_data[i+90]=laserdata->mRange[i];
//    ROS_ERROR("%f",laser_data[i+90]);
//    ROS_ERROR("%d",i+90);
  }
}

void SL::odomCallback(const nav_msgs::OdometryConstPtr &odom)
{
  OdometryData data(odom);
  odomdata = &data;
}
void SL::timeCallback(const ros::TimerEvent& event)
{
  
  tim+=1;
  //ROS_ERROR("TIME:%d",tim);
}
bool SL::maching(const mpose& pose1,const mpose& pose2,const double& in)
{
  
}
void SL::turn(const int& ix)
{
  double angle=0;
  switch(ix)
  {
    case 4:
      angle=0;
      break;
    case 5:
      angle=20*PI/180;
      break;
    case 6:
      angle=40*PI/180;
      break;
    case 7:
      angle=60*PI/180;
      break;
    case 8:
      angle=80*PI/180;
      break;
    case 0:
      angle=-80*PI/180;
      break;
    case 1:
      angle=-60*PI/180;
      break;
    case 2:
      angle=-40*PI/180;
      break;
    case 3:
      angle=-20*PI/180;
  }

  angle=angle+odomdata->Theta;
  geometry_msgs::Twist cmd_vel;
  ros::Rate r(10);
  double angled=0;
  while(ros::ok())
  {
    angled=angle-odomdata->Theta;
    if(angled<0.05)
    {break;}
    else
    {
      cmd_vel.angular.z=angled;
    }
    cmd_vel.linear.x=0.0;
    ROS_ERROR("angle:%f",angled);
    //pub_vel.publish(cmd_vel);
    ros::spinOnce();
    r.sleep();
  }

}
bool SL::moveforward(const LaserData* scan)
{
  geometry_msgs::Twist cmd_vel;
  ros::Rate r3(10);
  cmd_vel.linear.x=0.2;
 
  
  while(ros::ok())
  {
    Area area(scan);

    r3.sleep();
    ros::spinOnce();
    if(area.dccarea)
    {
      double crx=odomdata->rateX/10;
      cmd_vel.linear.x=odomdata->rateX-crx;
    }
    else if(area.stoparea)
    {
      cmd_vel.linear.x=0.0;
      return true;
    }

    //pub_vel.publish(cmd_vel);
  }

}
//分区：减速区0.3和停止区0.1
//计算某个方向上的权直
int SL::find_dir()
{
  
   
  //最远探测距离为2m
  const double l_max=2.0;
  double l_temp=0;
//180度数据  


  double temp=0;
  int number=0;
//将激光数据分成9个20度的扇区
  for(int i=0;i < laser_data.size();i+20)
  {

    for(int j=i;j<20+i;j++)
    {
       if(laser_data[j]>l_max)
       { l_temp += l_max;}
       else
       {l_temp +=laser_data[j];}
    } 
    if(l_temp>temp)
    {
      temp=l_temp;
      number=i%20;
    }
  }
  ROS_ERROR("N:%d",number);
  return number;
 
}

void SL::loop()
{
  double scan_dis=0.0;
  int scan_num=0;
  bool betomove=false;
  std_srvs::Empty srv;
  ros::Rate r2(10);
  int time=tim;
  geometry_msgs::Twist cmd_vel;
  while(ros::ok())
  {
    if(tim-time>timec1)
    {
      if(!betomove)
      {
        betomove=true;
        if(gl.call(srv))
        {}
      }
    }
      
    cmd_vel.angular.z=a;
    cmd_vel.linear.x=vel_x;
    if(tim-time>timec2)
    {
      ROS_INFO("self localization finished!");
      return;
        
    }
    pub_vel.publish(cmd_vel);
    ros::spinOnce();
    r2.sleep();
  }
  
}
/*
void SL::loop()
{
  double scan_dis=0.0;
  int scan_num=0;
  bool betomove=false;
  std_srvs::Empty srv;
  ros::Rate r2(10);
  while(ros::ok())
  {
    ros::spinOnce();
    r2.sleep();
    if(laser_data[0]!=0.0)
    {
      find_dir();
    }
/*
    if(!betomove)
    {
      betomove=true;
      if(gl.call(srv))
      {}
    }
    turn(best_dir);
    
    if(moveforward())
    {}

  }

}
/*
void SL::loop()
{
  double l=0.0;
  double angletarg=0.0;

  //double vel_x=0.0;
  double acc_x=0.0;
  double g_x=0.0,g_y=0.0;

  

  std_srvs::Empty srv;
  geometry_msgs::Twist cmd_vel;
  ros::Rate r(10);
  static int s=0;
  static int time=tim;
  double px1=0.0;
  double px2=0.0;
  double py1=0.0;
  double py2=0.0;
  double distaold=0.0;
  double dista=0.0;
  bool betomove=false;
  while(ros::ok())
  {
    
    ros::spinOnce();
    r.sleep();




/*
    if(!betomove)
    {
      betomove=true;
      if(gl.call(srv))
      {}
    }
      
      cmd_vel.angular.z=a;
      cmd_vel.linear.x=vel_x;
      if(tim-time>timec)
      {
        return;
        
      }
*/
/*
    switch(s)
    {
    case 0:
      if(!betomove)
      {
        betomove=true;
        if(gl.call(srv));
        ROS_ERROR("0\n");
      }
      cmd_vel.angular.z=a;
      cmd_vel.linear.x=vel_x;
  
      if(tim-time>timec)
      {
        ROS_ERROR("TIME");
        px1=px;
        py1=py;   
        if(onceorloop)
        {
          return;
        }
        else
        {
          s=1;
        }
      }      
      break;
    case 1:
      ROS_ERROR("1\n");
      
      angletarg=find_dir();
      a=angle_diff(angletarg,current_theta);
      if(betomove)
      {
        g_x=current_x+error_dis*sin(angletarg);
        g_y=current_y+error_dis*cos(angletarg);
      }
      error_dis=fabs(g_x-current_x)+fabs(g_y-current_y);
      if(betomove)
      {
        distaold=error_dis;
        betomove=false;
      }
      vel_x=std::max(std::min(error_dis *k_line,max_vel),min_vel);
      acc_x=(vel_x-current_ratex)/10;
      vel_x=current_ratex+acc_x;
    
    
      if(error_dis<tolerance&&error_dis!=0)
      { 
        time=0;
          tim=time;
        if(gl.call(srv))       
          s = 2;
        //进入下一个状态，大撒粒子
      }

      cmd_vel.angular.z=a;
      cmd_vel.linear.x=vel_x;
      
      break;   
    case 2:
      ROS_ERROR("2\n");
      a=0.4;
      vel_x=0.0;
      cmd_vel.angular.z=a;
      cmd_vel.linear.x=vel_x;
     //ROS_ERROR("DTIME:%d",time-tim);
      if(tim-time>timec)
      {
        px2=px;
        py2=py;
        dista=fabs(px1-px2)+fabs(py1-py2);
        if(fabs(dista-distaold)<tolerance*2)
        {
          return;
        }
        else
        {
          betomove=false;
          time=0;
          tim=time;
          s=0;
        }
      }
     
    }

    pub_vel.publish(cmd_vel);
    

  }
  
}  

*/

int main(int argc,char **argv)
{
  ros::init(argc,argv,"sl");
  SL sl;
  // sl.loop();
  ros::spin();
  return 0;  

}
