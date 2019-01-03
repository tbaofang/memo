/*******************************************************
@company: Copyright (C) 2018, Leishen Intelligent System
@product: LS_SERVER_ROBOT
@filename:
@brief:
@version:       date:       author:     comments:
@v1.0           18-5-26     fu          new
*******************************************************/


#include "lsrobot_node/ls_es.h"

namespace ls {

const char *LSES::bool2str(const int &e)
{
  return (e ? "TRUE" : "FALSE");
}

const char *LSES::bool2str(const int *e)
{
  return (*e ? "TRUE" : "FALSE");
}

const char *LSES::enum2str(LS_STATE_ENUM e)
{
  const char * str = "UNKNOW_LS_STATE";

  switch (e) {
  case LS_STATE_IDLE:
    str = "LS_STATE_IDLE";
    break;
  case LS_STATE_MAPPING:
    str = "LS_STATE_MAPPING";
    break;
  case LS_STATE_LOAD_MAP:
    str = "LS_STATE_LOAD_MAP";
    break;
  case LS_STATE_NAVIGATION:
    str = "LS_STATE_NAVIGATION";
    break;
  case LS_STATE_ERROR:
    str = "LS_STATE_NAVIGATION";
    break;
  default:
    break;
  }

  return str;
}

const char *LSES::enum2str(LS_CMD_CODE_ENUM e)
{
  const char * str = "UNKNOW_LS_CMD_CODE";

  switch (e) {
  case LS_CMD_START_MAP:
    str = "LS_CMD_START_MAP";
    break;
  case LS_CMD_SAVE_MAP:
    str = "LS_CMD_SAVE_MAP";
    break;
  case LS_CMD_STOP_MAP:
    str = "LS_CMD_STOP_MAP";
    break;
  case LS_CMD_IMPORT_MAP:
    str = "LS_CMD_IMPORT_MAP";
    break;
  case LS_CMD_START_AMCL:
    str = "LS_CMD_START_AMCL";
    break;
  case LS_CMD_STOP_AMCL:
    str = "LS_CMD_STOP_AMCL";
    break;
  default:
    break;
  }

  return str;
}


}
