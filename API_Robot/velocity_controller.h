#ifndef velocity_controller_h
#define velocity_controller_h

#include "Arduino.h"

class Velocity_controller
{
public:
	Velocity_controller(int motor);
	void stop();
	void setVelocity(float vel);

	//getTrajectory(); from RPi??
};

#endif