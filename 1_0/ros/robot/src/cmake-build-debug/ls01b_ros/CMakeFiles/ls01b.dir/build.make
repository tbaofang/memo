# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/tbf/downloads/softwares/clion-2018.1.3/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/tbf/downloads/softwares/clion-2018.1.3/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/tbf/projects/work/robot/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tbf/projects/work/robot/src/cmake-build-debug

# Include any dependencies generated for this target.
include ls01b_ros/CMakeFiles/ls01b.dir/depend.make

# Include the progress variables for this target.
include ls01b_ros/CMakeFiles/ls01b.dir/progress.make

# Include the compile flags for this target's objects.
include ls01b_ros/CMakeFiles/ls01b.dir/flags.make

ls01b_ros/CMakeFiles/ls01b.dir/src/main.cpp.o: ls01b_ros/CMakeFiles/ls01b.dir/flags.make
ls01b_ros/CMakeFiles/ls01b.dir/src/main.cpp.o: ../ls01b_ros/src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tbf/projects/work/robot/src/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object ls01b_ros/CMakeFiles/ls01b.dir/src/main.cpp.o"
	cd /home/tbf/projects/work/robot/src/cmake-build-debug/ls01b_ros && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ls01b.dir/src/main.cpp.o -c /home/tbf/projects/work/robot/src/ls01b_ros/src/main.cpp

ls01b_ros/CMakeFiles/ls01b.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ls01b.dir/src/main.cpp.i"
	cd /home/tbf/projects/work/robot/src/cmake-build-debug/ls01b_ros && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tbf/projects/work/robot/src/ls01b_ros/src/main.cpp > CMakeFiles/ls01b.dir/src/main.cpp.i

ls01b_ros/CMakeFiles/ls01b.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ls01b.dir/src/main.cpp.s"
	cd /home/tbf/projects/work/robot/src/cmake-build-debug/ls01b_ros && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tbf/projects/work/robot/src/ls01b_ros/src/main.cpp -o CMakeFiles/ls01b.dir/src/main.cpp.s

ls01b_ros/CMakeFiles/ls01b.dir/src/main.cpp.o.requires:

.PHONY : ls01b_ros/CMakeFiles/ls01b.dir/src/main.cpp.o.requires

ls01b_ros/CMakeFiles/ls01b.dir/src/main.cpp.o.provides: ls01b_ros/CMakeFiles/ls01b.dir/src/main.cpp.o.requires
	$(MAKE) -f ls01b_ros/CMakeFiles/ls01b.dir/build.make ls01b_ros/CMakeFiles/ls01b.dir/src/main.cpp.o.provides.build
.PHONY : ls01b_ros/CMakeFiles/ls01b.dir/src/main.cpp.o.provides

ls01b_ros/CMakeFiles/ls01b.dir/src/main.cpp.o.provides.build: ls01b_ros/CMakeFiles/ls01b.dir/src/main.cpp.o


ls01b_ros/CMakeFiles/ls01b.dir/src/ls01b_driver.cpp.o: ls01b_ros/CMakeFiles/ls01b.dir/flags.make
ls01b_ros/CMakeFiles/ls01b.dir/src/ls01b_driver.cpp.o: ../ls01b_ros/src/ls01b_driver.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tbf/projects/work/robot/src/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object ls01b_ros/CMakeFiles/ls01b.dir/src/ls01b_driver.cpp.o"
	cd /home/tbf/projects/work/robot/src/cmake-build-debug/ls01b_ros && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ls01b.dir/src/ls01b_driver.cpp.o -c /home/tbf/projects/work/robot/src/ls01b_ros/src/ls01b_driver.cpp

ls01b_ros/CMakeFiles/ls01b.dir/src/ls01b_driver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ls01b.dir/src/ls01b_driver.cpp.i"
	cd /home/tbf/projects/work/robot/src/cmake-build-debug/ls01b_ros && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tbf/projects/work/robot/src/ls01b_ros/src/ls01b_driver.cpp > CMakeFiles/ls01b.dir/src/ls01b_driver.cpp.i

ls01b_ros/CMakeFiles/ls01b.dir/src/ls01b_driver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ls01b.dir/src/ls01b_driver.cpp.s"
	cd /home/tbf/projects/work/robot/src/cmake-build-debug/ls01b_ros && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tbf/projects/work/robot/src/ls01b_ros/src/ls01b_driver.cpp -o CMakeFiles/ls01b.dir/src/ls01b_driver.cpp.s

ls01b_ros/CMakeFiles/ls01b.dir/src/ls01b_driver.cpp.o.requires:

.PHONY : ls01b_ros/CMakeFiles/ls01b.dir/src/ls01b_driver.cpp.o.requires

ls01b_ros/CMakeFiles/ls01b.dir/src/ls01b_driver.cpp.o.provides: ls01b_ros/CMakeFiles/ls01b.dir/src/ls01b_driver.cpp.o.requires
	$(MAKE) -f ls01b_ros/CMakeFiles/ls01b.dir/build.make ls01b_ros/CMakeFiles/ls01b.dir/src/ls01b_driver.cpp.o.provides.build
