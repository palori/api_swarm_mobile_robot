#include <motor.h>
#include <pins.h>
#include <utils.h>
#include <Encoder.h>
#include <math.h>
#include "imu.h"
#include <ir.h>
#include "comm_1.h"
#include <Servo.h>

IntervalTimer myTimer;
IntervalTimer reading;
IntervalTimer writing;

Servo myServo;

Motor motor1(LEFT_MOTOR);
Motor motor2(RIGHT_MOTOR);

Encoder encoder1(PIN_LEFT_ENCODER_A,PIN_LEFT_ENCODER_B);
Encoder encoder2(PIN_RIGHT_ENCODER_A,PIN_RIGHT_ENCODER_B);

IR ir_1(PIN_IR_RAW_1);
IR ir_2(PIN_IR_RAW_2);

COMM_TSY comm_tsy;

//motor and encoder constants
double pulses_per_rotation = 48.0; //encoder give 48 pulses per revolution
double gear_ratio = 9.68; //gearing ratio between shaft and encoder is 1:9.68

//PID values
double vel1 = 0.0;
double vel2 = 0.0;
double velocity1 = 0.0;
double velocity2 = 0.0;


//PID constants
double Kp = 1;
double Ki = 1;

double Kp_Th = 10;
double Ki_Th = 1;

double Kp_d = 0.1;
double Ki_d = 1;

//initialize arrays for PID
double KP[5]={0,0,0,0,0};
double KI[5]={0,0,0,0,0};
double T[5]={0,0,0,0,0};
double e[5]={0,0,0,0,0};
double e_i[5]={0,0,0,0,0};

//indexes of PID controller
int VEL1=0;
int VEL2=1;
int FOLLOW=2;
int THETA=3;
int TURN=4;

//getVelocity variables
unsigned long oldtime[2] = {0,0};
int pulse[2] = {0,0};
int newpulse[2] = {1,1};
int vel_counter = 0;
double velocity_filter[5]={0,0,0,0,0};
double velocity[2] = {0.0,0.0};
double new_velocity = 0.0;
double K_vel = 0.1;

//odometry variables
double wheel_radius = 0.045;    // change into actual number
#define PI 3.1415926535897932384626433832795
double wheels_distance = 0.156;  // change into actual number
double odoTh = 0.0;
double odoX = 0.0;
double odoY = 0.0;
double left_wheel_pos_old = 0.0;
double right_wheel_pos_old = 0.0;
double dTravel = 0.0;
double c_r = 2 * PI * wheel_radius / pulses_per_rotation / gear_ratio;   //wheel_radius is the same for both wheels for now
double c_l = 2 * PI * wheel_radius / pulses_per_rotation / gear_ratio;

//initial values
double x_0;
double y_0;
double Th_0;
double v_max;

//forward and turn functions
int starting_position = 1;
double dist_error = 0.0;
double dist_curr = 0.0;
double angle_ref_abs = 0.0;
double angle_error = 0.0;
enum Command {TRN, FWD, TRNR};
bool newCommand = true;
bool new_drive = false;

//drive function
double ThTw=0.0;
double XTw=0.0;
double YTw=0.0;

double dX=0.0;
double dY=0.0;
double dTh=0.0;

double kp=0.6;
double ka=0.8;
double kb=-0.15; 

//follow line function
double final_dist = 0.0;



double sign(double in){
  if (in < 0) return -1.0;
  else return 1.0;  
}

double WrapTo2PI(double ang){ 
  if (fabs(ang - PI) <= PI)  return ang;
  else return WrapTo2PI(ang - 2 * sign(ang) * PI);
}

double Saturate(double sat_value, double sat_level){
  if (fabs(sat_value) > fabs(sat_level)) return fabs(sat_level) * sign(sat_value);
  else return sat_value;  
}

double Th_difference (double first_val, double second_val){
  if (fabs(first_val - second_val) > PI) return 2 * PI - second_val - sign(first_val - second_val) * first_val;
  else return first_val - second_val;
}




