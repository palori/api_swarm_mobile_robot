#ifndef image_analysis_h
#define image_analysis_h

//#include <ctime>
#include <iostream>
#include <thread>
#include <chrono>		// only for sleeping the threads while testing

#include "camera.h"				// comment to compile when NO Raspi
#include "../publisher.h"
#include "../subscriber.h"
#include "messages.h"
#include "item.h"
#include "utils.h"

#include "computer_vision.h"

// opencv libraries???
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"				// cv::Mat

using namespace std;
using namespace cv;

class Image_analysis
{
public:
	Image_analysis();
	~Image_analysis();

	Image_analysis(int port_image, int port_task); // add other input arguments

//private:
	// params
	Item<int> port_image;
	Item<int> port_task;
	//Item<int> image_height;
	//Item<int> image_width;

	Item<string> message;			// ?? need to be encoded, might not need to be an attr
	Items<int> tasks;
	Publisher pub_image;
	//Subscriber subs;
	Item<Mat> picture;


//public:

	// Getters
	//string get_message();	// might not need to be an attr
	//int get_task();
	//Mat get_picture();


	// Setters
	//void set_message(string s);		// might not need to be an attr
	//void set_task(int i);
	//void set_picture(Mat m);



	void get_new_task();	// thread that subscribe to task_planner for ever
	//void send_data();		//publish to the topic
	void take_picture();	// in charge of taking puctures


	

	void run();				// loop forever
	
};

#endif