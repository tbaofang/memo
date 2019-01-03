# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "lsrobot: 4 messages, 1 services")

set(MSG_I_FLAGS "-Ilsrobot:/home/tbf/projects/work/robot/src/lsrobot/msg;-Istd_msgs:/opt/ros/kinetic/share/std_msgs/cmake/../msg;-Igeometry_msgs:/opt/ros/kinetic/share/geometry_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(lsrobot_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/tbf/projects/work/robot/src/lsrobot/srv/cmd.srv" NAME_WE)
add_custom_target(_lsrobot_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "lsrobot" "/home/tbf/projects/work/robot/src/lsrobot/srv/cmd.srv" ""
)

get_filename_component(_filename "/home/tbf/projects/work/robot/src/lsrobot/msg/LSPoints.msg" NAME_WE)
add_custom_target(_lsrobot_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "lsrobot" "/home/tbf/projects/work/robot/src/lsrobot/msg/LSPoints.msg" "lsrobot/LSPoint"
)

get_filename_component(_filename "/home/tbf/projects/work/robot/src/lsrobot/msg/LSVirtualWalls.msg" NAME_WE)
add_custom_target(_lsrobot_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "lsrobot" "/home/tbf/projects/work/robot/src/lsrobot/msg/LSVirtualWalls.msg" "lsrobot/LSVirtualWall"
)

get_filename_component(_filename "/home/tbf/projects/work/robot/src/lsrobot/msg/LSPoint.msg" NAME_WE)
add_custom_target(_lsrobot_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "lsrobot" "/home/tbf/projects/work/robot/src/lsrobot/msg/LSPoint.msg" ""
)

