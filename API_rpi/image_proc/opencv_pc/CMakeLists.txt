cmake_minimum_required (VERSION 2.8) 
project (opencv_on_pc)
find_package(OpenCV 3.4 REQUIRED)
set(CMAKE_GXX_FLAGS ${CMAKE_GXX_FLAGS} "pkg-config --cflags opencv")

IF  ( OpenCV_FOUND )
	MESSAGE(STATUS "COMPILING OPENCV TESTS")
        include_directories(${OpenCV_INCLUDE_DIRS})	
	add_executable (cv2 opencv_pc.cpp)  
	target_link_libraries (cv2 ${openCV_LIBS})
	#add_definitions(-Draspicam_CV_LIBS)
ELSE()
	MESSAGE(FATAL_ERROR "OPENCV NOT FOUND IN YOUR SYSTEM")
ENDIF()



