#ifndef bully_h
#define bully_h

#include <algorithm>	// std::min_element
#include <iterator>		// std::begin, std::end
#include <chrono>		// storing detecting times


#include "../utils/utils.h"
#include "../utils/item.h"

using namespace std;



class Bully
{
public:
	Bully();
	~Bully();
	Bully(int my_id, float time_threshold);

	void init();
	void init(int my_id, float time_threshold);

	// Attributes
	int MAX_LEN = 10;				// default
	Item<int> leader;				// current leader
	Item<int> my_id;
	Item<bool> trigger;				// true == leader election
	Items<int> robots_ids;			// list of all on-line robots
	Items<int> proposed_leader;		// who all robots propose as a leader

	Item<chrono::_V2::system_clock::time_point> time_detected;
	Item<float> time_threshold;
	Item<bool> i_detected;

	Item<bool> debug;

	// in the case of having Items as attributes
	/*int get_MAX_LEN();
	void set_MAX_LEN(int i);*/

	//void print_info();
	//KeepAlive & operator=(KeepAlive & rp);

	void trigger_election();
	bool election(int & leader, int & proposed_leader);

	bool is_election_time();
	float get_time();

	// leds in RPi to notify leader election process and
	// when its done to know who is the leader.


};


#endif