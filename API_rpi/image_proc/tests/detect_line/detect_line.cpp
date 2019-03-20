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
int threshold_value = 180;//150;
int threshold_type = 0;
int const max_value = 255;
int const max_type = 4;
int const max_BINARY_value = 255;
int lowThreshold = 50;
const int thres_ratio = 4;
const int kernel_size = 3;
const int CAM_W = 320;
const int CAM_H = 240;
//closing
int closing_elem = MORPH_ELLIPSE;
int closing_size = 7;
int opening_elem = MORPH_ELLIPSE;
int opening_size = 11;


raspicam::RaspiCam_Cv Camera;
COMM_RPI cr;
enum Side { LEFT, MIDDLE, RIGHT };
SimpleBlobDetector::Params params;
vector<KeyPoint> keypoints;

SimpleBlobDetector detector(params);

void display_image(Mat img, string title)
{
	namedWindow( title, WINDOW_AUTOSIZE );  
	imshow( title, img );
}

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

/*void GammaMapping(Mat& src, Mat& dst, float fGamma) {

	CV_Assert(src.data);

	//accept only char type matrices
	CV_Assert(src.depth != sizeof(uchar));

	//build look up table
	unsigned char lut[256];
	for (int i = 0; i<256; i++){
		 lut[i] = saturate_cast<uchar>(pow((float)(i/255.0),fGamma) * 255.0f);
	}

	dst = src.clone();

	MatIterator_<uchar> it, end;
	for (it = dst. begin<uchar>(), end = dst.end<uchar>(); it != end; it++)
		*it = lut[(*it)];

}*/

float take_pic_get_cm(int i, Side side){
	
	//start timer
	double function_time = (double)getTickCount();

	//Start capture - gray image
	Mat img;
	cout<<"Capturing "+to_string(i)+"..."<<endl;
	Camera.grab();
	Camera.retrieve (img);

	//convert to gray
	//Mat img_gray;
	//cvtColor(img, img_gray, COLOR_RGB2GRAY);
	
	//histogram stretch
	Mat img_hist;
	equalizeHist(img, img_hist);

	//gamma mapping
	Mat img_gamma = img_hist;
	float gamma = 2.5;	
	//GammaMapping(img_hist, img_gamma, gamma);

	//cropping
	Mat img_crop = img_gamma(Rect(0,CAM_H/2,CAM_W,CAM_H/2));
	
	//blurring
	//Mat img_blur;
	//blur(img_gray, img_blur, Size(6,6));

	//thresholding
	Mat img_th;
	bool bad_threshold = true;
	float white_percent = 0.0;

	while (bad_threshold) {
		
		threshold(img_crop, img_th, threshold_value, max_BINARY_value,threshold_type);

		
		int sum_white = 0;
		int sum_all = 0;
		for(int i = img_th.rows/2 ; i<img_th.rows; i++){
			for(int j = 0; j<img_th.cols; j++){
				if (img_th.at<uchar>(i,j) > threshold_value){
					sum_white++;	
				}
				sum_all++;
			}
		}
		white_percent = sum_white/(float)sum_all;
		
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
			threshold(img_crop, img_th, 150, max_BINARY_value,threshold_type);
			threshold_value = 150;
		}


	}

	//thresholding for canny with otsu method - consider using it as adaptive
	Mat img_otsu;
	threshold(img_gamma,img_otsu,0,255,CV_THRESH_BINARY | CV_THRESH_OTSU);


	//canny edge detection
	Mat img_canny;
	Canny(img_otsu, img_canny, lowThreshold, lowThreshold * thres_ratio, kernel_size);
	

	//BLOB DETECTION

	params.filterByArea = true;
	params.minArea = 100;
	params.minThreshold = 50;
	params.maxThreshold = 200;
	params.filterByCircularity = false;
	params.minCircularity = 0.1;
	params.filterByConvexity = false;
	params.minConvexity = 0.87;
	params.filterByInertia = false;
	params.minInertiaRatio = 0.01;
	SimpleBlobDetector detector(params);
	detector.detect(img_canny, keypoints);
	Mat im_with_keypoints;
	drawKeypoints(img_canny,keypoints,im_with_keypoints,Scalar(0,0,255),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	
	
	
	if (false){
		display_image(img, "img");
		display_image(img_hist, "img_hist");
		display_image(img_gamma, "img_gamma");
		display_image(img_th, "img_th");
		//display_image(img_canny,"img_canny");
	}

	//save images
	//imwrite("pic_gray.jpg",img_gray);
	//imwrite("pic_blur.jpg",img_blur);
	//imwrite("pic_th.jpg",img_th);
	//imwrite("pic_canny.jpg",img_canny);

	//CLOSING

	//Mat img_dil, img_closed, img_opened, img_ero;
	
	//Mat element_closing = getStructuringElement(closing_elem, 
	//	                                Size(2*closing_size-1,2*closing_size-1),
	//	                                Point(closing_size,closing_size));

	//Mat element_opening = getStructuringElement(opening_elem, 
	//	                                Size(2*opening_size-1,2*opening_size-1),
	//	                                Point(opening_size,opening_size));
	
	//erode(img_th, img_ero , element_opening);
	//dilate(img_ero, img_opened, element_opening);
	

	//dilate(img_th, img_dil, element_closing);
	//erode(img_dil, img_closed , element_closing);
	
	string pic_name_hist = "pics/pic_hist_"+to_string(i)+".png";
	imwrite(pic_name_hist,img_hist);

	string pic_name_img = "pics/pic_img_"+to_string(i)+".png";
	imwrite(pic_name_img,img);

	string pic_name_canny = "pics/pic_canny_"+to_string(i)+".png";
	imwrite(pic_name_canny,img_canny);

	string pic_name = "pics/pic_th_"+to_string(i)+".png";
	imwrite(pic_name,img_th);

	string pic_name_blob = "pics/pic_bl_"+to_string(i)+".png";
	imwrite(pic_name_blob,im_with_keypoints);
	

	int sum_y = 0;
	int count_y = 0;
	for(int i = img_th.rows/2; i<img_th.rows; i++){
		for(int j = 0; j<img_th.cols; j++){
			if (img_th.at<uchar>(i,j) > threshold_value){
				sum_y += j;
				count_y++;
			}
		}
	}
	float cm_y = 0;
	if (count_y>0) cm_y= sum_y/count_y - CAM_W/2;
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
				//printf("Y: %f\n",y);
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
