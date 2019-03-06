cmake_minimum_required (VERSION 2.8) 
project (hbpzeq_tests)
MESSAGE(STATUS "COMPILING HBP ZEROEQ TESTS")
find_package(ZeroEQ REQUIRED)

include_directories( ${ZEROEQ_INCLUDE_DIRS} )
add_executable (p pub.cpp)  
#target_link_libraries (find_line )
	