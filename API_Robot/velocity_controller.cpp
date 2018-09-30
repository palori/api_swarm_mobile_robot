#include "Arduino.h"
#include "utils.h"
#include "pins.h"
#include "motor.h"
#include "Encoder.h"
#include "velocity_controller.h"

using namespace std;


Velocity_controller::Velocity_controller(int motor_id):motor(LEFT_MOTOR),encoder(PIN_LEFT_ENCODER_A,PIN_LEFT_ENCODER_B){
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

}

float Velocity_controller::getVelocity(){

	pulse=encoder.read();
	delay(time_interval);
	newpulse=encoder.read();
	velocity=-(newpulse-pulse)*2*M_PI/pulses_per_rotation/gear_ratio/(time_interval/1000.0);

	return velocity;
}

void Velocity_controller::setVelocity(float vel){
	_vel=vel;
	motor.setVelocity(_vel);

}