#include "Arduino.h"
#include "ir.h"
using namespace std;

IR::IR(int dist_pin){
	_dist_pin = dist_pin;
	pinMode(_dist_pin,INPUT);
}
	
float IR::getData(){
	float sum = 0;
	for (int i = 0; i < 5; ++i){
		sum += analogRead(_dist_pin); 
	}
	return sum/5;
}

float IR::getDistance(){
	// ...
	_dist = getData();
	return _dist;
}

//void IR::sendStatus(){}
