#include <ir.h>
#include <pins.h>
#include <utils.h>

IR ir_1(PIN_IR_RAW_1);
IR ir_2(PIN_IR_RAW_2);

void setup() {
  // put your setup code here, to run once:
  setUpPowerPins();
  Serial.begin(9600);
  delay(1000);
  ir_1.setCalibration();
  ir_2.setCalibration();
}



void loop() {
  
  checkBattery();

  // Test

  uint16_t data1 = ir_1.getData();
  uint16_t data2 = ir_2.getData();
  
  float dist1 = ir_1.getDistance();
  float dist2 = ir_2.getDistance();
  
  Serial.println("Data1: "+String(data1)+"      Distance 1: "+String(dist1));
  Serial.println("Data2: "+String(data2)+"      Distance 2: "+String(dist2));
  
  delay(100);
}
