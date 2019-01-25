
//#include <comm_msg.h>
#include <comm_.h>
//#include <stdio.h>
//using namespace std;

/*
  1. listen to serial
  2. decode message
  3. compute new target
  4. encode target
  5. send to serial
*/

int LED_PIN = 13;                         //pin in Teensy 3.0
int BLINK_FREQ = 1000;                    //time [ms]
target new_pose = {0.0,                   //new_pose.x
                   0.0,                   //new_pose.y
                   0.0,                   //new_pose.th
                   0.0
                  };                  //new_pose.servo
target target_pose = new_pose;
String msg = "initial message!";
/*
  new_pose.servo = 0.0;
  new_pose.x = 0.0;
  new_pose.y = 0.0;
  new_pose.th = 0.0;*/


void led_blink(int led_pin, int blink_freq) {
  digitalWrite(led_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(blink_freq);             // wait for a second
  digitalWrite(led_pin, LOW);    // turn the LED off by making the voltage LOW
  delay(blink_freq);             // wait for a second
}

void setup() {
  // put your setup code here, to run once:

  int tr[] = {300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, 115200}; //Transmission rate
  Serial.begin(tr[11]);
  delay(5000);

  pinMode(LED_PIN, OUTPUT); // initialize the digital pin as an output.
  //init_output_buffer();
}


void loop() {
  /* try to read the message 5 times
    int count = 0;
    while(count <5){
    msg = recv_msg(); // 1
    //Serial.println("message recv= "+msg);
    msg2target(new_pose, msg, false); // 2
    count+=1;
    delay(0.5);
    }
  */
  msg = recv_msg(); // 1
  Serial.println("\nmessage recv= "+msg);
  msg2target(target_pose, msg, false); // 2
  Serial.println("x = "+String(new_pose.x));
  if (target_pose.x == 77.77) led_blink(LED_PIN, BLINK_FREQ);
  else delay(10);

  if(target_pose.x - new_pose.x > 0.15){
    new_pose.x += 0.10; // 3
    if (new_pose.x > 100.0) new_pose.x = 0.0;
  }
  target2msg(new_pose, msg, false, false); // 4
  //Serial.println("message send= "+msg);
  
  /* try to send the message 5 times
    int count = 0;
    while(count <5){
    send_msg(msg); // 5
    count+=1;
    delay(0.5);
    }*/
  send_msg(msg); //instead just try to send once
  delay(50);
  
  
}
