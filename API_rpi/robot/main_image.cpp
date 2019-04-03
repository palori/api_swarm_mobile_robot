

#include <iostream>
#include "image_analysis.h"
#include "utils.h"

#include "opencv2/highgui/highgui.hpp"		// Check which one is needed for using this constants:
#include "opencv2/imgproc/imgproc.hpp"		// OpenCV param: CV_8UC1 (gray), CV_8UC3 (RGB)
#include "opencv2/opencv.hpp"

using namespace std;





int main(int argc, char const *argv[])
{	
	// read YAML file with params to define the robot
	// can start by getting them as arguments

	// default params
	string hostname = "localhost";
	int port_image = 7100;			// publisher_port
	int port_task = 7101;			// subscriber_port
	double image_format = 0.0;		// OpenCV param: CV_8UC1 (gray), CV_8UC3 (RGB)
	int image_height = 240;
	int image_width = 320;

	bool ready2run = false;

	Image_analysis ia;

	cout << "**************************************** " << endl;

	if (argc == 6){
		port_image = str2int(argv[1]);
		port_task = str2int(argv[2]);
		if(argv[3] == "gray"){
			image_format = CV_8UC1;
		}
		else if (argv[3] == "rgb"){
			image_format = CV_8UC3;
		}
		else {
			cout << "  Image format not valid!" << endl;
			cout << "    - image_format (string: gray, rgb)" << endl;
		}
		
		image_height = str2int(argv[4]);
		image_width = str2int(argv[5]);

		cout << "Welcome!" << endl;
		Image_analysis ia(port_image, port_task, image_format, image_height, image_width);
		//ia.run();		// while true loop
	}
	else {
		cout << "Wrong inputs!" << endl;
		cout << "  Inputs needed:" << endl;
		cout << "    - port_image or publisher (int)" << endl;
		cout << "    - port_task or subscriber (int)" << endl;
		cout << "    - image_format (string: gray, rgb)" << endl;
		cout << "    - image_height (int)" << endl;
		cout << "    - image_width (int)" << endl;
	}

	cout << "**************************************** " << endl;

	
	return 0;
}