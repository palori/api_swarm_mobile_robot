
#include "leds.h"



Leds::Leds(){
	leader = new GPIOClass(xtos(LED_LEADER));
	
	cout << "\nSetup Leds\n" << endl;
    wiringPiSetup();
    pinMode(LED_KA, OUTPUT);
    pinMode(LED_LEADER, OUTPUT);
    pinMode(LED_PLAN_NAV, OUTPUT);
    pinMode(LED_TEST, OUTPUT);

    // setting initial state to 0 (turn all off)
    ka = 0;
    leader = 0;
    plan_nav = 0;
    task = 0;

    set_state(LED_KA, 0);
    set_state(LED_LEADER, 0);
    set_state(LED_PLAN_NAV, 0);
    set_state(LED_TEST, 0);

}

Leds::~Leds(){}

int Leds::set_state(int pin_led, int state){
	int new_state = -1;
	if (state == 0){			// turn off
		new_state = LOW;
		// here is not important what is the state of the variables (ka, le, plan_nav, task)
	}
	else if (state == 1){		// turn on
		new_state = HIGH;
		// here is not important what is the state of the variables (ka, le, plan_nav, task)
	}
	else if (state == 2){		// blink
		
		if (pin_led == LED_KA){
			if (ka == 0){
				ka = 1;
				new_state = HIGH;
			}
			else{
				ka = 0;
				new_state = LOW;
			}
		}
		else if (pin_led == LED_LEADER){
			if (leader == 0){
				leader = 1;
				new_state = HIGH;
			}
			else{
				leader= 0;
				new_state = LOW;
			}
		}
		else if (pin_led == LED_PLAN_NAV){
			if (plan_nav == 0){
				plan_nav = 1;
				new_state = HIGH;
			}
			else{
				plan_nav = 0;
				new_state = LOW;
			}
		}
		else if (pin_led == LED_TASK){
			if (task == 0){
				task = 1;
				new_state = HIGH;
			}
			else{
				task = 0;
				new_state = LOW;
			}
		}

	}
	return new_state;
}


// leader election (LE)
void Leds::is_leader(int state){
	int new_state = set_state(LED_LEADER, state);
	digitalWrite (LED_LEADER, new_state);
	delay(T_blink/2);
}

void Leds::election(){
	set_state(LED_LEADER, 2);	// blink
	digitalWrite (LED_LEADER, new_state);
	delay(T_blink/2);
}

// keep alive (KA) - to know that the robot is still on
void Leds::keep_alive(int state){
	set_state(LED_KA, state);
	digitalWrite (LED_LEADER, new_state);
	delay(T_blink/2);
}

// task related
void Leds::task_doing(){}
void Leds::task_allocation(){}		// the leader computes it

// planning and navigation
void Leds::planning_route(){}
void Leds::navigating(){}

