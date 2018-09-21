#ifndef velocity_controller_h
#define velocity_controller_h

#include "Arduino.h"
#include "Encoder.h"
#include "motor.h"
#include <PID_v1.h> //Version 1.2.1, by Brett Beauregard (need to install from Arduino IDE)

using namespace std;

class Velocity_controller
{
	int _motor_id;
	int _encoder_pinA;
	int _encoder_pinB;
	Motor motor;
	Encoder encoder;
	PID _pid;


public:
	Velocity_controller(int motor);
	void stop();
	void setVelocity(double vel);
	double getVelocity();
	void setSP(double vel_sp);
	void run();
	

	//getTrajectory(); from RPi??

private:
	
	double _vel;
	double gear_ratio=9.68;
	double pulses_per_rotation=48.0;
	int newpulse;
	int pulse;
	double time_interval=100;  //delay time 
	double velocity;
	//Motor motor;//(int _motor_id);
	//Encoder encoder;//(int _encoder_pinA, int _encoder_pinB);
	double Input, Output, Setpoint, Kp, Ki, Kd;
	int Ts;
};

#endif