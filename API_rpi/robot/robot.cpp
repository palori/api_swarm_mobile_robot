
#include "robot.h"

Robot::Robot(){
	//params.hostname.set("localhost");

	this->master.hostname.set("localhost");

	Robot_params rp("test",1,2,3,4,5);
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

	debug.set(false);
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
Robot::Robot(string hostname_master, 
			 string hostname, 
			 string hostname_a, 
			 string hostname_b, 
			 int id,
			 int max_len, 
			 int port_image, 
			 int port_task, 
			 int port_info, 
			 int port_info_robot_a, 
			 int port_info_robot_b,
			 int port_info_master,
			 int id_robot_a,
			 int id_robot_b,
			 int id_master){
	/* prints for debuging
	cout << "Robot input data:\n";
	cout << "  - hostname_master = " << hostname_master << endl;
	cout << "  - hostname = " << hostname << endl;
	cout << "  - hostname_a = " << hostname_a << endl;
	cout << "  - hostname_b = " << hostname_b << endl;
	cout << "  - id = " << id << endl;
	cout << "  - max_len = " << max_len << endl;
	cout << "  - port_image = " << port_image << endl;
	cout << "  - port_task = " << port_task << endl;
	cout << "  - port_info = " << port_info << endl;
	cout << "  - port_info_robot_a = " << port_info_robot_a << endl;
	cout << "  - port_info_robot_b = " << port_info_robot_b << endl;
	cout << "  - port_info_master = " << port_info_master << endl;
	cout << "  - id_robot_a = " << id_robot_a << endl;
	cout << "  - id_robot_b = " << id_robot_b << endl;
	cout << "  - id_master = " << id_master << endl;
	cout << endl << endl;*/

	//cout << "start robot constructor" << endl;
	this->master.init(hostname_master, id_master, max_len, port_info_master);

	this->params.init(hostname, id, max_len, port_info, port_image, port_task);
	//cout << "this params done" << endl;
	
	this->robot_a.init(hostname_a, id_robot_a, max_len, port_info_robot_a);

	this->robot_b.init(hostname_b, id_robot_b, max_len, port_info_robot_b);


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

	//pub_image_task(this->params.port_info_image.get());
	//pub_robot_info(this->params.port_info.get());

	pub_image_task.set_port(this->params.port_task.get());
	pub_image_task.setup();

	pub_robot_info.set_port(this->params.port_info.get());
	pub_robot_info.setup();

	bully.init(id, 5.0);

	run_mission.set(false);

	debug.set(false);
	
}




void Robot::serial(){

	// may want to send some initialization
	// params like: wheel diameter, dist btw wheels, gearbox ratio...

	string msg_master = "", old_msg_master = "1";
	string update_pose = "", old_update_pose = "";
	string msg_image = "", old_msg_image = "";
	string msg_drive = "", old_msg_drive = "";
	string data = "";

	string QWERT = "qwert";
	master_data.set(QWERT);
	init_pose.set(QWERT);
	drive_command.set(QWERT);
	image_data.set(QWERT);

	serial_comm.serial_open();
	int count = 0, millis = 50;
	while(true){

		//if(run_mission.get()){

			//---------------WRITING SERIAL---------------//
			count ++;
			
			// update msg
			msg_master = master_data.get();
			update_pose = init_pose.get();
			msg_drive = drive_command.get();
			msg_image = image_data.get();

			if (msg_master != QWERT) {
				serial_comm.serial_write(msg_master);
				//cout << "master msg: " << msg_master << endl;
				master_data.set(QWERT);
			}
			else if (update_pose != QWERT) {
				serial_comm.serial_write(update_pose);
				cout << "init_pose: " << update_pose << endl;
				init_pose.set(QWERT);
			}
			else if (msg_drive != QWERT) {
				serial_comm.serial_write(msg_drive);
				//cout << "drive msg: " << msg_drive << endl;
				drive_command.set(QWERT);
			}
			else if (msg_image != QWERT) {
				serial_comm.serial_write(msg_image);
				//cout << "image msg: " << msg_image << endl;
				image_data.set(QWERT);
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

		bool print_position = false;
		if (print_position){	// some other times we  want to see the position, for debuging
			cout << "X: " << sensors.x.get() << "    Y: " << sensors.y.get_last_item() << "    th: " << sensors.th.get_last_item() << endl;
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
		decode_robot_params(info, robot_a);
		robot_a.ka.times.add_item(chrono::system_clock::now());

		check_le_messages(info);	// leader election message
	}

}


void Robot::listen_robot_b(){
	Subscriber subs_robot_b(robot_b.port_info.get(), robot_b.hostname.get());
	string info;
	cout << "listenning to robot " << robot_b.hostname.get() << "..." << endl;
	while(true){
		info = subs_robot_b.listen();		// blocking call
		decode_robot_params(info, robot_b);
		robot_b.ka.times.add_item(chrono::system_clock::now());

		check_le_messages(info);	// leader election message
	}

}

void Robot::listen_master(){
	string master = this->master.hostname.get();
	int port = this->master.port_info.get();
	Subscriber subs_master(port, master);
	string msg;
	int action = -1;
	cout << "listenning to master '" << master << "'..." << endl;
	while(true){
		msg = subs_master.listen();		// blocking call
		cout << "Master says: " << msg << endl;
		// decode info message
		//action = decode_master_commands(msg, params.hostname.get());
		//cout << "action = " << action << endl;

		//if (action != -1)
		master_data.set(msg);		// maybe need to change the name to the param (image_data)
		pub_image_task.publish(msg);

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

void Robot::send_keep_alive(){
	string msg_ka = "";
	while(true){
		// send KA
		msg_ka = encode_keep_alive(params.id.get());
		pub_robot_info.publish(msg_ka);
		leds.keep_alive();	//blink led
	}
}


void Robot::check_keep_alives(){
	cout << "start 'check_keep_alives'\n";

	// to do!
	bool a_alive = false, a_alive_old = false;
	bool b_alive = false, b_alive_old = false;
	bool just_started = true;
	
	while(true){

		// check other robots KA
		a_alive = robot_a.ka.is_alive();
		b_alive = robot_b.ka.is_alive();

		//if (debug.get()){
			cout << "*** a_alive = " << a_alive << ", old = " << a_alive_old;
			cout << "  |  b_alive = " << b_alive << ", old = " << b_alive_old << "\n\n";
		//}

		if (a_alive != a_alive_old || b_alive != b_alive_old || just_started){

			just_started = false;

			// trigger leader election
			a_alive_old = a_alive;
			b_alive_old = b_alive;
			
			//cout << "  a_alive = " << a_alive << endl;
			//cout << "  b_alive = " << b_alive << endl;
			if (!bully.trigger.get()){
				cout << "    trigger election\n";
				bully.trigger_election();
				bully.i_detected.set(true);
				bully.robots_ids.add_unique_item(params.id.get());
				if (a_alive) bully.robots_ids.add_unique_item(robot_a.id.get());
				if (b_alive) bully.robots_ids.add_unique_item(robot_b.id.get());
			}
		}

		// sleep the thread
		int millis_sleep = 1000;	// period for checking ka's
		this_thread::sleep_for(chrono::milliseconds(millis_sleep));
	}
}

void Robot::leader_election(){
	cout << "start 'leader_election'\n";
	string msg;
	int my_id, leader, proposed_leader;
	bool leader_elected = false;
	while(true){
		msg = "";
		leader = -1;
		proposed_leader = -1;
		
		bully.robots_ids.add_unique_item(params.id.get()); 	// repeting here just in case
		leader_elected = bully.election(leader, proposed_leader);

		if (debug.get()){
			cout << "\n### leader = " << leader;
			cout << "\n    proposed_leader = " << proposed_leader << "\n\n";
		}

		if (leader > -1 && proposed_leader > -1 && !leader_elected){
			msg = encode_leader_election(params.id.get(), leader, proposed_leader);
			pub_robot_info.publish(msg);
		}
		if(bully.trigger.get()) leds.election();
		else if(!bully.trigger.get() && bully.am_i_leader()) leds.is_leader(1);
		else leds.is_leader(0);
		this_thread::sleep_for(chrono::milliseconds(1000));
	}
}


void Robot::check_le_messages(string msg){
	// check if some robot triggers leader election
	int id = -1;
	int leader = -1, proposed_leader = -1;
	decode_leader_election(msg, id, leader, proposed_leader);

	// prints for debugging
	cout << "\n\n  check_le_messages, msg = " << msg << "\n\n";

	if (id > -1 && leader > -1 && proposed_leader > -1) {
		bully.trigger_election();
		bully.robots_ids.add_unique_item(id);
		bully.proposed_leader.add_unique_item(proposed_leader);
	}
}


void Robot::run(){

	// prints for debugging
	params.print_info();
	robot_a.print_info();
	robot_b.print_info();
	master.print_info();


	//thread task_planning(task_planner_run);	// may be in the same thread for now
	
	thread thread_serial(&Robot::serial, this);
	thread thread_image(&Robot::listen_image_process, this);
	thread thread_robot_a(&Robot::listen_robot_a, this); // maybe loop for listenning to other robots??
	thread thread_robot_b(&Robot::listen_robot_b, this);
	thread thread_master(&Robot::listen_master, this);
	thread thread_ka_send(&Robot::send_keep_alive, this);
	thread thread_ka_check(&Robot::check_keep_alives, this);
	thread thread_le(&Robot::leader_election, this);

	// get stuck here to test KA and LE
	cout << "\n\n*** ROBOT RUN ***\n\n";
	while(true){
		this_thread::sleep_for(chrono::milliseconds(10000));

		//Graph* map;
		//map = map_test_square("square1"); // just an example

		// example of usage
		//navigate_0(map, start_id, end_id);
		// - call 'navigate_0' or whatever funtion for navigating from one node to an other
		// - 'start_id' current node where you are now
		// - 'end_id' destiny node


		// just for testing the leds
		//leds.task_doing(1);
		//leds.navigating(1);
	}

	

	thread_serial.join();	// it will never reach this point, but good to have
	thread_image.join();
	thread_robot_a.join();
	thread_robot_b.join();
	thread_master.join();
	thread_ka_send.join();
	thread_ka_check.join();
	thread_le.join();
}


void Robot::update_pose(float x0, float y0, float th0){
	string msg = "@x0=" + to_string(x0) + ",y0=" + to_string(y0) + ",th0=" + to_string(th0) + "$";
	auto route_start = chrono::system_clock::now();
	float tol = 0.02;
	while (true){
		init_pose.set(msg);
		this_thread::sleep_for(chrono::milliseconds(100));

		float x0_s = sensors.x.get();
		float y0_s = sensors.y.get_last_item();
		float th0_s = sensors.th.get_last_item();
		cout << "\nx0_s: " << x0_s;
		cout << "\ny0_s: " << y0_s;
		cout << "\nth0_s: " << th0_s;
		if(x0_s > x0-tol && x0_s < x0+tol && y0_s > y0-tol && y0_s < y0+tol && th0_s > th0-tol && th0_s < th0+tol ) break;

		auto route_end = chrono::system_clock::now();
		chrono::duration<double> route_elapsed = route_end - route_start;
		if (route_elapsed.count() > 10) break;

	}
	sensors.print_info();
}


void Robot::compute_distance(float x, float y, float *d_w, float *th_w){
	
	float delta_x = x - sensors.x.get();
	float delta_y = y - sensors.y.get_last_item();
	

	*d_w = sqrt(delta_x*delta_x + delta_y*delta_y);
	*th_w = atan2(delta_y, delta_x);
}
















void Robot::navigate_0(Graph* map, string start_id, string end_id){
	//sensors.print_info();
	//update_pose(1.0, 1.0, 1.0);
	//update_pose(-0.05, 2.9, 0.0);

	string hn = params.hostname.get();
	
	map->reset_nodes();
	Dijkstra dijkstra(map);
	/*if (hn == "192.168.43.38")      {dijkstra.find_route("a", "i");}
	else if (hn == "192.168.43.138") {dijkstra.find_route("a", "r");}
	else if (hn == "192.168.43.174") {dijkstra.find_route("a", "a9");}
	*/
	dijkstra.find_route(start_id, end_id);
	dijkstra.print_route();
	Edge* edge;
	Node* start;
	Node* end;
	float th_w;
	float d_w;

	float threshold_xy = 0.05; // to say that the robot got to the final place
	int count_drive = sensors.newCommand.get_last_item() - 1;
	//cout << "count_drive: " << count_drive << endl;
	

	for (int i = 1; i < dijkstra.route.size(); ++i)
	{
		start = dijkstra.route.at(i-1);
		end = dijkstra.route.at(i);
		edge = map->find_edge(start, end);
		
		d_w = edge->distance;
		th_w = edge->get_th_w(start);

		if (edge->line==0) compute_distance(end->x,end->y,&d_w,&th_w);	
		
			
		//while(!sensors.newCommand.get_last_item()){
		//	this_thread::sleep_for(chrono::milliseconds(100));
		//}
		cout << "Node: " << start->id << endl;
		cout << "-------------turn\n";
		float trn = th_w - sensors.th.get_last_item();

		string msg = "";
		if (map->id != "straight"){
			msg = "@i=21,a=16,b=1,v=" + to_string(edge->vel) + ",trn=" + to_string(trn) + "$";
			count_drive++;
			drive_command.set(msg); 
			
				//this_thread::sleep_for(chrono::milliseconds(5000));
			//cout << "count_drive: " << count_drive << ", nc: " <<  sensors.newCommand.get_last_item() << endl;			
			while(count_drive == sensors.newCommand.get_last_item()){
				//cout << "nc: " << sensors.newCommand.get_last_item() << endl;
				//cout << "th: " << sensors.th.get_last_item() << endl;
				this_thread::sleep_for(chrono::milliseconds(10));
				cout << "turning!" << endl;
			}
			//cout << "count_drive: " << count_drive << ", nc: " << sensors.newCommand.get_last_item() << endl;
		}


		cout << "-------------fwd\n"; 
		sensors.print_info();
		msg = "@i=22,a=";
		/*
		if (start->id == "d") {
			msg += to_string(RACE);
		} 
		else
		*/
		if (edge->line == 0){
			if (map->id == "straight") msg += to_string(STAIRS);
			else msg += to_string(FWD);
		}
		else {
			string _msg = "";
			if (start->id == "f") {
				_msg = encode_task(BALL,edge->line);
				pub_image_task.publish(_msg);
				this_thread::sleep_for(chrono::milliseconds(2000));
				d_w = sensors.obst_dist.get_last_item() ; 
				if (d_w == 0.0) d_w = 0.45;
				d_w -= 0.05; //we want to stop 5cm in front of the ball
				cout << "distance to BALL: " << to_string(d_w) << endl;
			}
			else{
				_msg = encode_task(LINE,edge->line);
				pub_image_task.publish(_msg);
			}
			//pub_image_task.publish(_msg);
			//cout << "image task: " << LINE << ", edge: " << edge->line << ", msg: " << msg_ << endl;
			cout << "image task msg: " << _msg << endl;
			msg += to_string(FOLLOW);
		}
		msg += ",b=1,fwd=" + to_string(d_w) + ",v=" + to_string(edge->vel) + "$";
		count_drive++;
		drive_command.set(msg); 
		
		

	}	
	
}







