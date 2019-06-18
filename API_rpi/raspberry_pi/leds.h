#ifndef leds_h
#define leds_h

//#include <unistd.h>				// usleep()

#include "../utils/utils.h"
#include "pins.h"
#include <wiringPi.h>


using namespace std;


class Leds
{
public:
	Leds();
	~Leds();

	void turn_off_all();

	void set_state(int pin_led, int state);

	// use the leds to show how are the different states of the robots

	// pins
	int ka;				// green
	int leader;			// yellow
	int plan_nav;		// blue
	int task;			// red

	int T_blink;		// blinking period


	/*
	
	DELETE DELAYS!!!
	
	try to use elapsed time instead of delays!!!

	*/

	// leader election (LE)
	void is_leader(int state);
	void election();

	// keep alive (KA) - to know that the robot is still on
	void keep_alive();

	// task related
	void task_doing(int state);
	void task_allocation();		// the leader computes it

	// planning and navigation
	void planning_route();
	void navigating(int state);

	
};

#endif