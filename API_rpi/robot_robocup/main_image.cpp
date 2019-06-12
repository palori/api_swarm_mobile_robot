

#include <iostream>
#include "image_analysis.h"
#include "utils.h"

using namespace std;





int main(int argc, char const *argv[])
{	
	// read YAML file with params to define the robot
	// can start by getting them as arguments

	// default params
	string hostname = "localhost";
	int port_image = 7000;			// publisher
	int port_task = 7001;			// subscriber

	//bool ready2run = false;

	//Image_analysis ia;

	cout << "**************************************** " << endl;

	if (argc == 1 || argc == 3){
		if (argc == 3){
			port_image = str2int(argv[1]);
			port_task = str2int(argv[2]);
		}
		cout << "Welcome!" << endl;
		Image_analysis ia(port_image, port_task);
		ia.run();		// while true loop
	}
	else {
		cout << "Wrong inputs!" << endl;
		cout << "  Inputs needed:" << endl;
		cout << "    - port_image or publisher (int)" << endl;
		cout << "    - port_task or subscriber (int)" << endl;
	}

	cout << "**************************************** " << endl;

	
	return 0;
}