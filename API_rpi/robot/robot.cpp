
#include "robot.h"

Robot::Robot(int id){

}

Robot::~Robot(){}

Robot::Robot(int id, string hostname){ // params from other 2 robots (port and hostname)
	Subscriber subs_robot_a();
	Subscriber subs_robot_b();
	Robot_params params(hostname);
}



// Getters
string get_image_data(){
	mtx_image_data.lock();
	string image_data_m = image_data;
	mtx_image_data.unlock();
	return image_data_m;
}

// Setters
void set_image_data(string s){
	mtx_image_data.lock();
	image_data = s;
	mtx_image_data.unlock();
}





void Robot::serial(){

	// may want to send some initialization
	// params like: wheel diameter, dist btw wheels, gearbox ratio...

	string msg = "", data = "";

	serial_comm.serial_open();
	while(true){
		cout << "reading serial..." << endl;
		data = serial_comm.serial_read();
		msg2sensorData(data, & sensors);
		// save data
		// need to be decoded to be used (can be done here or in localization...)
		// maybe easier to modify 'comm_rpi_1.cpp' and do it there
		// then update 'robot_params'

		cout << "writing serial..." << endl;
		// update msg
		msg = get_image_data();		// probably there are other cases, now we want to test this
		cr.serial_write(msg);
	}
	serial_comm.serial_close();
}


void Robot::listen_image_process(){
	string data = "";
	while(true){
		cout << "listenning to image processing..." << endl;
		data = subs_image_data.listen();	// blocking call
		// decode data into params to use for localization and send to TSY

		set_image_data(data);
	}
}

void Robot::listen_robot_a(int id){
	string info;
	while(true){
		cout << "listenning to robot " << id << "..." << endl;
		info = subs_robot_a.listen();		// blocking call
		// decode info message

		// save data in 'robot_a'

	}

}


void Robot::listen_robot_b(int id){
	string info;
	while(true){
		cout << "listenning to robot " << id << "..." << endl;
		info = subs_robot_b.listen();		// blocking call
		// decode info message

		// save data in 'robot_b'

	}

}






void Robot::run(){
	//thread task_planning(task_planner_run);	// may be in the same thread for now
	thread thread_serial(serial);
	thread thread_image(listen_image_process);
	thread thread_robot_a(listen_robot_a, 1, subs_robot_a); // maybe loop for listenning to other robots??
	thread thread_robot_b(listen_robot_b, 2, subs_robot_b);



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
