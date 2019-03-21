#ifndef ROBOT_PARAMS_H
#define ROBOT_PARAMS_H

#include <iostream>
#include <vector>
#include <mutex>

//#include "utils.h"

using namespace std;


class Robot_params
{
public:
	Robot_params();
	~Robot_params();
	Robot_params(string hostname, int port_info);
	Robot_params(string hostname, int port_image, int port_task, int port_info, int port_info_robot_a, int port_info_robot_b);

private:
	// Attributes
	int id;
	string hostname;
	int port_image;
	int port_task;
	int port_info;
	int port_info_robot_a;
	int port_info_robot_b;
	float x;
	float y;
	float z;
	float th;
	string task;					// maybe int if taks is enum???
	vector<string> tasks_done;


	// Mutex (for critical sections, reading and writing attributes from different threads)
	mutex mtx_x;
	mutex mtx_y;
	mutex mtx_z;
	mutex mtx_th;
	mutex mtx_task;

public:
	// Getters
	string get_hostname();
	int get_port_image();
	int get_port_task();
	int get_port_info();
	int get_port_info_robot_a();
	int get_port_info_robot_b();
	float get_x();
	float get_y();
	float get_z();
	float get_th();
	string get_task();
	// tasks_done() ???

	// Setters
	void set_hostname(string s);
	void set_port_image(int i);
	void set_port_task(int i);
	void set_port_info(int i);
	void set_port_info_robot_a(int i);
	void set_port_info_robot_b(int i);
	void set_x(float f);
	void set_y(float f);
	void set_z(float f);
	void set_th(float f);
	void set_task(string s);
};

#endif