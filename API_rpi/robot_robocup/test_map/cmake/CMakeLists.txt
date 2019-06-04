cmake_minimum_required (VERSION 2.8) 
project (main_map_project)


find_package(Threads)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-lpthread -lzmqpp -lzmq -lsodium -Wl,--no-as-needed")

IF  (Threads_FOUND)
	MESSAGE(STATUS "COMPILING robot + map with cmake")
	add_executable (main_map ../../maps/maps.cpp ../../graph/node.cpp ../../graph/edge.cpp ../../graph/graph.cpp ../../graph/dijkstra.cpp ../../main_robot.cpp ../../robot.cpp ../../utils.cpp ../../item.h ../../sensors.cpp ../../robot_params.cpp ../../controllers.cpp ../../messages.cpp ../../comm_rpi_2.cpp ../../pub_subs/publisher.cpp ../../pub_subs/subscriber.cpp)  

	target_link_libraries (main_map ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_C_FLAGS})
ELSE()
	MESSAGE(FATAL_ERROR "threads not found in your system")
ENDIF()