get_filename_component(_filename "/home/tbf/projects/work/robot/src/lsrobot/msg/LSVirtualWall.msg" NAME_WE)
add_custom_target(_lsrobot_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "lsrobot" "/home/tbf/projects/work/robot/src/lsrobot/msg/LSVirtualWall.msg" ""
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(lsrobot
  "/home/tbf/projects/work/robot/src/lsrobot/msg/LSPoints.msg"
  "${MSG_I_FLAGS}"
  "/home/tbf/projects/work/robot/src/lsrobot/msg/LSPoint.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/lsrobot
)
_generate_msg_cpp(lsrobot
  "/home/tbf/projects/work/robot/src/lsrobot/msg/LSVirtualWalls.msg"
  "${MSG_I_FLAGS}"
  "/home/tbf/projects/work/robot/src/lsrobot/msg/LSVirtualWall.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/lsrobot
)
_generate_msg_cpp(lsrobot
  "/home/tbf/projects/work/robot/src/lsrobot/msg/LSPoint.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/lsrobot
)
_generate_msg_cpp(lsrobot
  "/home/tbf/projects/work/robot/src/lsrobot/msg/LSVirtualWall.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/lsrobot
)

### Generating Services
_generate_srv_cpp(lsrobot
  "/home/tbf/projects/work/robot/src/lsrobot/srv/cmd.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/lsrobot
)

### Generating Module File
_generate_module_cpp(lsrobot
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/lsrobot
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(lsrobot_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(lsrobot_generate_messages lsrobot_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/tbf/projects/work/robot/src/lsrobot/srv/cmd.srv" NAME_WE)
add_dependencies(lsrobot_generate_messages_cpp _lsrobot_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/tbf/projects/work/robot/src/lsrobot/msg/LSPoints.msg" NAME_WE)
add_dependencies(lsrobot_generate_messages_cpp _lsrobot_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/tbf/projects/work/robot/src/lsrobot/msg/LSVirtualWalls.msg" NAME_WE)
add_dependencies(lsrobot_generate_messages_cpp _lsrobot_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/tbf/projects/work/robot/src/lsrobot/msg/LSPoint.msg" NAME_WE)
add_dependencies(lsrobot_generate_messages_cpp _lsrobot_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/tbf/projects/work/robot/src/lsrobot/msg/LSVirtualWall.msg" NAME_WE)
add_dependencies(lsrobot_generate_messages_cpp _lsrobot_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(lsrobot_gencpp)
add_dependencies(lsrobot_gencpp lsrobot_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS lsrobot_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(lsrobot
  "/home/tbf/projects/work/robot/src/lsrobot/msg/LSPoints.msg"
  "${MSG_I_FLAGS}"
  "/home/tbf/projects/work/robot/src/lsrobot/msg/LSPoint.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/lsrobot
)
_generate_msg_eus(lsrobot
  "/home/tbf/projects/work/robot/src/lsrobot/msg/LSVirtualWalls.msg"
  "${MSG_I_FLAGS}"
  "/home/tbf/projects/work/robot/src/lsrobot/msg/LSVirtualWall.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/lsrobot
)
_generate_msg_eus(lsrobot
  "/home/tbf/projects/work/robot/src/lsrobot/msg/LSPoint.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/lsrobot
)
_generate_msg_eus(lsrobot
  "/home/tbf/projects/work/robot/src/lsrobot/msg/LSVirtualWall.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/lsrobot
)

### Generating Services
_generate_srv_eus(lsrobot
  "/home/tbf/projects/work/robot/src/lsrobot/srv/cmd.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/lsrobot
)

### Generating Module File
_generate_module_eus(lsrobot
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/lsrobot
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(lsrobot_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(lsrobot_generate_messages lsrobot_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/tbf/projects/work/robot/src/lsrobot/srv/cmd.srv" NAME_WE)
add_dependencies(lsrobot_generate_messages_eus _lsrobot_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/tbf/projects/work/robot/src/lsrobot/msg/LSPoints.msg" NAME_WE)
add_dependencies(lsrobot_generate_messages_eus _lsrobot_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/tbf/projects/work/robot/src/lsrobot/msg/LSVirtualWalls.msg" NAME_WE)
add_dependencies(lsrobot_generate_messages_eus _lsrobot_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/tbf/projects/work/robot/src/lsrobot/msg/LSPoint.msg" NAME_WE)
add_dependencies(lsrobot_generate_messages_eus _lsrobot_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/tbf/projects/work/robot/src/lsrobot/msg/LSVirtualWall.msg" NAME_WE)
add_dependencies(lsrobot_generate_messages_eus _lsrobot_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(lsrobot_geneus)
add_dependencies(lsrobot_geneus lsrobot_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS lsrobot_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(lsrobot
  "/home/tbf/projects/work/robot/src/lsrobot/msg/LSPoints.msg"
  "${MSG_I_FLAGS}"
  "/home/tbf/projects/work/robot/src/lsrobot/msg/LSPoint.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/lsrobot
)
_generate_msg_lisp(lsrobot
  "/home/tbf/projects/work/robot/src/lsrobot/msg/LSVirtualWalls.msg"
  "${MSG_I_FLAGS}"
  "/home/tbf/projects/work/robot/src/lsrobot/msg/LSVirtualWall.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/lsrobot
)
_generate_msg_lisp(lsrobot
  "/home/tbf/projects/work/robot/src/lsrobot/msg/LSPoint.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/lsrobot
)
_generate_msg_lisp(lsrobot
  "/home/tbf/projects/work/robot/src/lsrobot/msg/LSVirtualWall.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/lsrobot
)

### Generating Services
_generate_srv_lisp(lsrobot
  "/home/tbf/projects/work/robot/src/lsrobot/srv/cmd.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/lsrobot
)

### Generating Module File
_generate_module_lisp(lsrobot
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/lsrobot
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(lsrobot_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(lsrobot_generate_messages lsrobot_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/tbf/projects/work/robot/src/lsrobot/srv/cmd.srv" NAME_WE)
add_dependencies(lsrobot_generate_messages_lisp _lsrobot_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/tbf/projects/work/robot/src/lsrobot/msg/LSPoints.msg" NAME_WE)
add_dependencies(lsrobot_generate_messages_lisp _lsrobot_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/tbf/projects/work/robot/src/lsrobot/msg/LSVirtualWalls.msg" NAME_WE)
add_dependencies(lsrobot_generate_messages_lisp _lsrobot_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/tbf/projects/work/robot/src/lsrobot/msg/LSPoint.msg" NAME_WE)
add_dependencies(lsrobot_generate_messages_lisp _lsrobot_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/tbf/projects/work/robot/src/lsrobot/msg/LSVirtualWall.msg" NAME_WE)
add_dependencies(lsrobot_generate_messages_lisp _lsrobot_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(lsrobot_genlisp)
add_dependencies(lsrobot_genlisp lsrobot_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS lsrobot_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(lsrobot
  "/home/tbf/projects/work/robot/src/lsrobot/msg/LSPoints.msg"
  "${MSG_I_FLAGS}"
  "/home/tbf/projects/work/robot/src/lsrobot/msg/LSPoint.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/lsrobot
)
_generate_msg_nodejs(lsrobot
  "/home/tbf/projects/work/robot/src/lsrobot/msg/LSVirtualWalls.msg"
  "${MSG_I_FLAGS}"
  "/home/tbf/projects/work/robot/src/lsrobot/msg/LSVirtualWall.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/lsrobot
)
_generate_msg_nodejs(lsrobot
  "/home/tbf/projects/work/robot/src/lsrobot/msg/LSPoint.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/lsrobot
)
_generate_msg_nodejs(lsrobot
  "/home/tbf/projects/work/robot/src/lsrobot/msg/LSVirtualWall.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/lsrobot
)

### Generating Services
_generate_srv_nodejs(lsrobot
  "/home/tbf/projects/work/robot/src/lsrobot/srv/cmd.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/lsrobot
)

### Generating Module File
_generate_module_nodejs(lsrobot
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/lsrobot
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(lsrobot_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(lsrobot_generate_messages lsrobot_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/tbf/projects/work/robot/src/lsrobot/srv/cmd.srv" NAME_WE)
add_dependencies(lsrobot_generate_messages_nodejs _lsrobot_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/tbf/projects/work/robot/src/lsrobot/msg/LSPoints.msg" NAME_WE)
add_dependencies(lsrobot_generate_messages_nodejs _lsrobot_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/tbf/projects/work/robot/src/lsrobot/msg/LSVirtualWalls.msg" NAME_WE)
add_dependencies(lsrobot_generate_messages_nodejs _lsrobot_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/tbf/projects/work/robot/src/lsrobot/msg/LSPoint.msg" NAME_WE)
add_dependencies(lsrobot_generate_messages_nodejs _lsrobot_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/tbf/projects/work/robot/src/lsrobot/msg/LSVirtualWall.msg" NAME_WE)
add_dependencies(lsrobot_generate_messages_nodejs _lsrobot_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(lsrobot_gennodejs)
add_dependencies(lsrobot_gennodejs lsrobot_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS lsrobot_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(lsrobot
  "/home/tbf/projects/work/robot/src/lsrobot/msg/LSPoints.msg"
  "${MSG_I_FLAGS}"
  "/home/tbf/projects/work/robot/src/lsrobot/msg/LSPoint.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/lsrobot
)
_generate_msg_py(lsrobot
  "/home/tbf/projects/work/robot/src/lsrobot/msg/LSVirtualWalls.msg"
  "${MSG_I_FLAGS}"
  "/home/tbf/projects/work/robot/src/lsrobot/msg/LSVirtualWall.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/lsrobot
)
_generate_msg_py(lsrobot
  "/home/tbf/projects/work/robot/src/lsrobot/msg/LSPoint.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/lsrobot
)
_generate_msg_py(lsrobot
  "/home/tbf/projects/work/robot/src/lsrobot/msg/LSVirtualWall.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/lsrobot
)

### Generating Services
_generate_srv_py(lsrobot
  "/home/tbf/projects/work/robot/src/lsrobot/srv/cmd.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/lsrobot
)

### Generating Module File
_generate_module_py(lsrobot
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/lsrobot
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(lsrobot_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(lsrobot_generate_messages lsrobot_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/tbf/projects/work/robot/src/lsrobot/srv/cmd.srv" NAME_WE)
add_dependencies(lsrobot_generate_messages_py _lsrobot_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/tbf/projects/work/robot/src/lsrobot/msg/LSPoints.msg" NAME_WE)
add_dependencies(lsrobot_generate_messages_py _lsrobot_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/tbf/projects/work/robot/src/lsrobot/msg/LSVirtualWalls.msg" NAME_WE)
add_dependencies(lsrobot_generate_messages_py _lsrobot_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/tbf/projects/work/robot/src/lsrobot/msg/LSPoint.msg" NAME_WE)
add_dependencies(lsrobot_generate_messages_py _lsrobot_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/tbf/projects/work/robot/src/lsrobot/msg/LSVirtualWall.msg" NAME_WE)
add_dependencies(lsrobot_generate_messages_py _lsrobot_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(lsrobot_genpy)
add_dependencies(lsrobot_genpy lsrobot_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS lsrobot_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/lsrobot)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/lsrobot
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(lsrobot_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()
if(TARGET geometry_msgs_generate_messages_cpp)
  add_dependencies(lsrobot_generate_messages_cpp geometry_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/lsrobot)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/lsrobot
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(lsrobot_generate_messages_eus std_msgs_generate_messages_eus)
endif()
if(TARGET geometry_msgs_generate_messages_eus)
  add_dependencies(lsrobot_generate_messages_eus geometry_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/lsrobot)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/lsrobot
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(lsrobot_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()
if(TARGET geometry_msgs_generate_messages_lisp)
  add_dependencies(lsrobot_generate_messages_lisp geometry_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/lsrobot)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/lsrobot
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(lsrobot_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()
if(TARGET geometry_msgs_generate_messages_nodejs)
  add_dependencies(lsrobot_generate_messages_nodejs geometry_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/lsrobot)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/lsrobot\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/lsrobot
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(lsrobot_generate_messages_py std_msgs_generate_messages_py)
endif()
if(TARGET geometry_msgs_generate_messages_py)
  add_dependencies(lsrobot_generate_messages_py geometry_msgs_generate_messages_py)
endif()
