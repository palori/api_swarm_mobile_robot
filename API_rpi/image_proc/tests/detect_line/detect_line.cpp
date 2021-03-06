
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/aruco.hpp"
//#include "home/pi/git/opencv_contrib-3.4/modules/aruco/include/opencv2/aruco.hpp"
#include "opencv2/core/core.hpp"


//#include "opencv2/core/utility.hpp"
//#include "opencv2/imgcodecs.hpp"

#include <raspicam/raspicam_cv.h>

#include <ctime>
#include <iostream>
//#include <stdlib.h>
#include <stdio.h>
#include <cmath>

#include <string.h>
#include <string>
#include <vector>

#include <signal.h> //deprecated, see csignal
//#include <csignal>

#include "../../../comm/comm_rpi_2.h"
#define PI 3.14159265


using namespace cv;
using namespace std;

///////////////////////////////

vector<Vec4d> aruco_markers;
double th_x = 25 * PI / 36;
double th_z = PI / 2; 
Vec3d tr = {0.0366,0,0.0713};

///////////////////////////////

// Thresholding == binarize
int threshold_value = 100;//150;
int threshold_type = 0;

int const max_value = 255;
int const max_type = 4;
int const max_BINARY_value = 255;

int lowThreshold=170; //% = 40;
const int thres_ratio = 4;
const int kernel_size = 3;
const int CAM_W = 320;
const int CAM_H = 240;
double old_left = 0.0;
double old_right = 0.0;
//closing
/*
int closing_elem = MORPH_ELLIPSE;
int closing_size = 7;
int opening_elem = MORPH_ELLIPSE;
int opening_size = 11;
*/

raspicam::RaspiCam_Cv Camera;
COMM_RPI cr;
enum Side { LEFT, MIDDLE, RIGHT };
enum Feature {NOTHING, LINE, Y_SPLIT, T_MAIN, T_LEFT, T_RIGHT, CROSS};
Feature feature = NOTHING;
bool t_main_candidate = false;  //{NOTHING, LINE, Y_SPLIT, T_MAIN, T_LEFT, T_RIGHT, CROSS};
bool y_split_candidate = false;
bool t_left_candidate = false;
bool t_right_candidate = false;
bool cross_candidate = false;

Mat img, img_gamma, img_blur, img_canny, canny_edges, img_hist, img_canny_crop, img_cont; 

vector<vector<Point>> contours;
vector<vector<Point>> good_contours;
vector<Vec4i> hierarchy;

/*SimpleBlobDetector::Params params;
vector<KeyPoint> keypoints;
SimpleBlobDetector detector(params);

void display_image(Mat img, string title)
{
	namedWindow( title, WINDOW_AUTOSIZE );  
	imshow( title, img );
}*/

int camera_init(){
	Camera.set( CV_CAP_PROP_FORMAT, CV_8UC1 );
	Camera.set(CV_CAP_PROP_FRAME_WIDTH, CAM_W);
	Camera.set(CV_CAP_PROP_FRAME_HEIGHT, CAM_H);
}

int camera_init_color(){
	Camera.set( CV_CAP_PROP_FORMAT, CV_8UC3 );
	Camera.set(CV_CAP_PROP_FRAME_WIDTH, CAM_W);
	Camera.set(CV_CAP_PROP_FRAME_HEIGHT, CAM_H);
}

int camera_aruco_init(){
	Camera.set( CV_CAP_PROP_FORMAT, CV_8UC1 );
	Camera.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	Camera.set(CV_CAP_PROP_FRAME_HEIGHT, 960);
}

int camera_start(){
	//Open camera
	cout<<"Opening Camera..."<<endl;
	if (!Camera.open()) {cerr<<"Error opening the camera"<<endl;return -1;}
	//Camera.set(CV_CAP_PROP_EXPOSURE, 100);
	usleep(2000000);
	return 0;	
}

void camera_stop(){
	cout<<"Stop camera..."<<endl;
	Camera.release();
}

void close_all(){
	camera_stop();
	cr.serial_close();
}

