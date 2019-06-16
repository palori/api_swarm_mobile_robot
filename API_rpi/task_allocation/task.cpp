
#include "task.h"


Task::Task(){
	int type = -1;			// default attributes
	string name = "";
	int num_robots = -1;
	int priority = -1;
	init(type, name, num_robots, priority);
}

Task::~Task(){}

Task::Task(int type, string name, int num_robots, int priority){
	init(type, name, num_robots, priority);
}

void Task::init(int type, string name, int num_robots, int priority){
	this->type.set(type);
	this->name.set(name);
	this->num_robots.set(num_robots);
	this->priority.set(priority);
}

