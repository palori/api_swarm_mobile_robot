#ifndef motor_h
#define motor_h

#include "Arduino.h"

#define MAX_VELOCITY 6.5

class Motor
{
public:
	Motor();
	Motor(int motor);
	void setVelocity(double velocity);
	//setVoltage();
	//void enableMotors();

private:
	int _motor;
	int _dir_pin;
	int _pwm_pin;
	double _velocity;
	int _pwm;
};

int vel2pwm(double velocity);

#endif