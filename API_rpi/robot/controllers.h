#ifndef controllers_h
#define controllers_h

#include "utils.h"

using namespace std;


class Controllers
{
public:
	Controllers();
	~Controllers();

	// may want to add an extra level of abstraction:
	// component (const, dest, get, set...)

private:
	// Attributes

	int MAX_LEN = 10;		// default

	vector<float> s;
	vector<float> th;


	// Mutex (for critical sections, reading and writing attributes from different threads)
	mutex mtx_s;
	mutex mtx_th;


public:
	// Getters
	int get_MAX_LEN();

	vector<float> get_s();
	vector<float> get_th();

	// Setters
	void set_MAX_LEN(int i);

	void set_s(float f);
	void set_th(float f);
	
};

#endif