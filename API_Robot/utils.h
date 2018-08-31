#ifndef utils_h
#define utils_h

#include "Arduino.h"
#include "pins.h"

#include <vector>
using namespace std;

void setUpPowerPins();
void checkBattery();


void enableMotors();
void disableMotors();

void enableIR();
void disableIR();
vector<vector<String>> parse2DCsvFile(String inputFileName);

#endif