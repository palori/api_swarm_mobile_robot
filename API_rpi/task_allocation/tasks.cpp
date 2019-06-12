#include "tasks.h"



Tasks::Tasks(){
	init();
}

Tasks::~Tasks(){}


void Tasks::init(){
	this->to_do.set_name("To do");
	this->done.set_name("Done");
}



int Tasks::get_MAX_LEN(){return MAX_LEN;}

void Tasks::set_MAX_LEN(int i){
	MAX_LEN = i;
	to_do.set_MAX_LEN(i);
	done.set_MAX_LEN(i);
}

//void print_info();
//Tasks & operator=(Tasks & rp);

bool Tasks::mask_as_done(string task){
	bool is_done = false;
	is_done = to_do.remove_item(task);
	if (is_done) done.add_item(task);
	return is_done;
}
