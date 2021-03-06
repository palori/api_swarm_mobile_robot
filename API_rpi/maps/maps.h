#ifndef maps_h
#define maps_h

#include "../graph/graph.h"
#include "../comm/messages.h"

enum node_type{		// see how to use it
	TRANSITION,		// maybe goes in 'messages.h'
	TASK,
	TASK_START,
	TASK_END
};

#define PI 3.14159


// create a test map!
//Graph* map_test();
Graph* map_mission0();
Graph* map_mission007();
Graph* map_mission1();
Graph* map_mission2();


Graph* map_mission_easy(string id);
Graph* map_mission_ax(string id);

Graph* map_mission_ro(string id);
Graph* map_mission_tunnel(string id);
Graph* map_mission_race(string id);

// tests after the RoboCup (just for documenting)
Graph* map_test_square(string id);
Graph* map_test_straight(string id, float distance, float velocity);

// other tests
Graph* map_test_report1(string id, float velocity);
Graph* map_test_report2(string id, float velocity);

#endif

/*
Testing commands
----------------
start
rob=all,a=28

stop
rob=all,a=2,b=1

fwd
rob=all,a=2,b=1

stairs
rob=all,a=21,b=1,fwd=6,v=0.7

*/
