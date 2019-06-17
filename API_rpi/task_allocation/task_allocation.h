#ifndef task_allocation_h
#define task_allocation_h

#include <iostream>
#include <chrono>


#include "tasks_list.h"


using namespace std;

class Task_allocation
{
public:
	Task_allocation()
	~Task_allocation();

	// list/array/struct... of tasks (might read from YAML file?)

	// keep track of tasks accomplished, attempted, to do...

	void assign_task(Robot_params & params, Robot_params &robot_a, Robot_params & robot_b);
				// have enough info of tasks done by each robot so far and if they are still alive or not (so you can assign tasks to them or not)
				// think about what to return and how - passing arguments by reference and back in the robot using 'tasks.to_do' to sent to the robots if they are alive.



	//void publish_info();		// 'messages' encode the data and then it is 'published' by the robot


	// might want to keep track of last TIME the other robots published their info
	// send info = events & keep alive(heart beat)

	
};