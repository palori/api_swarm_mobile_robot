#ifndef SENSORS_1_H
#define SENSORS_H

#include <iostream>
#include <mutex>

//#include "utils.h"

using namespace std;


class Sensors
{
public:
	Sensors();
	~Sensors();

private:
	// Attributes
	float s;				// might want to be vectors so we can use
	float x;				// historical data to compute stuff like
	float y;				// good localization :)
	float th;
	
	float ir1;
	float ir2;
	float obst_dist;
	bool obst_found;

	float gyro[3];			// then those will be matrices (see how to do that)
	float acc[3];
	float comp[3];


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
	float get_s();
	float get_x();
	float get_y();
	float get_th();

	float get_ir1();
	float get_ir2();
	float get_obst_dist();
	bool get_obst_found();

	/*float get_gyro();				// maybe with pointers???
	float get_acc();
	float get_comp();*/


	// Setters
	void set_s(float f);
	void set_x(float f);
	void set_y(float f);
	void set_th(float f);

	void set_ir1(float f);
	void set_ir2(float f);
	void set_obst_dist(float f);
	void set_obst_found(bool b);

	void set_gyro(float f1, float f2, float f3);
	void set_acc(float f1, float f2, float f3);
	void set_comp(float f1, float f2, float f3);
};

#endif