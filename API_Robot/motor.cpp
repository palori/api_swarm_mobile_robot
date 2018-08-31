#include "Arduino.h"
#include "motor.h"
#include "pins.h"
#include <cmath>

Motor::Motor(int motor){
	_motor=motor;

  	if (_motor==LEFT_MOTOR){
  		_dir_pin=PIN_LEFT_DIR;
  		_pwm_pin=PIN_LEFT_PWM;
  	} else if (_motor==RIGHT_MOTOR) {
  		_dir_pin=PIN_RIGHT_DIR;
  		_pwm_pin=PIN_RIGHT_PWM;
  	}

  	pinMode(_dir_pin, OUTPUT);
  	pinMode(_pwm_pin, OUTPUT);

  	analogWriteFrequency(_pwm_pin,20000);
  	analogWriteResolution(12);
  	

}

void Motor::setVelocity(float velocity){
	_velocity=velocity;
  _pwm=vel2pwm(abs(_velocity);

	if (_velocity<0){
		digitalWrite(_dir_pin,LOW);
		analogWrite(_pwm_pin,_pwm);		
	}else if (_velocity >0){
		digitalWrite(_dir_pin,HIGH);
		analogWrite(_pwm_pin,_pwm);
	}
		
}

int vel2pwm(float velocity){
  _pwm_result=4096(1-_velocity/MAX_VELOCITY);
  return _pwm_result;
}



