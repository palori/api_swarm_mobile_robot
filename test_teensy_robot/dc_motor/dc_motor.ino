#include <motor.h>
#include <pins.h>
#include <utils.h>

Motor motor1(LEFT_MOTOR);
Motor motor2(RIGHT_MOTOR);

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
    int speed = Serial.parseInt();
    
    if (speed >= 500 && speed <= 3500)
    {
      Serial.print(String(speed));
      motor1.setVelocity(speed);
      motor2.setVelocity(4096-speed);
    } else {
      //digitalWrite(right_pwm_pin, LOW);  
    }
  }
}
