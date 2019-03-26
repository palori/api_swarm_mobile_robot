#include "robot_params.h"

Robot_params::Robot_params(){}
Robot_params::~Robot_params(){}

Robot_params::Robot_params(string hostname, int port_info){
	set_hostname(hostname);
	set_port_info(port_info);
}

Robot_params::Robot_params(string hostname, int port_image, int port_task, int port_info, int port_info_robot_a, int port_info_robot_b){
	set_hostname(hostname);
	set_port_image(port_image);
	set_port_task(port_task);
	set_port_info(port_info);
	set_port_info_robot_a(port_info_robot_a);
	set_port_info_robot_b(port_info_robot_b);
}


// Getters
int Robot_params::get_MAX_LEN(){return MAX_LEN;}
string Robot_params::get_hostname(){return hostname;}
int Robot_params::get_port_image(){return port_image;}
int Robot_params::get_port_task(){return port_task;}
int Robot_params::get_port_info(){return port_info;}
int Robot_params::get_port_info_robot_a(){return port_info_robot_a;}
int Robot_params::get_port_info_robot_b(){return port_info_robot_b;}

vector<float> Robot_params::get_x(){
	mtx_x.lock();
	vector<float> xm = x;
	mtx_x.unlock();
	return xm;
}
vector<float> Robot_params::get_y(){
	mtx_y.lock();
	vector<float> ym = y;
	mtx_y.unlock();
	return ym;
}
vector<float> Robot_params::get_z(){
	mtx_z.lock();
	vector<float> zm = z;
	mtx_z.unlock();
	return zm;
}
vector<float> Robot_params::get_th(){
	mtx_th.lock();
	vector<float> thm = th;
	mtx_th.unlock();
	return thm;
}
vector<int> Robot_params::get_tasks(){
	mtx_task.lock();
	vector<int>taskm = task;
	mtx_task.unlock();
	return taskm;
}
// tasks_done() ???


// Setters
void Robot_params::set_MAX_LEN(int i){MAX_LEN = i;}
void Robot_params::set_hostname(string s){hostname = s;}
void Robot_params::set_port_image(int i){port_image = i;}
void Robot_params::set_port_task(int i){port_task = i;}
void Robot_params::set_port_info(int i){port_info = i;}
void Robot_params::set_port_info_robot_a(int i){port_info_robot_a = i;}
void Robot_params::set_port_info_robot_b(int i){port_info_robot_b = i;}

void Robot_params::set_x(float f){
	mtx_x.lock();
	x = add2vector(x, f, get_MAX_LEN());
	mtx_x.unlock();
}
void Robot_params::set_y(float f){
	mtx_y.lock();
	y = add2vector(y, f, get_MAX_LEN());
	mtx_y.unlock();
}
void Robot_params::set_z(float f){
	mtx_z.lock();
	z = add2vector(z, f, get_MAX_LEN());
	mtx_z.unlock();
}
void Robot_params::set_th(float f){
	mtx_th.lock();
	th = add2vector(th, f, get_MAX_LEN());
	mtx_th.unlock();
}
void Robot_params::set_task(int i){
	mtx_task.lock();
	tasks = add2vector(tasks, i, get_MAX_LEN());
	mtx_task.unlock();
}
