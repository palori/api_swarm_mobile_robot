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

//PID values
float vel1=0.0;
float vel2=0.0;
float error=0.0;
float iError1=0.0;
float iError2=0.0;
float out_vel1=0.0;
float out_vel2=0.0;
float velocity1=0.0;
float velocity2=0.0;

double errorTh=0.0;
double iErrorTh=0.0;

double errorD=0.0;
double iErrorD=0.0;

//PID constants
float Kp=0.1;
float Ki=1;

double Kp_Th=1;
double Ki_Th=1;

double Kp_d=1;
double Ki_d=1;

    
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

//forward function

int starting_position=1;
int disableM=0;


int sign(double in){
  if (in < 0) return -1;
  if (in >= 0) return 1;  
}

double WrapTo2PI(double ang){
  if (fabs(ang-PI) > PI) ang = ang + 2 * sign(ang) * PI;  
}




float getSpeed(int id, unsigned long newtime){ 

  if (id==LEFT_MOTOR){
    newpulse[id-1]=encoder1.read(); 
    if (newtime==oldtime1 || oldtime1==0){
      velocity = 0;  
    } else {
      velocity=-(newpulse[id-1]-pulse[id-1])*2*M_PI/pulses_per_rotation/gear_ratio/((newtime-oldtime1)/1000.0);
    } 
    oldtime1 = newtime;

    //Serial.println(oldtime1);
  } else if (id==RIGHT_MOTOR) {
    newpulse[id-1]=encoder2.read();
    if (newtime==oldtime2 || oldtime2==0){
      velocity = 0;  
    } else {
      velocity=-(newpulse[id-1]-pulse[id-1])*2*M_PI/pulses_per_rotation/gear_ratio/((newtime-oldtime2)/1000.0);
    } 
    oldtime2 = newtime;
    //Serial.println(oldtime2);
  } 
  
  pulse[id-1] = newpulse[id-1];
  
  velocity = velocity * wheel_radius;
  return velocity;
}

void updatePosition(double left_wheel_pos, double right_wheel_pos){
  
  double dLeft=(left_wheel_pos - left_wheel_pos_old)  * 2 * PI * wheel_radius / pulses_per_rotation / gear_ratio;
  double dRight=-(right_wheel_pos - right_wheel_pos_old)  * 2 * PI * wheel_radius / pulses_per_rotation / gear_ratio;

  //Serial.println("dleft: "+String(dLeft));
  //Serial.println("dright: "+String(dRight));

  double dCenter= (dLeft + dRight) / 2.0;

  double phi=(dRight - dLeft) / wheels_distance;

  odoTh += phi;

  if (odoTh > 2.0 * PI) odoTh -= 2.0 * PI;
  if (odoTh < 0 ) odoTh += 2.0 * PI;
  
  odoX += dCenter*cos(odoTh);
  odoY += dCenter*sin(odoTh);  

  left_wheel_pos_old = left_wheel_pos;
  right_wheel_pos_old = right_wheel_pos;
  
}

void updatePID(){
  
  error = vel1 - velocity1;
  iError1 += error * 0.01;
  out_vel1 = Kp * error + Ki * iError1;
    
  error = - vel2 - velocity2;    // added minus for opposite direction
  iError2 += error * 0.01;
  out_vel2 = Kp * error + Ki * iError2;  
  
}

double updatePID_dist(){
   iErrorD += dist_error * 0.01;
   return Kp_d * dist_error + Ki_d * iErrorD;  // check if the dist error calculation is correct
}

