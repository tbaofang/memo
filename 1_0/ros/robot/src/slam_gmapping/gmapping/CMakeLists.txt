cmake_minimum_required(VERSION 2.8)
project(gmapping)

find_package(catkin REQUIRED nav_msgs openslam_gmapping roscpp tf rosbag_storage lsrobot)

find_package(Boost REQUIRED signals)

include_directories(${Boost_INCLUDE_DIRS} ${catkin_INCLUDE_DIRS})
include_directories(include)
include_directories(src)

catkin_package()

add_executable(slam_gmapping src/slam_gmapping.cpp src/main.cpp)
target_link_libraries(slam_gmapping ${Boost_LIBRARIES} ${catkin_LIBRARIES})
if(catkin_EXPORTED_TARGETS)
  add_dependencies(slam_gmapping ${catkin_EXPORTED_TARGETS})
endif()


add_executable(slam_gmapping_server src/slam_gmapping.cpp src/gmapping_server.cpp)
target_link_libraries(slam_gmapping_server ${Boost_LIBRARIES} ${catkin_LIBRARIES})

add_library(slam_gmapping_nodelet src/slam_gmapping.cpp src/nodelet.cpp)
target_link_libraries(slam_gmapping_nodelet ${catkin_LIBRARIES})


install(TARGETS slam_gmapping slam_gmapping_server
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
