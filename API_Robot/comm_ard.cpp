
/*
HELP:
    https://www.arduino.cc/reference/en/language/functions/communication/serial/
*/

#include <Arduino.h>
#include "comm_ard.h"

void test_write(){
	if (Serial.available()){
    	float speed = Serial.parseFloat();
	    if (speed < 0){
		    //Serial.println("vel = 0");
		    Serial.write("0");
	    }
	    else if (speed <10){
			//Serial.println("vel = [0,10)");
			Serial.write("5");
	    }
	    else {
			//Serial.println("vel > 10");
			Serial.write("A");
	    }
 	}
}
/*
void test_write_str(){
	if(Serial.available()){
		String s = Serial.readString();
		Serial.write(s);
	}
}

void test_write_bytes(int msg_len){
	char c[msg_len];
	if(Serial.available()){
		Serial.readBytes(c, msg_len);
		Serial.write(c, msg_len);
	}
}
*/
void test_read(){
	String msg = Serial.readString();
	Serial.println("    New message: "+msg);
}