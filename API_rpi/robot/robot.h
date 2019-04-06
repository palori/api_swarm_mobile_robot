#ifndef robot_h
#define robot_h

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>		// only for sleeping the threads while testing

#include "item.h"
#include "robot_params.h"
#include "sensors.h"
//#include "controllers.h"
#include "messages.h"
#include "comm_rpi_2.h"
#include "../publisher.h"
#include "../subscriber.h"
//#include "localization.h"
//#include "task_planner.h"
#include "utils.h"

using namespace std;


class Robot
{
public:
	Robot();
	~Robot();

	//Robot(string hostname);
	Robot(string hostname, string hostname_a, string hostname_b, int max_len, int port_image, int port_task, int port_info, int port_info_robot_a, int port_info_robot_b);

	////////////////
	// Attributes //
	////////////////
	Robot_params params;		// Sensible data, 2 threads may try
								// to read/write at the same time!!!
								// Using mutex to prevent that ;)
	Robot_params robot_a;		// Save info from the other robots
	Robot_params robot_b;

	Sensors sensors;			// Store last sensor data read from serial
								// and image data from the image process


	COMM_RPI serial_comm;
	Publisher pub_image_task;		// what to look for in the image pocess
	Publisher pub_robot_info;		// send info to the rest of the robots
	//Subscriber subs_image_data;	// features extracted from image processing
	//Subscriber subs_robot_a;		// susbcribe to 2 other robots A and B
	//Subscriber subs_robot_b;		// maybe in an array????
	//Localization loc;
	// Task_planner task_planner;


	Item<string> image_data;
	Item<bool> run_mission;


	int count=0; // only to test (delete after)


	/////////////
	// Methods //
	/////////////


	// Each of them will run in a separate thread
	void serial();
	void listen_image_process();
	void listen_robot_a(); 	// maybe make it general to be scalable to 'n' robots
	void listen_robot_b();

	void send_task();

	void run();						// start the threads and while true loop
	
};

#endif
