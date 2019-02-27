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


vector<Mat> blur_imgs(vector<Mat> imgs, vector<string> title){
	vector<Mat> blu(imgs.size());
	for (int i = 0; i < imgs.size(); ++i)
	{
		blur(imgs[i], blu[i], Size(5,5));
		display_image(blu[i],title[i]);
		string image_name = "images/img_" + title[i] + ".png";
		imwrite(image_name, blu[i]);
	}
	return blu;

}

vector<Mat> binarize_imgs(vector<Mat> imgs, vector<string> title, vector<int> threshold_value){
	vector<Mat> bin(imgs.size());
	for (int i = 0; i < imgs.size(); ++i)
	{
		threshold( imgs[i], bin[i], threshold_value[i], max_BINARY_value,threshold_type );
		display_image(bin[i],title[i]);
		string image_name = "images/img_" + title[i] + ".png";
		imwrite(image_name, bin[i]);
	}
	return bin;
}

//// NOT WORKING!!!
vector<Mat> hist_imgs(vector<Mat> imgs, vector<string> title){
	/// Establish the number of bins
	int histSize = 256;

	/// Set the ranges ( for B,G,R) )
	float range[] = { 0, 256 } ;
	const float* histRange = { range };

	bool uniform = true; bool accumulate = false;

	// Draw the histograms for R, G and B
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound( (double) hist_w/histSize );
	Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

	vector<Mat> img_hist(imgs.size());

	/// Compute the histograms:
	for (int i = 0; i < imgs.size(); ++i)
	{
  		calcHist( &imgs[i], 1, 0, Mat(), img_hist[i], 1, &histSize, &histRange, uniform, accumulate );
  		/// Normalize the result to [ 0, histImage.rows ]
		normalize(img_hist[i], img_hist[i], 0, histImage.rows, NORM_MINMAX, -1, Mat() );
		for( int i = 1; i < histSize; i++ )
		{
			line( histImage, Point( bin_w*(i-1), hist_h - cvRound(img_hist[i].at<float>(i-1)) ) ,
				Point( bin_w*(i), hist_h - cvRound(img_hist[i].at<float>(i)) ),
				Scalar( 255, 0, 0), 2, 8, 0  );
		}
  		display_image(histImage,title[i]);
		string image_name = "images/img_" + title[i] + ".png";
		imwrite(image_name, histImage);
  	}
  	return img_hist;
}


int main( ){

	string path = "../../../../test_images/bdrs_sally_images_20180302/images_20180302/";
    string image_name = "image_20180302_161046.359_005.png";
    //string image_name = "image_20180302_161159.774_015.png";
    string image_path = path + image_name;
    Mat img;
    img = imread(image_path, CV_LOAD_IMAGE_COLOR);  
    cout << "Image size: " <<  img.size() << endl;
    cout << "Image channels: " <<  img.channels() << endl;

    Mat img_gray, img_blur, bin;
 
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