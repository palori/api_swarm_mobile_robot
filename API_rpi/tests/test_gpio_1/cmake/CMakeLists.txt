cmake_minimum_required (VERSION 2.8) 
project (test_gpio_project)


find_package(Threads)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-lpthread -lzmqpp -lzmq -lsodium -lwiringPi -Wl,--no-as-needed")

IF  (Threads_FOUND)
	MESSAGE(STATUS "COMPILING test gpio")
	add_executable (test_gpio ../test_gpio_led.cpp ../../../raspberry_pi/gpio/gpio.cpp ../../../raspberry_pi/leds.cpp ../../../raspberry_pi/pins.h ../../../utils/utils.cpp)  

	target_link_libraries (test_gpio ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_C_FLAGS})
ELSE()
	MESSAGE(FATAL_ERROR "threads not found in your system")
ENDIF()




