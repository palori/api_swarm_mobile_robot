

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
	string hostname_a = "";
	string hostname_b = "";
	int max_len = 20;
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
	else if (argc == 5 || argc == 10){
		hostname = argv[1];
		hostname_a = argv[2];
		hostname_b = argv[3];
		max_len = str2int(argv[4]);
		if (argc == 10){
			port_image = str2int(argv[5]);
			port_task = str2int(argv[6]);
			port_info = str2int(argv[7]);
			port_info_robot_a = str2int(argv[8]);
			port_info_robot_b = str2int(argv[9]);
		}

		cout << "Welcome " << hostname << "!" << endl;
		Robot robot(hostname, hostname_a, hostname_b, max_len, port_image, port_task, port_info, port_info_robot_a, port_info_robot_b);
		//cout << "robot constructor done!" << endl;
		//robot.params.hostname.set(hostname);
		//robot.params.print_info();
		robot.run();		// while true loop
	}
	else {
		cout << "Wrong inputs!" << endl;
		cout << "  Inputs needed:" << endl;
		cout << "    - hostname (string)" << endl;
		cout << "    - hostname robot_a (string)" << endl;
		cout << "    - hostname robot_b (string)" << endl;
		cout << "    - max_len (int)" << endl;
		cout << "  Optional inputs:" << endl;
		cout << "    - port_image (int)" << endl;
		cout << "    - port_task (int)" << endl;
		cout << "    - port_info (int)" << endl;
		cout << "    - port_info_robot_a (int)" << endl;
		cout << "    - port_info_robot_b (int)" << endl;
	}

	cout << "**************************************** " << endl;

	
	return 0;
}