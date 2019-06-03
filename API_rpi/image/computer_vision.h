#ifndef computer_vision_h
#define computer_vision_h

// libraries!!!!

#include <ctime>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <string.h>
#include <string>
#include <vector>
#include "../comm/messages.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
//#include "opencv2/aruco.hpp"

using namespace std;
using namespace cv;


/* TASKS:
 * - idle
 * - follow line
 * - detect ball
 * - detect hole
 * - detect shape
 * - find ArUco code
 */
void idle();
string follow_line(Mat img, int side);
string track_ball(Mat img);
string hole(Mat img);
string shape(Mat img);
//string obstacle(Mat img);		// ?? maybe not needed
string ArUco(Mat img);

void GammaMapping(Mat& src, Mat& dst, float fGamma);
void HistStretch(Mat& src, Mat& dst);
bool compareContoursHeight(vector<Point> contour1, vector<Point> contour2);
bool compareCircles(Vec3f circle1, Vec3f circle2);

#endif
