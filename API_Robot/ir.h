#ifndef ir_h
#define ir_h

#include "Arduino.h"

class IR
{
public:
	IR(int pin);
	float getData();
	float getDistance();
	void calibrate(float data_10, float data_20, float data_40, float data_80);
	//void sendStatus();

private:
	int _dist_pin;
	float _data;
	float _dist;
	float _data_10;
	float _data_20;
	float _data_40;
	float _data_80;
};

#endif