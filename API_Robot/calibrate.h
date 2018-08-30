#ifndef CALIBRATE_H
#define CALIBRATE_H

/*
	Modify the values when runing "/test_teensy_robot/calibration/calibration.ino".
*/

// Calibrate IR 1
#define IR_1_10		300
#define IR_1_20		250
#define IR_1_40		100
#define IR_1_80		30

// Calibrate IR 2
#define IR_2_10		300
#define IR_2_20		250
#define IR_2_40		100
#define IR_2_80		30



#include "Arduino.h"

extern int menu_step;
extern int sub_step;
extern String menu[4];

int getInput();
void printHeader();
void printEnding();
void printSectionHeader(String text);
void printSectionEnding(String text);
void printMenu();

#endif
