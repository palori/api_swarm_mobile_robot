#ifndef ir_h
#define ir_h

#include "Arduino.h"

class IR
{
public:
	IR(int pin);
	uint16_t getData();
	float getDistance();
	void setCalibration();
	String getCalibration();
	//void sendStatus();

private:
	int _dist_pin;
	//uint16_t irCal10cm;
	uint16_t irCal20cm;
	//uint16_t irCal40cm;
	uint16_t irCal80cm;
	uint16_t _irRaw;
	float irA;
	float irB;
	float _dist;
};

#endif