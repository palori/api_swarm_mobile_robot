
#include "leds.h"



Leds::Leds(){
	#ifndef TEST_NO_RPI
		cout << "\nSetup Leds\n" << endl;
		wiringPiSetup();
		pinMode(LED_KA, OUTPUT);
		pinMode(LED_LEADER, OUTPUT);
		pinMode(LED_PLAN_NAV, OUTPUT);
		pinMode(LED_TASK, OUTPUT);

		// setting initial state to 0 (turn all off)
		turn_off_all();
	#endif
	T_blink_ms.set(1000.0);	// default, may be changed
	get_time();
	blink2.set(now2.get() - std::chrono::hours(1));
	allow_blink.set(false);

}

Leds::~Leds(){
	turn_off_all();
}


void Leds::turn_off_all(){
	#ifndef TEST_NO_RPI
		ka.set(OFF);
		leader.set(OFF);
		plan_nav.set(OFF);
		task.set(OFF);
	#endif
	refresh();
}


void Leds::refresh(){
	is_blinking_time();
	#ifndef TEST_NO_RPI
		set_state(LED_KA, ka.get());
		set_state(LED_LEADER, leader.get());
		set_state(LED_PLAN_NAV, plan_nav.get());
		set_state(LED_TASK, task.get());

		digitalWrite (LED_KA, ka_state.get());
		digitalWrite (LED_LEADER, leader_state.get());
		digitalWrite (LED_PLAN_NAV, plan_nav_state.get());
		digitalWrite (LED_TASK, task_state.get());
	#endif
}


//chrono::milliseconds Leds::get_time(){
void Leds::get_time(){
	auto now_ = chrono::system_clock::now();
	this->now2.set(now_);
}

void Leds::is_blinking_time(){
	bool is_blink = false;
	get_time();
	chrono::duration<float> elapsed2 = now2.get() - blink2.get();
	
	//printf("  Elapsed2:\t%f\n", (elapsed2.count()));

	if (elapsed2.count() >= T_blink_ms.get()/2000.0) {
		is_blink = true;
		blink2.set(now2.get());
		//cout << "\n\n\n\n*** blinking time ***\n\n\n\n";
	}
	allow_blink.set(is_blink);
}


int Leds::set_state(int pin_led, int state){
	int new_state = -1;
	#ifndef TEST_NO_RPI
		if (state == OFF){			// turn off
			new_state = LOW;
			// here is not important what is the state of the variables (ka, le, plan_nav, task)
		}
		else if (state == ON){		// turn on
			new_state = HIGH;
			// here is not important what is the state of the variables (ka, le, plan_nav, task)
		}
		else if (state == BLINK && allow_blink.get()){		// blink
			
			if (pin_led == LED_KA){
				if (ka_state.get() == OFF){
					ka_state.set(ON);
					new_state = HIGH;
				}
				else{
					ka_state.set(OFF);
					new_state = LOW;
				}
			}

			else if (pin_led == LED_LEADER){
				if (leader_state.get() == OFF){
					leader_state.set(ON);
					new_state = HIGH;
				}
				else{
					leader_state.set(OFF);
					new_state = LOW;
				}
			}

			else if (pin_led == LED_PLAN_NAV){
				if (plan_nav_state.get() == OFF){
					plan_nav_state.set(ON);
					new_state = HIGH;
				}
				else{
					plan_nav_state.set(OFF);
					new_state = LOW;
				}
			}

			else if (pin_led == LED_TASK){
				if (task_state.get() == OFF){
					task_state.set(ON);
					new_state = HIGH;
				}
				else{
					task_state.set(OFF);
					new_state = LOW;
				}
			}

		}
	#endif
	return new_state;
}

