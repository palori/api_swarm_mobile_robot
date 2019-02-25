
#ifndef COMM_RPI_1_H
#define COMM_RPI_1_H


#include <stdio.h>
#include <fcntl.h>  /* File Control Definitions          */
#include <termios.h>/* POSIX Terminal Control Definitions*/
#include <unistd.h> /* UNIX Standard Definitions         */
#include <errno.h>  /* ERROR Number Definitions          */
#include <iostream>
//#include <string.h> // already in utils
#include <vector>
#include <time.h>
#include <unistd.h> // time sleep microseconds
#include <chrono> // the best one to count time in [s, ms, ns...]


#include "utils.h"

using namespace std;
using namespace std::chrono;


#define USB_SERIAL_PORT = "/dev/ttyACM0"		// maybe into a configuration file


/*
 * IF YOU MAKE ANY CHANGE IN THIS FILE, YOU HAVE TO MODIFY ACCORDINGLY
 * THE FOLLOWING FILES:
 * 
 *   - comm_1.h
 *   - comm_1.cpp
 *   - ../API_rpi/comm_rpi_1.h
 *   - ../API_rpi/comm_rpi_1.cpp
 */


/*
 * SUGGESTION: may be useful to switch the home made reading & writing
 *			   of the serial port for a Serial library on its own.
 *
 *			   E.g: http://wjwwood.io/serial/
 */




class COMM_RPI
{
public:
	COMM_RPI();
	~COMM_RPI();
	// Serial communication functions
	void serial_open();
	void serial_close();
	void serial_write(string msg);
	string serial_read();

	// Might be called from other nodes
	void params2msg(string & msg);		// encode the new params to send the message
	void msg2sensorData(string msg);	// decode the received message

	// getters
	int get_fd() {return fd;}
	bool get_port_open() {return port_open;}
	bool get_debug() {return debug;}


	// setters
	void set_fd(int i){fd = i;}
	void set_port_open(bool b) {port_open = b;}
	void set_debug(bool b) {debug = b;}


	// other methods
	string to_string();
	void debug_params();

private:
	bool debug = true;
	int fd = -1;								// indicates port state, default: -1 (closed)
	bool port_open = false;

	enum Actions {
		CONNECT,			// To know when the connection started and ended to send (or not) messages
		RESET_ENC,
		STOP,				// Immediatelly stops the driving when turned on (security)
		AVOID_OBSTACLES,	// If true, it will stop if IR detect and obstacle closer than a certain distance
							// else it will ignore any obstacle
		IR_ON,				// Enable/Disable reading IR data
		IR_SEND,			// Enable/Disable sending IR data
		
		IMU_ON,				// Enable/Disable reading IMU data
		IMU_GYRO_SEND,		// Enable/Disable sending IMU gyroscope data
		IMU_ACC_SEND,		// Enable/Disable sending IMU accelerometer data
		IMU_COMP_SEND,		// Enable/Disable sending IMU compass data

		MOTORS_ON,			// Enable/Disable motors
		DEBUG,				// Enable/Disable debugging messages

		// this are params, not actions
		
		SET_PID_M1,			// All 'SET_PID_...' need to be sent together with, at least one
		SET_PID_M2,			// of the following: 'kp', 'ki'
		SET_PID_TH,
		//...	
	};

	struct Command {
		string A = 'a';					// Action
		string B = 'b';					// Value of the action
		string FWD = "fwd";				// Drive forward a certain distance [mm]
		string TRN = "trn";				// Turn certain degrees [º]
		string TRNR = "trnr";			// Turning radius [mm]
		string V = "v";					// Maximum speed [mm/s]
		string S = "s";					// Servo position [?]
		string OD = "od";				// Distance to detect obstacles [mm]
		string KP = "kp";				// P gain
		string KI = "ki";				// I gain
	};










	// OLD functions


	/*/ Message stiles
	// might go in an other place
	// @@@@ NEED TO DESIGN IT -> TO SEND STATUS MESSAGES BETWEEN 2 BOARDS @@@@
	struct flags{ // info RPi send to Teensy
		// some possible examples...
		int robot_id;
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

		// Sensing the environment
		// IR sensors
		float ir1;
		float ir2;

		// IMU
		// ...


		// Current robot position
		// robot position in robot coord. syst.
		float x;
		float y;
		float th;
		// robot position in world coord. syst.
		float xw;
		float yw;
		float thw;
	};

	// have to go with struct sensors
	void print_sensors(sensors new_sens);
	*/


	/**** TO DO ****

	msg2sensors();
	sensors2msg();

	msg2flags();
	flags2msg();

	****************/
}

#endif