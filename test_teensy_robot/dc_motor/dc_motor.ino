#include <motor.h>
#include <pins.h>
#include <utils.h>

Motor motor1(LEFT_MOTOR);
Motor motor2(RIGHT_MOTOR);

int speed=4096;

void setup() 
{ 
  setUpPowerPins();
  

  Serial.begin(9600);
  
  while (! Serial);
  Serial.println("Speed 0 to 4096");
} 
 
 
void loop() 
{ 
  if (Serial.available())
  {
    speed = Serial.parseInt();
    
    if (speed >= 2048 && speed <= 4096)
    {
      enableMotors();
      Serial.print(String(speed));
      motor1.setVelocity(speed);
      motor2.setVelocity(speed);
    } else {
      //digitalWrite(right_pwm_pin, LOW); 
      disableMotors(); 
    }
  }
}
