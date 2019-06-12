#include <iostream>
#include <chrono>

using namespace std;

class Task_planner
{
public:
	Task_planner()
	~Task_planner();

	// list/array/struct... of tasks (might read from YAML file?)

	// keep track of tasks accomplished, attempted, to do...

	void assingn_task();

	void publish_info();

	// might want to keep track of last TIME the other robots published their info
	// send info = events & keep alive(heart beat)

	
};