#ifndef node_task_h
#define node_task_h

#include "../utils/utils.h"
#include "../utils/item.h"

using namespace std;


#define NO_TASK -1


class NodeTask
{
public:
	NodeTask();
	~NodeTask();
	NodeTask(string node_id);
	NodeTask(string node_id, int task_id, int aruco_id);

	void init(string node_id, int task_id, int aruco_id);

	// Attributes
	Item<string> node_id;
	Item<int> task_id;
	Item<int> aruco_id;

	//void print_info();
	//Tasks & operator=(Tasks & rp);

};


#endif