cmake_minimum_required(VERSION 2.8.3)
project(lsrobot)

# Load catkin and all dependencies required for this package
find_package(catkin REQUIRED COMPONENTS
  message_generation
  tf
  roscpp
  sensor_msgs
  rospy
  geometry_msgs
  move_base_msgs
  actionlib_msgs
  std_msgs
)

add_service_files(FILES
  cmd.srv
)

add_message_files(DIRECTORY msg
    FILES
    LSPoint.msg
    LSPoints.msg
    LSVirtualWall.msg
    LSVirtualWalls.msg
)

generate_messages(DEPENDENCIES
  std_msgs
  geometry_msgs
)

# What other packages will need to use this package
catkin_package(
    CATKIN_DEPENDS std_msgs tf roscpp sensor_msgs message_runtime rospy geometry_msgs move_base_msgs actionlib_msgs
)

#catkin_add_env_hooks(25.turtlebot-navigation SHELLS sh DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/env-hooks)


###########
## Build ##
###########
include_directories(${catkin_INCLUDE_DIRS})


# Add_executables


#############
## Install ##
#############

# Mark executables and/or libraries for installation
#install(TARGETS laser_footprint_filter
#  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
#)

# Mark anything (useful) else for installation
install(DIRECTORY laser
        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

install(DIRECTORY launch
        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

install(DIRECTORY param
        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

install(DIRECTORY maps
        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
install(DIRECTORY srv
        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

install(DIRECTORY msg
        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

install(PROGRAMS
  scripts/SerialControl_Server.py
  scripts/SerialControl_Client.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)


