
#include "node_task.h"


NodeTask::NodeTask(){	
	string node_id = "";		// default attributes
	int task_id = NO_TASK;
	int aruco_id = NO_TASK;
	init(node_id, task_id, aruco_id);
}

NodeTask::~NodeTask(){}

NodeTask::NodeTask(string node_id){
	int task_id = NO_TASK;			// default attributes
	int aruco_id = NO_TASK;
	init(node_id, task_id, aruco_id);
}

NodeTask::NodeTask(string node_id, int task_id, int aruco_id){
	init(node_id, task_id, aruco_id);
}

void NodeTask::init(string node_id, int task_id, int aruco_id){
	this->node_id.set(node_id);
	this->task_id.set(task_id);
	this->aruco_id.set(aruco_id);
}
