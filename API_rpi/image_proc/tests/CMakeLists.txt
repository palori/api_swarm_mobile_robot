cmake_minimum_required(VERSION 2.8)
project( First_image_tests )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( t tests.cpp )
target_link_libraries( t ${OpenCV_LIBS} )