

#include <iostream>
#include "robot.h"
#include "utils.h"

using namespace std;





int main(int argc, char const *argv[])
{	
	// read YAML file with params to define the robot
	// can start by getting them as arguments

	// default params
	string hostname = "";
	int port_image = 7000;
	int port_task = 7001;
	int port_info = 8000;
	int port_info_robot_a = 8000;
	int port_info_robot_b = 8000;

	bool ready2run = false;

	//Robot robot;

	cout << "**************************************** " << endl;

	if (argc < 2){
		cout << "Minimum need a 'hostname' argument or include also the ports." << endl;

	}
	else if (argc == 2 || argc == 7){
		hostname = argv[1];
		if (argc == 7){
			port_image = str2int(argv[2]);
			port_task = str2int(argv[3]);
			port_info = str2int(argv[4]);
			port_info_robot_a = str2int(argv[5]);
			port_info_robot_b = str2int(argv[6]);
		}

		cout << "Welcome " << hostname << "!" << endl;
		Robot robot;//(hostname, port_image, port_task, port_info, port_info_robot_a, port_info_robot_b);
		//robot.params.hostname.set(hostname);
		//robot.params.print_info();
		//robot.run();		// while true loop
	}
	else {
		cout << "Wrong inputs!" << endl;
		cout << "  Inputs needed:" << endl;
		cout << "    - hostname (string)" << endl;
		cout << "    - port_image (int)" << endl;
		cout << "    - port_task (int)" << endl;
		cout << "    - port_info (int)" << endl;
		cout << "    - port_info_robot_a (int)" << endl;
		cout << "    - port_info_robot_b (int)" << endl;
	}

	cout << "**************************************** " << endl;

	
	return 0;
}