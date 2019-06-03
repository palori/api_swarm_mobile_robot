cmake_minimum_required (VERSION 2.8) 
project (aruco_test)

#set(CMAKE_MODULE_PATH /usr/local/lib/cmake)
#set(CMAKE_MODULE_PATH /home/andrija1911/installs/opencv/cmake)

#find_package(raspicam REQUIRED)
find_package(OpenCV REQUIRED)
#find_package(aruco REQUIRED)	#https://github.com/grace-/opencv-3.0.0-cvpr

IF  (OpenCV_FOUND)# AND raspicam_CV_FOUND)
	MESSAGE(STATUS "COMPILING OPENCV TESTS")
	add_executable (ar aruco.cpp)  
	#target_link_libraries (aruco ${raspicam_CV_LIBS})
	#target_link_libraries (ar aruco)
	#add_definitions(-Draspicam_CV_LIBS)

	#include_directories(${aruco_INCLUDE_DIRS})

ELSE()
	MESSAGE(FATAL_ERROR "OPENCV NOT FOUND IN YOUR SYSTEM")
ENDIF()




