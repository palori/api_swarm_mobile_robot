#ifndef keep_alive_h
#define keep_alive_h

#include <chrono>				// storing keep alive (KA) times

#include "../utils/utils.h"
#include "../utils/item.h"

using namespace std;



class KeepAlive
{
public:
	KeepAlive();
	~KeepAlive();
	KeepAlive(float threshold_time, int max_len);


	// Attributes
	int MAX_LEN = 10;			// default
	Items<auto> times; 		// keep alive (KA)
	Item<float> threshold_time;
	Items<bool> alive;
	Item<bool> is_now_alive;

	int get_MAX_LEN();
	void set_MAX_LEN(int i);

	//void print_info();
	//KeepAlive & operator=(KeepAlive & rp);

	float get_time();
	bool is_alive();

private:
	void init_items();

};


#endif