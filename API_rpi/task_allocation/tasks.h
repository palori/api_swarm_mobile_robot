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
	Tasks();


	// Attributes
	int MAX_LEN = 10;			// default
	Items<auto> to_do;
	Items<bool> done;

	int get_MAX_LEN();
	void set_MAX_LEN(int i);

	//void print_info();
	//Tasks & operator=(Tasks & rp);

	void mask_as_done(string task);

private:
	void init_items();

};


#endif