double getVelocity(int id, unsigned long newtime){ 

  if (id==LEFT_MOTOR){
    newpulse[id-1]=encoder1.read(); 
  } else if (id==RIGHT_MOTOR) {
    newpulse[id-1]=-encoder2.read();  //when positive speed, left encoder is increasing and right one is decresing
  } 
   
  if (newtime!=oldtime[id-1] && oldtime[id-1]!=0){
    new_velocity = (newpulse[id-1] - pulse[id-1]) * c_l / ((newtime-oldtime[id-1]) / 1000.0);  //resolution of this measurement is 0.061 m/s, too much, needs low pass filter
  } 
  oldtime[id-1] = newtime;
  pulse[id-1] = newpulse[id-1];

  velocity[id-1] += K_vel * (new_velocity - velocity[id-1]);  //low pass filter
  
  //Serial.println("velocity: "+String(velocity[id-1]));
  //Serial.println("new velocity: "+String(new_velocity));
  
  return velocity[id-1];
}    



void updatePosition(double left_wheel_pos, double right_wheel_pos){
  
  double dLeft = (left_wheel_pos - left_wheel_pos_old) * c_l;   
  double dRight = -(right_wheel_pos - right_wheel_pos_old) * c_r; //when positive speed, left encoder is increasing and right one is decresing

  //Serial.println("dleft: "+String(dLeft));
  //Serial.println("dright: "+String(dRight));

  double dCenter = (dLeft + dRight) / 2.0;
  dTravel += dCenter; 
  double phi = (dRight - dLeft) / wheels_distance;

  odoTh += phi;
  odoX += dCenter*cos(odoTh);
  odoY += dCenter*sin(odoTh);  

  left_wheel_pos_old = left_wheel_pos;
  right_wheel_pos_old = right_wheel_pos; 
}

void initializePID(int index, double K_P, double K_I, double time_period){
    KP[index] = K_P;
    KI[index] = K_I;
    T[index] = time_period;  
    e[index] = 0.0;
    e_i[index] = 0.0;
}

