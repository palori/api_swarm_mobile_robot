#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <raspicam/raspicam_cv.h>

#include <ctime>
#include <iostream>
//#include <stdlib.h>
#include <stdio.h>

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
int threshold_value = 150;//150;
int threshold_type = 0;
int const max_value = 255;
int const max_type = 4;
int const max_BINARY_value = 255;
int lowThreshold = 0;
const int ratio = 3;
const int kernel_size = 3;
const int CAM_W = 640;
const int CAM_H = 480;

raspicam::RaspiCam_Cv Camera;
COMM_RPI cr;
enum Side { LEFT, MIDDLE, RIGHT };

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


float take_pic_get_cm(int i, Side side){
	// Take pic example
	//time_t timer_begin,timer_end;
	
	Mat img;
	//int nCount=100;
	//set camera params
	
	
	//Start capture
	cout<<"Capturing "+to_string(i)+"..."<<endl;
	//time ( &timer_begin );
	Camera.grab();
	Camera.retrieve (img);
	

	//show time statistics
	//time ( &timer_end ); /* get current time; same as: timer = time(NULL)  */
	//double secondsElapsed = difftime ( timer_end,timer_begin );
	//cout<< secondsElapsed<<" seconds for "<< nCount<<"  frames : FPS = "<<  ( float ) ( ( float ) ( nCount ) /secondsElapsed ) <<endl;
	


	
	//img = imread(image_path, CV_LOAD_IMAGE_COLOR);  
	//cout << "Image size: " <<  img.size() << endl;
	//cout << "Image channels: " <<  img.channels() << endl;

	Mat img_gray, img_canny;
 
	//cvtColor(img, img_gray, COLOR_RGB2GRAY);
	img_gray = img;
	Mat img_blur (img_gray.size(), img_gray.type());
	blur(img_gray, img_blur, Size(5,5));
	Mat img_th (img_blur.size(), img_blur.type());
	//threshold_value = i*10;
	bool bad_threshold = true;
	int count_bad = 0;
	//threshold(img_blur, img_th, threshold_value, max_BINARY_value,threshold_type);
	
	while (bad_threshold) {
		threshold(img_blur, img_th, threshold_value, max_BINARY_value,threshold_type);

		
		//display_image(img_th, "img_th");
		int sum_white = 0;
		int sum_all = 0;
		for(int i = CAM_H*3/4; i<CAM_H; i++){
			for(int j = 0; j<CAM_W; j++){
				if (img_th.at<uchar>(i,j) > threshold_value){
					sum_white++;	
				}
				sum_all++;
			}
		}
		float white_percent = sum_white/(float)sum_all;
		//cout << "white: " << sum_white << endl;
		//cout << "all: " << sum_all << endl;
		//cout << "percent: " << white_percent << endl;
		if (white_percent<0.15) threshold_value-=10;
		else if (white_percent>0.15 && white_percent<0.46) bad_threshold = false;  //one line should cover around 22% of the bottom quarter of image
		else threshold_value+=10;			// change these constants if camera position changes

		if (bad_threshold == true) {
			string name = "pics/thres_"+to_string(threshold_value)+".png";
			imwrite(name,img_th);

		}
		count_bad++;
		if (count_bad>30){   //if it cannot find good img, return the first one
			bad_threshold = false;
			count_bad = 0;
			threshold(img_blur, img_th, 120, max_BINARY_value,threshold_type);
		}


	}

	
	//adaptiveThreshold(img_blur, img_th, max_BINARY_value, ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 5, 5);
	//Canny(img_th, img_canny, lowThreshold, lowThreshold * ratio, kernel_size);
	
	if (false){
		display_image(img, "img");
		display_image(img_gray, "img_gray");
		display_image(img_blur, "img_blur");
		display_image(img_th, "img_th");
		//display_image(img_canny,"img_canny");
	}

	//save images
	//imwrite("pic_gray.jpg",img_gray);
	//imwrite("pic_blur.jpg",img_blur);
	//imwrite("pic_th.jpg",img_th);
	//imwrite("pic_canny.jpg",img_canny);


	string pic_name = "pics/pic_th_"+to_string(i)+".png";
	imwrite(pic_name,img_th);
	//cout<<"Image saved at 'pic.jpg'"<<endl;


	//Printing white pixels
	/*cout << "\n\nBinary values:";
	for(int i = 0; i < 960; i++){
		cout << endl << "[" << i << "] ";
		for(int j = 0; j < 1280; j++){
			//if(255 == bin.at<uchar>(i,j))
			cout << bin.at<uchar>(i,j) << " ";
		}
	}*/


	int sum_y = 0;
	int count_y = 0;
	for(int i = CAM_H*3/4; i<CAM_H; i++){
		for(int j = 0; j<CAM_W; j++){
			if (img_th.at<uchar>(i,j) > threshold_value){
				sum_y += j;
				count_y++;
			}
		}
	}
	float cm_y = 0;
	if (count_y>0) cm_y= sum_y/count_y - CAM_W/2;
	else cout<<"---- NO line found ----"<<endl;
	cout<<"CM_y: "<<cm_y<<endl;
	switch(side){

		case LEFT:
			cm_y-=20;
			break;
		case MIDDLE:
			cm_y+=0;
			break;
		case RIGHT:
			cm_y+=20;
			break;
	}
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



/*void send_msg(COMM_RPI cr, string msg){

    
    cr.serial_write(msg);
    usleep(100000);
    int count = 0;
   /* while(count<20){
        usleep(100000);
        cr.serial_read();
        printf("read count %d\n",count);
        count++;
    }*/
    
//}

void pic_cm_comm1(){

	camera_init();
	if (camera_start() >= 0){
		
	    cr.serial_open();
	    int i=0;
	    float y=0.0;
	    string msg = "@a=19,b=1,v=0.25,fwd=1.5$";
	    cr.serial_write(msg);
	    usleep(10000);
	    while (i<500){
	    		//camera_start();
				y = take_pic_get_cm(i,LEFT);
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
