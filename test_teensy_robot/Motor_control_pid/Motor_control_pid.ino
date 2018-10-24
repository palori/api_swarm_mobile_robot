#include <motor.h>
#include <pins.h>
#include <utils.h>
#include <Encoder.h>
#include <PID_v1.h>

Motor motor1(LEFT_MOTOR);
Motor motor2(RIGHT_MOTOR);

//encoder give 48 pulses per revolution
//gearing ratio between shaft and encoder is 1:9.68

Encoder encoder1(PIN_LEFT_ENCODER_A,PIN_LEFT_ENCODER_B);
Encoder encoder2(PIN_RIGHT_ENCODER_A,PIN_RIGHT_ENCODER_B);

float pulses_per_rotation=48.0;
float gear_ratio=9.68;
float R_WEEL = 4.5; // [m]

float speed = -1.0;
int pulse[2]={0,0};
int newpulse[2]={1,1};
float velocity=0;

//unsigned long newtime1,newtime2;
unsigned long calltime=0;
unsigned long oldtime1=0;
unsigned long oldtime2=0;

// PID
double Setpoint[2]={0.0,0.0}, Velocity[2]={0.0,0.0}, Output[2]={0.0,0.0};
double kp[2]={1.5,1.5}, ki[2]={1,1}, kd[2]={0,0};
//Specify the links and initial tuning parameters
PID pid1(&Velocity[0], &Output[0], &Setpoint[0], kp[0], ki[0], kd[0], DIRECT);
PID pid2(&Velocity[1], &Output[1], &Setpoint[1], kp[1], ki[1], kd[1], DIRECT);


float getSpeed(int id, unsigned long newtime){ 

  if (id==LEFT_MOTOR){
    newpulse[id-1]=encoder1.read(); 
    if (newtime==oldtime1 || oldtime1==0){
      velocity=0;  
    } else {
      velocity=-(newpulse[id-1]-pulse[id-1])*2*M_PI/pulses_per_rotation/gear_ratio/((newtime-oldtime1)/1000.0); // [rad/s]
    } 
    oldtime1=newtime;

    //Serial.println(oldtime1);
  } else if (id==RIGHT_MOTOR) {
    newpulse[id-1]=encoder2.read();
    if (newtime==oldtime2 || oldtime2==0){
      velocity=0;  
    } else {
      velocity=-(newpulse[id-1]-pulse[id-1])*2*M_PI/pulses_per_rotation/gear_ratio/((newtime-oldtime2)/1000.0); // [rad/s]
    } 
    oldtime2=newtime;
    //Serial.println(oldtime2);
  } 
  
  pulse[id-1]=newpulse[id-1];
  

  return velocity*R_WEEL; // [m/s]
}



void setup() 
{ 
  setUpPowerPins();
  
  //turn the PID on
  pid1.SetMode(AUTOMATIC);
  pid2.SetMode(AUTOMATIC);
  
  Serial.begin(9600);  
  while (! Serial);
  //Serial.println("Speed 0 to 10.0");
  Serial.println("Time,Vel1,Vel2,SP1,SP2,Out1,Out2");// Uncomment to save in .csv format
} 
 
 
void loop() 
{ 
  
    delay(10);
    calltime=millis();
    //Serial.println("\nTime: "+ String(calltime/1000.0));
    Velocity[0]=getSpeed(LEFT_MOTOR,calltime);
    calltime=millis();
    Velocity[1]=getSpeed(RIGHT_MOTOR,calltime);
    //Serial.println("Vel: "+String(Velocity[0])+", "+String(Velocity[1]));

   if (Serial.available()){
      speed = Serial.parseFloat();
      if (speed > -1 && speed <= 10)
      {
        enableMotors();
        Setpoint[0] = (double)speed;
        Setpoint[1] = (double)speed;
        
      } else {
        //digitalWrite(right_pwm_pin, LOW);
        Setpoint[0] = 0.0;
        Setpoint[1] = 0.0;
        disableMotors(); 
      }
      //Serial.println("Speed: "+String(speed));
   }
   //Serial.println("SP: "+String(Setpoint[0])+", "+String(Setpoint[1]));
   //pid1.Compute();
   //pid2.Compute();
   Output[0] = Setpoint[0];
   Output[1] = Setpoint[1];
   //Serial.println("Out: "+String(Output[0])+", "+String(Output[1]));
   motor1.setVelocity(Output[0]);
   motor2.setVelocity(Output[1]);

   // Uncomment to save in .csv format
   if (speed>-1){
      String s = String(calltime/1000.0);
      s += ","+String(Velocity[0])+","+String(Velocity[1]);
      s += ","+String(Setpoint[0])+","+String(Setpoint[1]);
      s += ","+String(Output[0])+","+String(Output[1]);
      Serial.println(s);
   }
   /* // Save in a 'csv' from a python script
   else if (speed == -10){
      delay(5000);
      Serial.println("123456"); //end string to save in the 'csv'
      speed = -1;
      Serial.println("Speed = -1");
   }*/
}
