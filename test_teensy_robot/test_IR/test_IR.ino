//#include <servo.h> //use the one already done by Arduino
//#include <velocity_controller.h>
#include <ir.h>
//#include <encoder.h>
//#include <motor.h>
//#include <mpu9150.h>
#include <pins.h>
#include <utils.h>


IR ir1(PIN_IR_RAW_1);
IR ir2(PIN_IR_RAW_2);

void setup() {
  // put your setup code here, to run once:
  setUpPowerPins();
 
  Serial.begin(9600);
}



void loop() {
  
  checkBattery();

  // Test
  float dist1=ir1.getDistance();
  float dist2=ir2.getDistance();
  
  Serial.print("Distance 1: ");
  Serial.println(String(dist1));
  Serial.print("Distance 2: ");
  Serial.println(String(dist2));
  delay(1000);
}
