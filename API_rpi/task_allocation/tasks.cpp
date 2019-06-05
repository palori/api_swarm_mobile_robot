#include "tasks.h"



Tasks::Tasks(){
	init_items();
}

Tasks::~Tasks();

Tasks::Tasks(){
	init_items();
	
}

void Tasks::init_items(){

}


// Attributes
int MAX_LEN = 10;			// default
Items<auto> to_do;
Items<bool> done;

int Tasks::get_MAX_LEN();
void Tasks::set_MAX_LEN(int i);

//void print_info();
//Tasks & operator=(Tasks & rp);

void Tasks::mask_as_done(string task);
