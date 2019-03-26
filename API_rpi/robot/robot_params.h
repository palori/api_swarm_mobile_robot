#ifndef robot_params_h
#define robot_params_h

#include "utils.h"

using namespace std;


class Robot_params
{
public:
	Robot_params();
	~Robot_params();
	Robot_params(string hostname, int port_info);
	Robot_params(string hostname, int port_image, int port_task, int port_info, int port_info_robot_a, int port_info_robot_b);

	// may want to add an extra level of abstraction:
	// component (const, dest, get, set...)


private:
	// Attributes
	int MAX_LEN = 10;			// default

	int id;
	string hostname;
	int port_image;
	int port_task;
	int port_info;
	int port_info_robot_a;
	int port_info_robot_b;
	vector<float> x;
	vector<float> y;
	vector<float> z;
	vector<float> th;
	vector<int> tasks;			// maybe change to task object
	


	// Mutex (for critical sections, reading and writing attributes from different threads)
	mutex mtx_x;
	mutex mtx_y;
	mutex mtx_z;
	mutex mtx_th;
	mutex mtx_task;

public:
	// Getters
	int get_MAX_LEN();
	string get_hostname();
	int get_port_image();
	int get_port_task();
	int get_port_info();
	int get_port_info_robot_a();
	int get_port_info_robot_b();
	vector<float> get_x();
	vector<float> get_y();
	vector<float> get_z();
	vector<float> get_th();
	vector<int> get_tasks();

	// Setters
	void set_MAX_LEN(int i);
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
	void set_task(int i);
};

#endif