/*******************************************************
@company: Copyright (C) 2018, Leishen Intelligent System
@product: LS_SERVER_ROBOT
@filename:
@brief:
@version:       date:       author:     comments:
@v1.0           18-5-26     fu          new
*******************************************************/
#include "lsrobot_node/ls_state.h"
#include <assert.h>

namespace ls {

const LS_CONTROL_TABLE_STRUCT LSState::control_table[LS_CMD_CODE_MAX - LS_CMD_CODE_MIN -1] = {
  //                  IDLE  MAPPING  LOAD_MAP NAVIGATION  ERROR
  {LS_CMD_START_MAP,    {1,     1,      0,      0,      0}},
  {LS_CMD_SAVE_MAP,     {0,     1,      1,      1,      0}},
  {LS_CMD_STOP_MAP,     {0,     1,      0,      0,      0}},

  {LS_CMD_IMPORT_MAP,   {1,     0,      1,      0,      0}},
  {LS_CMD_START_AMCL,   {0,     0,      1,      1,      0}},
  {LS_CMD_STOP_AMCL,    {0,     0,      0,      1,      0}}
};

const LS_STATE_ENUM LSState::transist_table[LS_STATE_MAX - LS_STATE_MIN - 1][LS_CMD_CODE_MAX - LS_CMD_CODE_MIN - 1] = {
   //            start_map           save_map        stop_map        import_map          start_amcl           stop_amcl
  /* IDLE    */ {LS_STATE_MAPPING, LS_STATE_ERROR,   LS_STATE_ERROR, LS_STATE_LOAD_MAP, LS_STATE_ERROR,      LS_STATE_ERROR},
  /* MAPPING */ {LS_STATE_MAPPING, LS_STATE_MAPPING, LS_STATE_IDLE,  LS_STATE_ERROR,    LS_STATE_ERROR,      LS_STATE_ERROR},
  /* LOADMAP */ {LS_STATE_ERROR,   LS_STATE_LOAD_MAP,   LS_STATE_ERROR,  LS_STATE_LOAD_MAP,LS_STATE_NAVIGATION, LS_STATE_ERROR},
  /* NAV     */ {LS_STATE_ERROR,   LS_STATE_NAVIGATION,   LS_STATE_ERROR,  LS_STATE_ERROR,   LS_STATE_NAVIGATION, LS_STATE_IDLE},
};

LSState *LSState::instance()
{
  static LSState obj;
  return &obj;
}

int LSState::cmdPermitted(LS_CMD_CODE_ENUM cmd_code, int &permitted)
{
  int error_code = LS_OK;
  if (cmd_code <= LS_CMD_CODE_MIN || cmd_code >= LS_CMD_CODE_MAX)
  {
    error_code = LSSV_PERMIT_ERROR;
    return error_code;
  }
  if (control_table[cmd_code-LS_CMD_CODE_MIN-1].cmd_code != cmd_code)
  {
    error_code = LSSV_PERMIT_ERROR;
    return error_code;
  }

  permitted = control_table[cmd_code-LS_CMD_CODE_MIN-1].permitted[robot_state_-LS_STATE_MIN-1];
  if (!permitted)
  {
    error_code = LSSV_PERMIT_ERROR;
  }
  return error_code;
}

int LSState::updateState(LS_CMD_CODE_ENUM cmd_code)
{
  int error_code = LS_OK;
  if (cmd_code <= LS_CMD_CODE_MIN || cmd_code >= LS_CMD_CODE_MAX)
  {
    error_code = LSSV_STATE_ERROR;
    return error_code;
  }
  if (robot_state_ <= LS_STATE_MIN || robot_state_ >= LS_STATE_MAX)
  {
    error_code = LSSV_STATE_ERROR;
    return error_code;
  }

  robot_state_ = transist_table[robot_state_-LS_STATE_MIN-1][cmd_code-LS_CMD_CODE_MIN-1];
  return error_code;
}

LS_STATE_ENUM LSState::getState()
{
  return robot_state_;
}

LSState::LSState():robot_state_(LS_STATE_IDLE)
{

}

LSState::~LSState()
{

}


}
