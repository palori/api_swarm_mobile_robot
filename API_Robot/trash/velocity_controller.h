#ifndef velocity_controller_h
#define velocity_controller_h

#include "Arduino.h"
#include "Encoder.h"
#include "motor.h"
using namespace std;

using namespace std;

class Velocity_controller
{

public:

	//Velocity_controller() {}
	
	Velocity_controller(int motor);
	void stop();
	void setVelocity(float vel);
	float getVelocity();

	

	//getTrajectory(); from RPi??

private:
	
	float _vel;
	float gear_ratio=9.68;
	float pulses_per_rotation=48.0;
	int newpulse;
	int pulse;
	float time_interval=100;  //delay time 
	float velocity;
	//Motor motor;//(int _motor_id);
	//Encoder encoder;//(int _encoder_pinA, int _encoder_pinB);

		int _motor_id;
	int _encoder_pinA;
	int _encoder_pinB;
	Motor motor;
	Encoder encoder = Encoder(0,0);


};

#endif