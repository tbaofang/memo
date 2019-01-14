# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "follow: 2 messages, 0 services")

set(MSG_I_FLAGS "-Ifollow:/home/tbf/proj/follow/src/follow/msg;-Istd_msgs:/opt/ros/kinetic/share/std_msgs/cmake/../msg;-Igeometry_msgs:/opt/ros/kinetic/share/geometry_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(follow_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/tbf/proj/follow/src/follow/msg/Leg.msg" NAME_WE)
add_custom_target(_follow_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "follow" "/home/tbf/proj/follow/src/follow/msg/Leg.msg" "geometry_msgs/Point"
)

get_filename_component(_filename "/home/tbf/proj/follow/src/follow/msg/LegArray.msg" NAME_WE)
add_custom_target(_follow_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "follow" "/home/tbf/proj/follow/src/follow/msg/LegArray.msg" "follow/Leg:std_msgs/Header:geometry_msgs/Point"
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(follow
  "/home/tbf/proj/follow/src/follow/msg/LegArray.msg"
  "${MSG_I_FLAGS}"
  "/home/tbf/proj/follow/src/follow/msg/Leg.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/follow
)
_generate_msg_cpp(follow
  "/home/tbf/proj/follow/src/follow/msg/Leg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/follow
)

### Generating Services

### Generating Module File
_generate_module_cpp(follow
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/follow
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(follow_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(follow_generate_messages follow_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/tbf/proj/follow/src/follow/msg/Leg.msg" NAME_WE)
add_dependencies(follow_generate_messages_cpp _follow_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/tbf/proj/follow/src/follow/msg/LegArray.msg" NAME_WE)
add_dependencies(follow_generate_messages_cpp _follow_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(follow_gencpp)
add_dependencies(follow_gencpp follow_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS follow_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(follow
  "/home/tbf/proj/follow/src/follow/msg/LegArray.msg"
  "${MSG_I_FLAGS}"
  "/home/tbf/proj/follow/src/follow/msg/Leg.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/follow
)
_generate_msg_eus(follow
  "/home/tbf/proj/follow/src/follow/msg/Leg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/follow
)

### Generating Services

### Generating Module File
_generate_module_eus(follow
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/follow
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(follow_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(follow_generate_messages follow_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/tbf/proj/follow/src/follow/msg/Leg.msg" NAME_WE)
add_dependencies(follow_generate_messages_eus _follow_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/tbf/proj/follow/src/follow/msg/LegArray.msg" NAME_WE)
add_dependencies(follow_generate_messages_eus _follow_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(follow_geneus)
add_dependencies(follow_geneus follow_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS follow_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(follow
  "/home/tbf/proj/follow/src/follow/msg/LegArray.msg"
  "${MSG_I_FLAGS}"
  "/home/tbf/proj/follow/src/follow/msg/Leg.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/follow
)
_generate_msg_lisp(follow
  "/home/tbf/proj/follow/src/follow/msg/Leg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/follow
)

### Generating Services

### Generating Module File
_generate_module_lisp(follow
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/follow
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(follow_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(follow_generate_messages follow_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/tbf/proj/follow/src/follow/msg/Leg.msg" NAME_WE)
add_dependencies(follow_generate_messages_lisp _follow_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/tbf/proj/follow/src/follow/msg/LegArray.msg" NAME_WE)
add_dependencies(follow_generate_messages_lisp _follow_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(follow_genlisp)
add_dependencies(follow_genlisp follow_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS follow_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(follow
  "/home/tbf/proj/follow/src/follow/msg/LegArray.msg"
  "${MSG_I_FLAGS}"
  "/home/tbf/proj/follow/src/follow/msg/Leg.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/follow
)
_generate_msg_nodejs(follow
  "/home/tbf/proj/follow/src/follow/msg/Leg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/follow
)

### Generating Services

### Generating Module File
_generate_module_nodejs(follow
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/follow
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(follow_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(follow_generate_messages follow_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/tbf/proj/follow/src/follow/msg/Leg.msg" NAME_WE)
add_dependencies(follow_generate_messages_nodejs _follow_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/tbf/proj/follow/src/follow/msg/LegArray.msg" NAME_WE)
add_dependencies(follow_generate_messages_nodejs _follow_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(follow_gennodejs)
add_dependencies(follow_gennodejs follow_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS follow_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(follow
  "/home/tbf/proj/follow/src/follow/msg/LegArray.msg"
  "${MSG_I_FLAGS}"
  "/home/tbf/proj/follow/src/follow/msg/Leg.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/follow
)
_generate_msg_py(follow
  "/home/tbf/proj/follow/src/follow/msg/Leg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/follow
)

### Generating Services

### Generating Module File
_generate_module_py(follow
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/follow
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(follow_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(follow_generate_messages follow_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/tbf/proj/follow/src/follow/msg/Leg.msg" NAME_WE)
add_dependencies(follow_generate_messages_py _follow_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/tbf/proj/follow/src/follow/msg/LegArray.msg" NAME_WE)
add_dependencies(follow_generate_messages_py _follow_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(follow_genpy)
add_dependencies(follow_genpy follow_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS follow_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/follow)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/follow
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(follow_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()
if(TARGET geometry_msgs_generate_messages_cpp)
  add_dependencies(follow_generate_messages_cpp geometry_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/follow)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/follow
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(follow_generate_messages_eus std_msgs_generate_messages_eus)
endif()
if(TARGET geometry_msgs_generate_messages_eus)
  add_dependencies(follow_generate_messages_eus geometry_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/follow)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/follow
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(follow_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()
if(TARGET geometry_msgs_generate_messages_lisp)
  add_dependencies(follow_generate_messages_lisp geometry_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/follow)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/follow
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(follow_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()
if(TARGET geometry_msgs_generate_messages_nodejs)
  add_dependencies(follow_generate_messages_nodejs geometry_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/follow)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/follow\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/follow
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(follow_generate_messages_py std_msgs_generate_messages_py)
endif()
if(TARGET geometry_msgs_generate_messages_py)
  add_dependencies(follow_generate_messages_py geometry_msgs_generate_messages_py)
endif()
