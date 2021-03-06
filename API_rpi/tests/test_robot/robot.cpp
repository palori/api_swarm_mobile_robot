
#include "robot.h"

Robot::Robot(){
	//params.hostname.set("localhost");

	this->hostname_master.set("localhost");

	Robot_params rp("test",1,2,3,4,5,10);
	this->params = rp;

	/*Publisher pub(this->params.port_image.get());
	pub.disconnect();
	pub_image_task = pub;
	pub_image_task.setup();

	Publisher pub1(this->params.port_info.get());
	pub1.disconnect();
	pub_robot_info = pub1;
	pub_robot_info.setup();*/

	//pub_image_task(this->params.port_image.get());
	//pub_robot_info(this->params.port_info.get());

	pub_image_task.set_port(this->params.port_image.get());
	pub_image_task.setup();

	pub_robot_info.set_port(this->params.port_image.get());
	pub_robot_info.setup();

	run_mission.set(false);
}

Robot::~Robot(){}


/*/ probably never used...
Robot::Robot(string hostname){ // params from other 2 robots (port and hostname)
	//Subscriber subs_robot_a();
	//Subscriber subs_robot_b();
	Robot_params rp(hostname,5000,20);
	this->params = rp;
	//this->params(hostname,1,2,3,4,5,10);
	//params.hostname.set(hostname);
	//cout << "hostname: " << this->params.hostname.get_last_item() << endl;

}*/

// always use this one
Robot::Robot(string hostname_master, string hostname, string hostname_a, string hostname_b, int max_len, int port_image, int port_task, int port_info, int port_info_robot_a, int port_info_robot_b){
	//cout << "start robot constructor" << endl;
	this->hostname_master.set(hostname_master);

	Robot_params rp(hostname, port_image, port_task, port_info, port_info_robot_a, port_info_robot_b,max_len);
	this->params = rp;
	//cout << "this params done" << endl;
	
	Robot_params rp_a(hostname_a, port_info_robot_a, max_len);
	this->robot_a = rp_a;

	Robot_params rp_b(hostname_b, port_info_robot_b, max_len);
	this->robot_b = rp_b;



	/*
	Publisher pub(this->params.port_image.get());
	pub.disconnect();
	pub_image_task = pub;
	pub_image_task.setup();

	Publisher pub1(this->params.port_info.get());
	pub1.disconnect();
	pub_robot_info = pub1;
	pub_robot_info.setup();
	*/

	//pub_image_task(this->params.port_image.get());
	//pub_robot_info(this->params.port_info.get());

	pub_image_task.set_port(this->params.port_task.get());
	pub_image_task.setup();

	pub_robot_info.set_port(this->params.port_info.get());
	pub_robot_info.setup();

	run_mission.set(false);
	
}




void Robot::serial(){

	// may want to send some initialization
	// params like: wheel diameter, dist btw wheels, gearbox ratio...

	string msg = "", old_msg = "1", data = "";
	string msg_master = "", old_msg_master = "1";
	string update_pose = "", old_update_pose = "1";
	//msg = "@a=15,b=1,fwd=5,v=0.6$";				// delete after testing

	serial_comm.serial_open();
	int count = 0;
	while(true){

		if(run_mission.get()){

			//---------------WRITING SERIAL---------------//
			count ++;
			//int millis_sleep = 2000;
			//this_thread::sleep_for(chrono::milliseconds(millis_sleep));

			// update msg
			msg = image_data.get();		// probably there are other cases, now we want to test this
			//cout << "(abans if msg =" << msg << endl;
			if (msg != old_msg) {
				//cout << "writing serial..." << endl;
				//cout << "msg =" << msg << endl;
				serial_comm.serial_write(msg);
				old_msg = msg;
				//cout << "serial message: " << msg << endl;
			}

			msg_master = master_data.get();
			if (msg_master != old_msg_master) {
				serial_comm.serial_write(msg_master);
				old_msg_master = msg_master;
				//this_thread::sleep_for(chrono::milliseconds(500));
			}

			
			
			//int millis_sleep = 50;
			//this_thread::sleep_for(chrono::milliseconds(millis_sleep));


			//---------------READING SERIAL---------------//
			// Commented for testing in Pau's pc
			//cout << "reading serial..." << endl;
			data = serial_comm.serial_read();
			//cout << "\n*******\nserial data: " << data << "\n*******\n";
			decode_sensors(data, sensors);
			
			//sensors.print_info();
			// save data
			// need to be decoded to be used (can be done here or in localization...)
			// maybe easier to modify 'comm_rpi_1.cpp' and do it there
			// then update 'robot_params'
			//
			int millis_sleep = 10;
			this_thread::sleep_for(chrono::milliseconds(millis_sleep));
			//cout << "serial------------\n";
			
			
			
		}
		else{
			if (msg != old_msg) {
				// Maybe send to stop --> ask Andrija
				cout << "Should send STOP to Teensy, but not implemented yet!" << endl;
				old_msg = msg;
			}
		}
		
	}
	serial_comm.serial_close();
}


