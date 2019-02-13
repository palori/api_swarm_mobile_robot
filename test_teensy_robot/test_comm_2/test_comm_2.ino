
#include <comm_.h>
//#include "header_conditionals.h" //test, it can be deleted :)
//#include <stdio.h>
//using namespace std;


int LED_PIN = 13;                         //pin in Teensy 3.0
int BLINK_FREQ = 1000;                    //time [ms]
target new_pose = {0.0,                   //new_pose.x
                   0.0,                   //new_pose.y
                   0.0,                   //new_pose.th
                   0.0};                  //new_pose.servo
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


void loop() {
  //func4("hi!!!");
  /*if (target_pose.x == 77.77) led_blink(LED_PIN, BLINK_FREQ);
  else delay(5);*/

  read_sensors(); //read IR and IMU values and save
  drive(); // drive to target using encoders and motors
  comm(); // read if new target_pose and/or special events, send last sensor values
  
}
