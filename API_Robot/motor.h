#ifndef motor_h
#define motor_h

#include "Arduino.h"

#define MAX_VELOCITY 6.5

class Motor
{
public:
	Motor();
	Motor(int motor);
	void setVelocity(float velocity);
	//setVoltage();
	//void enableMotors();

private:
	int _motor;
	int _dir_pin;
	int _pwm_pin;
	float _velocity;
	int _pwm;
};

int vel2pwm(float velocity);

#endif