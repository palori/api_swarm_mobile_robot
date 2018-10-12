
#include "comm_ard.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  
  test_write();

  test_read();
}
