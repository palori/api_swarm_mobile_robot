#include <Servo.h>
#include <motor.h>
#include <pins.h>
#include <utils.h>
#include <Encoder.h>
#include <math.h>
#include "imu.h"
#include <ir.h>
#include "comm_1.h"

#define CPU_RESTART_ADDR (uint32_t *)0xE000ED0C
#define CPU_RESTART_VAL 0x5FA0004
#define CPU_RESTART (*CPU_RESTART_ADDR = CPU_RESTART_VAL);

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
float battery_voltage = 0.0;

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
int STOP=4;

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
double x0 = 100.0;
double y0_ = 100.0;
double th0 = 100.0;
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
enum Command {TRN, FWD, TRNR,};
bool newCommand = true;


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
double delta_vel = 0.0;
int count10=1;
double int_action[10]={0,0,0,0,0,0,0,0,0,0};

//counter for synchronizing with rpi
int count_drive = 1;

//imu variables
float _imu_cmps[3] = {0,0,0};
float _imu_gyro[3] = {0,0,0};
float _imu_accel[3] = {0,0,0};


void force_restart(){
  CPU_RESTART
  
}


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

  /*
  double x0_temp = (double) comm_tsy.get_x_0();
  double y0_temp = (double) comm_tsy.get_y_0();
  double th0_temp = (double) comm_tsy.get_th_0();
  
  if (x0 != x0_temp){
       odoX = x0_temp;
       x0 = x0_temp;
  }
  if (y0_ != y0_temp){
       odoY = y0_temp;
       y0_ = y0_temp;
  }
  if (th0 != th0_temp){
       odoTh = th0_temyp;
       th0 = th0_temp;
  }*/
  float x0_temp = comm_tsy.get_x_0();
  float y0_temp = comm_tsy.get_y_0();
  float th0_temp = comm_tsy.get_th_0();
  float QWERT = -100.0;
  if (x0_temp != QWERT || y0_temp != QWERT || th0_temp != QWERT){
       //CPU_RESTART;
       odoX = (double) x0_temp;
       comm_tsy.set_x_0(QWERT);
       odoY = (double) y0_temp;
       comm_tsy.set_y_0(QWERT);
       odoTh = (double) th0_temp;
       comm_tsy.set_th_0(QWERT);
  }/*
  if (y0_temp != QWERT){
       CPU_RESTART;
       odoY = (double) y0_temp;
       comm_tsy.set_y_0(QWERT);
  }
  if (th0_temp != QWERT){
       CPU_RESTART;
       odoTh = (double) th0_temp;
       comm_tsy.set_th_0(QWERT);
  }*/
}

void initializePID(int index, double K_P, double K_I, double time_period){
    KP[index] = K_P;
    KI[index] = K_I;
    T[index] = time_period;  
    e[index] = 0.0;
    e_i[index] = 0.0;
    for (int i=0;i < 10;i++) int_action[i]=0.0;
}

double update_PID(double referent_value, double current_value, int index){
    
    e[index] = referent_value - current_value;
    e_i[index] += e[index]*T[index];
    return KP[index] * e[index] + KI[index] * e_i[index];
  
}

