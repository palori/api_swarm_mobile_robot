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

/*
 * IF YOU MAKE ANY CHANGE IN THIS FILE, YOU HAVE TO MODIFY ACCORDINGLY THE FOLLOWING FILES:
 * 
 * 
 * 
 */

enum Command {
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
	VEL,				// Set maximum speed
	SET_PID_M1,			// All 'SET_PID_...' need to be sent together with, at least one
	SET_PID_M2,			// of the following: 'kp', 'ki'
	SET_PID_TH,
	//...


	FWD,				// Drive forward a certain distance
	TRN, 				// Turn arround the center of the wheels certain degrees
	TRNR
};

struct params{
	bool connect = false;
	bool reset_enc = true;
	bool stop = true;
	bool avoid_obst = false;
	float obst_dist = 0.0;			// [mm]

	bool ir_on = false;
	bool ir_send = false;

	bool imu_on = false;
	bool imu_gyro_send = false;
	bool imu_acc_send = false;
	bool imu_comp_send = false;

	bool motors_on = false;
	bool vel = false;

	float m1_kp = 1.0;
	float m1_ki = 1.0;

	float m2_kp = 1.0;
	float m2_ki = 1.0;

	float th_kp = 1.0;
	float th_ki = 1.0;

	float fwd_dist = 0.0;			// [mm]
	float trn_deg = 0.0;			// [ª]
};

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



void msg2target(String & msg);

#endif