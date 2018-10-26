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

float pulses_per_rotation=48.0;
float gear_ratio=9.68;

int pulse[2]={0,0};
int newpulse[2]={1,1};
float velocity=0;

//unsigned long newtime1,newtime2;
//unsigned long calltime=0;
unsigned long oldtime1=0;
unsigned long oldtime2=0;


float getSpeed(int id, unsigned long newtime){ 

  if (id==LEFT_MOTOR){
    newpulse[id-1]=encoder1.read(); 
    if (newtime==oldtime1 || oldtime1==0){
      velocity=0;  
    } else {
      velocity=-(newpulse[id-1]-pulse[id-1])*2*M_PI/pulses_per_rotation/gear_ratio/((newtime-oldtime1)/1000.0);
    } 
    oldtime1=newtime;

    //Serial.println(oldtime1);
  } else if (id==RIGHT_MOTOR) {
    newpulse[id-1]=encoder2.read();
    if (newtime==oldtime2 || oldtime2==0){
      velocity=0;  
    } else {
      velocity=-(newpulse[id-1]-pulse[id-1])*2*M_PI/pulses_per_rotation/gear_ratio/((newtime-oldtime2)/1000.0);
    } 
    oldtime2=newtime;
    //Serial.println(oldtime2);
  } 
  
  pulse[id-1]=newpulse[id-1];
  

  return velocity*0.045;
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
    //calltime=millis();
    float velocity1=getSpeed(LEFT_MOTOR,millis());
    float velocity2=getSpeed(RIGHT_MOTOR,millis());
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
