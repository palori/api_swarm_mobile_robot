#include "Arduino.h"
#include "ir.h"
using namespace std;

IR::IR(int dist_pin){
	_dist_pin = dist_pin;
	_dist = 10; // default
	pinMode(_dist_pin,INPUT);

	setCalibration();
}
	
uint16_t IR::getData(){
	uint16_t sum = 0;
	int iterations = 5;
	for (int i = 0; i < iterations; ++i){
		sum += analogRead(_dist_pin); 
	}
	return sum/iterations;
}

float IR::getDistance(){
	_irRaw = getData();
	if (_irRaw > 0) _dist = irA / (_irRaw - irB);//_dist = irA + irB/_irRaw;

	// conditions to set max value and others...
	/*
	if (_dist < 0) _dist = 0;
	else if (_dist > 10) _dist = 10;
	*/

	// else stays with the previous value, default in the constructor.
	return _dist;
}

void IR::setCalibration(){
	// Default values, tried manually
	irCal20cm = 135;//200.0;
	irCal80cm = 35;//711.0;
	// get from RPi new values of:
	// ...

	// Compute just once the params to convert the raw data to distance [m]
	irA = 0.2 * (irCal20cm - 4 * irCal80cm) / (irCal20cm - irCal80cm);
    irB = 0.6 * irCal20cm * irCal80cm / (irCal20cm - irCal80cm);

    irA = 23.5;
    irB = 13.0; //values from MATLAB script calibIRParams
    String msg = "************************\n* IR calibration "+String(_dist_pin)+"*\n* irA = "+String(irA)+"*\n* irB = "+String(irB)+"*\n************************";
    Serial.println(msg);
}

String IR::getCalibration(){
	return "{'irCal20cm':"+String(irCal20cm)+"'irCal20cm':"+String(irCal20cm)+"}";
}

//void IR::sendStatus(){}
