#include <motor.h>
#include <pins.h>
#include <utils.h>
#include <Encoder.h>
#include <math.h>
#include "imu.h"
#include <ir.h>

Motor motor1(LEFT_MOTOR);
Motor motor2(RIGHT_MOTOR);

Encoder encoder1(PIN_LEFT_ENCODER_A,PIN_LEFT_ENCODER_B);
Encoder encoder2(PIN_RIGHT_ENCODER_A,PIN_RIGHT_ENCODER_B);

IR ir_1(PIN_IR_RAW_1);
IR ir_2(PIN_IR_RAW_2);

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
int DIST=2;
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
double c_r = 2 * PI * wheel_radius / pulses_per_rotation / gear_ratio;   //wheel_radius is the same for both wheels for now
double c_l = 2 * PI * wheel_radius / pulses_per_rotation / gear_ratio;

//initial values
double x_0;
double y_0;
double Th_0;
double v_max;

//forward and turn functions
int starting_position = 1;
int disableM = 0;
double dist_error = 0.0;
double dist_curr = 0.0;
double angle_ref_abs = 0.0;
double angle_error = 0.0;
enum Command {TRN, FWD, TRNR};



double sign(double in){
  if (in < 0) return -1.0;
  if (in >= 0) return 1.0;  
}

double WrapTo2PI(double ang){ 
  if (fabs(ang - PI) <= PI)  return ang;
  else return WrapTo2PI(ang - 2 * sign(ang) * PI);
}

