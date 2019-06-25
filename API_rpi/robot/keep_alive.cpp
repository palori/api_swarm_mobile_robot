#include "keep_alive.h"



KeepAlive::KeepAlive(){
	init();
}

KeepAlive::KeepAlive(float threshold_time, int max_len){
	init(threshold_time, max_len);
}

KeepAlive::~KeepAlive(){}

void KeepAlive::init(){
	int MAX_LEN = 10;			// default
	set_MAX_LEN(MAX_LEN);
	auto now = chrono::system_clock::now();
	this->times.add_item_noMutex(now - std::chrono::hours(1));
	this->threshold_time.set(5.0);	// in seconds
	this->alive.add_item_noMutex(false);
	this->is_now_alive.set(false);
}

void KeepAlive::init(float threshold_time, int max_len){
	init();
	set_MAX_LEN(max_len);
	this->threshold_time.set(threshold_time);	// in seconds
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
	if (ka_elapsed > threshold_time.get()){ // migth be dead
		last_is_alive = false;
	}
	alive.add_item(last_is_alive);

	vector<bool> a = alive.get_items();
	int len = a.size();
	int last = len;
	int M = 5; // window used to determine if the robot is alive or not

	if (len >= M) len = M; // checking as maximum the last M items.

	// checking the window values
	int count_true = 0;
	for (int i = 0; i < len; i++){
		if (a.at(last-1-i)) count_true++;
	}
	// condition
	if (count_true >= len/2) is_now_alive.set(true);
	else is_now_alive.set(false);

	return is_now_alive.get(); 
}

