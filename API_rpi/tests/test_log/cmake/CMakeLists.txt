cmake_minimum_required (VERSION 2.8) 
project (test_log_project)


find_package(Threads)

SET(TEST_PC false CACHE BOOL "Some user-specified option")

IF (TEST_PC)
	set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-lpthread -lzmqpp -lzmq -lsodium -Wl,--no-as-needed")
ELSE()
	set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-lpthread -lzmqpp -lzmq -lsodium -lwiringPi -Wl,--no-as-needed")
ENDIF()

IF  (Threads_FOUND)
	MESSAGE(STATUS "COMPILING robot + map with cmake")
	add_executable (
		test_log

		../test_log.cpp 

		../../../utils/logging.cpp

		../../../utils/utils.cpp 
		../../../utils/item.h
	)  

	target_link_libraries (test_log ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_C_FLAGS})
ELSE()
	MESSAGE(FATAL_ERROR "threads not found in your system")
ENDIF()




