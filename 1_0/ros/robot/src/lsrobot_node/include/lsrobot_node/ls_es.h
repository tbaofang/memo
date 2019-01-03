/*******************************************************
@company: Copyright (C) 2018, Leishen Intelligent System
@product: LS_SERVER_ROBOT
@filename:
@brief:
@version:       date:       author:     comments:
@v1.0           18-5-26     fu          new
*******************************************************/
#ifndef LS_ES_H
#define LS_ES_H

#include "lsrobot_node/ls_state.h"

namespace ls {
class LSES{
public:
  static const char* bool2str(const int &e);
  static const char* bool2str(const int *e);

  static const char* enum2str(LS_STATE_ENUM e);
  static const char* enum2str(LS_CMD_CODE_ENUM e);

private:
  explicit LSES();
  ~LSES();
  LSES(const LSES &);
  LSES& operator=(const LSES &);

};
}

#endif // LS_ES_H
