#ifndef leds_h
#define leds_h

//#include <unistd.h>				// usleep()
#include <chrono>
#include <stdio.h>      /* printf */

#include "../utils/utils.h"
#include "../utils/item.h"
#include "pins.h"

#define TEST_NO_RPI
#ifndef TEST_NO_RPI
	#include <wiringPi.h>
#endif


using namespace std;

/* Maybe better to define them (see below)
enum led_state{
	OFF = 0,
	ON = 1,
	BLINK = 2
};
*/

#define OFF 	0
#define ON 		1
#define BLINK 	2

class Leds
{
public:
	Leds();
	~Leds();

	// use the leds to show how are the different states of the robots

	void turn_off_all();
	void refresh();

	// params that the robot can set
	Item<int> ka;
	Item<int> leader;
	Item<int> plan_nav;
	Item<int> task;

	Item<float> T_blink_ms; 	// blinking period [ms]

private:
	void get_time();
	void is_blinking_time();
	int set_state(int pin_led, int state);


	// internal states to use in case of the previous params are set to blink, then they can only be OFF or ON.
	Item<int> ka_state;
	Item<int> leader_state;
	Item<int> plan_nav_state;
	Item<int> task_state;

	Item<chrono::_V2::system_clock::time_point> now2;
	Item<chrono::_V2::system_clock::time_point> blink2;
	Item<bool> allow_blink;
	
};

#endif