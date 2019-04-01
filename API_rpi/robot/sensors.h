#ifndef sensors_h
#define sensors_h

#include "utils.h"
#include "item.h"

using namespace std;



class Sensors
{
public:
	Sensors();
	Sensors(int max_len);
	~Sensors();


	// Attributes
	int MAX_LEN = 10;				// default


	// we can use historical data to compute stuff like
	// good localization :)
	Items<float> s;
	Items<float> x;
	Items<float> y;
	Items<float> th;
	
	Items<float> ir1;
	Items<float> ir2;
	Items<float> obst_dist;
	Items<bool> obst_found;

	Items<float> gyro_x;
	Items<float> gyro_y;
	Items<float> gyro_z;

	Items<float> acc_x;
	Items<float> acc_y;
	Items<float> acc_z;

	Items<float> comp_x;
	Items<float> comp_y;
	Items<float> comp_z;

	Items<bool> obst_found_img;		// image params
	Items<int> cross;
	Items<float> th_t;
	Items<float> obst_dist_img;
	// may need more for the data of the ArUco codes!!!


	int get_MAX_LEN();
	void set_MAX_LEN(int i);

	void print_info();

	Sensors & operator=(Sensors & sens);

private:
	void init_items();

};

//#include "sensors.tpp"


#endif