#ifndef sensors_h
#define sensors_h

#include "utils.h"

using namespace std;



#ifndef ITEM 		// defined (or not) in 'utils.h'
class Sensors
{
public:
	Sensors();
	Sensors(int max_len);
	~Sensors();

	// may want to add an extra level of abstraction:
	// component (const, dest, get, set...)

private:
	// Attributes

	int MAX_LEN = 10;				// default

	vector<float> s;				// vectors, so we can use historical
	vector<float> x;				// data to compute stuff like
	vector<float> y;				// good localization :)
	vector<float> th;
	
	vector<float> ir1;
	vector<float> ir2;
	vector<float> obst_dist;
	vector<bool> obst_found;

	vector<float> gyro_x;
	vector<float> gyro_y;
	vector<float> gyro_z;

	vector<float> acc_x;
	vector<float> acc_y;
	vector<float> acc_z;

	vector<float> comp_x;
	vector<float> comp_y;
	vector<float> comp_z;


	// Mutex (for critical sections, reading and writing attributes from different threads)
	mutex mtx_s;
	mutex mtx_x;
	mutex mtx_y;
	mutex mtx_th;

	mutex mtx_ir1;
	mutex mtx_ir2;
	mutex mtx_od;
	mutex mtx_of;

	mutex mtx_gyro;
	mutex mtx_acc;
	mutex mtx_comp;

public:
	// Getters
	int get_MAX_LEN();

	vector<float> get_s();
	vector<float> get_x();
	vector<float> get_y();
	vector<float> get_th();

	vector<float> get_ir1();
	vector<float> get_ir2();
	vector<float> get_obst_dist();
	vector<bool> get_obst_found();

	vector<float> get_gyro_x();
	vector<float> get_gyro_y();
	vector<float> get_gyro_z();

	vector<float> get_acc_x();
	vector<float> get_acc_y();
	vector<float> get_acc_z();

	vector<float> get_comp_x();
	vector<float> get_comp_y();
	vector<float> get_comp_z();


	// Setters
	void set_MAX_LEN(int i);

	void set_s(float f);
	void set_x(float f);
	void set_y(float f);
	void set_th(float f);

	void set_ir1(float f);
	void set_ir2(float f);
	void set_obst_dist(float f);
	void set_obst_found(bool b);

	void set_gyro_x(float f);
	void set_gyro_y(float f);
	void set_gyro_z(float f);

	void set_acc_x(float f);
	void set_acc_y(float f);
	void set_acc_z(float f);

	void set_comp_x(float f);
	void set_comp_y(float f);
	void set_comp_z(float f);
};









#else  // ITEM


class Sensors
{
public:
	Sensors();
	Sensors(int max_len);
	~Sensors();

	// may want to add an extra level of abstraction:
	// component (const, dest, get, set...)

private:
	// Attributes

	int MAX_LEN = 10;				// default

	Items<float> s("Servo");				// vectors, so we can use historical
	Items<float> x("X");				// data to compute stuff like
	Items<float> y("Y");				// good localization :)
	Items<float> th("Th");
	
	Items<float> ir1("IR1");
	Items<float> ir2("IR2");
	Items<float> obst_dist("Obstacle distance");
	Items<bool> obst_found("Obstacle found");

	Items<float> gyro_x("Gyro. X");
	Items<float> gyro_y("Gyro. Y");
	Items<float> gyro_z("Gyro. Z");

	Items<float> acc_x("Accel. X");
	Items<float> acc_y("Accel. Y");
	Items<float> acc_z("Accel. Z");

	Items<float> comp_x("Comp. X");
	Items<float> comp_y("Comp. Y");
	Items<float> comp_z("Comp. Z");


public:
	int get_MAX_LEN();
	void set_MAX_LEN(int i);

};


#endif // ITEM

#endif