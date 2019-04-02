
#include "robot.h"

Robot::Robot(){
	//params.hostname.set("localhost");

	Robot_params rp("test",1,2,3,4,5,10);
	this->params = rp;
}

Robot::~Robot(){}


// probably never used...
Robot::Robot(string hostname){ // params from other 2 robots (port and hostname)
	//Subscriber subs_robot_a();
	//Subscriber subs_robot_b();
	Robot_params rp(hostname,5000,20);
	this->params = rp;
	//this->params(hostname,1,2,3,4,5,10);
	//params.hostname.set(hostname);
	//cout << "hostname: " << this->params.hostname.get_last_item() << endl;

}

// always use this one
Robot::Robot(string hostname, int port_image, int port_task, int port_info, int port_info_robot_a, int port_info_robot_b){
	Robot_params rp(hostname, port_image, port_task, port_info, port_info_robot_a, port_info_robot_b,20);
	this->params = rp;
}




void Robot::serial(){

	// may want to send some initialization
	// params like: wheel diameter, dist btw wheels, gearbox ratio...

	string msg = "", data = "";

	serial_comm.serial_open();
	while(true){
		cout << "reading serial..." << endl;
		data = serial_comm.serial_read();
		decode_sensors(data, sensors);
		// save data
		// need to be decoded to be used (can be done here or in localization...)
		// maybe easier to modify 'comm_rpi_1.cpp' and do it there
		// then update 'robot_params'

		cout << "writing serial..." << endl;
		// update msg
		msg = image_data.get();		// probably there are other cases, now we want to test this
		serial_comm.serial_write(msg);
	}
	serial_comm.serial_close();
}


void Robot::listen_image_process(){
	string data = "";
	while(true){
		cout << "listenning to image processing..." << endl;
		data = subs_image_data.listen();	// blocking call
		// decode data into params to use for localization and send to TSY

		image_data.set(data);
	}
}

void Robot::listen_robot_a(){
	string info;
	while(true){
		cout << "listenning to robot " << robot_a.hostname.get() << "..." << endl;
		info = subs_robot_a.listen();		// blocking call
		// decode info message

		// save data in 'robot_a'

	}

}


void Robot::listen_robot_b(){
	string info;
	while(true){
		cout << "listenning to robot " << robot_b.hostname.get() << "..." << endl;
		info = subs_robot_b.listen();		// blocking call
		// decode info message

		// save data in 'robot_b'

	}

}






void Robot::run(){
	//thread task_planning(task_planner_run);	// may be in the same thread for now
	thread thread_serial(&Robot::serial, this);
	thread thread_image(&Robot::listen_image_process, this);
	thread thread_robot_a(&Robot::listen_robot_a, this); // maybe loop for listenning to other robots??
	thread thread_robot_b(&Robot::listen_robot_b, this);



	while(true){
		// localization
		// task planner
		// path planning -> if there is one

	}

	thread_serial.join();	// it will never reach this point, but good to have
	thread_image.join();
	thread_robot_a.join();
	thread_robot_b.join();
}
