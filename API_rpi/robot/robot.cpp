
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

	string msg_master = "", old_msg_master = "1";
	string update_pose = "", old_update_pose = "";
	string msg_image = "", old_msg_image = "";
	string msg_drive = "", old_msg_drive = "";
	string data = "";

	serial_comm.serial_open();
	int count = 0, millis = 50;
	while(true){

		//if(run_mission.get()){

			//---------------WRITING SERIAL---------------//
			count ++;
			
			// update msg
			msg_master = master_data.get();
			update_pose = init_pose.get();
			msg_image = image_data.get();
			msg_drive = drive_command.get();

			if (msg_master != old_msg_master) {
				serial_comm.serial_write(msg_master);
				cout << "master msg: " << msg_master << endl;
				old_msg_master = msg_master;
				this_thread::sleep_for(chrono::milliseconds(millis));
			}
			else if (update_pose != old_update_pose) {
				serial_comm.serial_write(update_pose);
				cout << "init_pose: " << update_pose << endl;
				old_update_pose = update_pose;
				this_thread::sleep_for(chrono::milliseconds(millis));
			}
			else if (msg_image != old_msg_image) {
				serial_comm.serial_write(msg_image);
				cout << "image msg: " << msg_image << endl;
				old_msg_image = msg_image;
				this_thread::sleep_for(chrono::milliseconds(millis));
			}
			else if (msg_drive != old_msg_drive) {
				serial_comm.serial_write(msg_drive);
				cout << "drive msg: " << msg_drive << endl;
				old_msg_drive = msg_drive;
				this_thread::sleep_for(chrono::milliseconds(millis));
			}
			
			
			//int millis_sleep = 50;
			//this_thread::sleep_for(chrono::milliseconds(millis_sleep));
		/*}
		else{
			if (msg_master != old_msg_master) {
				// Maybe send to stop --> ask Andrija
				cout << "Should send STOP to Teensy, but not implemented yet!" << endl;
				old_msg_master = msg_master;
			}
		}*/

		//---------------READING SERIAL---------------//
		// Commented for testing in Pau's pc
		//cout << "reading serial..." << endl;
		data = serial_comm.serial_read();
		//cout << "\n*******\nserial data: " << data << "\n*******\n";
		decode_sensors(data, sensors);
		
		/*if (count>= 1000){		// DELETE! only for debuging
			cout << endl << "### new task to Teensy: " << msg << " ###" << endl << endl;
			sensors.print_info();
			count = 0;
		}*/
		//sensors.print_info();
		// save data
		// need to be decoded to be used (can be done here or in localization...)
		// maybe easier to modify 'comm_rpi_1.cpp' and do it there
		// then update 'robot_params'
		//
		int millis_sleep = 10;
		//this_thread::sleep_for(chrono::milliseconds(millis_sleep));
		
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
	while(true){
		msg = subs_master.listen();		// blocking call
		cout << "Master says: " << msg << endl;
		// decode info message
		action = decode_master_commands(msg, params.hostname.get());
		cout << "action = " << action << endl;

		if (action != -1) master_data.set(msg);		// maybe need to change the name to the param (image_data)

		if (action == START) run_mission.set(true);
		else if (action == PAUSE) run_mission.set(false);
		action = -1;
	}

}


/*
void Robot::send_task(){//Publisher pub_image_task){
	int task = this->params.tasks.get_last_item();
	// maybe only send if it is different than the previous one
	string msg = encode_task();
	pub_image_task.publish(msg);
}
*/





void Robot::run(){
	//thread task_planning(task_planner_run);	// may be in the same thread for now
	
	thread thread_serial(&Robot::serial, this);
	thread thread_image(&Robot::listen_image_process, this);
	thread thread_robot_a(&Robot::listen_robot_a, this); // maybe loop for listenning to other robots??
	thread thread_robot_b(&Robot::listen_robot_b, this);
	thread thread_master(&Robot::listen_master, this);

	//this_thread::sleep_for(chrono::milliseconds(100));
	sensors.print_info();

	// only for testing -> always follow line
	//this->params.tasks.add_item(LINE);			// could be also added if master sends @a=19,b=1$
	//int task = -1, old_task = -1;
	//string msg_task = "";
	//send_task();
	bool test_nav = true;

	while(true){

		if(run_mission.get()){
			// localization
			// task planner
			// path planning -> if there is one
			
			
			int millis_sleep = 500;
			//this_thread::sleep_for(chrono::milliseconds(millis_sleep));
			
			//send_task();
			/*
			task = this->params.tasks.get_last_item();
			if(task != old_task){
				msg_task = encode_task(task);
				pub_image_task.publish(msg_task,MIDDLE);
			}*/

			if (test_nav) {
				cout << "navigate test" << endl;
				navigate_test();
				test_nav=false;
			}
		}

	}

	thread_serial.join();	// it will never reach this point, but good to have
	thread_image.join();
	thread_robot_a.join();
	thread_robot_b.join();
	thread_master.join();
}

