#include "Arduino.h"
#include "calibrate.h"
#include "ir.h"
#include "pins.h"
using namespace std;


// Calibrate IR 1
float IR_1_10 = 300;
float IR_1_20 = 250;
float IR_1_40 = 100;
float IR_1_80 = 50;

// Calibrate IR 2
float IR_2_10 = 300;
float IR_2_20 = 250;
float IR_2_40 = 100;
float IR_2_80 = 50;
int menu_step = 0;
int sub_step = 0;
IR ir1(PIN_IR_RAW_1);
IR ir2(PIN_IR_RAW_2);
irCalibData calib;

String menu[] = {"0. Press to return to this menu",
               "1. IR sensors (still working on it)",
               "2. IMU (not done)",
               "3. ?"};

int getInput(){
  int input = -1;
  while(input==-1){
    delay(1000);
    input = Serial.parseInt();
    if (input>0) break;
    input = -1;
  }
  return input;
}

void printHeader(){
  Serial.println("\n###########################");
  Serial.println("# CALIBRATION STARTING... #");
  Serial.println("###########################\n\n");
  delay(1000);
}

void printEnding(){
  Serial.println("\n###########################");
  Serial.println("# CALIBRATION ENDING... #");
  Serial.println("###########################\n\n");
}

void printSectionHeader(String text){
  Serial.println("\n###########################");
  Serial.println(text);
  sub_step = 0;
  delay(1000);
}

void printSectionEnding(String text){
  Serial.println("\nSaving calibration: "+text);
  Serial.println("###########################\n");
  menu_step = 0;
  delay(2000);
}

void printMenu(){
  Serial.println("\nWhat would you like to calibrate?");

  Serial.println("   " + menu[1]);
  Serial.println("   " + menu[2]);
  Serial.println("   " + menu[3]);

  Serial.println("\n   (A bigger integer will end the calibration process.)");

  Serial.println("\nPlease introduce now the integer:\n");
}

bool wait4confirmation(){
  bool isCorrect = false;
  while(true){
    delay(500);
    int i = getInput();
    if(i==1){
      isCorrect = true;
      break;
    }
    else if(i==2){
      break;
    }
  }
  return isCorrect;
}

irData getIrData(){
  irData ir;
  ir.array[0]=ir1.getData();
  ir.array[1]=ir2.getData();
  Serial.println("    IR 1: " + String(ir.array[0]));
  Serial.println("    IR 2: " + String(ir.array[1]));
  delay(1000);
  return ir;
}

void calibrateDistance(int cm){
  Serial.println("Place the 2 IR at "+String(cm)+"cm from a wall, then introduce '1'.");
  int index = 0;
  if(cm==10) index=0;
  else if(cm==20) index=1;
  else if(cm==40) index=2;
  else if(cm==80) index=3;
  else Serial.println("NOT possible to calibrate!");
  bool isCorrect = wait4confirmation();
  if (isCorrect){
    irData ir = getIrData();
    calib.calib1[index] = ir.array[0];
    calib.calib2[index] = ir.array[1];
    acknowledge();
  }
}

void acknowledge(){
  Serial.println("Are you happy with the values? '1'=yes, '2'=no\n");
  bool isHappy = wait4confirmation();
  if(isHappy) sub_step++;
}

void saveIrCalib(){
  // Calibrate IR 1
  IR_1_10 = calib.calib1[0];
  IR_1_20 = calib.calib1[1];
  IR_1_40 = calib.calib1[2];
  IR_1_80 = calib.calib1[3];

  // Calibrate IR 2
  IR_2_10 = calib.calib2[0];
  IR_2_20 = calib.calib2[1];
  IR_2_40 = calib.calib2[2];
  IR_2_80 = calib.calib2[3];

  // save in a txt file!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

void printCurrentCalib(){
  Serial.println("\nIR 1:");
  Serial.println("    10cm: "+String(IR_1_10));
  Serial.println("    20cm: "+String(IR_1_20));
  Serial.println("    40cm: "+String(IR_1_40));
  Serial.println("    80cm: "+String(IR_1_80));

  Serial.println("\nIR 2:");
  Serial.println("    10cm: "+String(IR_2_10));
  Serial.println("    20cm: "+String(IR_2_20));
  Serial.println("    40cm: "+String(IR_2_40));
  Serial.println("    80cm: "+String(IR_2_80)+"\n");
}