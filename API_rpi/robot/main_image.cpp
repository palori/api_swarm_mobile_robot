

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
	int image_height = 240;
	int image_width = 320;

	//bool ready2run = false;

	//Image_analysis ia;

	cout << "**************************************** " << endl;

	if (argc == 1 || argc == 5){
		if (argc == 5){
			port_image = str2int(argv[1]);
			port_task = str2int(argv[2]);
			image_height = str2int(argv[3]);
			image_width = str2int(argv[4]);
		}
		cout << "Welcome!" << endl;
		Image_analysis ia(port_image, port_task, image_height, image_width);
		ia.run();		// while true loop
	}
	else {
		cout << "Wrong inputs!" << endl;
		cout << "  Inputs needed:" << endl;
		cout << "    - port_image or publisher (int)" << endl;
		cout << "    - port_task or subscriber (int)" << endl;
		cout << "    - image_height (int)" << endl;
		cout << "    - image_width (int)" << endl;
	}

	cout << "**************************************** " << endl;

	
	return 0;
}