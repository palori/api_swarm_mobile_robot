
#include <comm_.h>
#include <comm_1.h>
//#include "header_conditionals.h" //test, it can be deleted :)
//#include <stdio.h>
//using namespace std;


int LED_PIN = 13;                         //pin in Teensy 3.0
int BLINK_FREQ = 1000;                    //time [ms]
target new_pose;/* = {0.0,                   //new_pose.x
                   0.0,                   //new_pose.y
                   0.0,                   //new_pose.th
                   0.0};                  //new_pose.servo*/
/*new_pose.servo = 0.0;
new_pose.x = 0.0;
new_pose.y = 0.0;
new_pose.th = 0.0;*/
target target_pose = new_pose;
//target_pose.x = 0.5;
String msg = "initial message!";
bool connected2pi = false;


/*************
 * LED BLINK *
 *************/
void led_blink(int led_pin, int blink_freq) {
  digitalWrite(led_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(blink_freq);             // wait for a second
  digitalWrite(led_pin, LOW);    // turn the LED off by making the voltage LOW
  delay(blink_freq);             // wait for a second
}



/*****************
 * COMMUNICATION *
 *****************/
void comm(){

  if (!connected2pi){

    send_msg("@,tsy_conn,$");
    Serial.println("Trying to connect...");
      
    msg = recv_msg(); // 1
    msg2target(msg);
    if (msg == "rpi_conn"){
      connected2pi = true;
      Serial.println("Connected");
    }
    delay(1000);
  }
  else{
    /*
      1. listen to serial
      2. decode message
      3. compute new target
      4. encode target
      5. send to serial
    */
    msg = recv_msg(); // 1
    Serial.println("\nmessage recv= "+msg);
    msg2target(target_pose, msg, false); // 2
    //Serial.println("x = "+String(new_pose.x));
    delay(5);
  
          //Simulate fwd kinematics
          float dx = target_pose.x - new_pose.x;
          if(abs(dx) > 0.15){
            Serial.print("***dx***");
            new_pose.x += 0.10; // 3
            if (new_pose.x > 100.0) new_pose.x = 0.0;
          }
    
    target2msg(new_pose, msg, false, false); // 4
    //Serial.println("message send= "+msg);
    
    send_msg(msg); //try to send once
    delay(5);
  }
}

void test_comm1_msg2params(){ //need to make all 'comm_1.h' methods public to be able to test it
  
  COMM c;
  
  int t_delay = 10;
  //-1
  c.msg2params("@,a=-1,b=60,$");
  c.debug_params();delay(t_delay);
  //0
  c.msg2params("@,a=0,b=1,fwd=1,$");
  c.debug_params();delay(t_delay);
  //1
  c.msg2params("@,a=1,b=1,trn=90,$");
  c.debug_params();delay(t_delay);
  //2
  c.msg2params("@,a=2,b=1,trnr=20,$");
  c.debug_params();delay(t_delay);
  //3
  c.msg2params("@,a=3,b=1,od=25,$");
  c.debug_params();delay(t_delay);
  //4
  c.msg2params("@,a=4,b=1,v=0.3,$");
  c.debug_params();delay(t_delay);
  //5
  c.msg2params("@,a=5,b=1,fwd=2,$");
  c.debug_params();delay(t_delay);
  //6
  c.msg2params("@,a=6,b=1,s=500$");
  c.debug_params();delay(t_delay);
  //7
  c.msg2params("@,a=7,b=1$");
  c.debug_params();delay(t_delay);
  //8
  c.msg2params("@,a=8,b=1$");
  c.debug_params();delay(t_delay);
  //9
  c.msg2params("@,a=9,b=1$");
  c.debug_params();delay(t_delay);
  //10
  c.msg2params("@,a=10,b=1$");
  c.debug_params();delay(t_delay);
  //11.1
  c.msg2params("@,a=11,b=0$");                      //set_debug(false)
  c.debug_params();delay(t_delay);
  //12
  c.msg2params("@,a=11,b=1,a=12,kp=12,ki=2$");      //set_debug(true) + PI M1 (2 actions in 1 msg)
  c.debug_params();delay(t_delay);
  //13
  c.msg2params("@,a=13,kp=13,ki=3$");
  c.debug_params();delay(t_delay);
  //14
  c.msg2params("@,a=14,ki=4,kp=14,ki=0$");          //setting 2 times the same param (ki)
  c.debug_params();delay(5000);
  
  delay(5000);
}

void test_comm1_read(){
  COMM c;
  c.read_serial();
}


/*********
 * DRIVE *
 *********/
void drive(){
  // Andrija is working on it
  //Serial.println("---drive()---");
}


/****************
 * READ SENSORS *
 ****************/
void read_sensors(){
  // Check the test .ino files of IR and IMU
  //Serial.println("---read_sensors()---");
}

void setup() {
  // put your setup code here, to run once:

  int tr[] = {300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, 115200}; //Transmission rate
  Serial.begin(tr[11]);
  delay(3000);

  pinMode(LED_PIN, OUTPUT); // initialize the digital pin as an output.
  //init_output_buffer();
}

enum Command {TRN, FWD};
void test_enum(){
  Serial.print("turn: "+String(TRN)); //delay(2000);
  Serial.println(FWD); //delay(2000);
}

void loop() {
  //func4("hi!!!");
  /*if (target_pose.x == 77.77) led_blink(LED_PIN, BLINK_FREQ);
  else delay(5);*/
  //test_enum();  //working

  
  read_sensors(); //read IR and IMU values and save
  drive(); // drive to target using encoders and motors

  
  //comm(); // read if new target_pose and/or special events, send last sensor values
  //test_comm1_msg2params(); // working!!! //need to make all 'comm_1.h' methods public to be able to test it
  test_comm1_read();
}