double update_PID(double referent_value, double current_value, int index){

    e[index] = referent_value - current_value;
    e_i[index] += e[index]*T[index];
    return KP[index] * e[index] + KI[index] * e_i[index];
  
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


void setUpIMU(){
  
    // Initialize the 'Wire' class for the I2C-bus.
    Wire_setup();

    // Clear the 'sleep' bit to start the sensor.
    MPU9150_writeSensor(MPU9150_PWR_MGMT_1, 0);

    MPU9150_setupCompass();
  
}

int IMU_cmps(char coordinate){
  int sensorValue = 0;
  switch(coordinate){
    case 'X':
        sensorValue = MPU9150_readSensor(MPU9150_CMPS_XOUT_L,MPU9150_CMPS_XOUT_H);
      break;
    case 'Y':
        sensorValue = MPU9150_readSensor(MPU9150_CMPS_YOUT_L,MPU9150_CMPS_YOUT_H);
      break;
    case 'Z':
        sensorValue = MPU9150_readSensor(MPU9150_CMPS_ZOUT_L,MPU9150_CMPS_ZOUT_H);
      break;
    }
  return sensorValue;
}

int IMU_gyro(char coordinate){
  int sensorValue = 0;
  switch(coordinate){
    case 'X':
        sensorValue = MPU9150_readSensor(MPU9150_GYRO_XOUT_L,MPU9150_GYRO_XOUT_H);
      break;
    case 'Y':
        sensorValue = MPU9150_readSensor(MPU9150_GYRO_YOUT_L,MPU9150_GYRO_YOUT_H);
      break;
    case 'Z':
        sensorValue = MPU9150_readSensor(MPU9150_GYRO_ZOUT_L,MPU9150_GYRO_ZOUT_H);
      break;
    }
  return sensorValue;
}
int IMU_accel(char coordinate){
  int sensorValue = 0;
    switch(coordinate){
      case 'X':
          sensorValue = MPU9150_readSensor(MPU9150_ACCEL_XOUT_L,MPU9150_ACCEL_XOUT_H);
        break;
      case 'Y':
          sensorValue = MPU9150_readSensor(MPU9150_ACCEL_YOUT_L,MPU9150_ACCEL_YOUT_H);
        break;
      case 'Z':
          sensorValue = MPU9150_readSensor(MPU9150_ACCEL_ZOUT_L,MPU9150_ACCEL_ZOUT_H);
        break;
      }
    return sensorValue;
}
void forward(double dist_ref){
  
    x_0 = odoX;
    y_0 = odoY;
    Th_0 = odoTh;  
    
    dist_curr = sign(dist_ref) * sqrt(pow(odoX - x_0 , 2.0) + pow(odoY - y_0 , 2.0)); 
    dist_error = dist_ref - dist_curr;
    
    enableMotors();

    initializePID(VEL1,Kp,Ki,0.01);
    initializePID(VEL2,Kp,Ki,0.01);
    initializePID(THETA,Kp_Th,Ki_Th,0.01);
    
}

void turn(double angle_ref){

    //angle_ref=WrapTo2PI(angle_ref);
    
    if (fabs(angle_ref) > PI) angle_ref = 2 * PI - sign(angle_ref) * angle_ref; //makes sure to turn in the right direction
    
    Th_0 = odoTh;
    angle_ref_abs = angle_ref + Th_0;
    angle_error = angle_ref_abs - odoTh;
    
    enableMotors();
    
    //initializePID(TURN,Kp_Th,Ki_Th,0.01);
    initializePID(VEL1,3*Kp,Ki,0.01);
    initializePID(VEL2,3*Kp,Ki,0.01);
    
}

void turnr(double angle_ref){

    //if (fabs(angle_ref) > PI) angle_ref = 2 * PI - sign(angle_ref) * angle_ref; //makes sure to turn in the right direction
    
    Th_0 = odoTh;
    angle_ref_abs = angle_ref + Th_0;
    angle_error = angle_ref_abs - odoTh;
    
    enableMotors();
    
    //initializePID(TURN,Kp_Th,Ki_Th,0.01);
    initializePID(VEL1,Kp,Ki,0.01);
    initializePID(VEL2,Kp,Ki,0.01);
    
}

void drive(double Xr, double Yr, double Thr) {

  
    ThTw=odoTh+Thr;
    XTw=cos(odoTh)*Xr-sin(odoTh)*Yr+odoX;
    YTw=sin(odoTh)*Xr+cos(odoTh)*Yr+odoY;
    
    dX=-transformX(odoX,odoY,odoTh,XTw,YTw,ThTw);
    dY=-transformY(odoX,odoY,odoTh,XTw,YTw,ThTw);
    dTh=transformTh(odoX,odoY,odoTh,XTw,YTw,ThTw);

    enableMotors();
    initializePID(VEL1,2*Kp,Ki,0.01);
    initializePID(VEL2,2*Kp,Ki,0.01);

    // set x,y,th to 0 

    comm_tsy.set_x_t(0.0);
    comm_tsy.set_y_t(0.0);
    comm_tsy.set_th_t(0.0);
}

void followline (double dist) {
    Serial.println("Oops I did it again! :o-----------------------");
    //Th_0 = odoTh;
    final_dist = dTravel + dist;
    
    enableMotors();

    initializePID(VEL1,1*Kp,Ki,0.01);
    initializePID(VEL2,1*Kp,Ki,0.01);
    initializePID(FOLLOW,0.001,0,0.01);
}

void emergency_stop(){   //shouldnt wait until new command = true
  
    comm_tsy.set_fwd(false);
    comm_tsy.set_trn(false);
    comm_tsy.set_trnr(false);
    comm_tsy.set_drive(false); 
    comm_tsy.set_followline(false);
    
    
    initializePID(VEL1,Kp,Ki,0.01);
    initializePID(VEL2,Kp,Ki,0.01);
  
};


void update_velocity(int drive_command){

  
  // forward:

  switch (drive_command) {

        case comm_tsy.STOP:
            if (fabs(velocity1)<0.05 && fabs(velocity2)<0.05) {
              disableMotors(); 
              newCommand = true;
              comm_tsy.set_stop(false);     
            } else {
              vel1=0;
              vel2=0; 
            }
        case comm_tsy.TRN:
            if (fabs(angle_error)>0.01){
      
                //vel2 = update_PID(angle_ref_abs,odoTh,TURN);
                //vel1 = -update_PID(angle_ref_abs,odoTh,TURN);
          
                vel1 = - sign(angle_error) * comm_tsy.get_vel();
                vel2 = sign(angle_error) * comm_tsy.get_vel();
                
                v_max = sqrt(fabs(angle_error) * wheels_distance / 2.0); 
                vel1 = Saturate(vel1 , v_max);   
                vel2 = Saturate(vel2 , v_max);

                //vel1 = Saturate(vel1 , 0.5);   
                //vel2 = Saturate(vel2 , 0.5);
                
                angle_error = angle_ref_abs - odoTh;
                //Serial.println("angle error:                                "+String(angle_error));
                
            } else {
                vel1 = 0.01;
                vel2 = 0.01;
                disableMotors();  
                newCommand = true;
                comm_tsy.set_trn(false);       
            }
          
          break;

        case comm_tsy.TRNR:   //turnr function: v2/v1=(R+b/2)/(R-b/2)
            if (fabs(angle_error)>0.01){
      
                //vel2 = update_PID(angle_ref_abs,odoTh,TURN);
                //vel1 = -update_PID(angle_ref_abs,odoTh,TURN);
                double turning_radius = 0.3;
                double vel_ratio = (turning_radius + wheels_distance / 2.0) / (turning_radius - wheels_distance / 2.0);
                
                vel1 = sign(angle_error) * comm_tsy.get_vel();
                vel2 = vel1 * vel_ratio;
                
                double v_max1 = sign(angle_error) * sqrt(fabs(angle_error * (turning_radius - sign(angle_error) * wheels_distance / 2.0))); 
                double v_max2 = sign(angle_error) * sqrt(fabs(angle_error * (turning_radius + sign(angle_error) * wheels_distance / 2.0)));
                
                if (angle_error > 0.0) {
                  vel2 = Saturate(vel2 , v_max2);
                  vel1 = vel2 / vel_ratio;  
                } else {
                  vel1 = Saturate(vel1 , v_max1);  
                  vel2 = vel1 * vel_ratio; 
                }
                
                angle_error = angle_ref_abs - odoTh;
                //Serial.println("vel1:"+String(vel1));
                //Serial.println("vel2:"+String(vel2));
                
            } else {
                vel1 = 0.01;
                vel2 = 0.01;
                disableMotors();
                newCommand = true;  
                comm_tsy.set_trnr(false);       
            }
          
          break;
          
        case comm_tsy.FWD:
        
            if (fabs(dist_error) > 0.02){

                //Serial.println("dist_error: "+String(dist_error));
                
                vel1=comm_tsy.get_vel() - update_PID(Th_0,odoTh,THETA);
                vel2=comm_tsy.get_vel() + update_PID(Th_0,odoTh,THETA);
                v_max=sign(comm_tsy.get_fwd_dist()) * sqrt(0.4 * fabs(dist_error));
                
                vel1 = Saturate(vel1 , v_max);        //saturation should be used just in case of reaching nominal speed, the control should implement steady state wanted speed
                vel2 = Saturate(vel2 , v_max);
          
                //vel1 = Saturate(vel1 , 0.5);   
                //vel2 = Saturate(vel2 , 0.5);
                //Serial.println("vel1: "+String(vel1));
                //Serial.println("vel2: "+String(vel2));
                  
                dist_curr=sign(comm_tsy.get_fwd_dist()) * sqrt(pow(odoX - x_0 , 2.0)+pow(odoY - y_0 , 2.0));
                dist_error = comm_tsy.get_fwd_dist() - dist_curr;
            
            } else {
                vel1 = 0.01;
                vel2 = 0.01;
                disableMotors();  
                newCommand = true;    
                comm_tsy.set_fwd(false); 
            }
          break;

       case comm_tsy.DRIVE:

            if ((fabs(dX)>0.03) || (fabs(dY)>0.03)){
      
              double P = sqrt(pow(dX,2) + pow(dY,2));
              double A = - dTh + atan2(dY,dX);
              double B = - dTh - A;
        
              //Serial.println("P: "+String(P));
              //Serial.println("A: "+String(A));
              //Serial.println("B: "+String(B));
              
              double v = kp * P;
              //double v = 0.3;
              //double v_m = sqrt(fabs(P));
              //v = Saturate(v , v_m);
              
              double w = ka * A + kb * B;
          
              vel1 = v - (w * wheels_distance) / 2;  //check if minus and plus are fine, seems 
              vel2 = v + (w * wheels_distance) / 2;
        
              //Serial.println("dX: "+String(dX));
              //Serial.println("dY: "+String(dY));
              
              vel1 = Saturate(vel1 , 0.5);   
              vel2 = Saturate(vel2 , 0.5);
               
        
              dX = -transformX(odoX,odoY,odoTh,XTw,YTw,ThTw);
              dY = -transformY(odoX,odoY,odoTh,XTw,YTw,ThTw);
              dTh = transformTh(odoX,odoY,odoTh,XTw,YTw,ThTw);
              
           }  else {
            
              vel1 = 0.01;
              vel2 = 0.01;
              disableMotors(); 
              newCommand = true;    
              comm_tsy.set_drive(false); 
           }

        break;

        case comm_tsy.FOLLOW:
            Serial.println("dTravel: "+String(dTravel));
            Serial.println("delta travel: "+String(final_dist - dTravel));
            if (fabs(final_dist - dTravel) > 0.02){
                
                
                vel1=comm_tsy.get_vel() - update_PID(0,Saturate(comm_tsy.get_th_t() , 100),FOLLOW);   // in robot coord. syst.
                vel2=comm_tsy.get_vel() + update_PID(0,Saturate(comm_tsy.get_th_t() , 100),FOLLOW);
                
                v_max = sqrt(1.0 * fabs(final_dist - dTravel));
                
                vel1 = Saturate(vel1 , v_max);        //saturation should be used just in case of reaching nominal speed, the control should implement steady state wanted speed
                vel2 = Saturate(vel2 , v_max);
          
                //vel1 = Saturate(vel1 , 0.5);   
                //vel2 = Saturate(vel2 , 0.5);
                
                //Serial.println("remaining distance: "+String(final_dist - dTravel));
                Serial.println("vel1: "+String(vel1));
                Serial.println("vel2: "+String(vel2));
              
                  
            } else {
                Serial.println("Follow line STOP!!!");
                vel1 = 0.01;
                vel2 = 0.01;
                disableMotors();  
                newCommand = true;    
                comm_tsy.set_followline(false); 
            }
          break;
  }

}

double input=0.05;
 
void setup() 
{ 
  Serial.begin(9600);
  setUpPowerPins(); 
  setUpIMU();

  enableMotors();
    
  motor1.setVelocity(input);   //sets motor to small speed where they dont move
  motor2.setVelocity(input);

  ir_1.setCalibration();
  ir_2.setCalibration();

  myTimer.begin(update10ms,10000);
  myTimer.priority(0);
  //reading.begin(reading100ms,100000);
  //reading.priority(1);
  //writing.begin(read_sensors,1000000);
  //writing.priority(2);
  myServo.attach(PIN_SERVO1);    //write as:  myServo.write(position)  position = [0,180]
   
} 

Command RPI_command = TRNR;
double RPI_value = PI;

int drive_command=-1;
int pos=0;

void loop() // @,a=15,b=1,fwd=2,$
{ 
   

   reading100ms();
   //Serial.println("****************************************");
   checkBattery();

}


void read_sensors(){

  double _odo[3] = {odoX, odoY, odoTh};
  float _ir[2] = {0.0,0.0};
  int _imu_cmps[3] = {0,0,0};
  int _imu_gyro[3] = {0,0,0};
  int _imu_accel[3] = {0,0,0};
  bool _obstacle_found = false; // closer than a certain especified distance
  

  if (comm_tsy.get_ir_on()){
     _ir[0] = ir_1.getDistance();
     _ir[1] = ir_2.getDistance();
    
    if (comm_tsy.get_avoid_obst() && (_ir[0]<comm_tsy.get_obst_dist() || _ir[1]<comm_tsy.get_obst_dist())){
      // obstacle closer than a certain distance
      //emergency_stop();                               // work on that
      comm_tsy.set_stop(true);
      _obstacle_found = true;
    }
  }

  if (comm_tsy.get_imu_on()){
    int _imu_cmps[3] = {IMU_cmps('X'), IMU_cmps('Y'), IMU_cmps('Z')};
    int _imu_gyro[3] = {IMU_gyro('X'), IMU_gyro('Y'), IMU_gyro('Z')};
    int _imu_accel[3] = {IMU_accel('X'), IMU_accel('Y'), IMU_accel('Z')};
  } 
 
  comm_tsy.write_serial(_odo,_ir,_imu_cmps,_imu_gyro,_imu_accel, _obstacle_found);
}

void update10ms(){

    //Serial.println("time:                    "+String(millis()));
    //Serial.println("angle ref abs:                                "+String(angle_ref_abs));
    velocity1 = getVelocity(LEFT_MOTOR , millis());
    velocity2 = getVelocity(RIGHT_MOTOR , millis());
    updatePosition((double)encoder1.read() , (double)encoder2.read());   //check overflow, it should overflow when int/double overflows

    Serial.println("velocity1: "+String(velocity1));
    Serial.println("velocity2: "+String(velocity2));
    //Serial.println("odoX: "+String(odoX));
    //Serial.println("odoY: "+String(odoY));
    //Serial.println("odoTh: "+String(odoTh));
    //Serial.println("odoTh: "+String(WrapTo2PI(odoTh))); 
    //Serial.println("ir1: "+String(ir_1.getDistance()));
    //Serial.println("ir1: "+String(ir_2.getDistance()));
    
    
    if (comm_tsy.get_stop()) drive_command = comm_tsy.STOP;
      else if (comm_tsy.get_followline()) drive_command = comm_tsy.FOLLOW;
      else if (comm_tsy.get_drive()) drive_command = comm_tsy.DRIVE;
      else if (comm_tsy.get_fwd()) drive_command = comm_tsy.FWD;
      else if (comm_tsy.get_trn()) drive_command = comm_tsy.TRN;
      else if (comm_tsy.get_trnr()) drive_command = comm_tsy.TRNR; 
      else {
        drive_command=-1;
        newCommand=true;
    }

    //if (comm_tsy.get_x_t()!=0.0 || comm_tsy.get_y_t()!=0.0 || comm_tsy.get_th_t()!=0.0) new_drive = true;
    
    

    //Serial.println("*** drive command: "+String(drive_command)+ " , new command: "+String(newCommand));
    if (newCommand == true || comm_tsy.get_stop() || new_drive){   //new_drive = 1 if x,y,th != 0
      switch (drive_command) {
        case comm_tsy.STOP:
          emergency_stop();
          newCommand = false;
          break;
        case comm_tsy.FOLLOW:
          followline(double(comm_tsy.get_fwd_dist()));
          newCommand = false;
          break;
        case comm_tsy.TRN:
          turn(double(comm_tsy.get_trn_deg()));
          newCommand = false;
          break;
        case comm_tsy.FWD:
          forward(double(comm_tsy.get_fwd_dist()));
          newCommand = false;
          break;
        case comm_tsy.TRNR:
          turnr(double(comm_tsy.get_trn_deg()));
          newCommand = false;
          break;
        case comm_tsy.DRIVE:
          drive(double(comm_tsy.get_x_t()),double(comm_tsy.get_y_t()),double(comm_tsy.get_th_t()));
          newCommand=false;
          new_drive = 0;
          break;
      }
      //newCommand = false;
    } 
    
    
    update_velocity(drive_command);

    motor1.setVelocity(update_PID(vel1,velocity1,VEL1));   
    motor2.setVelocity(update_PID(vel2,velocity2,VEL2));
      
    //delay(10);

   
    
    
  
}

void reading100ms (){
  
   comm_tsy.read_serial();   
}
