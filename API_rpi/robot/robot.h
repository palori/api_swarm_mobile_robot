
#include "comm_rpi_1.h"
#include "publisher.h"
#include "subscriber.h"
//#include "localization.h"
//#include "task_planner.h"
#include "utils.h"

#include <iostream>
#include <thread>

using namespace std;

class Robot
{
public:
	Robot(int id);
	~Robot();
	void run();			// while true loop

	int robot_id;
	float x;
	float y;
	float z;
	float th;
	string task;

	// getters and setters!!!





	COMM_RPI serial_comm;
	Publisher pub_image_task;	// what to look for in the image pocess
	Publisher pub_robot_info;	// send info to the rest of the robots
	Subscriber subs_image_data;	// features extracted from image processing
	Subscriber subs_robot_a;	// susbcribe to 2 other robots A and B
	Subscriber subs_robot_b;	// maybe in an array????
	//Localization loc;
	// Task_planner task_planner;

	void listen_image_process();
	void listen_robot(int id);
	void task_planner_run();
	
};


int main(int argc, char const *argv[])
{	
	// read YAML file with params to define the robot
	Robot robot(0);
	robot.run();		// while true loop
	return 0;
}