double updatePID_theta(double Th0){
    
    errorTh = Th0 - odoTh;      // make function which calculates theta difference correctly
    iErrorTh += error_Th * 0.01;
    return Kp_Th * error_Th + Ki_Th * iErrorTh;
  
}
void forward(double dist_ref){

   
    double x0 = odoX;
    double y0 = odoY;
    double Th0 = odoTh;

    double dist_curr = sqrt(pow(odoX - x0,2.0)+pow(odoY - y0,2.0)); 
    if (dist_ref < 0) dist_curr = -dist_curr;
    double dist_error = dist_ref - dist_curr;
    if (disableM == 0) enableMotors();
    while (fabs(dist_error) > 0.03){
      delay(10);
      velocity1 = getSpeed(LEFT_MOTOR,millis());
      velocity2 = getSpeed(RIGHT_MOTOR,millis());
      updatePosition((double)encoder1.read(),(double)encoder2.read());   //check overflow
      Serial.println("odoX: "+String(odoX));
      Serial.println("odoY: "+String(odoY));
      Serial.println("odoTh: "+String(odoTh));
      
      vel1 = updatePID_dist() + updatePID_Th(Th0);
      vel2 = updatePID_dist() - updatePID_Th(Th0);
      
      if (fabs(vel1) > 1.0) vel1 = 1.0 * sign(vel1);
      if (fabs(vel2) > 1.0) vel2 = 1.0 * sign(vel2);
      
      updatePID();
        
      motor1.setVelocity(out_vel1);
      motor2.setVelocity(out_vel2);
      dist_curr=sqrt(pow(odoX - x0,2.0)+pow(odoY - y0,2.0));
      if (dist_ref < 0) dist_curr = -dist_curr;
      dist_error = dist_ref - dist_curr;
      
    }
    disableMotors();
 
}

void turn(double angle_ref){

    if (angle_ref > PI) angle_ref = 2 * PI - angle_ref;
    if (angle_ref < -PI) angle_ref = 2 * PI + angle_ref;
    double Th0 = odoTh;
    double angle_ref_abs = angle_ref + Th0;
    if (angle_ref_abs < 0) angle_ref_abs += 2 * PI;
    if (angle_ref_abs > 2 * PI) angle_ref_abs -= 2 * PI;
    double angle_error=0.0;
    if (angle_ref_abs >= odoTh) {
        if ((angle_ref_abs - odoTh) > PI) angle_error = angle_ref_abs - odoTh - 2 * PI;
        else  angle_error = angle_ref_abs - odoTh;    
    } else {
        if ((angle_ref_abs - odoTh) < -PI) angle_error = angle_ref_abs - odoTh + 2 * PI;
        else  angle_error = angle_ref_abs - odoTh;
    }
    
    if (disableM==0) enableMotors();
    
    while (fabs(angle_error)>0.02){
      delay(10);
      velocity1=getSpeed(LEFT_MOTOR,millis());
      velocity2=getSpeed(RIGHT_MOTOR,millis());
      updatePosition((double)encoder1.read(),(double)encoder2.read());   //check overflow
      Serial.println("odoX: "+String(odoX));
      Serial.println("odoY: "+String(odoY));
      Serial.println("odoTh: "+String(odoTh));

      vel2=-angle_error;
      vel1=angle_error;
      
      if (fabs(vel2) > 0.2) vel2=0.2*sign(vel2);
      if (fabs(vel1) > 0.2) vel1=0.2*sign(vel1);
      
      
      updatePID();
      
      motor1.setVelocity(out_vel1);
      motor2.setVelocity(out_vel2);
      
      if (angle_ref_abs>=odoTh) {
        if ((angle_ref_abs - odoTh) > PI) angle_error = angle_ref_abs - odoTh - 2 * PI;
        else  angle_error = angle_ref_abs - odoTh;    
      } else {
        if ((angle_ref_abs - odoTh) < -PI) angle_error = angle_ref_abs - odoTh + 2 * PI;
        else  angle_error = angle_ref_abs - odoTh;
      }
    
      Serial.println("angle error:"+String(angle_error));
    }
    disableMotors();
    
  
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
    //forward(1.0);

    delay(3000); 

     
    if (Serial.available()) {
      double input = Serial.parseFloat();
      if (fabs(input) < 2 * PI) turn(input);
    }
  
}
