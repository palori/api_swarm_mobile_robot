#ifndef tasks_h
#define tasks_h

#include "../utils/utils.h"
#include "../utils/item.h"

using namespace std;



class Tasks
{
public:
	Tasks();
	~Tasks();

	void init();

	// Attributes
	int MAX_LEN = 10;			// default
	Items<string> to_do;
	Items<string> done;

	int get_MAX_LEN();
	void set_MAX_LEN(int i);

	//void print_info();
	//Tasks & operator=(Tasks & rp);

	bool mask_as_done(string task);

};


#endif