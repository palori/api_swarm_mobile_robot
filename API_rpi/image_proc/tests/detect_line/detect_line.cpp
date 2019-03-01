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

///////////////////////////////
///////////////////////////////

// Thresholding == binarize
int threshold_value = 150;
int threshold_type = 3;
int const max_value = 255;
int const max_type = 4;
int const max_BINARY_value = 255;
int lowThreshold = 0;
const int ratio = 3;
const int kernel_size = 3;

void display_image(Mat img, string title)
{
    namedWindow( title, WINDOW_AUTOSIZE );  
    imshow( title, img );
}


int main( ){
	// Take pic example
	time_t timer_begin,timer_end;
	raspicam::RaspiCam_Cv Camera;
	Mat img;
    int nCount=100;
    //set camera params
    Camera.set( CV_CAP_PROP_FORMAT, CV_8UC3 );
    //Open camera
    cout<<"Opening Camera..."<<endl;
    if (!Camera.open()) {cerr<<"Error opening the camera"<<endl;return -1;}
    //Start capture
    cout<<"Capturing..."<<endl;
    time ( &timer_begin );
    Camera.grab();
    Camera.retrieve (img);
    cout<<"Stop camera..."<<endl;
    Camera.release();

    //show time statistics
    time ( &timer_end ); /* get current time; same as: timer = time(NULL)  */
    double secondsElapsed = difftime ( timer_end,timer_begin );
    cout<< secondsElapsed<<" seconds for "<< nCount<<"  frames : FPS = "<<  ( float ) ( ( float ) ( nCount ) /secondsElapsed ) <<endl;
    //save image 
    imwrite("pic.jpg",img);
    cout<<"Image saved at 'pic.jpg'"<<endl;
	


    
    //img = imread(image_path, CV_LOAD_IMAGE_COLOR);  
    cout << "Image size: " <<  img.size() << endl;
    cout << "Image channels: " <<  img.channels() << endl;

    Mat img_gray, img_blur, bin, edges;
 
    cvtColor(img, img_gray, COLOR_RGB2GRAY);
	blur(img_gray, img_blur, Size(3,3));
	Canny(img_blur, edges, lowThreshold, lowThreshold * ratio, kernel_size);
	threshold(img_blur, bin, threshold_value, max_BINARY_value,threshold_type);
    

    display_image(img, "img");
	display_image(img_gray, "img_gray");
	display_image(img_blur, "img_blur");
    display_image(bin, "bin");
    display_image(edges,"edges");

    imwrite("pic_gray.jpg",img_gray);
    imwrite("pic_blur.jpg",img_blur);
    imwrite("pic_bin.jpg",bin);
    imwrite("pic_canny.jpg",img_edges);

	//Printing white pixels
	/*cout << "\n\nBinary values:";
	for(int i = 0; i < 960; i++){
		cout << endl << "[" << i << "] ";
	    for(int j = 0; j < 1280; j++){
	        //if(255 == bin.at<uchar>(i,j))
	        cout << bin.at<uchar>(i,j) << " ";
	    }
	}*/
}