#include <motor.h>
#include <pins.h>
#include <utils.h>
#include <Encoder.h>

Motor motor1(LEFT_MOTOR);
Motor motor2(RIGHT_MOTOR);

//encoder give 48 pulses per revolution
//gearing ratio between shaft and encoder is 1:9.68
Encoder encoder1(PIN_LEFT_ENCODER_A,PIN_LEFT_ENCODER_B);
Encoder encoder2(PIN_RIGHT_ENCODER_A,PIN_RIGHT_ENCODER_B);

float revolutions1;
float revolutions2;

void setup() 
{ 
  setUpPowerPins(); 
  Serial.begin(9600);  
  while (! Serial);
  Serial.println("Speed 0 to 10.0");
} 
 
 
void loop() 
{ 
  if (Serial.available())
  {
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
    
    revolutions1=-encoder1.read()/48/9.68;
    revolutions2=-encoder2.read()/48/9.68;
    Serial.print(revolutions1);
    Serial.print("    ");
    Serial.println(revolutions2);
  }
}
