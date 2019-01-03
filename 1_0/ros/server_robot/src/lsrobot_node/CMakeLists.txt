cmake_minimum_required(VERSION 2.8.3)
project(lsrobot_node)

find_package(catkin REQUIRED lsrobot roscpp tf)
find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED)

find_package(PkgConfig)
pkg_check_modules(NEW_YAMLCPP yaml-cpp>=0.5)
if(NEW_YAMLCPP_FOUND)
add_definitions(-DHAVE_NEW_YAMLCPP)
endif(NEW_YAMLCPP_FOUND)

catkin_package(
    CATKIN_DEPENDS
        lsrobot
	roscpp
	tf
)

include_directories( include ${catkin_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

add_executable(lsrobot_node src/ls_node.cpp
                            src/ls_server.cpp
                            src/ls_topic.cpp
                            src/ls_state.cpp
                            src/ls_es.cpp
                            src/ls_safety_controller.cpp
                            )
add_dependencies(lsrobot_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(lsrobot_node
    yaml-cpp ${catkin_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBS})

install(TARGETS lsrobot_node
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
