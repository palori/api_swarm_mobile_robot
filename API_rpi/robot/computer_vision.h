

// libraries!!!!

#include <ctime>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <string.h>
#include <string>
#include <vector>
#include "messages.h"

/* TASKS:
 * - idle
 * - follow line
 * - detect ball
 * - detect hole
 * - detect shape
 * - find ArUco code
 */
void idle();
string follow_line(Mat img);
string ball(Mat img);
string hole(Mat img);
string shape(Mat img);
//string obstacle(Mat img);		// ?? maybe not needed
string ArUco(Mat img);

void GammaMapping(Mat& src, Mat& dst, float fGamma);
void HistStretch(Mat& src, Mat& dst);
bool compareContoursHeight(vector<Point> contour1, vector<Point> contour2);