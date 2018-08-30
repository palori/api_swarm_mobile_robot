#ifndef utils_h
#define utils_h

#include "Arduino.h"
#include "pins.h"

void setUpPowerPins();
void checkBattery();


void enableMotors();
void disableMotors();

void enableIR();
void disableIR();


#endif