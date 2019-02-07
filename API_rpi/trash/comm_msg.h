
#ifndef COMM_MSG_H
#define COMM_MSG_H

#include <stdio.h>
#include <fcntl.h>  /* File Control Definitions          */
//#include <termios.h>/* POSIX Terminal Control Definitions*/
#include <unistd.h> /* UNIX Standard Definitions         */
#include <errno.h>  /* ERROR Number Definitions          */
#include <iostream>
#include <string.h>
#include <vector>
#include <time.h>
#include <unistd.h> // time sleep microseconds
#include <chrono> // the best one to count time in [s, ms, ns...]
//#include <Arduino.h>
using namespace std;
using namespace std::chrono;


const float BIG_NUM = 191919.191919;

// might go in an other place
// @@@@ NEED TO DESIGN IT -> TO SEND STATUS MESSAGES BETWEEN 2 BOARDS @@@@
struct flags{ // info RPi send to Teensy
	// some possible examples...
    bool running;
    bool stop;
    // ...
};

// have to go with struct flags
void print_flags(flags status);


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



// might go in an other place
// @@@@ talk with Andrija if we need it or not
struct sensors{ // info coming from Teensy to RPi
    // IR sensors
    float ir1;
    float ir2;

    // IMU
    // ...

    // robot position in world coord. syst.
    float xw;
    float yw;
    float thw;
};

// have to go with struct sensors
void print_sensors(sensors new_sens);



// this should go in utils
float str2float(string s);

// decode the received message into target
void msg2target(target & new_pose, string msg, bool print_msg);

// encode the target to send the message
void target2msg(target new_pose, string & msg, bool print_msg, bool send_only_if_updaded);


/**** TO DO ****

* Some of them are for both RPi and Teensy
	-> figure out how to share the code in both folders
	   (maybe some sh commands so copy folders???)

msg2sensors();
sensors2msg();

msg2flags();
flags2msg();

****************/

#endif
