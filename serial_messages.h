
#ifndef SERIAL_MESSAGES_H
#define SERIAL_MESSAGES_H


/******************
 * IMPORTANT NOTE *
 ******************

 THIS DOCUMENT IS THE BASIS TO DESCIBE THE TYPE OF INFORMATION
 EXCHANGE BETWEEN THE 2 BOARDS (Teensy (Arduino based) and
 Raspberry Pi).

 FOR THIS REASON, IF YOU CHANGE SOMETHING HERE YOU MIGHT ALSO
 DO IT IN THE FILES WHERE YOU INCLUDE IT, SUCH AS
     /API_Robot/comm_.cpp
     /API_Robot/comm_.h
     /API_rpi/comm_rpi.cpp
     /API_rpi/comm_rpi.h

 AND MAYBE OTHERS TOO.

 ******************/



// Message stiles
// might go in an other place
// @@@@ NEED TO DESIGN IT -> TO SEND STATUS MESSAGES BETWEEN 2 BOARDS @@@@
struct flags{ // info RPi send to Teensy
	// some possible examples...
	int robot_id;
    bool running;
    bool stop;
    // ...
};


// might go in an other place
struct target{ // info RPi send to Teensy
	// tartet position in robot coord. syst.
    float x;
    float y;
    float th;

    // servo setpoint
    float servo; // maybe not here?
};


// might go in an other place
// @@@@ talk with Andrija if we need it or not
struct sensors{ // info coming from Teensy to RPi

	/* Sensing the environment */
    // IR sensors
    float ir1;
    float ir2;

    // IMU
    // ...


    /* Current robot position */
    // robot position in robot coord. syst.
    float x;
    float y;
    float th;
    // robot position in world coord. syst.
    float xw;
    float yw;
    float thw;
};


#endif