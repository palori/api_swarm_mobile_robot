
#include "leds.h"



Leds::Leds(){
	
	cout << "\nSetup Leds\n" << endl;
    wiringPiSetup();
    pinMode(LED_KA, OUTPUT);
    pinMode(LED_LEADER, OUTPUT);
    pinMode(LED_PLAN_NAV, OUTPUT);
    pinMode(LED_TASK, OUTPUT);

    // setting initial state to 0 (turn all off)
    turn_off_all();

    T_blink = 1000;

}

Leds::~Leds(){
	turn_off_all();
}

void Leds::turn_off_all(){
	ka = 0;
    leader = 0;
    plan_nav = 0;
    task = 0;

	set_state(LED_KA, 0);
    set_state(LED_LEADER, 0);
    set_state(LED_PLAN_NAV, 0);
    set_state(LED_TASK, 0);

    digitalWrite (LED_KA, 0);
    digitalWrite (LED_LEADER, 0);
    digitalWrite (LED_PLAN_NAV, 0);
    digitalWrite (LED_TASK, 0);
}

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
	//delay(T_blink/2);
}

void Leds::election(){
	int new_state = set_state(LED_LEADER, 2);	// blink
	digitalWrite (LED_LEADER, new_state);
	//delay(T_blink/2);
}

// keep alive (KA) - to know that the robot is still on
void Leds::keep_alive(){
	int new_state = set_state(LED_KA, 2);
	digitalWrite (LED_KA, new_state);
	delay(T_blink/2);
}

// task related
void Leds::task_doing(int state){
	int new_state = set_state(LED_TASK, state);
	digitalWrite (LED_TASK, new_state);
	//delay(T_blink/2);
}
void Leds::task_allocation(){	// the leader computes it
	int new_state = set_state(LED_TASK, 2);
	digitalWrite (LED_TASK, new_state);
	//delay(T_blink/2);
}

// planning and navigation
void Leds::planning_route(){
	int new_state = set_state(LED_PLAN_NAV, 2);
	digitalWrite (LED_PLAN_NAV, new_state);
	//delay(T_blink/2);
}

void Leds::navigating(int state){
	int new_state = set_state(LED_PLAN_NAV, state);
	digitalWrite (LED_PLAN_NAV, new_state);
	//delay(T_blink/2);
}

