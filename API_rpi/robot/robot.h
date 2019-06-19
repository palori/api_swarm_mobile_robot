#ifndef robot_h
#define robot_h

#include <iostream>
#include <thread>
#include <chrono>		// only for sleeping the threads while testing
#include <math.h>

#include <errno.h>		//some of them needed for handling Ctrl^C
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "../utils/utils.h"
#include "../utils/item.h"

//#include "../robot/controllers.h"
#include "../robot/robot_params.h"
#include "../robot/sensors.h"

#include "../comm/messages.h"
#include "../comm/comm_rpi_2.h"
#include "../comm/pub_subs/publisher.h"
#include "../comm/pub_subs/subscriber.h"

//#include "localization.h" // non-existing file
//#include "../task_allocation/task_planner.h"

#include "../leader_election/bully.h"

#include "../path_planning/dijkstra.h"

#include "../maps/maps.h"

#include "../raspberry_pi/leds.h"

using namespace std;


// handle Ctrl^C
extern bool ctrl_c_pressed;
void sig_handler(int sig);




class Robot
{
public:
	Robot();
	~Robot();

	//Robot(string hostname);
	Robot(string hostname_master, 
		  string hostname, 
		  string hostname_a, 
		  string hostname_b, 
		  int id,
		  int max_len, 
		  int port_image, 
		  int port_task, 
		  int port_info, 
		  int port_info_robot_a, 
		  int port_info_robot_b,
		  int port_info_master,
		  int id_robot_a,
		  int id_robot_b,
		  int id_master);

	////////////////
	// Attributes //
	////////////////
	Robot_params params;		// Sensible data, 2 threads may try
								// to read/write at the same time!!!
								// Using mutex to prevent that ;)
	Robot_params robot_a;		// Save info from the other robots
	Robot_params robot_b;
	Robot_params master;

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

	Item<bool> debug;


	// leader election
	Bully bully;

	// visualisation of the robot states (KA, leader, tasks, planning&navitation)
	Leds leds;


	/////////////
	// Methods //
	/////////////


	// Each of them will run in a separate thread
	void serial();
	void listen_image_process();
	void listen_robot_a(); 	// maybe make it general to be scalable to 'n' robots
	void listen_robot_b();
	void listen_master();

	void send_keep_alive();
	void check_keep_alives();
	void leader_election();

	void check_le_messages(string msg);		// le = leader election

	//void send_task();

	void run();						// start the threads and while true loop
	
	void update_pose(float x0, float y0, float th0);
	void compute_distance(float x, float y, float *d_w, float *th_w);

	void navigate_0(Graph* map, string start_id, string end_id);
};

#endif
