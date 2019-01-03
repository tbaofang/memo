# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "sl: 1 messages, 0 services")

set(MSG_I_FLAGS "-Isl:/home/tbf/projects/work/robot/src/sl/msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(sl_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/tbf/projects/work/robot/src/sl/msg/cmd.msg" NAME_WE)
add_custom_target(_sl_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "sl" "/home/tbf/projects/work/robot/src/sl/msg/cmd.msg" ""
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(sl
  "/home/tbf/projects/work/robot/src/sl/msg/cmd.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/sl
)

### Generating Services

### Generating Module File
_generate_module_cpp(sl
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/sl
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(sl_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(sl_generate_messages sl_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/tbf/projects/work/robot/src/sl/msg/cmd.msg" NAME_WE)
add_dependencies(sl_generate_messages_cpp _sl_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(sl_gencpp)
add_dependencies(sl_gencpp sl_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS sl_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(sl
  "/home/tbf/projects/work/robot/src/sl/msg/cmd.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/sl
)

### Generating Services

### Generating Module File
_generate_module_eus(sl
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/sl
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(sl_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(sl_generate_messages sl_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/tbf/projects/work/robot/src/sl/msg/cmd.msg" NAME_WE)
add_dependencies(sl_generate_messages_eus _sl_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(sl_geneus)
add_dependencies(sl_geneus sl_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS sl_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(sl
  "/home/tbf/projects/work/robot/src/sl/msg/cmd.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/sl
)

### Generating Services

### Generating Module File
_generate_module_lisp(sl
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/sl
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(sl_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(sl_generate_messages sl_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/tbf/projects/work/robot/src/sl/msg/cmd.msg" NAME_WE)
add_dependencies(sl_generate_messages_lisp _sl_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(sl_genlisp)
add_dependencies(sl_genlisp sl_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS sl_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(sl
  "/home/tbf/projects/work/robot/src/sl/msg/cmd.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/sl
)

### Generating Services

### Generating Module File
_generate_module_nodejs(sl
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/sl
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(sl_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(sl_generate_messages sl_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/tbf/projects/work/robot/src/sl/msg/cmd.msg" NAME_WE)
add_dependencies(sl_generate_messages_nodejs _sl_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(sl_gennodejs)
add_dependencies(sl_gennodejs sl_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS sl_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(sl
  "/home/tbf/projects/work/robot/src/sl/msg/cmd.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/sl
)

### Generating Services

### Generating Module File
_generate_module_py(sl
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/sl
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(sl_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(sl_generate_messages sl_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/tbf/projects/work/robot/src/sl/msg/cmd.msg" NAME_WE)
add_dependencies(sl_generate_messages_py _sl_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(sl_genpy)
add_dependencies(sl_genpy sl_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS sl_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/sl)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/sl
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/sl)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/sl
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/sl)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/sl
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/sl)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/sl
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/sl)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/sl\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/sl
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
