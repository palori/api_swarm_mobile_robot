

#include <iostream>

#include <errno.h>		//some of them needed for handling Ctrl^C
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "robot/robot.h"
#include "utils/utils.h"

using namespace std;


extern bool ctrl_c_pressed = false;

void sig_handler(int sig)
{
    write(0,"\nCtrl^C pressed in sig handlern",32);
    ctrl_c_pressed = true;
}


int main(int argc, char const *argv[])
{	
	// read YAML file with params to define the robot
	// can start by getting them as arguments

	// default params
	string hostname_master = "";
	string hostname = "";
	string hostname_a = "";
	string hostname_b = "";

	int id = 0;
	int max_len = 20;

	int port_image = 7000;
	int port_task = 7001;

	int port_info = 8000;
	int port_info_robot_a = 8000;
	int port_info_robot_b = 8000;
	int port_info_master = 8000;

	int id_robot_a = 0;
	int id_robot_b = 0;
	int id_master = 0;

	bool ready2run = false;

	//Robot robot;

	cout << "**************************************** " << endl;
	cout << "argc = " << argc << endl;
	for (int i = 0; i < argc; ++i)
	{
		cout << "  argv[" << i << "] = " << argv[i] << endl;
	}
	if (argc < 2){
		cout << "Minimum need a 'hostname' argument or include also the ports." << endl;

	}
	else if (argc == 7 || argc == 16){
		hostname_master = argv[1];
		hostname = argv[2];
		hostname_a = argv[3];
		hostname_b = argv[4];
		id = str2int(argv[5]);
		max_len = str2int(argv[6]);
		if (argc == 16){
			port_image = str2int(argv[7]);
			port_task = str2int(argv[8]);
			port_info = str2int(argv[9]);
			port_info_robot_a = str2int(argv[10]);
			port_info_robot_b = str2int(argv[11]);
			port_info_master = str2int(argv[12]);
			id_robot_a = str2int(argv[13]);
			id_robot_b = str2int(argv[14]);
			id_master = str2int(argv[15]);
		}

		cout << "Welcome " << hostname << "!" << endl;
		Robot robot(hostname_master, hostname, hostname_a, hostname_b, id, max_len, port_image, port_task, port_info, port_info_robot_a, port_info_robot_b, port_info_master, id_robot_a, id_robot_b, id_master);
		//cout << "robot constructor done!" << endl;
		//robot.params.hostname.set(hostname);
		//robot.params.print_info();
		robot.run();		// while true loop
	}
	else {
		cout << "Wrong inputs!" << endl;
		cout << "  Inputs needed:" << endl;
		cout << "    - hostname master (string)" << endl;
		cout << "    - hostname (string)" << endl;
		cout << "    - hostname robot_a (string)" << endl;
		cout << "    - hostname robot_b (string)" << endl;
		cout << "    - id (int)" << endl;
		cout << "    - max_len (int)" << endl;
		cout << "  Optional inputs:" << endl;
		cout << "    - port_image (int)" << endl;
		cout << "    - port_task (int)" << endl;
		cout << "    - port_info (int)" << endl;
		cout << "    - port_info_robot_a (int)" << endl;
		cout << "    - port_info_robot_b (int)" << endl;
		cout << "    - id_robot_a (int)" << endl;
		cout << "    - id_robot_b (int)" << endl;
	}

	cout << "**************************************** " << endl;

	
	return 0;
}