#include "robot_params.h"


Robot_params::Robot_params(){
	init_items();
	set_MAX_LEN(MAX_LEN);
}
Robot_params::~Robot_params(){}

Robot_params::Robot_params(string hostname, int port_info, int max_len){
	init_items();
	this->hostname.set_noMutex(hostname);
	this->port_info.set_noMutex(port_info);
	set_MAX_LEN(max_len);
}

Robot_params::Robot_params(string hostname, int port_image, int port_task, int port_info, int port_info_robot_a, int port_info_robot_b, int max_len){
	init_items();
	this->hostname.set_noMutex(hostname);
	this->port_image.set_noMutex(port_image);
	this->port_task.set_noMutex(port_task);
	this->port_info.set_noMutex(port_info);
	this->port_info_robot_a.set_noMutex(port_info_robot_a);
	this->port_info_robot_b.set_noMutex(port_info_robot_b);
	set_MAX_LEN(max_len);
}

int Robot_params::get_MAX_LEN(){return MAX_LEN;}


void Robot_params::set_MAX_LEN(int i){
	MAX_LEN = i;
	x.set_MAX_LEN(i);
	y.set_MAX_LEN(i);
	//z.set_MAX_LEN(i);
	th.set_MAX_LEN(i);
	tasks.set_MAX_LEN(i);
}



void Robot_params::print_info(){

	cout << endl << endl << STAR_LINE << endl << endl;
	cout << "Robot info (last)" << endl << endl;

	cout << "  Name:              " << hostname.get_noMutex() << endl;
	cout << "  Port_image:        " << port_image.get_noMutex() << endl;
	cout << "  Port_task:         " << port_task.get_noMutex() << endl;
	cout << "  Port_info:         " << port_info.get_noMutex() << endl;
	cout << "  Port_info_robot_a: " << port_info_robot_a.get_noMutex() << endl;
	cout << "  Port_info_robot_b: " << port_info_robot_b.get_noMutex() << endl;
	cout << endl;
	cout << "  Data: " << endl;
	cout << "    Xw:   " << x.get_last_item_noMutex() << endl;
	cout << "    Yw:   " << y.get_last_item_noMutex() << endl;
	//cout << "    Zw:   " << z.get_last_item_noMutex() << endl;
	cout << "    Thw:  " << th.get_last_item_noMutex() << endl;
	cout << "    Task: " << tasks.get_last_item_noMutex() << "          (currently)" << endl;

	cout << endl << STAR_LINE << endl << endl;
	
}





void Robot_params::init_items(){
	this->hostname.set_noMutex("localhost");
	this->port_image.set_noMutex(7002);
	this->port_task.set_noMutex(7001);
	this->port_info.set_noMutex(7000);
	this->port_info_robot_a.set_noMutex(7000);
	this->port_info_robot_b.set_noMutex(7000);

	x.set_name("Xw");		// maybe just the current position???
	y.set_name("Yw");
	//z.set_name("Z world");
	th.set_name("Tw");
	tasks.set_name("Tasks");	// list of completed tasks by this robot (the last one is the current, working on)
	
	this->x.add_item_noMutex(0.0);
	this->y.add_item_noMutex(0.0);
	//this->z.add_item_noMutex(0.0);
	this->th.add_item_noMutex(0.0);
	this->tasks.add_item_noMutex(0);
}





Robot_params & Robot_params::operator=(Robot_params & rp){
	set_MAX_LEN(rp.get_MAX_LEN());
	//this->hostname.set(rp.hostname.get());
	this->hostname = rp.hostname;
	this->port_image = rp.port_image;
	this->port_task = rp.port_task;
	this->port_info = rp.port_info;
	this->port_info_robot_a = rp.port_info_robot_a;
	this->port_info_robot_b = rp.port_info_robot_b;

	this->x = rp.x;
	this->y = rp.y;
	this->th = rp.th;
	this->tasks = rp.tasks;

	return *this;
}