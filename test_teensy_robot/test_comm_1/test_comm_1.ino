
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

target new_pose = {0.0, 0.0, 0.0, 0.0};
String msg = "initial message!";
/*
new_pose.servo = 0.0;
new_pose.x = 0.0;
new_pose.y = 0.0;
new_pose.th = 0.0;*/

void setup() {
  // put your setup code here, to run once:

  int tr[] = {300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, 115200}; //Transmission rate
  Serial.begin(tr[11]);
  delay(2000);
}


void loop() {
  //msg = recv_msg(); // 1
  //Serial.println("message recv= "+msg);
  //msg2target(new_pose, msg, false); // 2

  new_pose.y += 10.0; // 3
  target2msg(new_pose, msg, false, false); // 4
  Serial.println("message send= "+msg);
  send_msg(msg); // 5
  delay(1000);
  
}
