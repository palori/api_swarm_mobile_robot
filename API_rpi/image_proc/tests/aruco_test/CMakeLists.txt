cmake_minimum_required (VERSION 2.8) 
project (raspicam_test_aruco)

set(CMAKE_MODULE_PATH /usr/local/lib/cmake)

find_package(raspicam REQUIRED)
find_package(OpenCV 3.4)
IF  ( OpenCV_FOUND AND raspicam_CV_FOUND)
	MESSAGE(STATUS "COMPILING OPENCV TESTS")
	add_executable (aruco aruco_test.cpp)  
	target_link_libraries (aruco ${raspicam_CV_LIBS})
	add_definitions(-Draspicam_CV_LIBS)

	include_directories(${aruco_INCLUDE_DIRS})

ELSE()
	MESSAGE(FATAL_ERROR "OPENCV NOT FOUND IN YOUR SYSTEM")
ENDIF()