void Robot::wait2drive(){
	this_thread::sleep_for(chrono::milliseconds(1000));
			
	while(!sensors.newCommand.get_last_item()){
		this_thread::sleep_for(chrono::milliseconds(100));
	}
}

void Robot::navigate_test(){//Graph* map){
	//sensors.print_info();
	init_pose.set("@i=20,x0=0.0,y0=3.0,th0=0.0$");
	this_thread::sleep_for(chrono::milliseconds(1000));
	sensors.print_info();

	
	Graph* map = map_mission0();//map_test();
	map->reset_nodes();
	Dijkstra dijkstra(map);
	dijkstra.find_route("a", "b");

	Edge* edge;
	Node* start;
	Node* end;
	float th_w;
	bool wait;

	float threshold_xy = 0.1, threshold_th; // to say that the robot got to the final place

	for (int i = 1; i < dijkstra.route.size(); ++i)
	{
		start = dijkstra.route.at(i-1);
		end = dijkstra.route.at(i);
		edge = map->find_edge(start, end);
		th_w = edge->get_th_w(start);
		//if (th_w != NULL){
			//set msg to send to tsy
			
			
			while(!sensors.newCommand.get_last_item()){
				this_thread::sleep_for(chrono::milliseconds(100));
			}
			cout << "-------------turn\n";
			float trn = th_w - sensors.th.get_last_item();
			string msg_task = encode_task(IDLE,NO_LINE);
			pub_image_task.publish(msg_task);
			string msg = "@i=21,a=16,b=1,v=" + to_string(edge->vel) + ",trn=" + to_string(trn) + "$";
			drive_command.set(msg); 
			wait2drive();

			cout << "-------------fwd\n";
			sensors.print_info();
			msg = "@i=22,a=";
			if (edge->line == 0) msg += to_string(FWD);
			else {
				msg_task = encode_task(LINE,edge->line);
				pub_image_task.publish(msg_task);

				msg += to_string(FOLLOW);
			}
			msg += ",b=1,v=" + to_string(edge->vel) + ",fwd=" + to_string(edge->distance) + "$";
			drive_command.set(msg); 
			wait2drive();
			sensors.print_info();
			cout << "-------------recovery\n";

			msg_task = encode_task(IDLE,NO_LINE);
			pub_image_task.publish(msg_task);

			float delta_x = end->x - sensors.x.get_last_item();
			float delta_y = end->y - sensors.y.get_last_item();
			float distance = sqrt(delta_x*delta_x + delta_y*delta_y);
			trn = atan2(delta_y, delta_x);

			msg = "@i=23,a=16,b=1,v=" + to_string(edge->vel) + ",trn=" + to_string(trn) + "$";
			drive_command.set(msg); 
			wait2drive();

			msg = "@i=24,a=15,b=1,v=" + to_string(edge->vel) + ",fwd=" + to_string(distance) + "$";
			drive_command.set(msg); 
			wait2drive();

			//this_thread::sleep_for(chrono::milliseconds(10000));
			/* still to test
			wait = true;
			while (wait){
				cout << "sleep" << endl;
				//int millis_sleep = 500;
				//this_thread::sleep_for(chrono::milliseconds(millis_sleep));

				
				if(
					(sensors.x.get_last_item() > (end->x - threshold) ||
						sensors.x.get_last_item() < (end->x + threshold)) &&
					(sensors.y.get_last_item() > (end->y - threshold) ||
						sensors.y.get_last_item() < (end->y + threshold))
					){
					wait = false:
					cout << "reach target position" << endl;
				}
				
			}*/
		//}
		//else cout << "could not find the th_w" << endl;
	}
	
}
