#ifndef robot_h
#define robot_h

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>		// only for sleeping the threads while testing
#include <math.h>

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

#include "maps/maps.h"

using namespace std;


class Robot
{
public:
	Robot();
	~Robot();

	//Robot(string hostname);
	Robot(string hostname_master, string hostname, string hostname_a, string hostname_b, int max_len, int port_image, int port_task, int port_info, int port_info_robot_a, int port_info_robot_b);

	////////////////
	// Attributes //
	////////////////
	Robot_params params;		// Sensible data, 2 threads may try
								// to read/write at the same time!!!
								// Using mutex to prevent that ;)
	Robot_params robot_a;		// Save info from the other robots
	Robot_params robot_b;
	Item<string> hostname_master;

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


	Item<string> master_data;
	Item<string> init_pose;
	Item<string> image_data;
	Item<string> drive_command;
	Item<bool> run_mission;



	/////////////
	// Methods //
	/////////////


	// Each of them will run in a separate thread
	void serial();
	void listen_image_process();
	void listen_robot_a(); 	// maybe make it general to be scalable to 'n' robots
	void listen_robot_b();
	void listen_master();

	//void send_task();

	void run();						// start the threads and while true loop
	
	void update_pose(float x0, float y0, float th0);
	void compute_distance(float x, float y, float *d_w, float *th_w);

	void navigate_0(Graph* map, string start_id, string end_id);
	void navigate_test();
};

#endif
