
#include "robot.h"

Robot::Robot(int id){

}




void Robot::run(){
	thread task_planning(task_planner_run);
	thread listen_image(listen_image_process);
	thread (listen_robot, 1); // maybe loop for listenning to other robots??
	thread ();
	thread ();
	thread ();
	thread ();
	thread ();
}


void Robot::listen_image_process(){
	string data = "";
	while(true){
		cout << "listenning to image processing..." << endl;
		data = subs_image_data.listen();	// blocking call
		// decode data into params to use for localization and send to TSY
	}
}

void Robot::listen_robot(int id){
	while(true){
		cout << "listenning to robot " << id << "..." << endl;
	}

}


void Robot::task_planner_run(){

}