/*******************************************************
@company: Copyright (C) 2018, Leishen Intelligent System
@product: LS_SERVER_ROBOT
@filename:
@brief:
@version:       date:       author:     comments:
@v1.0           18-5-26     fu          new
*******************************************************/
#ifndef LS_H
#define LS_H

namespace ls {

#define LS_OK (0)

/**
  *@brief base error code
  */
#define LSSR_BASE_ERROR  (0x4c531000)  // SensoR error
#define LSFW_BASE_ERROR  (0x4c532000)  // FirmWare error
#define LSAG_BASE_ERROR  (0X4C533000)  // AlGorithm error
#define LSSM_BASE_ERROR  (0x4c534000)  // SysteM error
#define LSSV_BASE_ERROR  (0x4c535000)  // SeVer robot node error

// sensor error
#define LSSR_SCAN_ERROR       (LSSR_BASE_ERROR + 0x000)
#define LSSR_SONAR_ERROR      (LSSR_BASE_ERROR + 0x100)
#define LSSR_CLIFF_ERROR      (LSSR_BASE_ERROR + 0x200)
#define LSSR_GPS_ERROR        (LSSR_BASE_ERROR + 0x300)
#define LSSR_BUMMPER_ERROR    (LSSR_BASE_ERROR + 0x400)
#define LSSR_INFRARED_ERROR   (LSSR_BASE_ERROR + 0x500)

// firmware error
#define LSFW_STATE_ERROR      (LSFW_BASE_ERROR + 0x000)
#define LSFW_PARAM_ERROR      (LSFW_BASE_ERROR + 0x100)
#define LSFW_FATAL_ERROR      (LSFW_BASE_ERROR + 0x200)
#define LSFW_MEMORY_ERROR     (LSFW_BASE_ERROR + 0x300)

// algorithem error
#define LSAG_DRIVER_ERROR     (LSAG_BASE_ERROR + 0x000)
#define LSAG_MAP_ERROR        (LSAG_BASE_ERROR + 0x100)
#define LSAG_AMCL_ERROR       (LSAG_BASE_ERROR + 0x200)
#define LSAG_MOVEBASE_ERROR   (LSAG_BASE_ERROR + 0x300)

// server error
#define LSSV_STATE_ERROR      (LSSV_BASE_ERROR + 0x0)
#define LSSV_PARAM_ERROR      (LSSV_BASE_ERROR + 0x1)
#define LSSV_FATAL_ERROR      (LSSV_BASE_ERROR + 0x2)
#define LSSV_PERMIT_ERROR     (LSSV_BASE_ERROR + 0x3)
#define LSSV_SAVE_MAP_ERROR   (LSSV_BASE_ERROR + 0x4)
#define LSSV_LOAD_MAP_ERROR   (LSSV_BASE_ERROR + 0x5)


/**
 * @brief define
 */
#define RAD2DEGREE            57.29577951308232087685
#define DEGREE2RAD            0.01745329251994329577


}


#endif // LS_H
