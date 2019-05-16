cmake_minimum_required (VERSION 2.8) 
project (raspicam_test_find_line)
find_package(raspicam REQUIRED)
find_package(OpenCV 3.4)
IF  ( OpenCV_FOUND AND raspicam_CV_FOUND)
	MESSAGE(STATUS "COMPILING OPENCV TESTS")
	add_executable (find_line detect_line.cpp ../../../comm_rpi_1.cpp ../../../utils.cpp)  
	target_link_libraries (find_line ${raspicam_CV_LIBS})
	add_definitions(-Draspicam_CV_LIBS)
ELSE()
	MESSAGE(FATAL_ERROR "OPENCV NOT FOUND IN YOUR SYSTEM")
ENDIF()




