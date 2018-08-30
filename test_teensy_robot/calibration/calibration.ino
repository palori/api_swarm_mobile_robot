#include <utils.h>
#include <calibrate.h>


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
    sub_step = 0;
    
    if(sub_step==0){
      Serial.println("Calibrate IR at 10cm");
      int curr_sub_step = sub_step;
      while(curr_sub_step<=sub_step){
        delay(1000);
        if(getInput()==1){
          sub_step++;
          break;
        }
      }
    }
    else if(sub_step==1){
      
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
  else{
    printEnding();
    while(true); // end of calibration
  }
}
