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
include sl/CMakeFiles/Laser.dir/depend.make

# Include the progress variables for this target.
include sl/CMakeFiles/Laser.dir/progress.make

# Include the compile flags for this target's objects.
include sl/CMakeFiles/Laser.dir/flags.make

sl/CMakeFiles/Laser.dir/src/laser.cpp.o: sl/CMakeFiles/Laser.dir/flags.make
sl/CMakeFiles/Laser.dir/src/laser.cpp.o: ../sl/src/laser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tbf/projects/work/robot/src/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object sl/CMakeFiles/Laser.dir/src/laser.cpp.o"
	cd /home/tbf/projects/work/robot/src/cmake-build-debug/sl && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Laser.dir/src/laser.cpp.o -c /home/tbf/projects/work/robot/src/sl/src/laser.cpp

sl/CMakeFiles/Laser.dir/src/laser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Laser.dir/src/laser.cpp.i"
	cd /home/tbf/projects/work/robot/src/cmake-build-debug/sl && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tbf/projects/work/robot/src/sl/src/laser.cpp > CMakeFiles/Laser.dir/src/laser.cpp.i

sl/CMakeFiles/Laser.dir/src/laser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Laser.dir/src/laser.cpp.s"
	cd /home/tbf/projects/work/robot/src/cmake-build-debug/sl && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tbf/projects/work/robot/src/sl/src/laser.cpp -o CMakeFiles/Laser.dir/src/laser.cpp.s

sl/CMakeFiles/Laser.dir/src/laser.cpp.o.requires:

.PHONY : sl/CMakeFiles/Laser.dir/src/laser.cpp.o.requires

sl/CMakeFiles/Laser.dir/src/laser.cpp.o.provides: sl/CMakeFiles/Laser.dir/src/laser.cpp.o.requires
	$(MAKE) -f sl/CMakeFiles/Laser.dir/build.make sl/CMakeFiles/Laser.dir/src/laser.cpp.o.provides.build
.PHONY : sl/CMakeFiles/Laser.dir/src/laser.cpp.o.provides

sl/CMakeFiles/Laser.dir/src/laser.cpp.o.provides.build: sl/CMakeFiles/Laser.dir/src/laser.cpp.o


sl/CMakeFiles/Laser.dir/src/odom.cpp.o: sl/CMakeFiles/Laser.dir/flags.make
sl/CMakeFiles/Laser.dir/src/odom.cpp.o: ../sl/src/odom.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tbf/projects/work/robot/src/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object sl/CMakeFiles/Laser.dir/src/odom.cpp.o"
	cd /home/tbf/projects/work/robot/src/cmake-build-debug/sl && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Laser.dir/src/odom.cpp.o -c /home/tbf/projects/work/robot/src/sl/src/odom.cpp

sl/CMakeFiles/Laser.dir/src/odom.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Laser.dir/src/odom.cpp.i"
	cd /home/tbf/projects/work/robot/src/cmake-build-debug/sl && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tbf/projects/work/robot/src/sl/src/odom.cpp > CMakeFiles/Laser.dir/src/odom.cpp.i

sl/CMakeFiles/Laser.dir/src/odom.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Laser.dir/src/odom.cpp.s"
	cd /home/tbf/projects/work/robot/src/cmake-build-debug/sl && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tbf/projects/work/robot/src/sl/src/odom.cpp -o CMakeFiles/Laser.dir/src/odom.cpp.s

sl/CMakeFiles/Laser.dir/src/odom.cpp.o.requires:

.PHONY : sl/CMakeFiles/Laser.dir/src/odom.cpp.o.requires

sl/CMakeFiles/Laser.dir/src/odom.cpp.o.provides: sl/CMakeFiles/Laser.dir/src/odom.cpp.o.requires
	$(MAKE) -f sl/CMakeFiles/Laser.dir/build.make sl/CMakeFiles/Laser.dir/src/odom.cpp.o.provides.build
.PHONY : sl/CMakeFiles/Laser.dir/src/odom.cpp.o.provides

sl/CMakeFiles/Laser.dir/src/odom.cpp.o.provides.build: sl/CMakeFiles/Laser.dir/src/odom.cpp.o


# Object files for target Laser
Laser_OBJECTS = \
"CMakeFiles/Laser.dir/src/laser.cpp.o" \
"CMakeFiles/Laser.dir/src/odom.cpp.o"

# External object files for target Laser
Laser_EXTERNAL_OBJECTS =

devel/lib/libLaser.so: sl/CMakeFiles/Laser.dir/src/laser.cpp.o
devel/lib/libLaser.so: sl/CMakeFiles/Laser.dir/src/odom.cpp.o
devel/lib/libLaser.so: sl/CMakeFiles/Laser.dir/build.make
devel/lib/libLaser.so: sl/CMakeFiles/Laser.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tbf/projects/work/robot/src/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX shared library ../devel/lib/libLaser.so"
	cd /home/tbf/projects/work/robot/src/cmake-build-debug/sl && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Laser.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
sl/CMakeFiles/Laser.dir/build: devel/lib/libLaser.so

.PHONY : sl/CMakeFiles/Laser.dir/build

sl/CMakeFiles/Laser.dir/requires: sl/CMakeFiles/Laser.dir/src/laser.cpp.o.requires
sl/CMakeFiles/Laser.dir/requires: sl/CMakeFiles/Laser.dir/src/odom.cpp.o.requires

.PHONY : sl/CMakeFiles/Laser.dir/requires

sl/CMakeFiles/Laser.dir/clean:
	cd /home/tbf/projects/work/robot/src/cmake-build-debug/sl && $(CMAKE_COMMAND) -P CMakeFiles/Laser.dir/cmake_clean.cmake
.PHONY : sl/CMakeFiles/Laser.dir/clean

sl/CMakeFiles/Laser.dir/depend:
	cd /home/tbf/projects/work/robot/src/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tbf/projects/work/robot/src /home/tbf/projects/work/robot/src/sl /home/tbf/projects/work/robot/src/cmake-build-debug /home/tbf/projects/work/robot/src/cmake-build-debug/sl /home/tbf/projects/work/robot/src/cmake-build-debug/sl/CMakeFiles/Laser.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : sl/CMakeFiles/Laser.dir/depend