void Robot::listen_image_process(){
	Subscriber subs_image_data(params.port_image.get(), "localhost");//params.hostname.get());
	string data = "", new_target;
	while(true){
		//cout << "listenning to image processing..." << endl;
		data = subs_image_data.listen();	// blocking call
		image_data.set(data);
		//cout << "image data to decode: " << data << endl;
		// decode data into params to use for localization and send to TSY
		decode_image(data, sensors);
	}
}

void Robot::listen_robot_a(){
	Subscriber subs_robot_a(robot_a.port_info.get(), robot_a.hostname.get());
	string info;
	cout << "listenning to robot " << robot_a.hostname.get() << "..." << endl;
	while(true){
		info = subs_robot_a.listen();		// blocking call
		// decode info message

		// save data in 'robot_a'

	}

}


void Robot::listen_robot_b(){
	Subscriber subs_robot_b(robot_b.port_info.get(), robot_b.hostname.get());
	string info;
	cout << "listenning to robot " << robot_b.hostname.get() << "..." << endl;
	while(true){
		info = subs_robot_b.listen();		// blocking call
		// decode info message

		// save data in 'robot_b'

	}

}

void Robot::listen_master(){
	string master = this->hostname_master.get();
	Subscriber subs_master(8001, master);
	string msg;
	int action = -1;
	cout << "listenning to master '" << master << "'..." << endl;

	run_mission.set(true);
	
	while(true){
		msg = subs_master.listen();		// blocking call

		// decode info message
		action = decode_master_commands(msg, params.hostname.get());
		cout << "action = " << action << endl;

		if (action != -1) master_data.set(msg);		// maybe need to change the name to the param (image_data)

		if (action == START) run_mission.set(true);
		else if (action == PAUSE) run_mission.set(false);
		else if (action >= IDLE && action <= ARUCO) pub_image_task.publish(msg);//this->params.tasks.add_item(action);
		action = -1;
	}

}








void Robot::run(){
	//thread task_planning(task_planner_run);	// may be in the same thread for now
	
	thread thread_serial(&Robot::serial, this);
	thread thread_image(&Robot::listen_image_process, this);
	thread thread_robot_a(&Robot::listen_robot_a, this); // maybe loop for listenning to other robots??
	thread thread_robot_b(&Robot::listen_robot_b, this);
	thread thread_master(&Robot::listen_master, this);



	// only for testing -> always follow line
	//this->params.tasks.add_item(LINE);			// could be also added if master sends @a=19,b=1$
	//int task = -1, old_task = -1;
	string msg_task = "";
	//send_task();

	while(true){

		if(run_mission.get()){
			// localization
			// task planner
			// path planning -> if there is one
			//cout << "----------run\n";
			sensors.print_info();
			int millis_sleep = 100;
			this_thread::sleep_for(chrono::milliseconds(millis_sleep));
			
			//send_task();
			/*task = this->params.tasks.get_last_item();
			if(task != old_task){
				msg_task = encode_task(task);
				pub_image_task.publish(msg_task);
			}*/

		}

	}

	thread_serial.join();	// it will never reach this point, but good to have
	thread_image.join();
	thread_robot_a.join();
	thread_robot_b.join();
	thread_master.join();
}

