
#include "comm_ard.h"
#include <ArduinoJson.h>

unsigned long oldtime = 0;
unsigned long calltime = 0;

unsigned int msg_index = 0;
String msg[] = {"ir1=1,ir2=2,",
                "s1=100,s2=200,",
                "v1=1.0,v2=2.0,"};

void send_msg_array(){
  String txt = msg[msg_index];
  char s[sizeof(txt)];
  for (int i = 0; i < sizeof(txt); i++) {
    s[i] = txt[i];
  }
  Serial.write(s);
  msg_index++;
  if (msg_index > 3) {
    Serial.write("$");
    calltime = millis();
    Serial.println(calltime-oldtime);
    oldtime = calltime;
    msg_index = 0;
    //delay(20);
    Serial.write("@,");
  }
}

void read_msg_array_read(){
  char c[] = Serial.read();
  Serial.println(c);
}

void read_msg_array_readBytesUntil(){
  char character = '$';
  int len = 64;
  char c[len]; //buffer
  Serial.readBytesUntil(character, c, len);
}

void read_msg_array_readString(){
  String s = Serial.readString();
  Serial.println(s);
}

void read_msg_array_readStringUntil(){
  String terminator = "$";
  String s = Serial.readStringUntil(terminator);
  Serial.println(s);
}


/* No need to use that
void serialEvent() {
  String event = Serial.read();
  Serial.println("*****************");
  Serial.println("Event: " + event);
  Serial.println("*****************");
}*/

void setup() {
  // put your setup code here, to run once:

  int tr[] = {300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, 115200}; //Transmission rate
  Serial.begin(tr[11]);
  delay(2000);
}


void loop() {
  // put your main code here, to run repeatedly:
  //delay(1000);

  //******************************
  // Initial tests:
  //test_write();
  //test_read();

  //******************************
  // Tested and working ;)
  send_msg_array(); 

  //******************************
  // Testing...
  read_msg_array_read();
  
  //******************************
  // To test
  read_msg_array_readBytesUntil();
  read_msg_array_readString();
  read_msg_array_readStringUntil();
  
}
