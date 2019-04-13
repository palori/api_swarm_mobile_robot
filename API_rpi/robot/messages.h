#ifndef messages_h
#define messages_h

#include "utils.h"
#include "controllers.h"
#include "robot_params.h"
#include "sensors.h"
#include "item.h"


string detect_message(string msg);

string encode_task(int task);
string encode_image_params(int task, bool obst_found, float obst_dist, float theta, int crossing);
//string encode_init(string you_are, string robot_a, string robot_b, int max_len);
//void params2msg(string & msg);

void decode_task(string msg, Items<int> & tasks);
void decode_ctrl(string msg, Controllers & ctrl);	// NOT fully tested, but the important ones yes
void decode_robot_params(string msg, Robot_params & rob);
void decode_image(string msg, Sensors & sens, string & new_target);
void decode_sensors(string msg, Sensors & sens);
void decode_master_commands(string msg, string hostname, int & action, float & fwd, float & vel);


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
	RACE,
	STAIRS,
	//STOP,				// already included in the top	
	//WAIT				// may include waiting for other robots??

	// Image processing options
	IDLE,				// do nothing, might also work for driving (need to be set up in the TEENSY)
	LINE,				// looking for lines (include detecting crossings)
	BALL,				// find balls (orange golf balls in this case)
	HOLE,				// find a hole in the floor to place the ball
	SHAPE,				// find shapes (orange cross, orange circle, green triangle and green square)
	ARUCO,				// find ArUco codes (for localization) (similar to QR codes)

	// Mission options
	START,
	PAUSE

};


enum crossing{
	NO_CROSS,	// there is no crossing
	T_MIDDLE,
	T_LEFT,
	T_RIGHT,
	X,
	Y,
	XY			// combination of X and Y crossings
};

enum Side {  // on which side to follow the line 
	LEFT, 
	MIDDLE, 
	RIGHT 
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
	string OF_i = "of_i";			// obstacle found in the image
	string CROSS = "cr";			// which crossing type was found
	string OD_i = "od_i";			// Distance of the obstacle from the robot[mm]


	// robot
	string ROB = "rob";				// in case the message is sent to a specific robot (hostname)
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