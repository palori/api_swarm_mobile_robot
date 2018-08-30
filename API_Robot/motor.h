#ifndef motor_h
#define motor_h

#include "Arduino.h"

class Motor
{
public:
	Motor();
	void setVelocity(float velocity);
	void setDirection(float direction);
	//setVoltage();
	//void enableMotors();

private:
	int _motor;
	int _dir_pin;
	int _pwm_pin;
};

#endif