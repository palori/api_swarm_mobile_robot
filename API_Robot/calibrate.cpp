#include "Arduino.h"
#include "calibrate.h"

int menu_step = 0;
int sub_step = 0;

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
  Serial.println("\nEnd calibrating: "+text);
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
