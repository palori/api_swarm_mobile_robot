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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/andrija1911/Desktop/Thesis/api_swarm_mobile_robot/API_rpi/image_proc/tests/aruco_test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/andrija1911/Desktop/Thesis/api_swarm_mobile_robot/API_rpi/image_proc/tests/aruco_test

# Include any dependencies generated for this target.
include CMakeFiles/ar.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ar.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ar.dir/flags.make

CMakeFiles/ar.dir/aruco.cpp.o: CMakeFiles/ar.dir/flags.make
CMakeFiles/ar.dir/aruco.cpp.o: aruco.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/andrija1911/Desktop/Thesis/api_swarm_mobile_robot/API_rpi/image_proc/tests/aruco_test/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ar.dir/aruco.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ar.dir/aruco.cpp.o -c /home/andrija1911/Desktop/Thesis/api_swarm_mobile_robot/API_rpi/image_proc/tests/aruco_test/aruco.cpp

CMakeFiles/ar.dir/aruco.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ar.dir/aruco.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/andrija1911/Desktop/Thesis/api_swarm_mobile_robot/API_rpi/image_proc/tests/aruco_test/aruco.cpp > CMakeFiles/ar.dir/aruco.cpp.i

CMakeFiles/ar.dir/aruco.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ar.dir/aruco.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/andrija1911/Desktop/Thesis/api_swarm_mobile_robot/API_rpi/image_proc/tests/aruco_test/aruco.cpp -o CMakeFiles/ar.dir/aruco.cpp.s

CMakeFiles/ar.dir/aruco.cpp.o.requires:

.PHONY : CMakeFiles/ar.dir/aruco.cpp.o.requires

CMakeFiles/ar.dir/aruco.cpp.o.provides: CMakeFiles/ar.dir/aruco.cpp.o.requires
	$(MAKE) -f CMakeFiles/ar.dir/build.make CMakeFiles/ar.dir/aruco.cpp.o.provides.build
.PHONY : CMakeFiles/ar.dir/aruco.cpp.o.provides

CMakeFiles/ar.dir/aruco.cpp.o.provides.build: CMakeFiles/ar.dir/aruco.cpp.o


# Object files for target ar
ar_OBJECTS = \
"CMakeFiles/ar.dir/aruco.cpp.o"

# External object files for target ar
ar_EXTERNAL_OBJECTS =

ar: CMakeFiles/ar.dir/aruco.cpp.o
ar: CMakeFiles/ar.dir/build.make
ar: CMakeFiles/ar.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/andrija1911/Desktop/Thesis/api_swarm_mobile_robot/API_rpi/image_proc/tests/aruco_test/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ar"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ar.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ar.dir/build: ar

.PHONY : CMakeFiles/ar.dir/build

CMakeFiles/ar.dir/requires: CMakeFiles/ar.dir/aruco.cpp.o.requires

.PHONY : CMakeFiles/ar.dir/requires

CMakeFiles/ar.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ar.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ar.dir/clean

CMakeFiles/ar.dir/depend:
	cd /home/andrija1911/Desktop/Thesis/api_swarm_mobile_robot/API_rpi/image_proc/tests/aruco_test && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/andrija1911/Desktop/Thesis/api_swarm_mobile_robot/API_rpi/image_proc/tests/aruco_test /home/andrija1911/Desktop/Thesis/api_swarm_mobile_robot/API_rpi/image_proc/tests/aruco_test /home/andrija1911/Desktop/Thesis/api_swarm_mobile_robot/API_rpi/image_proc/tests/aruco_test /home/andrija1911/Desktop/Thesis/api_swarm_mobile_robot/API_rpi/image_proc/tests/aruco_test /home/andrija1911/Desktop/Thesis/api_swarm_mobile_robot/API_rpi/image_proc/tests/aruco_test/CMakeFiles/ar.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ar.dir/depend
