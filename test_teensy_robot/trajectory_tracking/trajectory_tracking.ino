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
double wheel_radius=0.045;    // change into actual number
#define PI 3.1415926535897932384626433832795
double wheels_distance=0.127;  // change into actual number
double odoTh=0.0;
double odoX=0.0;
double odoY=0.0;
double left_wheel_pos_old=0.0;
double right_wheel_pos_old=0.0;

//trajectory tracking

float kp=0.3;
float ka=0.8;
float kb=-0.15; 

int sign(double in){
  if (in<0) return -1;
  if (in>=0) return 1;  
}

double WrapToPI (double angle){
  if (fabs(angle)>PI) angle=angle-2*sign(angle)*PI;
  return angle;
}



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
  
  velocity = velocity*wheel_radius;
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

double transformX (double Xw, double Yw, double Thw, double XTw, double YTw, double ThTw){
    double XT=cos(ThTw)*(Xw-XTw)+sin(ThTw)*(Yw-YTw);
    return XT;
} 

double transformY (double Xw, double Yw, double Thw, double XTw, double YTw, double ThTw){
    double YT=-sin(ThTw)*(Xw-XTw)+cos(ThTw)*(Yw-YTw);
    return YT;  
} 

double transformTh (double Xw, double Yw, double Thw, double XTw, double YTw, double ThTw){
    double ThT=Thw-ThTw; 
    return ThT; 
} 

void updatePID(float vel1, float vel2){
  
  error=- vel1-velocity1;
  iError1+=error*0.01;
  out_vel1=Kp*error+Ki*iError1;
    
  error= vel2-velocity2;    // added minus for opposite direction
  iError2+=error*0.01;
  out_vel2=Kp*error+Ki*iError2;  
  
}

void drive(double Xr, double Yr, double Thr) {
  
    double ThTw=odoTh+Thr;
    double XTw=cos(odoTh)*Xr-sin(odoTh)*Yr+odoX;
    double YTw=sin(odoTh)*Xr+cos(odoTh)*Yr+odoY;
    
    double XT=transformX(odoX,odoY,odoTh,XTw,YTw,ThTw);
    double YT=transformY(odoX,odoY,odoTh,XTw,YTw,ThTw);
    double ThT=transformTh(odoX,odoY,odoTh,XTw,YTw,ThTw);

    double dX=-XT;
    double dY=-YT;
    double dTh=ThT;
    
    while ((fabs(dX)>0.05) || (fabs(dY)>0.05)){
      
      double P=sqrt(pow(dX,2)+pow(dY,2));
      double A=-dTh+atan2(dY,dX);
      double B=-dTh-A;

      Serial.println("P: "+String(P));
      Serial.println("A: "+String(A));
      Serial.println("B: "+String(B));
      
      double v=kp*P;
      double w=ka*A+kb*B;
  
      double v1=v+(w*wheels_distance)/2;  //check if minus and plus are fine, seems 
      double v2=v-(w*wheels_distance)/2;

      Serial.println("v1: "+String(v1));
      Serial.println("v2: "+String(v2));
      
      if (fabs(v1)>1) v1=1*sign(v1);
      if (fabs(v2)>1) v2=1*sign(v2);
     
      updatePID(v1,v2);
     
      enableMotors();
      motor1.setVelocity(out_vel1);
      motor2.setVelocity(out_vel2);  

      delay(20);
  
      velocity1=getSpeed(LEFT_MOTOR,millis());
      velocity2=getSpeed(RIGHT_MOTOR,millis());
  
      updatePosition((double)encoder1.read(),(double)encoder2.read());
      Serial.println("odoX: "+String(odoX));
      Serial.println("odoY: "+String(odoY));
      Serial.println("odoTh: "+String(odoTh));

      XT=transformX(odoX,odoY,odoTh,XTw,YTw,ThTw);
      YT=transformY(odoX,odoY,odoTh,XTw,YTw,ThTw);
      ThT=transformTh(odoX,odoY,odoTh,XTw,YTw,ThTw);

      dX=-XT;
      dY=-YT;
      dTh=ThT;
   }   
}




void setup() 
{ 
  setUpPowerPins(); 
  Serial.begin(9600);  
  while (! Serial);
  Serial.println("Speed -5 to 5");

  drive(1.0,1.0,0.0);
  
} 
 
 
void loop() 
{ 
    delay(5000);
    
    delay(5000);
    
}
