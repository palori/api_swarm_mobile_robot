#include "Arduino.h"
#include "velocity_controller.h"

velocity_controller::velocity_controller(int motor){
	_motor=motor;
	
	pinMode(PIN_LEFT_DIR, OUTPUT);
  	pinMode(PIN_LEFT_PWM, OUTPUT);
  	pinMode(PIN_RIGHT_PWM, OUTPUT);
  	pinMode(PIN_RIGHT_DIR, OUTPUT);

  	pinMode(M12DIS,OUTPUT);



}

