cmake_minimum_required (VERSION 2.8) 
project (test_bully_project)


find_package(Threads)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-lpthread -lzmqpp -lzmq -lsodium -Wl,--no-as-needed")

IF  (Threads_FOUND)
	MESSAGE(STATUS "COMPILING test bully")
	add_executable (test_bully ../test_bully.cpp ../../../leader_election/bully.cpp ../../../utils/utils.cpp ../../../utils/item.h)  

	target_link_libraries (test_bully ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_C_FLAGS})
ELSE()
	MESSAGE(FATAL_ERROR "threads not found in your system")
ENDIF()




