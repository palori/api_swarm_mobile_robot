#include <velocity_controller.h>
#include <pins.h>
#include <utils.h>

Velocity_controller motor1(LEFT_MOTOR);
Velocity_controller motor2(RIGHT_MOTOR);

void setup() {
  // put your setup code here, to run once:
  setUpPowerPins(); 
  Serial.begin(9600);  
  while (! Serial);
  Serial.println("Speed 0 to 10.0");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  float velocity1=motor1.getVelocity();
  float velocity2=motor2.getVelocity();
  Serial.print(velocity1);
  Serial.print("    ");
  Serial.println(velocity2);
  
  if (Serial.available()){
    float speed = Serial.parseFloat();
    
    
    if (speed > -1 && speed <= 10)
    {
      enableMotors();
      Serial.print("Speed: ");
      Serial.println(String(speed));
      motor1.setVelocity(speed);
      motor2.setVelocity(speed);
        
    } else {
      //digitalWrite(right_pwm_pin, LOW); 
      disableMotors(); 
    }
    
      
  }
  
}
