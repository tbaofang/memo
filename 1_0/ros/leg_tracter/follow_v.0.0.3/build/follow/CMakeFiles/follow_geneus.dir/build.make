# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/tbf/proj/follow/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tbf/proj/follow/build

# Utility rule file for follow_geneus.

# Include the progress variables for this target.
include follow/CMakeFiles/follow_geneus.dir/progress.make

follow_geneus: follow/CMakeFiles/follow_geneus.dir/build.make

.PHONY : follow_geneus

# Rule to build all files generated by this target.
follow/CMakeFiles/follow_geneus.dir/build: follow_geneus

.PHONY : follow/CMakeFiles/follow_geneus.dir/build

follow/CMakeFiles/follow_geneus.dir/clean:
	cd /home/tbf/proj/follow/build/follow && $(CMAKE_COMMAND) -P CMakeFiles/follow_geneus.dir/cmake_clean.cmake
.PHONY : follow/CMakeFiles/follow_geneus.dir/clean

follow/CMakeFiles/follow_geneus.dir/depend:
	cd /home/tbf/proj/follow/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tbf/proj/follow/src /home/tbf/proj/follow/src/follow /home/tbf/proj/follow/build /home/tbf/proj/follow/build/follow /home/tbf/proj/follow/build/follow/CMakeFiles/follow_geneus.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : follow/CMakeFiles/follow_geneus.dir/depend