double update_PID_follow(double referent_value, double current_value, int index){

    e[index] = referent_value - current_value;
    int_action[count10 - 1] = e[index];
    e_i[index] = 0;
    for (int i=0;i<10;i++){
        e_i[index] += int_action[i];
    }
    e_i[index] *= T[index];
    //adding derivative part:
    double e_d;
    if (count10>1) e_d = int_action[count10-1] - int_action[count10-2];
    else e_d = int_action[0] - int_action[9];
    e_d /= T[index];
    double KD = 0.005;
    count10++;
    if (count10 == 10) count10 = 1;

   
    double KV = (velocity1 + velocity2) / (2 * 0.3); 

    

    
    return KV * KP[index] * e[index] + KV * KI[index] * e_i[index] + KD * KV * e_d;
  
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


void forward(double dist_ref){
  
    x_0 = odoX;
    y_0 = odoY;
    dist_curr = sign(dist_ref) * sqrt(pow(odoX - x_0 , 2.0) + pow(odoY - y_0 , 2.0)); 
    dist_error = dist_ref - dist_curr;
    
    enableMotors();

    if (comm_tsy.get_race()) {
        Th_0 = 0;    //Th_0 has to be 0 for race
        initializePID(VEL1,2*Kp,Ki,0.01);
        initializePID(VEL2,2*Kp,Ki,0.01);
        initializePID(THETA,Kp_Th,Ki_Th,0.01);
    } else if (comm_tsy.get_stairs()){
        Th_0 = 0;
        initializePID(VEL1,2*Kp,Ki,0.01);
        initializePID(VEL2,2*Kp,Ki,0.01);
        initializePID(THETA,0,0,0.01);  //theta control has to be 0 for stairs       
    
    } else {
        Th_0 = odoTh;
        initializePID(VEL1,2*Kp,Ki,0.01);
        initializePID(VEL2,2*Kp,Ki,0.01);
        initializePID(THETA,Kp_Th,Ki_Th,0.01); 
    }
    
}

void turn(double angle_ref){

    //angle_ref=WrapTo2PI(angle_ref);
    
    if (fabs(angle_ref) > PI) angle_ref = 2 * PI - sign(angle_ref) * angle_ref; //makes sure to turn in the right direction
    
    Th_0 = odoTh;
    angle_ref_abs = angle_ref + Th_0;
    angle_error = angle_ref_abs - odoTh;
    
    enableMotors();
    
    initializePID(VEL1,3*Kp,Ki,0.01);
    initializePID(VEL2,3*Kp,Ki,0.01);
    
}

void turnr(double angle_ref){

    //if (fabs(angle_ref) > PI) angle_ref = 2 * PI - sign(angle_ref) * angle_ref; //makes sure to turn in the right direction
    
    Th_0 = odoTh;
    angle_ref_abs = angle_ref + Th_0;
    angle_error = angle_ref_abs - odoTh;
    
    enableMotors();
    
    
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
    
    //Th_0 = odoTh;
    final_dist = dTravel + dist;
    
    enableMotors();

    initializePID(VEL1,4*Kp,1*Ki,0.01);
    initializePID(VEL2,4*Kp,1*Ki,0.01);
    initializePID(FOLLOW,0.005,0.00,0.01); //0.0025
}

void emergency_stop(){   //shouldnt wait until new command = true
  
    comm_tsy.set_fwd(false);
    comm_tsy.set_trn(false);
    comm_tsy.set_trnr(false);
    comm_tsy.set_drive(false); 
    comm_tsy.set_followline(false);
    comm_tsy.set_race(false);
    comm_tsy.set_stairs(false);
    
    
    initializePID(VEL1,Kp,Ki,0.01);
    initializePID(VEL2,Kp,Ki,0.01);
  
};


void update_velocity(int drive_command){

  
  // forward:

  switch (drive_command) {

        case comm_tsy.STOP:
            if (fabs(velocity1)<0.05 && fabs(velocity2)<0.05) {
              //disableMotors(); 
              newCommand = true;
              count_drive++;
              Serial.println("STOPPED !!!!!!!");
              comm_tsy.set_stop(false);     
            } else {
              Serial.println("STOPPING !!!!!!!");
              vel1=0.000;
              vel2=0.000;
               
            }
        break;
        case comm_tsy.TRN:
            if (fabs(angle_error)>0.01){
      
                
                vel1 = - sign(angle_error) * comm_tsy.get_vel();
                vel2 = sign(angle_error) * comm_tsy.get_vel();
                
                v_max = sqrt(4*fabs(angle_error) * wheels_distance / 2.0); 
                vel1 = Saturate(vel1 , v_max);   
                vel2 = Saturate(vel2 , v_max);

                vel1 = Saturate(vel1 , 0.4);   
                vel2 = Saturate(vel2 , 0.4);
                
                angle_error = angle_ref_abs - odoTh;
                //Serial.println("angle error:                                "+String(angle_error));
                
            } else {
                vel1 = 0.000;
                vel2 = 0.000;
                //disableMotors();  
                newCommand = true;
                count_drive++;
                comm_tsy.set_trn(false);       
            }
          
          break;

        case comm_tsy.TRNR:   //turnr function: v2/v1=(R+b/2)/(R-b/2)
            if (fabs(angle_error)>0.01){
      
          
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
                vel1 = 0.0001;
                vel2 = 0.0001;
                //disableMotors();
                newCommand = true;  
                count_drive++;
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
                vel1 = 0.0001;
                vel2 = 0.0001;
                disableMotors();  
                newCommand = true; 
                count_drive++;   
                comm_tsy.set_fwd(false); 
                comm_tsy.set_race(false);
                comm_tsy.set_stairs(false);
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
            
              vel1 = 0.0001;
              vel2 = 0.0001;
              disableMotors(); 
              newCommand = true;
              count_drive++;    
              comm_tsy.set_drive(false); 
           }

        break;

        case comm_tsy.FOLLOW:
            //Serial.println("dTravel: "+String(dTravel));
            //Serial.println("delta travel: "+String(final_dist - dTravel));
            if (fabs(final_dist - dTravel) > 0.02){

                delta_vel = update_PID_follow(0,Saturate(comm_tsy.get_th_t() , 150),FOLLOW);
                //double cm = Saturate(comm_tsy.get_th_t() , 150);
                //double K = 0.004 - sign(cm) * cm / 50000;
                //K=0.002 * (velocity1 + velocity2) / 0.6; 
                //delta_vel = - K * cm;
                
                if (comm_tsy.get_th_t()<0) {
                  vel1=comm_tsy.get_vel() - delta_vel;   // in robot coord. syst.
                  vel2=comm_tsy.get_vel();// + delta_vel;
                }else{
                  vel1=comm_tsy.get_vel();// - delta_vel;
                  vel2=comm_tsy.get_vel() + delta_vel;
                }

                if (vel1 < 0) vel1 = 0;
                if (vel2 < 0) vel2 = 0;
                    
                v_max = sqrt(2.0 * fabs(final_dist - dTravel));
                
                vel1 = Saturate(vel1 , v_max);        //saturation should be used just in case of reaching nominal speed, the control should implement steady state wanted speed
                vel2 = Saturate(vel2 , v_max);
          
                //vel1 = Saturate(vel1 , 0.5);   
                //vel2 = Saturate(vel2 , 0.5);
                
                //Serial.println("remaining distance: "+String(final_dist - dTravel));
                Serial.println("vel1: "+String(vel1));
                Serial.println("vel2: "+String(vel2));
              
                  
            } else {
                Serial.println("Follow line STOP!!!");
                vel1 = 0.0001;
                vel2 = 0.0001;
                //disableMotors();  
                newCommand = true;   
                count_drive++;
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
  MPU9150_setup();

  enableMotors();
    
  motor1.setVelocity(input);   //sets motor to small speed where they dont move
  motor2.setVelocity(input);

  ir_1.setCalibration();
  ir_2.setCalibration();

  myTimer.begin(update10ms,10000);
  myTimer.priority(0);
  //reading.begin(reading100ms,100000);
  //reading.priority(1);
  writing.begin(read_sensors,100000);
  writing.priority(1);
  myServo.attach(PIN_SERVO2);    //write as:  myServo.write(position)  position = [0,180]
  
  
   
  
  

} 

Command RPI_command = TRNR;
double RPI_value = PI;

int drive_command=-1;
int servo_pos = comm_tsy.get_servo();

void loop() // @,a=15,b=1,fwd=2,$
{ 
   reading100ms();
   //Serial.println("****************************************");
   battery_voltage = checkBattery();
   //read_IMU();
   
}

void read_IMU(){
  if (i2c_connection()){
    _imu_cmps[0] = IMU_cmps('X');
    _imu_cmps[1] = IMU_cmps('Y');
    _imu_cmps[2] = IMU_cmps('Z');
    _imu_gyro[0] = IMU_gyro('X');
    _imu_gyro[1] = IMU_gyro('Y');
    _imu_gyro[2] = IMU_gyro('Z');
    _imu_accel[0] = IMU_accel('X');
    _imu_accel[1] = IMU_accel('Y');
    _imu_accel[2] = IMU_accel('Z');
  } 
}


void read_sensors(){
Serial.println("read_sensors!");
  double _odo[3] = {odoX, odoY, odoTh};
  float _ir[2] = {0.0,0.0};
  bool _obstacle_found = false; // closer than a certain especified distance
  float _batt = battery_voltage;
  
  

  if (comm_tsy.get_ir_on()){
     _ir[0] = ir_1.getDistance();
     _ir[1] = ir_2.getDistance();
    
    if (comm_tsy.get_avoid_obst() && (_ir[0]<comm_tsy.get_obst_dist() || _ir[1]<comm_tsy.get_obst_dist())){
      // obstacle closer than a certain distance
      //emergency_stop(); // work on that

      //Serial.println("OBSTACLE FOUND!!!!!");
      comm_tsy.set_stop(true);
      _obstacle_found = true;
      //count_drive++;
      comm_tsy.set_avoid_obst(false);
    } 
  }
  int _count_drive=count_drive;
 
  comm_tsy.write_serial(_count_drive,_odo,_ir,_batt,_imu_cmps,_imu_gyro,_imu_accel, _obstacle_found);
  Serial.println("");
}

void update10ms(){

    //Serial.println("time:                    "+String(millis()));
    //Serial.println("angle ref abs:                                "+String(angle_ref_abs));
    velocity1 = getVelocity(LEFT_MOTOR , millis());
    velocity2 = getVelocity(RIGHT_MOTOR , millis());
    updatePosition((double)encoder1.read() , (double)encoder2.read());   //check overflow, it should overflow when int/double overflows

    //Serial.println("velocity1: "+String(velocity1));
    //Serial.println("velocity2: "+String(velocity2));
    //Serial.println("odoX: "+String(odoX));
    //Serial.println("odoY: "+String(odoY));
    //Serial.println("odoTh: "+String(odoTh));
    //Serial.println("odoTh: "+String(WrapTo2PI(odoTh))); 
    //Serial.println("ir1: "+String(ir_1.getDistance()));
    //Serial.println("ir1: "+String(ir_2.getDistance()));
    
    
    if (comm_tsy.get_stop()) drive_command = comm_tsy.STOP;
      else if (comm_tsy.get_followline()) drive_command = comm_tsy.FOLLOW;
      else if (comm_tsy.get_drive()) drive_command = comm_tsy.DRIVE;
      else if (comm_tsy.get_fwd() || comm_tsy.get_race() || comm_tsy.get_stairs()) drive_command = comm_tsy.FWD;
      else if (comm_tsy.get_trn()) drive_command = comm_tsy.TRN;
      else if (comm_tsy.get_trnr()) drive_command = comm_tsy.TRNR;    
      else {
        drive_command=-1;
        newCommand=true;
    }

   
    
    

    //Serial.println("*** drive command: "+String(drive_command)+ " , new command: "+String(newCommand));
    if (newCommand == true || comm_tsy.get_stop()){  
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
          break;
        
          
      }
      //newCommand = false;
    } 
    
    
    update_velocity(drive_command);

    motor1.setVelocity(update_PID(vel1,velocity1,VEL1));   
    motor2.setVelocity(update_PID(vel2,velocity2,VEL2));
      
    //delay(10);

    //update servo position;

    if (myServo.read() < comm_tsy.get_servo() ) servo_pos++;
    else if (myServo.read() > comm_tsy.get_servo()) servo_pos--;

    myServo.write(servo_pos);  //update servo pos every 10ms
}
int reading_count = 0;

void reading100ms (){
  
   comm_tsy.read_serial(); 
   reading_count++;
   if (reading_count >= 1000){
      //Serial.println("***imu: "+String(comm_tsy.get_imu_on()));
      reading_count = 0;
   }
}