void GammaMapping(Mat& src, Mat& dst, float fGamma) {

	CV_Assert(src.data);

	//accept only char type matrices
	CV_Assert(src.depth() != sizeof(uchar));

	//build look up table
	unsigned char lut[256];
	for (int i = 0; i<256; i++){
		 lut[i] = saturate_cast<uchar>(pow((float)(i/255.0),fGamma) * 255.0f);
	}
	dst = src.clone();
	MatIterator_<uchar> it, end;
	for (it = dst. begin<uchar>(), end = dst.end<uchar>(); it != end; it++)
		*it = lut[(*it)];

}

void CannyThreshold(int param){

	Canny(img_blur, canny_edges, param, param + 50 , kernel_size);
	img_canny = Scalar::all(0);
	img_gamma.copyTo(img_canny, canny_edges);
	img_canny_crop = img_canny.clone();
	img_canny_crop = img_canny_crop(Rect(0,CAM_H*0.5,CAM_W,CAM_H*0.5));
	findContours(img_canny_crop, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
	img_cont = Mat::zeros(img_canny_crop.size(),CV_8UC1);
	//cout << "number of contours: "<< contours.size() << endl;
	for (int i=0;i < contours.size(); i++){
		Scalar color = Scalar(255,255,255);
		//cout << "Contour " << i << ". area: " << contourArea(contours[i]) << endl;
		//rectangle(img_cont,p1,p2,CV_RGB(255,255,255),1);
		if (arcLength(contours[i],false)>120){// && contourArea(contours[i])<20){ 
			good_contours.push_back(contours[i]);
			drawContours(img_cont, contours, i , color, 1, 8, hierarchy, 0, Point());
		}
	}

	string pic_name_th = "pics/pic_th_"+to_string(param)+".png";
	imwrite(pic_name_th,img_cont);

}

void HistStretch(Mat& src, Mat& dst) {

	CV_Assert(src.data);

	//accept only char type matrices
	CV_Assert(src.depth() != sizeof(uchar));

	//build look up table
	unsigned char lut[256];
	double vmax=140.0;
	double vmin=100.0;
	minMaxIdx(src, &vmin, &vmax);
	for (int i = 0; i<256; i++){
		 lut[i] = saturate_cast<uchar>(255.0f*((float)i-(float)vmin)/((float)vmax-(float)vmin));
	}

	dst = src.clone();
	MatIterator_<uchar> it, end;
	for (it = dst. begin<uchar>(), end = dst.end<uchar>(); it != end; it++)
		*it = lut[(*it)];

}

bool compareContoursHeight(vector<Point> contour1, vector<Point> contour2){
	Rect rect1 = boundingRect(contour1);
	Rect rect2 = boundingRect(contour2);
	return (rect1.height > rect2.height);
}


float take_pic_get_cm(int i, Side side){
	
	//start timer
	double function_time = (double)getTickCount();

	//Start capture - gray image
	//%Mat img;
	cout<<"Capturing "+to_string(i)+"..."<<endl;
	Camera.grab();
	Camera.retrieve (img);

	function_time = ((double)getTickCount()-function_time)/getTickFrequency();
	cout << "Function time: " << function_time << endl;

	//load image - just for testing
	//img = imread("../take_pic/crossings/pic_img_031.png",CV_LOAD_IMAGE_GRAYSCALE);

	//namedWindow("image", WINDOW_NORMAL);
	//imshow("image", img);
	//waitKey(0);
	
	//convert to gray
	//Mat img_gray;
	//cvtColor(img, img_gray, COLOR_RGB2GRAY);
	
	//histogram stretch
	//%Mat img_hist;
	//equalizeHist(img, img_hist);
	HistStretch(img,img_hist);
	
	//gamma mapping
	//%Mat img_gamma;
	float gamma = 3;	
	GammaMapping(img_hist, img_gamma, gamma);

	//thresholding
	//Mat img_th,img_crop;
	//bool bad_threshold = true;
	//float white_percent = 0.0;
	//bool otsu_thresholding = true;
	/*
	while (bad_threshold) {
		
		if (otsu_thresholding) threshold(img_gamma,img_th,0,255,CV_THRESH_BINARY | CV_THRESH_OTSU);
		else threshold(img_gamma, img_th, threshold_value, max_BINARY_value,threshold_type);
		otsu_thresholding = false;

		//cropping
		img_crop = img_th(Rect(0,CAM_H/2,CAM_W,CAM_H/2));

		int sum_white = 0;
		int sum_all = 0;
		for(int i = img_crop.rows/2 ; i<img_crop.rows; i++){
			for(int j = 0; j<img_crop.cols; j++){
				if (img_crop.at<uchar>(i,j) > threshold_value){
					sum_white++;	
				}
				sum_all++;
			}
		}
		white_percent = sum_white/(float)sum_all;
		
		//WATCH OUT FOR THIS PART WHEN ENTERING THE CROSSINGS
		if (white_percent<0.15) threshold_value-=10;
		else if (white_percent>0.15 && white_percent<0.46) bad_threshold = false;  //one line should cover around 22% of the bottom quarter of image
		else threshold_value+=10;			// change these constants if camera position changes

		if (bad_threshold == true) {
			cout << "New threshold value: " << threshold_value << endl;
			string name = "pics/thres_"+to_string(threshold_value)+".png";
			imwrite(name,img_th);

		}
		if (threshold_value<10 || threshold_value>240){   //if it cannot find good img, return the first one
			bad_threshold = false;
			threshold(img_gamma,img_th,0,255,CV_THRESH_BINARY | CV_THRESH_OTSU);
			threshold_value = 100;  //reinitialize threshold value
		}

	}*/


	//blurring
	//%Mat img_blur;
	blur(img_gamma, img_blur, Size(3,3));
	//medianBlur(img_gamma, img_blur, 9);

	//canny edge detection
	//%Mat img_canny,canny_edges;
	//%Canny(img_blur, canny_edges, lowThreshold, lowThreshold * thres_ratio , kernel_size);
	//%img_canny = Scalar::all(0);
	//%img_gamma.copyTo(img_canny, canny_edges);

	//namedWindow("contours",CV_WINDOW_AUTOSIZE); //%
	//createTrackbar("Min Threshold: ", "contours", &lowThreshold, 60, CannyThreshold); //%
	//CannyThreshold(0,0); //%

	//waitKey(0); //%


	//thresholding canny
	//Mat img_ct;
	//threshold(img_canny,img_ct,0,255,CV_THRESH_BINARY | CV_THRESH_OTSU);

	//BLOB DETECTION
	/*params.filterByArea = true;
	params.minArea = 50;
	params.minThreshold = 50;
	params.maxThreshold = 200;
	params.filterByCircularity = false;
	params.minCircularity = 0.1;
	params.filterByConvexity = false;
	params.minConvexity = 0.87;
	params.filterByInertia = true;
	params.maxInertiaRatio = 0.1;
	SimpleBlobDetector detector(params);
	detector.detect(img_canny, keypoints);
	Mat im_with_keypoints;
	drawKeypoints(img_canny,keypoints,im_with_keypoints,Scalar(0,0,255),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	*/

	//find contours
	
	/*
	img_canny_crop = img_canny.clone();
	img_canny_crop = img_canny_crop(Rect(0,CAM_H*0.5,CAM_W,CAM_H*0.5));
	findContours(img_canny_crop, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
	img_cont = Mat::zeros(img_canny_crop.size(),CV_8UC1);
	//cout << "number of contours: "<< contours.size() << endl;
	for (int i=0;i < contours.size(); i++){
		Scalar color = Scalar(255,255,255);
		//cout << "Contour " << i << ". area: " << contourArea(contours[i]) << endl;
		//rectangle(img_cont,p1,p2,CV_RGB(255,255,255),1);
		if (arcLength(contours[i],false)>120 && contourArea(contours[i])<20){ 
			good_contours.push_back(contours[i]);
			drawContours(img_cont, contours, i , color, 1, 8, hierarchy, 0, Point());
		}
	}*/
	
	bool bad_threshold = true;
	while (bad_threshold){
		good_contours.clear();
		contours.clear();
		CannyThreshold(lowThreshold);
		if (good_contours.size()==0) lowThreshold-=50;
		else bad_threshold = false;
		if (lowThreshold<50) {
			CannyThreshold(150);
			bad_threshold = false;
		}
	}

	lowThreshold = 170;
	
	//sort contours by arc length - assuming line contours are longer than noise contours
	sort(good_contours.begin(),good_contours.end(),compareContoursHeight);

	//initialize left and right contour
	vector<Point> left_contour;
	double left_cm;
	vector<Point> right_contour;
	double right_cm;
	vector<Point> middle_contour;
	double middle_cm;
	
	if (good_contours.size()>0){
		feature = LINE;
		for (int i=0; i < good_contours.size(); i++){
			//cout << "Good contour " << i << ". area: " << contourArea(good_contours[i]) << endl;
			Rect new_rect = boundingRect(good_contours[i]);
			Point p_cm;
			p_cm.x= new_rect.x + new_rect.width / 2;
			p_cm.y= new_rect.y + new_rect.height / 2;

			if (i==0) {
				left_contour = good_contours[i];
				right_contour = good_contours[i];
				left_cm = p_cm.x;
				right_cm = p_cm.x;
			} else if (p_cm.x <= left_cm && (new_rect.height > (img_cont.rows / 2) || i==1)) {
				left_contour = good_contours[i];
				left_cm = p_cm.x;
			} else if (p_cm.x > right_cm && (new_rect.height > (img_cont.rows / 2) || i==1)) {
				right_contour = good_contours[i];
				right_cm = p_cm.x;
			}

			if (new_rect.height < (img_cont.rows / 4) && new_rect.width > (3 * img_cont.cols / 4)) {
				if (t_main_candidate == true) {
					feature = T_MAIN;
					t_main_candidate = false;
				} else t_main_candidate = true;	
			} else t_main_candidate = false;

		}

		if (good_contours.size() == 1){
			Rect only_rect = boundingRect(good_contours[0]);
			if (only_rect.width > 65) {
				left_cm -= 32;
				right_cm += 32;
			} else if (abs(old_left-left_cm)<abs(old_right-right_cm)){
				right_cm += 65; 
			} else {
				left_cm -= 65;	
			}

		}

		Rect left_rect = boundingRect(left_contour);
		Rect right_rect = boundingRect(right_contour);

		Point left,right;
		left.y = left_rect.y + left_rect.height / 2;
		right.y = right_rect.y + right_rect.height / 2;

		if (abs(left.y - right.y) > img_cont.rows / 4 && left_rect.width > 50 || right_rect.width > 50) {

			if (left_rect.height > right_rect.height) {
				right_cm = left_cm + 32;
				left_cm -= 32;
			} else {
				left_cm = right_cm - 32;
				right_cm += 32;
			}

		}

		if (left_rect.height < img_cont.rows / 2 && left_rect.width > img_cont.cols / 4 && right_rect.height > img_cont.rows / 2 && right_rect.width < img_cont.cols/4) {
			if (t_left_candidate == true){
				feature = T_LEFT;
				t_left_candidate = false;
			} else t_left_candidate = true;
		} else t_left_candidate = false;


		if (left_rect.height > img_cont.rows / 2 && left_rect.width < img_cont.cols / 4 && right_rect.height < img_cont.rows / 2 && right_rect.width > img_cont.cols/4) {
			if (t_right_candidate == true){
				feature = T_RIGHT;
				t_right_candidate = false;
			} else t_right_candidate = true;
		} else t_right_candidate = false;


		if ( right_rect.x + right_rect.width - left_rect.x > img_cont.cols / 2) {
			if (y_split_candidate == true){
				feature = Y_SPLIT;
				y_split_candidate = false;
			} else y_split_candidate = true;
		} else y_split_candidate = false;


		// T_left : if (left_contour == short/wide and right_contour == tall/thin ); similarly for T_right

		// Ysplit : if contour between left and right contour becomes taller and taller , better: check distance between 2 tall contours

		// save only 3 contours and make decisions based on if there are 2 or 3 ??
	} else {
		feature = NOTHING;
		cout << "no line found !!!"<<endl;
		left_cm=CAM_W/2;
		right_cm=CAM_W/2;
	}

	cout << " FEATURE: " << feature << endl;	

	// plot only left and right rectangle;
	/*
	Rect rect_plot_l = boundingRect(left_contour);
	Rect rect_plot_r = boundingRect(right_contour);
	Point p1_l,p2_l,p1_r,p2_r; 
	p1_l.x = rect_plot_l.x;
	p1_l.y = rect_plot_l.y;
	p2_l.x = rect_plot_l.x + rect_plot_l.width;
	p2_l.y = rect_plot_l.y + rect_plot_l.height;
	rectangle(img_cont,p1_l,p2_l,CV_RGB(255,255,255),1);
	p1_r.x = rect_plot_r.x;
	p1_r.y = rect_plot_r.y;
	p2_r.x = rect_plot_r.x + rect_plot_r.width;
	p2_r.y = rect_plot_r.y + rect_plot_r.height;
	rectangle(img_cont,p1_r,p2_r,CV_RGB(255,255,255),1);
	*/
	
	/*
	if (false){
		display_image(img, "img");
		display_image(img_hist, "img_hist");
		display_image(img_gamma, "img_gamma");
		display_image(img_th, "img_th");
		display_image(img_canny,"img_canny");
	}*/

	//CLOSING
	/*
	Mat img_dil, img_closed, img_opened, img_ero;
	Mat element_closing = getStructuringElement(closing_elem, 
		                                Size(2*closing_size-1,2*closing_size-1),
		                                Point(closing_size,closing_size));
	Mat element_opening = getStructuringElement(opening_elem, 
		                                Size(2*opening_size-1,2*opening_size-1),
		                                Point(opening_size,opening_size));
	erode(img_th, img_ero , element_opening);
	dilate(img_ero, img_opened, element_opening);

	dilate(img_th, img_dil, element_closing);
	erode(img_dil, img_closed , element_closing);
	*/

	//SAVING IMAGES

	//string pic_name_th = "pics/pic_th_"+to_string(i)+".png";
	//imwrite(pic_name_th,img_th);

	string pic_name_img = "pics/pic_img_"+to_string(i)+".png";
	imwrite(pic_name_img,img);

	//string pic_name_canny = "pics/pic_canny_"+to_string(i)+".png";
	//imwrite(pic_name_canny,img_canny);

	//string pic_name = "pics/pic_crop_"+to_string(i)+".png";
	//imwrite(pic_name,img_canny_crop);

	string pic_name_cont = "pics/pic_cont_"+to_string(i)+".png";
	imwrite(pic_name_cont,img_cont);
	
	//CALCULATING CENTER OF MASS
	/*int sum_y = 0;
	int count_y = 0;
	for(int i = img_crop.rows/2; i<img_crop.rows; i++){
		for(int j = 0; j<img_crop.cols; j++){
			if (img_crop.at<uchar>(i,j) > threshold_value){
				sum_y += j;
				count_y++;
			}
		}
	}*/
	
	float cm = CAM_W/2;

	switch(side){

		case LEFT:
			cm = left_cm;    //determine this value based on the number of the lines - white area percentages
			break;
		case MIDDLE:
			cm = (left_cm + right_cm)/2;
			break;
		case RIGHT:
			cm = right_cm;
			break;
	}
	
	cm -= CAM_W/2; //cm_y= sum_y/count_y - CAM_W/2;

	cout<<"CM: "<<cm<<endl;
	
	old_left = left_cm;
	old_right = right_cm;
	return cm;	

/*
	// @@@@ NEED TO BE TESTED FROM HERE!

	// line with edges, find 2 points in the middle of the image.

	// ! check if img.size()[0] indexes are correct or need to be reversed!
	int MAX_COLS = 10;
	int cols[MAX_COLS];
	int col_count = 0;
	for (int i = 0; i < MAX_COLS; i++) cols[i] = 0; // initialization

	int row = img.size()[0]/2; // row to search for
	for (int i = 0; i < img.size()[1]; i++){
		if(255 == bin.at<uchar>(i,j)){
			cols[col_count] = i;
		}
	}
	cout<<"Edges: found "<< col_count << " edges..." <<endl;

	// considering that there are no crossings
	int col = 0; // col to search for
	bool col_found = false;
	if (col_count==2){
		// perfect case (data might be wrong, need good image processing)
		col = (cols[0] + cols[1])/2; // mean value
		col_found
	}
	else if (col_count==1){
		// migth find just one but we could use it as the middle point
		col = cols[0];
	}
	//else if (col_count<1 || col_count>2) // might want to discard and take a new picture or check another row

	if (col_found) cout<<"Target point (image coord. syst):\n  x = "<<row<"\n  y = "<<col<<endl;
	else cout<<"No clear target, try again..."<<endl;

	// Once target is found in image coord. syst. need to transform to
	// camera and robot coord. syst.
	int focal_length_px = 100; // [px] // random choice now, need to be computed (maybe with callibration)
	int image_center_px[2]; // [px]
	for (int i = 0; i < 2; i++) image_center_px[i] = img.size()[i]/2;
	// camera position in the robot coord. syst. -> got from the mechanical design
	// @@@@ might want to be readed from a configuration file, so it can be easily changed
	float camera_pose_translation[3] = {31.53, 0, 74.80}; // [mm]
	float camera_pose_rotation[3]; //still to figure out how to input in here (need to rotate X(90º) and Z(180º))

	float x_c = (row-image_center_px[0])/focal_length_px;
	float y_c = (col-image_center_px[1])/focal_length_px;
	float z_c; // how to compute profoundity???
*/

}

int shape_color (){

	Mat img = imread("pics/pic_7.png",CV_LOAD_IMAGE_COLOR);
	
	//convert to HSV
	Mat img_hsv;
	cvtColor(img, img_hsv, COLOR_BGR2HSV);
	
	//threshold for green
	Mat img_green;
	inRange(img_hsv,Scalar(38,0,0),Scalar(75,255,255),img_green);

	//threshold for orange
	Mat img_orange;
	inRange(img_hsv,Scalar(0,0,0),Scalar(22,255,255),img_orange);


	string name = "pics/green.png";
	imwrite(name,img_green);

	name = "pics/orange.png";
	imwrite(name,img_orange);


	return 1;
}

void initializeMarkers (){

	aruco_markers.push_back(Vec4d(3,0,0,0));


}

Vec3d getMarkerPose(int id){

	Vec3d markerPose = {0,0,0};
	for (unsigned int i=0;i < aruco_markers.size();i++){
		if (aruco_markers[i][0] == id) {
			markerPose[0]=aruco_markers[i][1];
			markerPose[1]=aruco_markers[i][2];
			markerPose[2]=aruco_markers[i][3];

			cout << "MATCH!" << endl;
		}
	}

	return markerPose;
}

Vec3d transform(double R[3][3], Vec3d t,Vec3d pose){   // from smaller to bigger coordinate system

		Vec3d newPose = {0,0,0};

		for (unsigned int i=0;i<3;i++){
			newPose[i] = R[i][0] * pose[0] + R[i][1] * pose[1] + R[i][2] * pose[2] + t[i];
		}

	return newPose;
}

Vec3d getPose(int id, Vec3d r, Vec3d t){

	Vec3d markerPose = getMarkerPose(id);

	double x_m = markerPose[0];
	double y_m = markerPose[1];
	double z_m = markerPose[2];


	double th = sqrt(pow(r[0],2) + pow(r[1],2) + pow(r[2],2));
	for (unsigned int i=0;i<3; i++){
		r[i] /= th;		
	}

	double c = cos(th);
	double s = sin(th);
	double c1 = (1-c);
	double x = r[0];
	double y = r[1];
	double z = r[2];

	double Rc[3][3] = {{c+pow(x,2)*c1, x*y*c1 - z*s, y*s + x*z*c1},{z*s + x*y*c1, c + pow(y,2)*c1, -x*s + y*z*c1},{-y*s + x*z*c1, x*s + y*z*c1,c + pow(z,2)*c1}};

	double Rr[3][3] = {{cos(th_z),sin(th_z) * cos(th_x),sin(th_x) * sin(th_z)},{sin(th_z), cos(th_z) * cos(th_x), -sin(th_x) * cos(th_z)},{0, sin(th_x), cos(th_x)}};

	Vec3d pose = transform(Rc, t, markerPose);
	pose = transform(Rr, tr, pose);

	cout << " new pose: ";

	for(unsigned int i=0;i<3;i++){

			cout << pose[i] << " ";
	}
	cout << endl;

	pose = transform(Rc, t, markerPose);

	//cout << "theta " << to_string(th) << endl;

	
	return pose;
}


void detectAruco(int i){

	double focal_length = 1007.568;
	double dx = 640;
	double dy = 480;
	Mat cameraMatrix = (Mat_<double>(3,3) << focal_length,0,dx,0,focal_length,dy,0,0,1);
	Mat distCoeffs = (Mat_<double>(1,5) << 0.2014,-0.5307,0,0,0.437);
	vector <Vec3d> rvecs,tvecs;
	//string name = "pics/aruco_"+to_string(i)+".png";
	//Mat inputImage = imread(name,CV_LOAD_IMAGE_GRAYSCALE);
	Mat inputImage;
	Camera.grab();
	Camera.retrieve(inputImage);

	vector<int> markerIds;
	vector<vector<Point2f>> markerCorners, rejectedCandidates;
	Ptr<aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(aruco::DICT_4X4_50);
	cv::aruco::detectMarkers(inputImage, dictionary, markerCorners, markerIds);
	if (markerIds.size()>0){
		cv::aruco::drawDetectedMarkers(inputImage, markerCorners, markerIds);
		cv::aruco::estimatePoseSingleMarkers(markerCorners,0.05,cameraMatrix,distCoeffs,rvecs,tvecs);
		cv::aruco::drawAxis(inputImage,cameraMatrix,distCoeffs,rvecs,tvecs,0.1);
		Vec3d pose = getPose(markerIds[0],rvecs[0],tvecs[0]);	
		string pic_name_img = "pics/aruco_detected_"+to_string(i)+".png";
		imwrite(pic_name_img,inputImage);
	} else {

		cout << "NO MARKERS FOUND!" << endl;
	}
	
	/*if (rvecs.size()){
		cout << "rvecs: ";
		for(unsigned int i=0;i<3;i++){

				cout << rvecs[0][i] << " ";
		}
		cout << endl;
	}
	if (tvecs.size()){
		cout << "tvecs: ";
		for(unsigned int i=0;i<3;i++){

				cout << tvecs[0][i] << " ";
		}
		cout << endl;
	}*/
	//string window_name = "ARUCO_"+to_string(i);
  	//namedWindow( window_name, CV_WINDOW_AUTOSIZE );
  	//imshow( window_name , inputImage);
  	
  	//waitKey(0);

}


void pic_cm_comm1(){
	bool followline = false;
	bool pictures = true;
	bool shapes = false;
	bool aruco = false;

	if (followline) camera_init();
	if (pictures) camera_init(); //camera_init_color();
	if (aruco) camera_aruco_init();

	if (camera_start() >= 0){
		
		if (followline) {
			cr.serial_open();
		    int i=0;
		    float y=0.0;
		    string msg = "@a=19,b=1,v=0.3,fwd=1.5$";
		    cr.serial_write(msg);
		   
			usleep(10000);
		    while (i<500){
		    		//camera_start();

					y = take_pic_get_cm(i,RIGHT);
					printf("Y: %f\n",y);

					if (feature != NOTHING) {
						msg = "@tht="+to_string(y)+"$";
						cr.serial_write(msg);
					}
					usleep(1000);
					i++;
					//camera_stop();
				
		    }
		    cr.serial_close();
		    camera_stop();	
		}
		if (pictures) {

			int j=0;
			Mat pic;
			while(j<30){
				cout<<"Capturing "+to_string(j)+"..."<<endl;
				Camera.grab();
				Camera.retrieve (pic);
				//Mat pic_color;
				//cvtColor(pic,pic_color,COLOR_RGB2BGR);
				//string pic_name = "pics/ball_cm_"+to_string(j)+".png";
				string pic_name = "pics/aruco_"+to_string(j)+".png";
				imwrite(pic_name,pic);
				j++;
				usleep(2000000);
			}

		}

		if (shapes) int j = shape_color();

		if (aruco) {
			int k;
			while (k<20){
				detectAruco();
				usleep(5000000);
			}
			


		}
	    
	}

}



int main(){

	//signal(SIGABRT,&close_all);//If program aborts go to assigned function "myFunction".
    //signal(SIGTERM,&close_all);//If program terminates go to assigned function "myFunction".
	//signal(SIGINT,&close_all);
	pic_cm_comm1();
	return 0;
}
