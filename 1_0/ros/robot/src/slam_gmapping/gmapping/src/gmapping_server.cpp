#include "gmapping_server.h"


// Signal handling
#include <signal.h>

GmappingServer::GmappingServer()
{
  service = n.advertiseService("gmapping_control", &GmappingServer::cmdCallback, this);
  slam_gmapping = NULL;
}

GmappingServer::~GmappingServer()
{
  delete slam_gmapping;
  slam_gmapping = NULL;
}

bool GmappingServer::cmdCallback(lsrobot::cmd::Request &req,
                                 lsrobot::cmd::Response &res)
{
  ROS_INFO("GmappingServer::cmdCallback cmd = %d", req.cmd);
  if (req.cmd == 1)
  {
    // start
//    delete slam_gmapping;
    if (slam_gmapping == NULL)
    {
      slam_gmapping = new SlamGMapping();
      ROS_ASSERT(slam_gmapping);
      slam_gmapping->startLiveSlam();
      ROS_INFO("cmd = 1,done");
    }
    res.result = 0;
  }
  else if (req.cmd == 2)
  {
    // stop
    if (slam_gmapping != NULL)
    {
      delete slam_gmapping;
      slam_gmapping = NULL;
      ROS_INFO("cmd = 2,done");
    }
    res.result = 0;
  }
  else if (req.cmd == 3)
  {
    // pause
    slam_gmapping->pauseLiveSlam();
  }
  else if (req.cmd == 4)
  {
    slam_gmapping->restartLiveSlam();
  }
  else
  {
    ROS_WARN("cmd error!");
  }

  ROS_INFO("GmappingServer::cmdCallback res = %d", res.result);
  return true;
}

void sigintHandler(int sig)
{
  // Save latest pose as we're shutting down.
//  amcl_node_ptr->savePoseToServer();
  ros::shutdown();
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "slam_gmapping_server");
  ros::NodeHandle nh;

  signal(SIGINT, sigintHandler);

  GmappingServer gs;

  ros::spin();
  return 0;
}
