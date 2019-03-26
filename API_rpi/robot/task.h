#ifndef tasks_h
#define tasks_h

#include "utils.h"

class Task
{
public:
	Task();
	~Task();

private:
	int task;
	string name; 	// is it needed?




public:

	// Getters
	int get_task();


	// Setters
	void set_task(int i);



	// Mutex
	mutex mtx_task;


	enum type{
		drive,
		image_find,
		obstacle, 		// does this go here?
	};

	
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


	int dist_avoid_obst; // may vary depending on the obstacle

};

#endif