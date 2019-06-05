#include "keep_alive.h"



KeepAlive::KeepAlive(){
	init_items();
}

KeepAlive::KeepAlive(float threshold_time, int max_len){
	init_items();
	set_MAX_LEN(max_len);
	this->threshold_time.set(threshold_time);	// in seconds
}

KeepAlive::~KeepAlive();

void KeepAlive::init_items(){
	int MAX_LEN = 10;			// default
	set_MAX_LEN(MAX_LEN);
	auto now = chrono::system_clock::now();
	this->times.add_item_noMutex(now);
	this->threshold_time.set(5);	// in seconds
	this->alive.add_item_noMutex(true);
	this->is_now_alive.set(true);
}

int KeepAlive::get_MAX_LEN(){return MAX_LEN;}

void KeepAlive::set_MAX_LEN(int i){
	MAX_LEN = i;
	times.set_MAX_LEN(i);
	alive.set_MAX_LEN(i);
}

//void KeepAlive::print_info();

//KeepAlive & KeepAlive::operator=(KeepAlive & rp){}

float KeepAlive::get_time(){
	auto now = chrono::system_clock::now();
	auto last_ka = times.get_last_item();
	chrono::duration<double> ka_elapsed = now - last_ka;
	return (float) ka_elapsed.count(); // in seconds
}


bool KeepAlive::is_alive(){
	bool last_is_alive = true;
	float ka_elapsed = get_time();
	if (ka_elapsed > threshold_time){ // migth be dead
		last_is_alive = false;
	}
	ka.add_item(last_is_alive);

	vector<bool> a = alive.get_items();
	int len = a.size();
	if (len >= 10) len = 10; // checking as maximum the last 10 items.
	int count_true = 0, count_false = 0;
	int last = a.end();
	for (int i = 0; i < len; i++){
		if (a.at(last-i)) count_true++;
		else count_false++;
	}

	// revise this conditions, this is too basic...
	if (count_true == len) is_now_alive.set(true);
	else if (count_false == len) is_now_alive.set(false);
	else if (count_true >= count_false && last_is_alive) is_now_alive.set(true);
	else is_now_alive.set(false);

	return is_now_alive.get(); 
}

