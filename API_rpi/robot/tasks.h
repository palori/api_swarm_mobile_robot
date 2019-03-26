#ifndef tasks_h
#define tasks_h

#include "utils.h"
#include "messages.h"

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

	

	int dist_avoid_obst; // may vary depending on the obstacle

};

#endif