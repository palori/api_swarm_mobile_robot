cmake_minimum_required (VERSION 2.8) 
project (raspicam_test_find_line)

set(CMAKE_MODULE_PATH /usr/local/lib/cmake)

find_package(raspicam REQUIRED)
<<<<<<< HEAD
find_package(OpenCV REQUIRED)
find_package(aruco REQUIRED)

IF  (OpenCV_FOUND AND raspicam_CV_FOUND)
=======
find_package(OpenCV 3.4)
IF  ( OpenCV_FOUND AND raspicam_CV_FOUND)
>>>>>>> 27fa771da4d636270ed9d21ddbc7c6c678a999fc
	MESSAGE(STATUS "COMPILING OPENCV TESTS")
	add_executable (find_line detect_line.cpp ../../../comm_rpi_1.cpp ../../../utils.cpp)  
	target_link_libraries (find_line ${raspicam_CV_LIBS})
	add_definitions(-Draspicam_CV_LIBS)

	include_directories(${aruco_INCLUDE_DIRS})

ELSE()
	MESSAGE(FATAL_ERROR "OPENCV NOT FOUND IN YOUR SYSTEM")
ENDIF()




