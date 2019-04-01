#ifndef robot_params_h
#define robot_params_h

#include "utils.h"
#include "item.h"

using namespace std;



class Robot_params
{
public:
	Robot_params();
	~Robot_params();
	Robot_params(string hostname, int port_info, int max_len);
	Robot_params(string hostname, int port_image,
				 int port_task, int port_info,
				 int port_info_robot_a, 
				 int port_info_robot_b, int max_len);


	// Attributes
	int MAX_LEN = 10;			// default

	Item<int> id;
	Item<string> hostname;
	Item<int> port_image;
	Item<int> port_task;
	Item<int> port_info;
	Item<int> port_info_robot_a;
	Item<int> port_info_robot_b;

	// vectors
	Items<float> x;		// maybe just the current position???
	Items<float> y;
	//Items<float> z;
	Items<float> th;
	Items<int> tasks;	// list of completed tasks by this robot (the last one is the current, working on)


	int get_MAX_LEN();
	void set_MAX_LEN(int i);

	void print_info();

private:
	void init_items();

};


#endif