double Saturate(double sat_value, double sat_level){
  if (fabs(sat_value) > sat_level) return sat_level * sign(sat_value);
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

double PID(double referent_value, double current_value, int index){

    e[index] = referent_value - current_value;
    e_i[index] += e[index]*T[index];
    return KP[index] * e[index] + KI[index] * e_i[index];
  
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
    
    if (disableM == 0) enableMotors();

    initializePID(VEL1,Kp,Ki,0.01);
    initializePID(VEL2,Kp,Ki,0.01);
    //initializePID(DIST,Kp_d,Ki_d,0.01);
    initializePID(THETA,Kp_Th,Ki_Th,0.01);
    
}

void turn(double angle_ref){

    if (fabs(angle_ref) > PI) angle_ref = 2 * PI - sign(angle_ref) * angle_ref; //makes sure to turn in the right direction
    
    Th_0 = odoTh;
    angle_ref_abs = angle_ref + Th_0;
    angle_error = angle_ref_abs - odoTh;
    
    if (disableM==0) enableMotors();
    
    //initializePID(TURN,Kp_Th,Ki_Th,0.01);
    initializePID(VEL1,Kp,Ki,0.01);
    initializePID(VEL2,Kp,Ki,0.01);
    
}

void turnr(double angle_ref){

    //if (fabs(angle_ref) > PI) angle_ref = 2 * PI - sign(angle_ref) * angle_ref; //makes sure to turn in the right direction
    
    Th_0 = odoTh;
    angle_ref_abs = angle_ref + Th_0;
    angle_error = angle_ref_abs - odoTh;
    
    if (disableM==0) enableMotors();
    
    //initializePID(TURN,Kp_Th,Ki_Th,0.01);
    initializePID(VEL1,Kp,Ki,0.01);
    initializePID(VEL2,Kp,Ki,0.01);
    
}

void update_velocity(Command RPI_command, double RPI_value){
  
  // forward:

  switch (RPI_command) {
        case TRN:
            if (fabs(angle_error)>0.01){
      
                //vel2 = PID(angle_ref_abs,odoTh,TURN);
                //vel1 = -PID(angle_ref_abs,odoTh,TURN);
          
                vel1 = - sign(angle_error) * 0.4;
                vel2 = sign(angle_error) * 0.4;
                
                v_max = sign(angle_error) * fabs(sqrt(angle_error * wheels_distance / 2.0)); 
                vel1 = Saturate(vel1 , v_max);   
                vel2 = Saturate(vel2 , v_max);
                
                
                angle_error = angle_ref_abs - odoTh;
                Serial.println("angle error:"+String(angle_error));
                
            } else {
                vel1 = 0.1;
                vel2 = 0.1;
                disableMotors();         
            }
          
          break;

        case TRNR:   //turnr function: v2/v1=(R+b/2)/(R-b/2)
            if (fabs(angle_error)>0.01){
      
                //vel2 = PID(angle_ref_abs,odoTh,TURN);
                //vel1 = -PID(angle_ref_abs,odoTh,TURN);
                double turning_radius = 0.3;
                double vel_ratio = (turning_radius + wheels_distance / 2.0) / (turning_radius - wheels_distance / 2.0);
                
                vel1 = sign(angle_error) * 0.3;
                vel2 = vel1 * vel_ratio;
                
                double v_max1 = sign(angle_error) * fabs(sqrt(angle_error * (turning_radius - sign(angle_error) * wheels_distance / 2.0))); 
                double v_max2 = sign(angle_error) * fabs(sqrt(angle_error * (turning_radius + sign(angle_error) * wheels_distance / 2.0)));
                
                if (angle_error > 0.0) {
                  vel2 = Saturate(vel2 , v_max2);
                  vel1 = vel2 / vel_ratio;  
                } else {
                  vel1 = Saturate(vel1 , v_max1);  
                  vel2 = vel1 * vel_ratio; 
                }
                
                angle_error = angle_ref_abs - odoTh;
                Serial.println("vel1:"+String(vel1));
                Serial.println("vel2:"+String(vel2));
                
            } else {
                vel1 = 0.1;
                vel2 = 0.1;
                disableMotors();         
            }
          
          break;
          
        case FWD:
        
            if (fabs(dist_error) > 0.01){
      
                //vel1 = PID(dist_ref,dist_curr,DIST) - PID(Th0,odoTh,THETA);
                //vel2 = PID(dist_ref,dist_curr,DIST) + PID(Th0,odoTh,THETA);
                
                vel1=0.3 - PID(Th_0,odoTh,THETA);
                vel2=0.3 + PID(Th_0,odoTh,THETA);
                v_max=sign(RPI_value) * sqrt(0.4 * fabs(dist_error));
                
                vel1 = Saturate(vel1 , v_max);        //saturation should be used just in case of reaching nominal speed, the control should implement steady state wanted speed
                vel2 = Saturate(vel2 , v_max);
          
                Serial.println("vel1: "+String(vel1));
                Serial.println("vel2: "+String(vel2));
                  
                dist_curr=sign(RPI_value) * sqrt(pow(odoX - x_0 , 2.0)+pow(odoY - y_0 , 2.0));
                dist_error = RPI_value - dist_curr;
            
            } else {
                vel1 = 0.1;
                vel2 = 0.1;
                disableMotors();       
            }
          break;
  }

}

double input=0.1;
 
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

  
      
} 

Command RPI_command = TRNR;
double RPI_value = PI;
bool newCommand = true;



void loop() 
{ 

    if (newCommand == true){
      switch (RPI_command) {
        case TRN:
          turn(RPI_value);
          break;
        case FWD:
          forward(RPI_value);
          break;
        case TRNR:
          turnr(RPI_value);
          break;
      }
      newCommand = false;
    }
    
    update_velocity(RPI_command,RPI_value);

    motor1.setVelocity(PID(vel1,velocity1,VEL1));   
    motor2.setVelocity(PID(vel2,velocity2,VEL2));
      
    delay(10);

    velocity1 = getVelocity(LEFT_MOTOR , millis());
    velocity2 = getVelocity(RIGHT_MOTOR , millis());
    updatePosition((double)encoder1.read() , (double)encoder2.read());   //check overflow, it should overflow when int/double overflows
    
    Serial.println("odoX: "+String(odoX));
    Serial.println("odoY: "+String(odoY));
    Serial.println("odoTh: "+String(WrapTo2PI(odoTh))); 
    Serial.println("dist1: "+String(ir_1.getDistance()));
    Serial.println("dist2: "+String(ir_2.getDistance()));    //calibrate each ir sensor, put a value which indicates values out of range
    
  
}
