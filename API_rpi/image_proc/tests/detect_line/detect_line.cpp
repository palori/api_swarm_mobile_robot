
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"

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

#include "../../../comm_rpi_1.h"



using namespace cv;
using namespace std;

///////////////////////////////
///////////////////////////////

// Thresholding == binarize
int threshold_value = 100;//150;
int threshold_type = 0;
int const max_value = 255;
int const max_type = 4;
int const max_BINARY_value = 255;
int lowThreshold = 40;
const int thres_ratio = 4;
const int kernel_size = 3;
const int CAM_W = 320;
const int CAM_H = 240;
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


float take_pic_get_cm(int i, Side side){
	
	//start timer
	double function_time = (double)getTickCount();

	//Start capture - gray image
	Mat img;
	cout<<"Capturing "+to_string(i)+"..."<<endl;
	Camera.grab();
	Camera.retrieve (img);

	//load image - just for testing
	img = imread("../take_pic/crossings/pic_img_040.png",CV_LOAD_IMAGE_GRAYSCALE);

	//namedWindow("image", WINDOW_NORMAL);
	//imshow("image", img);
	//waitKey(0);
	
	//convert to gray
	//Mat img_gray;
	//cvtColor(img, img_gray, COLOR_RGB2GRAY);
	
	//histogram stretch
	Mat img_hist;
	//equalizeHist(img, img_hist);
	HistStretch(img,img_hist);
	
	//gamma mapping
	Mat img_gamma;
	float gamma = 3;	
	GammaMapping(img_hist, img_gamma, gamma);

	//thresholding
	Mat img_th,img_crop;
	bool bad_threshold = true;
	float white_percent = 0.0;
	bool otsu_thresholding = true;

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

	}


	//blurring
	Mat img_blur;
	blur(img_gamma, img_blur, Size(3,3));
	//medianBlur(img_gamma, img_blur, 9);

	//canny edge detection
	Mat img_canny,canny_edges;
	Canny(img_blur, canny_edges, lowThreshold, lowThreshold * thres_ratio , kernel_size);
	img_canny = Scalar::all(0);
	img_gamma.copyTo(img_canny, canny_edges);

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

	vector<vector<Point>> contours;
	vector<vector<Point>> good_contours;
	vector<Vec4i> hierarchy;
	Mat img_canny_crop = img_canny.clone();
	img_canny_crop = img_canny_crop(Rect(0,CAM_H*0.5,CAM_W,CAM_H*0.5));
	findContours(img_canny_crop, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
	Mat img_cont = Mat::zeros(img_canny_crop.size(),CV_8UC1);
	cout << "number of contours: "<< contours.size() << endl;
	for (int i=0;i < contours.size(); i++){
		Scalar color = Scalar(255,255,255);
		cout << "Contour " << i << ". length: " << arcLength(contours[i],false) << endl;
		//rectangle(img_cont,p1,p2,CV_RGB(255,255,255),1);
		if (arcLength(contours.at(i),false)>120){ 
			good_contours.push_back(contours[i]);
			drawContours(img_cont, contours, i , color, 1, 8, hierarchy, 0, Point());
		}
	}

	vector<Point> left_contour;
	double left_cm;
	vector<Point> right_contour;
	double right_cm;

	if (good_contours.size()>0){
		for (int i=0; i < good_contours.size(); i++){
			Rect new_rect = boundingRect(good_contours[i]);
			Point p_cm,p1,p2;
			p1.x = new_rect.x;
			p1.y = new_rect.y;
			p2.x = new_rect.x + new_rect.width;
			p2.y = new_rect.y + new_rect.height;
			p_cm.x= (p1.x+p2.x)/ 2;
			p_cm.y= (p1.y+p2.y)/ 2;

			if (i==0) {
				left_contour = good_contours[i];
				right_contour = good_contours[i];
				left_cm = p_cm.x;
				right_cm = p_cm.x;
			} else if (p_cm.x < left_cm && p_cm.y > (img_cont.height / 4)) {
				left_contour = good_contours[i];
				left_cm = p_cm.x;
			} else if (p_cm.x > right_cm && p_cm.y > (img_cont.height / 4)) {
				right_contour = good_contours[i];
				right_cm = p_cm.x;
			}
			
		}
	} else {
		cout << "no line found !!!"<<endl;
		left_cm=CAM_W/2;
		right_cm=CAM_W/2;
	}	

	// plot only left and right rectangle;

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



	string pic_name_th = "pics/pic_th_"+to_string(i)+".png";
	imwrite(pic_name_th,img_th);

	string pic_name_img = "pics/pic_img_"+to_string(i)+".png";
	imwrite(pic_name_img,img);

	string pic_name_canny = "pics/pic_canny_"+to_string(i)+".png";
	imwrite(pic_name_canny,img_canny);

	string pic_name = "pics/pic_crop_"+to_string(i)+".png";
	imwrite(pic_name,img_canny_crop);

	string pic_name_cont = "pics/pic_cont_"+to_string(i)+".png";
	imwrite(pic_name_cont,img_cont);
	
	//CALCULATING CENTER OF MASS
	int sum_y = 0;
	int count_y = 0;
	for(int i = img_crop.rows/2; i<img_crop.rows; i++){
		for(int j = 0; j<img_crop.cols; j++){
			if (img_crop.at<uchar>(i,j) > threshold_value){
				sum_y += j;
				count_y++;
			}
		}
	}
	float cm_y = 0;
	if (count_y>0) cm_y=(right_cm+left_cm)/2 - - CAM_W/2;//cm_y= sum_y/count_y - CAM_W/2;
	else cout<<"---- NO line found ----"<<endl;
	
	int delta_cm = round(50*white_percent/0.22); 
	cout << "delta_cm: " << delta_cm << endl;
	switch(side){

		case LEFT:
			cm_y-= delta_cm;    //determine this value based on the number of the lines - white area percentages
			break;
		case MIDDLE:
			cm_y+=0;
			break;
		case RIGHT:
			cm_y+= delta_cm;
			break;
	}
	cout<<"CM_y: "<<cm_y<<endl;
	function_time = ((double)getTickCount()-function_time)/getTickFrequency();
	cout << "Function time: " << function_time << endl;
	return cm_y;	

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


void pic_cm_comm1(){

	camera_init();
	if (camera_start() >= 0){
		
	    cr.serial_open();
	    int i=0;
	    float y=0.0;
	    string msg = "@a=19,b=1,v=0.3,fwd=0.5$";
	    cr.serial_write(msg);
	    usleep(10000);
	    while (i<300){
	    		//camera_start();
				y = take_pic_get_cm(i,MIDDLE);
				printf("Y: %f\n",y);
				msg = "@tht="+to_string(y)+"$";
				cr.serial_write(msg);
				//usleep(10000);
				i++;
				//camera_stop();
			
	    }
	    cr.serial_close();
	    camera_stop();
	}

}


int main(){

	//signal(SIGABRT,&close_all);//If program aborts go to assigned function "myFunction".
    //signal(SIGTERM,&close_all);//If program terminates go to assigned function "myFunction".
	//signal(SIGINT,&close_all);
	pic_cm_comm1();
	return 0;
}
