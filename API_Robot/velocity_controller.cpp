#include "Arduino.h"
#include "utils.h"
#include "pins.h"
#include "motor.h"
#include "Encoder.h"
#include "velocity_controller.h"
#include <PID_v1.h> //Version 1.2.1, by Brett Beauregard (need to install from Arduino IDE)

using namespace std;


Velocity_controller::Velocity_controller(int motor_id):motor(motor_id),encoder(_encoder_pinA,_encoder_pinB),_pid(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT){
	//Serial.println("Motor ID: "+String(motor_id));
	_motor_id=motor_id;
	//motor = Motor(_motor_id);

	if (_motor_id==LEFT_MOTOR){
  		_encoder_pinA=PIN_LEFT_ENCODER_A;
  		_encoder_pinB=PIN_LEFT_ENCODER_B;
  	} else if (_motor_id==RIGHT_MOTOR) {
  		_encoder_pinA=PIN_RIGHT_ENCODER_A;
  		_encoder_pinB=PIN_RIGHT_ENCODER_B;
  	}

	//encoder = Encoder(_encoder_pinA,_encoder_pinB);
	Kp = 2;
	Ki = 0;
	Kd = 0;
	Setpoint = 0;
	Input = -1;
	Ts = 500; // milliseconds
  	_pid = PID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
  	_pid.SetMode(AUTOMATIC); // turn the PID on
  	_pid.SetSampleTime(Ts); // milliseconds
  	//_pid.SetOutputLimits(double min, double max);
  	//_pid.SetTunings(double Kp, double Ki, double Kd);
}

double Velocity_controller::getVelocity(){

	pulse=encoder.read();
	delay(time_interval);
	newpulse=encoder.read();
	velocity=-(newpulse-pulse)*2*M_PI/pulses_per_rotation/gear_ratio/(time_interval/1000.0);
	Serial.println("      # Vel m/ms = "+String(1000*velocity));
	return velocity;
}

void Velocity_controller::setVelocity(double vel){
	_vel=vel;
	motor.setVelocity(_vel);

}

void Velocity_controller::setSP(double vel_sp){
	Serial.println("M" + String(_motor_id));
	if (vel_sp != Setpoint){
		Setpoint = vel_sp;
		_pid = PID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
  		_pid.SetMode(AUTOMATIC);
	}
	Serial.println("  SP vel: " + String(Setpoint));
}

void Velocity_controller::run(){
	Input = getVelocity()-Setpoint;
	_pid.Compute();
	setVelocity(Output);
	Serial.println("M" + String(_motor_id));
	Serial.println("    in: " + String(Input) + "       out: " + String(Output));
}