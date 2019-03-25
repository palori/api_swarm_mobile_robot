#include "tasks.h"

Tasks::Tasks(){}
Tasks::~Tasks(){}




// Getters
int Tasks::get_MAX_LEN(){
	return MAX_LEN;
}

int Tasks::get_current_task(){
	mtx_task.lock();
	int taskm = tasks.back();
	mtx_task.unlock();
	return taskm;
}
vector<int> Tasks::get_tasks(){
	mtx_task.lock();
	vector<int> tasksm = tasks;
	mtx_task.unlock();
	return tasksm;
}


// Setters
void Tasks::set_MAX_LEN(int i){
	MAX_LEN = i;
}

void Tasks::set_task(int i){
	mtx_task.lock();
	tasks = add2vector(tasks, i, get_MAX_LEN());
	mtx_task.unlock();
}