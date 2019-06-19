cmake_minimum_required (VERSION 2.8) 
project (main_map_project)


find_package(Threads)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-lpthread -lzmqpp -lzmq -lsodium -lwiringPi -Wl,--no-as-needed")

IF  (Threads_FOUND)
	MESSAGE(STATUS "COMPILING robot + map with cmake")
	add_executable (
		main_map

		../../../main_robot.cpp 

		../../../robot/robot.cpp 
		../../../robot/sensors.cpp 
		../../../robot/robot_params.cpp 
		../../../robot/controllers.cpp 
		../../../robot/keep_alive.cpp 

		../../../utils/utils.cpp 
		../../../utils/item.h 

		../../../comm/messages.cpp 
		../../../comm/comm_rpi_2.cpp 
		../../../comm/pub_subs/publisher.cpp 
		../../../comm/pub_subs/subscriber.cpp

		../../../graph/node.cpp
		../../../graph/edge.cpp
		../../../graph/graph.cpp 

		../../../path_planning/dijkstra.cpp 

		../../../maps/maps.cpp

		../../../leader_election/bully.cpp

		../../../task_allocation/tasks.cpp

		../../../raspberry_pi/pins.h
		../../../raspberry_pi/leds.cpp
 
	)  

	target_link_libraries (main_map ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_C_FLAGS})
ELSE()
	MESSAGE(FATAL_ERROR "threads not found in your system")
ENDIF()




