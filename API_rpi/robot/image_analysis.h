
//#include <ctime>
#include <iostream>
#include <thread>
#include <mutex>

#include "camera.h"
#include "publisher.h"
#include "subscriber.h"
#include "utils.h"


class Image_analysis
{
public:
	Image_analysis();
	~Image_analysis();

	Image_analysis(int publisher_port, int subscriber_port, double image_format, int image_height, int image_width); // add other input arguments

private:
	Camera cam;
	string message;			// ?? need to be encoded, might not need to be an attr
	int task;				// ?? decide if int, enum...
	int old_task;
	Publisher pub;
	Subscriber subs;

	mutex mtx_task;

public:

	// Getters
	string get_message();	// might not need to be an attr
	int get_task();
	int get_old_task();


	// Setters
	void set_message(string s);		// might not need to be an attr
	void set_task(int i);
	void set_old_task(int i);



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
	void idle();
	void follow_line();
	void crossing();		// ?? maybe not needed, integrated in 'follow_line'
	void ball();
	void shape();
	void obstacle();		// ?? maybe not needed
	void ArUco();

	void run();				// loop forever
	
};