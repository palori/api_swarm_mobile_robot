#ifndef camera_h
#define camera_h

#include <iostream>
#include <ctime>
#include <raspicam/raspicam_cv.h>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"

class Camera
{
public:
	Camera();
	~Camera();

	Camera(int image_height, int image_width);

private:
	raspicam::RaspiCam_Cv camera;
	double image_format = CV_8UC1;			// OpenCV param: CV_8UC1 (gray), CV_8UC3 (RGB)
	int image_height = 240;
	int image_width = 320;
	int focal_length;
	// some calibration params...

public:
	// Getters
	double get_image_format();
	int get_image_height();
	int get_image_width();
	int get_focal_length();


	// Setters
	void set_image_format(double d);
	void set_image_height(int i);
	void set_image_width(int i);
	void set_focal_length(int i);


	// copy functions from: api_swarm_mobile_robot/API_rpi/image_proc/tests/detect_line/detect_line.cpp
	int init();
	int start();
	void stop();
	Mat take_picture();

	
};

#endif