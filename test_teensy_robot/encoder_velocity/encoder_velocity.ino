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

float pulse_per_rotation=48.0;
float gear_ratio=9.68;

float getVelocity (int enc){
  int newpulse;
  int pulse;
  float time_interval=100.0;
  
  if (enc==1){
    pulse=encoder1.read();  
  }else{
    pulse=encoder2.read();   
  }

 
  delay(time_interval);
  if (enc==1){
    newpulse=encoder1.read();  
  }else{
    newpulse=encoder2.read();   
  }
  time_interval=time_interval/1000.0;
  float velocity=-(newpulse-pulse)*2*M_PI/pulse_per_rotation/gear_ratio/time_interval;
return velocity;
}

void setup() 
{ 

  setUpPowerPins(); 
  Serial.begin(9600);  
  while (! Serial);
  Serial.println("Speed 0 to 10.0");
} 
 
 
void loop() 
{ 
    delay(1000);
    float velocity1=getVelocity(1);
    float velocity2=getVelocity(2);
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
