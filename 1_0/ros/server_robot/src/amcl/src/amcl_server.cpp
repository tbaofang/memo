#include "amcl/amcl_server.h"

AmclServer::AmclServer()
{
  // sl= n.serviceClient<std_srvs::Empty>("/self_localization");
  service = n.advertiseService("amcl_control", &AmclServer::cmdCallback, this);
  amcl_node_ptr = NULL;
}

AmclServer::~AmclServer()
{

}

bool AmclServer::cmdCallback(lsrobot::cmd::Request &req, lsrobot::cmd::Response &res)
{
  ROS_INFO("AmclServer::cmdCallback cmd = %ld", req.cmd);
  if (req.cmd == 1)
  {
    if (amcl_node_ptr == NULL)
    {
      amcl_node_ptr = new AmclNode();
      ROS_ASSERT(amcl_node_ptr);

      // std_srvs::Empty srv;
      // sl.call(srv);
    }
  }
  else if (req.cmd == 2)
  {
    if (amcl_node_ptr != NULL)
    {
      delete amcl_node_ptr;
      amcl_node_ptr = NULL;
    }
  }
  else
  {
    ROS_WARN("cmd error!");
  }

  ROS_INFO("AmclServer::cmdCallback res = %ld", res.result);
  return true;
}

#define USAGE "USAGE: amcl"

boost::shared_ptr<AmclServer> amcl_server_ptr;

void sigintHandler(int sig)
{
  // Save latest pose as we're shutting down.
  amcl_server_ptr->amcl_node_ptr->savePoseToServer();
  ros::shutdown();
}

int
main(int argc, char** argv)
{
  ros::init(argc, argv, "amcl_server");
  ros::NodeHandle nh;

  // Override default sigint handler
  signal(SIGINT, sigintHandler);

  // Make our node available to sigintHandler
  amcl_server_ptr.reset(new AmclServer());


  ros::spin();

  // To quote Morgan, Hooray!
  return(0);
}
