#ifndef tasks_h
#define tasks_h

#include "utils.h"

class Tasks
{
public:
	Tasks();
	~Tasks();

private:
	int MAX_LEN = 10;
	vector<int> tasks;		// sotre last 'MAX_LEN' tasks





public:

	// Getters
	int get_MAX_LEN();

	int get_current_task();
	vector<int> get_tasks();


	// Setters
	void set_MAX_LEN(int i);

	void set_task(int i);



	// Mutex
	mutex mtx_task;

	
	enum drive{
		IDLE,
		FOLLOW_LINE,
		FWD,
		TRN,
		STOP,
		WAIT			// may include waiting for other robots??
	};

	enum avoid_obstacles{
		NO,		// used to push the shapes, catch balls...
		YES
	};

	enum image_find{
		IDLE,
		LINE,
		CROSSING,	// maybe not needed and included in 'LINE'
		BALL,
		HOLE,		// to place the ball
		SHAPE,
		ARUCO		// ArUco codes (for localization)

	};

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
		AX-GATE,
		TUNNEL,
		PORT_3,

		ROUNDABOUT,			// ?

		GOAL
	};

	enum obst_start{
		YES,
		NO
	};

	enum line{
		YES,
		NO
	};

	enum crossing{
		NO,		// there is no crossing
		T,
		X,
		Y,
		XY		// combination of X and Y crossings
	};


	struct Command {
		string A = "a";					// Action
		string B = "b";					// Value of the action
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
	};










	int dist_avoid_obst; // may vary depending on the obstacle

};

#endif