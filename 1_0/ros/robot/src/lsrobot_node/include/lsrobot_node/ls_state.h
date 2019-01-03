/*******************************************************
@company: Copyright (C) 2018, Leishen Intelligent System
@product: LS_SERVER_ROBOT
@filename:
@brief:
@version:       date:       author:     comments:
@v1.0           18-5-26     fu          new
*******************************************************/
#ifndef LS_STATE_H
#define LS_STATE_H
#include "ls.h"
namespace ls {

typedef enum{
  LS_STATE_MIN = 0,
  LS_STATE_IDLE,
  LS_STATE_MAPPING,
  LS_STATE_LOAD_MAP,
  LS_STATE_NAVIGATION,
  LS_STATE_ERROR,
  LS_STATE_MAX
}LS_STATE_ENUM;

// cmd enum code
typedef enum{
  LS_CMD_CODE_MIN = 0,

  LS_CMD_START_MAP,
  LS_CMD_SAVE_MAP,
  LS_CMD_STOP_MAP,

  LS_CMD_IMPORT_MAP,
  LS_CMD_START_AMCL,
  LS_CMD_STOP_AMCL,

  LS_CMD_CODE_MAX
}LS_CMD_CODE_ENUM;

typedef struct{
  LS_CMD_CODE_ENUM cmd_code;
  int permitted[LS_STATE_MAX - LS_STATE_MIN - 1];
}LS_CONTROL_TABLE_STRUCT;

class LSState{
public:
  static LSState* instance();
  int cmdPermitted(LS_CMD_CODE_ENUM cmd_code, int &permitted);
  int updateState(LS_CMD_CODE_ENUM cmd_code);
  LS_STATE_ENUM getState();

private:
  explicit LSState();
  ~LSState();

  LS_STATE_ENUM robot_state_;
  static const LS_CONTROL_TABLE_STRUCT control_table[LS_CMD_CODE_MAX - LS_CMD_CODE_MIN -1];
  static const LS_STATE_ENUM transist_table[LS_STATE_MAX - LS_STATE_MIN - 1][LS_CMD_CODE_MAX - LS_CMD_CODE_MIN - 1];

};

}

#endif // LS_STATE_H
