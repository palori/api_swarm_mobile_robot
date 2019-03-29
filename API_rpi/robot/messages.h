#ifndef messages_h
#define messages_h

#include "utils.h"
#include "controllers.h"
#include "robot_params.h"
#include "item.h"


string detect_message(string msg);

string encode(Item<int> task);
string encode(Items<int> tasks);
string encode_image_params(Items<int> tasks, bool obst_found, float obst_dist, int theta, int crossing);
void decode(string msg, Controllers & c, Robot_params r);



enum Actions {
	// TEENSY parameters
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

	// Driving options
	FWD,				// drive straight forward
	TRN,				// turn in the spot
	TRNR,				// turning with a radius (not working)
	DRIVE,				// drive to a target point (not really good)
	FOLLOW,				// follow a line (heading control, theta)
	//STOP,				// already included in the top	
	//WAIT				// may include waiting for other robots??

	// Image processing options
	IDLE,				// do nothing, might also work for driving (need to be set up in the TEENSY)
	LINE,				// looking for lines (include detecting crossings)
	BALL,				// find balls (orange golf balls in this case)
	HOLE,				// find a hole in the floor to place the ball
	SHAPE,				// find shapes (orange cross, orange circle, green triangle and green square)
	ARUCO				// find ArUco codes (for localization) (similar to QR codes)

};


enum crossing{
	NO_CROSS,			// there is no crossing
	T_MIDDLE,
	T_LEFT,
	T_RIGHT,
	X,
	Y,
	XY			// combination of X and Y crossings
};


struct Command {
	// general
	string A = "a";					// Action
	string B = "b";					// Value of the action

	// commands and tunning TEENSY
	string FWD = "fwd";				// Drive forward a certain distance [mm]
	string TRN = "trn";				// Turn certain degrees [º]
	string TRNR = "trnr";			// Turning radius [mm]
	string V = "v";					// Maximum speed [mm/s]
	string S = "s";					// Servo position [?]
	string OD = "od";				// Distance to detect obstacles [mm]
	string KP = "kp";				// P gain
	string KI = "ki";				// I gain
	string X_t = "xt";				// X  coord. of target pose (in robot coord. syst.)
	string Y_t = "yt";				// Y  coord. of target pose (in robot coord. syst.)
	string TH_t = "tht";			// Th coord. of target pose (in robot coord. syst.)

	// sensor data
	string X_w = "xw";				// X  coord. of robot pose (in world coord. syst.)
	string Y_w = "yw";				// Y  coord. of robot pose (in world coord. syst.)
	string TH_w = "thw";			// Th coord. of robot pose (in world coord. syst.)
	string IR1 = "ir1";				// ir 1, sensor value
	string IR2 = "ir2";				// ir 2, sensor value
	string GYRO1 = "g1";			// gyroscope 1, sensor value
	string GYRO2 = "g2";			// gyroscope 2, sensor value
	string GYRO3 = "g3";			// gyroscope 3, sensor value
	string ACC1 = "a1";				// accelerometer 1, sensor value
	string ACC2 = "a2";				// accelerometer 2, sensor value
	string ACC3 = "a3";				// accelerometer 3, sensor value
	string COMP1 = "c1";			// compass 1, sensor value
	string COMP2 = "c2";			// compass 2, sensor value
	string COMP3 = "c3";			// compass 3, sensor value
	string OF = "of";				// obstacle found

	// image
	string CROSS = "cr";
};














// decide what to do with this enum...
enum obstacle{
		NO,					// there is no obstacle
		GUILLOTINE,
		RAMP_1,
		RAMP_2,
		SEE_SAW_BALL_1,
		STAIRS_UP,
		//STAIRS_DOWN,
		BALL_2,
		//...

		RACE,
		SHAPES,				// maybe expand: CROSS, CIRCLE, TRIANGLE and SQUARE
		AX_GATE,
		TUNNEL,
		PORT_3,

		ROUNDABOUT,			// ?

		GOAL
	};

#endif