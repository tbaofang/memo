cmake_minimum_required(VERSION 2.8.3)
project(sl)

find_package(catkin REQUIRED
        COMPONENTS
            roscpp
            message_generation
            tf
            costmap_2d
            nav_core
            sensor_msgs
        )
add_message_files(FILES cmd.msg)

generate_messages()
add_definitions(${EIGEN3_DEFINITIONS})

include_directories(include ${catkin_INCLUDE_DIRS})


add_library(Laser src/laser.cpp src/odom.cpp)

catkin_package(
    INCLUDE_DIRS include 
    LIBRARIES Laser 
    CATKIN_DEPENDS roscpp message_runtime tf costmap_2d sensor_msgs nav_core
    )
add_executable(sl src/sl.cpp)

target_link_libraries(sl Laser)
add_dependencies(sl ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(sl
    ${catkin_LIBRARIES}
    )

install(TARGETS sl
       ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
       LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
       RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
       )

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.h"
)

#install(FILES bgp_plugin.xml
#    DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
#)


