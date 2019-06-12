cmake_minimum_required (VERSION 2.8) 
project (main_image_project)

find_package(raspicam REQUIRED)
find_package(OpenCV)
find_package(Threads)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-lpthread -lzmqpp -lzmq -lsodium -Wl,--no-as-needed")

IF  ( OpenCV_FOUND AND raspicam_CV_FOUND AND Threads_FOUND)
	MESSAGE(STATUS "COMPILING OPENCV TESTS")
	add_executable (main_image ../main_image.cpp ../image_analysis.cpp ../utils.cpp ../item.h ../robot_params.cpp ../messages.cpp ../pub_subs/publisher.cpp ../pub_subs/subscriber.cpp ../computer_vision.cpp ../camera.cpp)  

	target_link_libraries (main_image ${raspicam_CV_LIBS} ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_C_FLAGS})
	add_definitions(-Draspicam_CV_LIBS)
ELSE()
	MESSAGE(FATAL_ERROR "OPENCV NOT FOUND IN YOUR SYSTEM")
ENDIF()