.PHONY : ls01b_ros/CMakeFiles/ls01b.dir/src/ls01b_driver.cpp.o.provides

ls01b_ros/CMakeFiles/ls01b.dir/src/ls01b_driver.cpp.o.provides.build: ls01b_ros/CMakeFiles/ls01b.dir/src/ls01b_driver.cpp.o


# Object files for target ls01b
ls01b_OBJECTS = \
"CMakeFiles/ls01b.dir/src/main.cpp.o" \
"CMakeFiles/ls01b.dir/src/ls01b_driver.cpp.o"

# External object files for target ls01b
ls01b_EXTERNAL_OBJECTS =

devel/lib/ls01b/ls01b: ls01b_ros/CMakeFiles/ls01b.dir/src/main.cpp.o
devel/lib/ls01b/ls01b: ls01b_ros/CMakeFiles/ls01b.dir/src/ls01b_driver.cpp.o
devel/lib/ls01b/ls01b: ls01b_ros/CMakeFiles/ls01b.dir/build.make
devel/lib/ls01b/ls01b: /opt/ros/kinetic/lib/libroscpp.so
devel/lib/ls01b/ls01b: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
devel/lib/ls01b/ls01b: /usr/lib/x86_64-linux-gnu/libboost_signals.so
devel/lib/ls01b/ls01b: /opt/ros/kinetic/lib/librosconsole.so
devel/lib/ls01b/ls01b: /opt/ros/kinetic/lib/librosconsole_log4cxx.so
devel/lib/ls01b/ls01b: /opt/ros/kinetic/lib/librosconsole_backend_interface.so
devel/lib/ls01b/ls01b: /usr/lib/x86_64-linux-gnu/liblog4cxx.so
devel/lib/ls01b/ls01b: /usr/lib/x86_64-linux-gnu/libboost_regex.so
devel/lib/ls01b/ls01b: /opt/ros/kinetic/lib/libxmlrpcpp.so
devel/lib/ls01b/ls01b: /opt/ros/kinetic/lib/libroscpp_serialization.so
devel/lib/ls01b/ls01b: /opt/ros/kinetic/lib/librostime.so
devel/lib/ls01b/ls01b: /opt/ros/kinetic/lib/libcpp_common.so
devel/lib/ls01b/ls01b: /usr/lib/x86_64-linux-gnu/libboost_system.so
devel/lib/ls01b/ls01b: /usr/lib/x86_64-linux-gnu/libboost_thread.so
devel/lib/ls01b/ls01b: /usr/lib/x86_64-linux-gnu/libboost_chrono.so
devel/lib/ls01b/ls01b: /usr/lib/x86_64-linux-gnu/libboost_date_time.so
devel/lib/ls01b/ls01b: /usr/lib/x86_64-linux-gnu/libboost_atomic.so
devel/lib/ls01b/ls01b: /usr/lib/x86_64-linux-gnu/libpthread.so
devel/lib/ls01b/ls01b: /usr/lib/x86_64-linux-gnu/libconsole_bridge.so
devel/lib/ls01b/ls01b: ls01b_ros/CMakeFiles/ls01b.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tbf/projects/work/robot/src/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ../devel/lib/ls01b/ls01b"
	cd /home/tbf/projects/work/robot/src/cmake-build-debug/ls01b_ros && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ls01b.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
ls01b_ros/CMakeFiles/ls01b.dir/build: devel/lib/ls01b/ls01b

.PHONY : ls01b_ros/CMakeFiles/ls01b.dir/build

ls01b_ros/CMakeFiles/ls01b.dir/requires: ls01b_ros/CMakeFiles/ls01b.dir/src/main.cpp.o.requires
ls01b_ros/CMakeFiles/ls01b.dir/requires: ls01b_ros/CMakeFiles/ls01b.dir/src/ls01b_driver.cpp.o.requires

.PHONY : ls01b_ros/CMakeFiles/ls01b.dir/requires

ls01b_ros/CMakeFiles/ls01b.dir/clean:
	cd /home/tbf/projects/work/robot/src/cmake-build-debug/ls01b_ros && $(CMAKE_COMMAND) -P CMakeFiles/ls01b.dir/cmake_clean.cmake
.PHONY : ls01b_ros/CMakeFiles/ls01b.dir/clean

ls01b_ros/CMakeFiles/ls01b.dir/depend:
	cd /home/tbf/projects/work/robot/src/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tbf/projects/work/robot/src /home/tbf/projects/work/robot/src/ls01b_ros /home/tbf/projects/work/robot/src/cmake-build-debug /home/tbf/projects/work/robot/src/cmake-build-debug/ls01b_ros /home/tbf/projects/work/robot/src/cmake-build-debug/ls01b_ros/CMakeFiles/ls01b.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : ls01b_ros/CMakeFiles/ls01b.dir/depend

