# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/tigakub/Documents/Robotics/slam

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tigakub/Documents/Robotics/slam/build

# Utility rule file for embeddedShaderData.

# Include any custom commands dependencies for this target.
include CMakeFiles/embeddedShaderData.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/embeddedShaderData.dir/progress.make

CMakeFiles/embeddedShaderData:
	cd .. && ./generateEmbeddedShaderData.sh

embeddedShaderData: CMakeFiles/embeddedShaderData
embeddedShaderData: CMakeFiles/embeddedShaderData.dir/build.make
.PHONY : embeddedShaderData

# Rule to build all files generated by this target.
CMakeFiles/embeddedShaderData.dir/build: embeddedShaderData
.PHONY : CMakeFiles/embeddedShaderData.dir/build

CMakeFiles/embeddedShaderData.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/embeddedShaderData.dir/cmake_clean.cmake
.PHONY : CMakeFiles/embeddedShaderData.dir/clean

CMakeFiles/embeddedShaderData.dir/depend:
	cd /home/tigakub/Documents/Robotics/slam/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tigakub/Documents/Robotics/slam /home/tigakub/Documents/Robotics/slam /home/tigakub/Documents/Robotics/slam/build /home/tigakub/Documents/Robotics/slam/build /home/tigakub/Documents/Robotics/slam/build/CMakeFiles/embeddedShaderData.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/embeddedShaderData.dir/depend
