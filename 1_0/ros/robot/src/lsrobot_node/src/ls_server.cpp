/*******************************************************
@company: Copyright (C) 2018, Leishen Intelligent System
@product: LS_SERVER_ROBOT
@filename:
@brief:
@version:       date:       author:     comments:
@v1.0           18-5-26     fu          new
*******************************************************/
#include <cstdio>
#include "lsrobot_node/ls_server.h"
#include "lsrobot_node/ls_state.h"
#include "lsrobot_node/ls_es.h"
#include "tf/LinearMath/Matrix3x3.h"
using namespace std;



namespace ls {

LSServer::LSServer()
{
  state_ = LSState::instance();
  topic_ = LSTopic::instance();
  ls_service = n_.advertiseService("lsserver_control", &LSServer::cmdCallback, this);
  gmapping_client = n_.serviceClient<lsrobot::cmd>("gmapping_control");
  amcl_client = n_.serviceClient<lsrobot::cmd>("amcl_control");
  map_client = n_.serviceClient<lsrobot::cmd>("map_server_control");


}

LSServer::~LSServer()
{

}

bool LSServer::cmdCallback(lsrobot::cmd::Request &req, lsrobot::cmd::Response &res)
{
  int error_code = LS_OK;
  LS_CMD_CODE_ENUM cmd = (LS_CMD_CODE_ENUM)req.cmd;
  ROS_INFO("cmd = %s", LSES::enum2str(cmd));

  int permitted;
  error_code = state_->cmdPermitted(cmd, permitted);
  ROS_INFO("permitted = %s", LSES::bool2str(permitted));
  if (permitted == 0 || error_code)
  {
    res.result = LSSV_PERMIT_ERROR;
    ROS_INFO("res.result = 0x%x, error_code = 0x%x", res.result, error_code);

    return true;
  }

  ROS_INFO("LSServer Pre_state = %s", LSES::enum2str(state_->getState()));

  switch(cmd)
  {
  case LS_CMD_START_MAP:
  {
    lsrobot::cmd srv;
    srv.request.cmd = 1;
    srv.request.str = req.str;

    ROS_INFO("LS_CMD_START_MAP");
    gmapping_client.call(srv);

    res.result = srv.response.result;
    if (res.result == 0)
    {
      state_->updateState(LS_CMD_START_MAP);
    }
    break;
  }
  case LS_CMD_SAVE_MAP:
  {
    std::string map_name = req.str;
    if(map_name == "")
    {
      res.result = LSSV_SAVE_MAP_ERROR;
      ROS_ERROR("save map error");
      return true;
    }
    //    int rtn = saveMap(map_name);
    int rtn = topic_->saveCVMap(map_name);
    res.result = rtn;
    break;
  }
  case LS_CMD_STOP_MAP:
  {
    ROS_INFO("LS_CMD_STOP_MAP");
    lsrobot::cmd srv;
    srv.request.cmd = 2;
    srv.request.str = req.str;
    gmapping_client.call(srv);

    res.result = srv.response.result;
    if (res.result == 0)
    {
      state_->updateState(LS_CMD_STOP_MAP);
    }
    break;
  }
  case LS_CMD_IMPORT_MAP:
  {
    ROS_INFO("LS_CMD_IMPORT_MAP");
    lsrobot::cmd srv;
    srv.request.cmd = 1;
    srv.request.str = req.str;
    map_client.call(srv);
//    error_code = topic_->loadMap(req.str);

    res.result = srv.response.result;
    if (res.result == 0)
    {
      state_->updateState(LS_CMD_IMPORT_MAP);
    }
    break;
  }
  case LS_CMD_START_AMCL:
  {
    ROS_INFO("LS_CMD_START_AMCL");
    lsrobot::cmd srv;
    srv.request.cmd = 1;
    srv.request.str = req.str;
    amcl_client.call(srv);

    res.result = srv.response.result;
    if (res.result == 0)
    {
      state_->updateState(LS_CMD_START_AMCL);
    }
    break;
  }
  case LS_CMD_STOP_AMCL:
  {
    ROS_INFO("LS_CMD_STOP_AMCL");
    lsrobot::cmd srv;
    srv.request.cmd = 2;
    srv.request.str = req.str;
    amcl_client.call(srv);
    res.result = srv.response.result;

    map_client.call(srv);
    res.result |= srv.response.result;
    if(res.result == 0)
    {
      state_->updateState(LS_CMD_STOP_AMCL);
    }
    break;
  }
  default:
    break;
  }

  ROS_INFO("LSServer After_state = %s", LSES::enum2str(state_->getState()));
  return true;
}


}
