#ifndef sensors_h
#define sensors_h

#include "utils.h"

using namespace std;


class Sensors
{
public:
	Sensors();
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

#endif