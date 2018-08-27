#include "Arduino.h"
#include "ir.h"

IR::IR(int dist_pin){
	_dist_pin = dist_pin;
	pinMode(_dist_pin,INPUT);
}
	
float IR::getData(){
	return analogRead(_dist_pin);
}

float IR::getDistance(){
	float sum = 0;
	for (int i = 0; i < 5; ++i)
	{
		sum += getData();
		//Serial.println("    Sum: " + String(sum));  
	}

	_dist = sum/5;
	return _dist;
}

void IR::calibrate(float data_10, float data_20, float data_40, float data_80){
	//
}

//void IR::sendStatus(){}
