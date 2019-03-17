
//#include <ctime>
#include <iostream>
#include <thread>
#include "camera.h"


class Image_analysis
{
public:
	Image_analysis();
	~Image_analysis();

private:
	Camera cam;
	string message;			// ?? need to be encoded, might not need to be an attr
	int task;				// ?? decide if int, enum...
	int old_task;
	int publish_port;
	int subscibe_port;

public:

	// Getters
	string get_message();	// might not need to be an attr
	int get_task();
	int get_old_task();
	int get_publish_port();
	int get_subscibe_port();


	// Setters
	void set_message();		// might not need to be an attr
	void set_task();
	void set_old_task();
	void set_publish_port();
	void set_subscibe_port();



	void get_new_task();	// thread that subscribe to task_planner for ever
	void send_data();		//publish to the topic


	/* TASKS:
	 * - idle
	 * - follow line
	 * - detect crossing
	 * - detect ball
	 * - detect shape
	 * - detect obstacle
	 * - find ArUco code
	 */
	void run();				// loop forever


	void idle();
	void follow_line();
	void crossing();
	void ball();
	void shape();
	void obstacle();		// ?? maybe not needed
	void ArUco();
	
};