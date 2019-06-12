
#ifndef COMM_RPI_2_H
#define COMM_RPI_2_H


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


#include "../utils/utils.h"
#include "../robot/sensors.h"
#include "messages.h"


using namespace std;
using namespace std::chrono;


#define USB_SERIAL_PORT "/dev/ttyACM0"		// maybe into a configuration file


/*
 * IF YOU MAKE ANY CHANGE IN THIS FILE, YOU HAVE TO MODIFY ACCORDINGLY
 * THE FOLLOWING FILES:
 * 
 *   - comm_1.h
 *   - comm_1.cpp
 *   - ../API_rpi/comm_rpi_2.h
 *   - ../API_rpi/comm_rpi_2.cpp
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

	//encoding and decoding messages -> both moved to 'messages.h'
	//void params2msg(string & msg);
	//void msg2sensorData(string msg, Sensors & sens);

	// getters
	int get_fd() {return fd;}
	bool get_port_open() {return port_open;}
	bool get_debug() {return debug;}


	// setters
	void set_fd(int i){fd = i;}
	void set_port_open(bool b) {port_open = b;}
	void set_debug(bool b) {debug = b;}


	// other methods
	//string to_string();		// change name, conflicts
	void debug_params();

private:
	bool debug = false;
	int fd = -1;								// indicates port state, default: -1 (closed)
	bool port_open = false;


public: // if we want to use them in other files -> maybe even outside the class...
	
	// enum Actions -> moved to 'messages.h'
	
	// struct Command -> moved to 'messages.h'
};

#endif