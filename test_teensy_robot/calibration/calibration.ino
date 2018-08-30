#include <utils.h>
#include <calibrate.h>
#include <ir.h>


void setup() {
  // put your setup code here, to run once:
  setUpPowerPins();
  Serial.begin(9600);
  delay(2000);
  printHeader();
}

void loop() {
  // put your main code here, to run repeatedly:
  checkBattery();
  if(menu_step==0){
    printMenu();
    menu_step = getInput();
  }
  else if(menu_step==1){
    printSectionHeader(menu[1]);
    pringCurrentCalib();
    while(true){
      if(sub_step==0){
        calibrateDistance(10);
      }
      else if(sub_step==1){
        calibrateDistance(20);
      }
      else if(sub_step==2){
        calibrateDistance(40);
      }
      else if(sub_step==3){
        calibrateDistance(80);
      }
      else{
        //save data into IR objects
        saveIrCalib();
        pringCurrentCalib();
        sub_step = 0;
        break;
      }
    }
    printSectionEnding(menu[1]);
    //menu_step = 0;
  }
  else if(menu_step==2){
    printSectionHeader(menu[2]);
    //sub_step = 0;
    // ...
    printSectionEnding(menu[2]);
    //menu_step = 0;
  }
  else if(menu_step==3){
    printSectionHeader(menu[3]);
    //sub_step = 0;
    // ...
    printSectionEnding(menu[3]);
    //menu_step = 0;
  }
  /*else{
    printEnding();
    while(true); // end of calibration
  }*/
}
