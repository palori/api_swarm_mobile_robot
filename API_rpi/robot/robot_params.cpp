#include "robot_params.h"


Robot_params::Robot_params(){
	string hostname = "localhost";
	int id = -1;
	int max_len = MAX_LEN;
	int p_info = 7000;
	int p_image = 7002;
	int p_task = 7001;
	init(hostname, id, max_len, p_info, p_image, p_task);
}
Robot_params::~Robot_params(){}

Robot_params::Robot_params(string hostname,
						   int id,
						   int max_len,
						   int port_info){
	init(hostname, id, max_len, port_info);
}

Robot_params::Robot_params(string hostname,
						   int id,
						   int max_len,
						   int port_info,
						   int port_image,
						   int port_task){
	init(hostname, id, max_len, port_info, port_image, port_task);
}



void Robot_params::init(string hostname,
						int id,
						int max_len,
						int port_info){
	int p_image = 7002;
	int p_task = 7001;
	init(hostname, id, max_len, port_info, p_image, p_task);
}


void Robot_params::init(string hostname,
						int id,
						int max_len,
						int port_info,
						int port_image,
						int port_task){
	this->hostname.set_noMutex(hostname);
	this->id.set_noMutex(id);
	set_MAX_LEN(max_len);
	this->port_info.set_noMutex(port_info);
	this->port_image.set_noMutex(port_image);
	this->port_task.set_noMutex(port_task);
	this->ka.init(5.0, max_len);
}




int Robot_params::get_MAX_LEN(){return MAX_LEN;}


void Robot_params::set_MAX_LEN(int i){
	MAX_LEN = i;
	route.set_MAX_LEN(i);
	tasks.set_MAX_LEN(i);
}



void Robot_params::print_info(){

	cout << endl << endl << STAR_LINE << endl << endl;
	cout << "Robot info (last)" << endl << endl;

	cout << "  Name:              " << hostname.get_noMutex() << endl;
	cout << "  ID:              " << id.get_noMutex() << endl;
	cout << "  Port_info:         " << port_info.get_noMutex() << endl;
	/*cout << "  Port_info_robot_a: " << port_info_robot_a.get_noMutex() << endl;
	cout << "  Port_info_robot_b: " << port_info_robot_b.get_noMutex() << endl;*/
	cout << "  Port_image:        " << port_image.get_noMutex() << endl;
	cout << "  Port_task:         " << port_task.get_noMutex() << endl;
	cout << endl;
	cout << "  Data: " << endl;
	/*cout << "    Xw:   " << x.get_last_item_noMutex() << endl;
	cout << "    Yw:   " << y.get_last_item_noMutex() << endl;
	//cout << "    Zw:   " << z.get_last_item_noMutex() << endl;
	cout << "    Thw:  " << th.get_last_item_noMutex() << endl;
	cout << "    Task: " << tasks.get_last_item_noMutex() << "          (currently)" << endl;*/
	cout << "    KA:   " << ka.is_now_alive.get() << endl;
	cout << "    previous node:   " << previous_node.get_noMutex() << endl;
	cout << "    destiny node:   " << destiny_node.get_noMutex() << endl;

	string r = route.to_string_cs(";");
	if (r == "") r = "-";
	cout << "    route:   " << r << endl;

	r = tasks.to_do.to_string_cs(";");
	if (r == "") r = "-";
	cout << "    tasks to do:   " << r << endl;

	r = tasks.done.to_string_cs(";");
	if (r == "") r = "-";
	cout << "    tasks done:   " << r << endl;

	cout << endl << STAR_LINE << endl << endl;
	
}






Robot_params & Robot_params::operator=(Robot_params & rp){
	set_MAX_LEN(rp.get_MAX_LEN());
	//this->hostname.set(rp.hostname.get());
	this->hostname = rp.hostname;
	this->port_image = rp.port_image;
	this->port_task = rp.port_task;
	this->port_info = rp.port_info;
	//this->port_info_robot_a = rp.port_info_robot_a;
	//this->port_info_robot_b = rp.port_info_robot_b;

	//this->x = rp.x;
	//this->y = rp.y;
	//this->th = rp.th;

	this->tasks.to_do = rp.tasks.to_do;		// might want to include operator= in tasks
	this->tasks.done = rp.tasks.done;

	this->previous_node = rp.previous_node;
	this->destiny_node = rp.destiny_node;
	this->route = rp.route;

	// see what to do with 'ka'
	//this->ka = rp.ka;						// might want to include operator= in tasks

	this->id = rp.id;

	return *this;
}

