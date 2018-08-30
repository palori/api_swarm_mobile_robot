#ifndef motor_h
#define motor_h

#include "Arduino.h"

class Motor
{
public:
	Motor(int motor);
	void setVelocity(int velocity);
	//setVoltage();
	//void enableMotors();

private:
	int _motor;
	int _dir_pin;
	int _pwm_pin;
	int _velocity;
};

#endif