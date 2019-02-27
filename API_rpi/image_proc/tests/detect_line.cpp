#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
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

void display_image(Mat img, string title)
{
    namedWindow( title, WINDOW_AUTOSIZE );  
    imshow( title, img );
}

	

int main( ){
	cout << "1" << endl;
	string path = "";
    string image_name = "image_example.png";
    //string image_name = "image_20180302_161159.774_015.png";
    string image_path = path + image_name;
    Mat img;
    img = imread(image_path, CV_LOAD_IMAGE_COLOR);  
    cout << "Image size: " <<  img.size() << endl;
    cout << "Image channels: " <<  img.channels() << endl;

    Mat img_gray, img_blur, bin;
 	cout << "2" << endl;
    cvtColor(img, img_gray, COLOR_RGB2GRAY);
	blur(img_gray, img_blur, Size(3,3));
	threshold(img_blur, bin, threshold_value, max_BINARY_value,threshold_type);
    
    display_image(img, "img");
	display_image(img_gray, "img_gray");
	display_image(img_blur, "img_blur");
    display_image(bin, "bin");

	//Printing white pixels
	cout << "\n\nBinary values:";
	for(int i = 0; i < 640; i++){
		cout << endl << "[" << i << "] ";
	    for(int j = 0; j < 480; j++){
	        //if(255 == bin.at<uchar>(i,j))
	        cout << bin.at<uchar>(i,j) << " ";
	    }
	}
}