#ifndef leds_h
#define leds_h

#include <unistd.h>				// usleep()

#include "../utils/utils.h"
#include "pins.h"
#include "gpio/gpio.h"


using namespace std;


class Leds
{
public:
	Leds();
	~Leds();

	//void init();

	// use the leds to show how are the different states of the robots

	// pins
	GPIOClass* leader;			// yellow
	GPIOClass* ka;				// green
	GPIOClass* task;			// red
	GPIOClass* plan_nav;		// blue

	// leader election (LE)
	void is_leader();
	void election();

	// keep alive (KA) - to know that the robot is still on
	void keep_alive();

	// task related
	void task_doing();
	void task_allocation();		// the leader computes it

	// planning and navigation
	void planning_route();
	void navigating();

	
};

#endif