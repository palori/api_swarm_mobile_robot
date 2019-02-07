
#include <comm_.h>

#include <ArduinoJson.h>
//#include <vector>
//#include <string>
//#include <iostream> //Error compiling for board Teensy 3.5.
#include <chrono>
#include <stdio.h>
//using namespace std::chrono;
//using namespace std;

unsigned long oldtime = 0;
unsigned long calltime = 0;

unsigned int msg_index = 0;
String msg[] = {"ir1=1,ir2=2,",
                        "s1=100,s2=200,",
                        "v1=1.0,v2=2.0,"};
/*
struct target{
  int s = 0; // servo setpoint
  //Target position
  float x = 0;
  float y = 0;
  float th = 0;
}
target tgt;
*/
void send_msg_array(){
  String txt = msg[msg_index];
  char s[sizeof(txt)];
  for (unsigned int i = 0; i < sizeof(txt); i++) {
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
  /*
  char c[64] = Serial.read();
  Serial.println(c);
  */
}

void read_msg_array_readBytesUntil(){
  char character = '$';
  int len = 64;
  char c[len]; //buffer
  Serial.readBytesUntil(character, c, len);
}
/*
void decode_msg(String msg, target & t){
  
}*/

void read_msg_array_readString(){
  String s = Serial.readString();
  if (s!=""){
    Serial.println(s);
    //decode_msg(s, tgt);
  }
}

//Error compiling for board Teensy 3.5. -> if trying to use <iostream> library!!! -> Fixed
//Error: undefined reference to `std::chrono::_V2::system_clock::now()'
/*
void test_read_serial_arduino(){
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

    //usleep(2.5e6);
    bool store=false, keep_reading=true;
    String msg[10];
    int count=-1;
    while(keep_reading){
        String s = Serial.readString();
        if (s!=""){
            if (s="@" && !store){
                store = true;
            }
            else if (s='$' && store){
                store=false;
                keep_reading=false;
            }
            else if (s!='$' && store){
                count++;
                msg[count] = s;//or maybe concatenate them
            }
        }
    }
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    //std::cout << "Read msg with Arduino Serial: " << time_span.count() << " seconds.\n";
    Serial.print("Read msg with Arduino Serial: ");
    Serial.print(time_span.count());
    Serial.print(" seconds.\n");
}
*/

void read_msg_array_readStringUntil(){
  /*
  String terminator = "$";
  String s = Serial.readStringUntil(terminator);
  Serial.println(s);
  */
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
  //send_msg_array(); 

  //******************************
  // Testing...
  read_msg_array_read();
  
  //******************************
  // To test
  //read_msg_array_readBytesUntil();
  //read_msg_array_readString(); //working fine!!
  //read_msg_array_readStringUntil();
  //test_read_serial_arduino(); // testing...
  
}
