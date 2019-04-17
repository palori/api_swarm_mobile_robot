#ifndef maps_h
#define maps_h

#include "../graph/dijkstra.h"
#include "../messages.h"

enum node_type{		// see how to use it
	TRANSITION,		// maybe goes in 'messages.h'
	TASK,
	TASK_START,
	TASK_END
};


// create a test map!
//Graph* map_test();
Graph* map_mission0();



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