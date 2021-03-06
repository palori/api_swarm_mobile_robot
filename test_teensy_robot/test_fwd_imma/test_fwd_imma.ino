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
float iError1=0.0;
float iError2=0.0;
float out_vel1=0.0;
float out_vel2=0.0;
float velocity1=0.0;
float velocity2=0.0;

float Kp=0.1;
float Ki=1;

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



float getSpeed(int id, unsigned long newtime){ 

  if (id==LEFT_MOTOR){
    newpulse[id-1]=encoder1.read(); 
    if (newtime==oldtime1 || oldtime1==0){
      velocity=0;  
    } else {
      velocity=-(newpulse[id-1]-pulse[id-1])*2*M_PI/pulses_per_rotation/gear_ratio/((newtime-oldtime1)/1000.0);
    } 
    oldtime1=newtime;

    ////Serial.println(oldtime1);
  } else if (id==RIGHT_MOTOR) {
    newpulse[id-1]=encoder2.read();
    if (newtime==oldtime2 || oldtime2==0){
      velocity=0;  
    } else {
      velocity=-(newpulse[id-1]-pulse[id-1])*2*M_PI/pulses_per_rotation/gear_ratio/((newtime-oldtime2)/1000.0);
    } 
    oldtime2=newtime;
    ////Serial.println(oldtime2);
  } 
  
  pulse[id-1]=newpulse[id-1];
  
  velocity = velocity*R_WHEEL;
  return velocity;
}

void updatePosition(double left_wheel_pos, double right_wheel_pos){
  
  double dLeft=(left_wheel_pos - left_wheel_pos_old)  * 2 * PI * wheel_radius / pulses_per_rotation / gear_ratio;
  double dRight=-(right_wheel_pos - right_wheel_pos_old)  * 2 * PI * wheel_radius / pulses_per_rotation / gear_ratio;

  ////Serial.println("dleft: "+String(dLeft));
  ////Serial.println("dright: "+String(dRight));

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

void updatePID(){
  
  error=vel-velocity1;
  iError1+=error*0.01;
  out_vel1=Kp*error+Ki*iError1;
    
  error=-vel-velocity2;    // added minus for opposite direction
  iError2+=error*0.01;
  out_vel2=Kp*error+Ki*iError2;  
  
}




void setup() 
{ 

  

  setUpPowerPins(); 
  //Serial.begin(9600);  
  //while (! Serial);
  //Serial.println("Speed -5 to 5");
} 
 
int count_temp = 0;
bool first_sp=true;
void loop() 
{ 
    delay(10);
   
    velocity1=getSpeed(LEFT_MOTOR,millis());
    velocity2=getSpeed(RIGHT_MOTOR,millis());

 
    updatePosition((double)encoder1.read(),(double)encoder2.read()); 
    //Serial.println("odoX: "+String(odoX));
    //Serial.println("odoY: "+String(odoY));
    //Serial.println("odoTh: "+String(odoTh));  

     
    updatePID();

    

    //if (out_vel<0.05){ out_vel=0;}

    motor1.setVelocity(out_vel1);
    motor2.setVelocity(out_vel2);

    //Serial.println("Velocity1: "+String(velocity1));
    //Serial.println("Velocity2: "+String(velocity2));

    ////Serial.println("Out_vel1: "+String(out_vel));

     count_temp = count_temp+1;
   //if (//Serial.available()){
     //float vel_temp = //Serial.parseFloat();
     float vel_temp= 0.5;
     
     if (!first_sp){
       if (count_temp<500) vel_temp = 0.0;
       else vel_temp=10.0;
     }
     else first_sp=false;
      if (vel_temp > -5 && vel_temp < 5 && vel_temp != 0)
      {
        vel = vel_temp;
        //Serial.println("\n\nSetpoint: "+String(vel)+"\n\n");
        enableMotors();
        ////Serial.print("Setpoint: ");
        ////Serial.println(String(vel));
        ////Serial.print("Speed: ");
        ////Serial.println(String(out_vel));
          
      } else if (vel_temp == 10.0){
        //digitalWrite(right_pwm_pin, LOW);
        //vel = 0;
        disableMotors(); 
      }  
    //}

    
        




    
  
}
