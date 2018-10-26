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
float R_WHEEL = 0.045;
int pulse[2]={0,0};
int newpulse[2]={1,1};
float velocity=0;

float vel=0.0;
float error=0.0;
float iError=0.0;
float out_vel=0.0;

float Kp=0.1;
float Ki=1;


//unsigned long newtime1,newtime2;
unsigned long calltime=0;
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
  
  velocity = velocity*R_WHEEL;
  return velocity;
}



void setup() 
{ 

  

  setUpPowerPins(); 
  Serial.begin(9600);  
  while (! Serial);
  Serial.println("Speed -5 to 5");
} 
 
 
void loop() 
{ 
    delay(10);

    float velocity1=getSpeed(LEFT_MOTOR,millis());
    float velocity2=getSpeed(RIGHT_MOTOR,millis());

    error=vel-velocity1;
    iError+=error*0.01;

    out_vel=Kp*error+Ki*iError;

    //if (out_vel<0.05){ out_vel=0;}

    motor1.setVelocity(out_vel);
    motor2.setVelocity(out_vel);

    Serial.println("Velocity1: "+String(velocity1));
    Serial.println("Velocity2: "+String(velocity2));

    Serial.println("Out_vel1: "+String(out_vel));

       
   if (Serial.available()){
      float vel_temp = Serial.parseFloat();
      if (vel_temp >= -5 && vel_temp <= 5)
      {
        vel = vel_temp;
        Serial.println("\n\nSetpoint: "+String(vel)+"\n\n");
        enableMotors();
        //Serial.print("Setpoint: ");
        //Serial.println(String(vel));
        //Serial.print("Speed: ");
        //Serial.println(String(out_vel));
          
      } else {
        //digitalWrite(right_pwm_pin, LOW);
        vel = 0;
        disableMotors(); 
      }  
    }

    
        




    
  
}
