#ifndef CALIBRATE_H
#define CALIBRATE_H

/*
	Modify the values when runing "/test_teensy_robot/calibration/calibration.ino".
*/

/*
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
*/


#include "Arduino.h"
using namespace std;

typedef struct{
  uint16_t array[2];
}irData;

typedef struct{
  uint16_t calib1[4];
  uint16_t calib2[4];
}irCalibData;

// Calibrate IR 1
extern uint16_t IR_1_10;
extern uint16_t IR_1_20;
extern uint16_t IR_1_40;
extern uint16_t IR_1_80;

// Calibrate IR 2
extern uint16_t IR_2_10;
extern uint16_t IR_2_20;
extern uint16_t IR_2_40;
extern uint16_t IR_2_80;

extern int menu_step;
extern int sub_step;
extern irCalibData calib;
extern String menu[4];

int getInput();
void printHeader();
void printEnding();
void printSectionHeader(String text);
void printSectionEnding(String text);
void printMenu();

bool wait4confirmation();
irData getIrData();
void calibrateDistance(int cm);
void acknowledge();
void saveIrCalib();
void printCurrentCalib();

#endif
