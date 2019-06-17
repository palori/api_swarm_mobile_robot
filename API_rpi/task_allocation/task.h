#ifndef task_h
#define task_h

#include "../utils/utils.h"
#include "../utils/item.h"

using namespace std;



class Task
{
public:
	Task();
	~Task();
	Task(int type, string name, int num_robots, int priority);

	void init(int type, string name, int num_robots, int priority);

	// Attributes
	Item<int> type;
	Item<string> name;
	Item<int> num_robots;
	Item<int> priority;

	//void print_info();
	//Tasks & operator=(Tasks & rp);

};


#endif