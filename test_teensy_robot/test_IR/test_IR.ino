//#include <servo.h> //use the one already done by Arduino
//#include <velocity_controller.h>
#include <ir.h>
//#include <encoder.h>
//#include <motor.h>
//#include <mpu9150.h>
#include <pins.h>
#include <utils.h>


void setup() {
  // put your setup code here, to run once:
  setUpPowerPins();
 
  Serial.begin(9600);
}



void loop() {
  
  checkBattery();

  // Test
  float data1=ir1.getData();
  float data2=ir2.getData();
  
  Serial.print("Distance 1: ");
  Serial.println(String(data1));
  Serial.print("Distance 2: ");
  Serial.println(String(data2));
  delay(1000);
}
