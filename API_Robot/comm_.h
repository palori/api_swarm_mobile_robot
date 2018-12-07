#ifndef comm__h
#define comm__h


#include "Arduino.h"
//#include "pins.h"
#include <string.h>
#include <vector>
#include <errno.h>  /* ERROR Number Definitions          */

using namespace std;


void send_msg(String msg);
String recv_msg();


/*
	FROM HERE COPIED FROM 'comm_msg.h'
*/

// might go in an other place
struct target{ // info RPi send to Teensy
	// tartet position in robot coord. syst.
    float x;
    float y;
    float th;

    // servo setpoint
    float servo; // maybe not here?
};

// have to go with struct target
void print_target(target new_pose);

// decode the received message into target
void msg2target(target & new_pose, String msg, bool print_msg);

// encode the target to send the message
void target2msg(target new_pose, String & msg, bool print_msg, bool send_only_if_updaded);


#endif