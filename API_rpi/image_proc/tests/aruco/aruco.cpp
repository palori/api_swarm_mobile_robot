
#include <iostream>
#include <string.h>
#include <string>
#include <vector>
#include <cmath>
#include <ctime>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"

#include "opencv2/core/core.hpp"
#include "opencv2/aruco.hpp"

//#include "opencv2/core/utility.hpp"
//#include "opencv2/imgcodecs.hpp"

#include <raspicam/raspicam_cv.h>


using namespace cv;
using namespace std;

///////////////////////////////
///////////////////////////////

void display_img(string title, Mat img){
	namedWindow(title, WINDOW_AUTOSIZE );
	imshow(title, img);
	//waitKey(0);
}

void draw_marker(){
	int id = 1, max = 200;

	Mat markerImage;
    Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::DICT_4X4_50);
    aruco::drawMarker(dictionary, id, max, markerImage, 1);
    display_img("marker_4x4_0_generated", markerImage);
}

void test_aruco(){
	Mat inputImage = imread("../pics/aruco_1_0.png",IMREAD_GRAYSCALE);//CV_LOAD_IMAGE_COLOR);
	display_img("Aruco 1, img 0", inputImage);

	waitKey(0);
	
	vector<int> markerIds;
	vector<vector<Point2f>> markerCorners, rejectedCandidates;
	Ptr<aruco::DetectorParameters> parameters;
	Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::DICT_4X4_50);
	aruco::detectMarkers(inputImage, dictionary, markerCorners, markerIds, parameters, rejectedCandidates);
	/*
	if (markerIds.size() > 0) aruco::drawDetectedMarkers(inputImage, markerCorners, markerIds);
	imwrite("pics/aruco_1_0_found",inputImage);
	display_img("Aruco 1, img 0 - found", inputImage);
	waitKey(0);
	*/
}

int main(){
	draw_marker();
	test_aruco();
	return 0;
}
