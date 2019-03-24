
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <raspicam/raspicam_cv.h>
#include <ctime>
#include <iostream>
//#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <vector>
 
using namespace cv;
using namespace std;


void display_image(Mat img, string title);
int take_pic_get_cm();
//int main();
