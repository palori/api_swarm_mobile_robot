#ifndef robot_params_h
#define robot_params_h

#include <chrono>				// storing keep alive (KA) times

#include "../utils/utils.h"
#include "../utils/item.h"
#include "../task_allocation/tasks.h"
#include "keep_alive.h"

using namespace std;



class Robot_params
{
public:
	Robot_params();
	~Robot_params();
	Robot_params(string hostname,
				 int id,
				 int max_len,
				 int port_info);
	Robot_params(string hostname,
				 int id,
				 int max_len,
				 int port_info,
				 int port_image,
				 int port_task
				 //int port_info_robot_a, 
				 //int port_info_robot_b,
				 );


	void init_items();


	// Attributes
	int MAX_LEN = 10;			// default

	Item<string> hostname;
	Item<int> port_image;
	Item<int> port_task;
	Item<int> port_info;
	//Item<int> port_info_robot_a;	// old
	//Item<int> port_info_robot_b;	// old


	/* Info of the specific position might not be needed
	Items<float> x;		// maybe just the current position???
	Items<float> y;		// old
	//Items<float> z;
	Items<float> th;*/

	//Items<int> tasks;			// old
	//Items<string> to_do;	// tasks assigned by the leader
	//Items<string> done;	// list of completed tasks by this robot (the last one is the current, working on)
	Tasks tasks;

	Item<string> previous_node;	// or start node of the route and then having current node???
	//Item<string> current_node;
	Item<string> destiny_node;

	Items<string> route;	// output from path planning??? (not ready for that, yet)

	KeepAlive ka; 		// keep alive (KA)
	Item<int> id;

	int get_MAX_LEN();
	void set_MAX_LEN(int i);

	void print_info();

	Robot_params & operator=(Robot_params & rp);


};


#endif