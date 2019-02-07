#ifndef comm__h
#define comm__h

/*
    CPU_IS_RASPBERRY
      true if RPi is the cpu running
      false if the cpu is f.i. an Arduino or Teensy board
*/
#define CPU_IS_RASPBERRY false 


#include "Arduino.h"
//#include "pins.h"
#include <string.h>
#include <vector>
#include <errno.h>  /* ERROR Number Definitions          */

using namespace std;


//void init_output_buffer();

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