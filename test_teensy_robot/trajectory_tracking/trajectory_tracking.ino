#include <motor.h>
#include <pins.h>
#include <utils.h>
#include <Encoder.h>
#include <math.h>


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
float iError1=0.0;
float iError2=0.0;
float out_vel1=0.0;
float out_vel2=0.0;
float velocity1=0.0;
float velocity2=0.0;

float Kp=1;
float Ki=1;
int t=0;

//unsigned long newtime1,newtime2;
unsigned long calltime=0;
unsigned long oldtime1=0;
unsigned long oldtime2=0;

//odometry variables
double wheel_radius=5.0;    // change into actual number
#define PI 3.1415926535897932384626433832795
double wheels_distance=10.0;  // change into actual number
double odoTh=0.0;
double odoX=0.0;
double odoY=0.0;
double left_wheel_pos_old=0.0;
double right_wheel_pos_old=0.0;

//trajectory tracking

float kp=3;
float ka=8;
float kb=-1.5; 



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

void updatePosition(double left_wheel_pos, double right_wheel_pos){
  
  double dLeft=(left_wheel_pos - left_wheel_pos_old)  * 2 * PI * wheel_radius / pulses_per_rotation / gear_ratio;
  double dRight=-(right_wheel_pos - right_wheel_pos_old)  * 2 * PI * wheel_radius / pulses_per_rotation / gear_ratio;

  //Serial.println("dleft: "+String(dLeft));
  //Serial.println("dright: "+String(dRight));

  double dCenter=(dLeft+dRight) / 2.0;

  double phi=(dRight-dLeft) / wheels_distance;

  odoTh += phi;

  if (odoTh > 2.0 * PI) odoTh -= 2.0 * PI;
  if (odoTh < 0 ) odoTh += 2.0 * PI;
  
  odoX += dCenter*cos(odoTh);
  odoY += dCenter*sin(odoTh);  

  left_wheel_pos_old=left_wheel_pos;
  right_wheel_pos_old=right_wheel_pos;
  
}

void updatePID(float vel1, float vel2){
  
  error=vel1-velocity1;
  iError1+=error*0.01;
  out_vel1=Kp*error+Ki*iError1;
    
  error=-vel2-velocity2;    // added minus for opposite direction
  iError2+=error*0.01;
  out_vel2=Kp*error+Ki*iError2;  
  
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
    delay(1);
    t++;
    
    if (t==20){

    velocity1=getSpeed(LEFT_MOTOR,millis());
    velocity2=getSpeed(RIGHT_MOTOR,millis());

    updatePosition((double)encoder1.read(),(double)encoder2.read());
    
    t=0;  
    }



 
     
    //Serial.println("odoX: "+String(odoX));
    //Serial.println("odoY: "+String(odoY));
    //Serial.println("odoTh: "+String(odoTh));  

    float P=sqrt(pow(dX,2)+pow(dY,2));
    float A=Th+atan2(dY,dX);
    float B=-Th-A;
    
    float v=kp*P;
    float w=ka*A+kb*B;

    float v1=v+(w*wheels_distance)/2;
    float v2=v-(w*wheels_distance)/2;
    updatePID(v1,v2);

    enableMotors();
    motor1.setVelocity(out_vel1);
    motor2.setVelocity(out_vel2);

    //Serial.println("Velocity1: "+String(velocity1));
    //Serial.println("Velocity2: "+String(velocity2));

    //Serial.println("Out_vel1: "+String(out_vel));

       
  
}
