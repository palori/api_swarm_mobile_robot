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

///////////////////////////////
///////////////////////////////
bool is_color_edge = false;
int edgeThresh = 110;                   // variable
//int edgeThreshScharr=1;
Mat image, img_blur, edge1, edge2, cedge;
const char* window_name1 = "Edge map : Canny default (Sobel gradient)";
//const char* window_name2 = "Edge map : Canny with custom gradient (Scharr)";
// define a trackbar callback
static void onTrackbar(int, void*)
{
    //blur(gray, blurImage, Size(3,3));
    // Run the edge detector on grayscale
    Canny(img_blur, edge1, edgeThresh, edgeThresh*3, 3);
    cedge = Scalar::all(0);
    if (is_color_edge)
    {
        image.copyTo(cedge, edge1);
        imshow(window_name1, cedge);
    }
    else
    {
        imshow(window_name1, edge1);
    }
    imwrite("images/img_edge.png", edge1);
}
///////////////////////////////
///////////////////////////////

/// Global variables

/** General variables */
Mat src, edges, src_gray, probabilistic_hough;
int min_threshold = 1;
int max_trackbar = 255;
int p_trackbar = max_trackbar;

const char* probabilistic_name = "Probabilistic Hough Lines Demo";

/**
 * @function Probabilistic_Hough
 */
void Probabilistic_Hough( int, void* )
{
  vector<Vec4i> p_lines;
  cvtColor( edge1, probabilistic_hough, COLOR_GRAY2BGR );

  /// 2. Use Probabilistic Hough Transform
  HoughLinesP( edge1, p_lines, 1, CV_PI/180, min_threshold + p_trackbar, 30, 10 );

  /// Show the result
  for( size_t i = 0; i < p_lines.size(); i++ )
     {
       Vec4i l = p_lines[i];
       line( probabilistic_hough, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,0,0), 3, LINE_AA);
     }

   imshow( probabilistic_name, probabilistic_hough );
   imwrite("images/img_hough.png", probabilistic_hough);
}

///////////////////////////////
///////////////////////////////

void display_image(Mat img, string title)
{
    namedWindow( title, WINDOW_AUTOSIZE );  
    imshow( title, img );
}

vector<Mat> split_channels(Mat img, string title, bool show_chs)
{
    vector<Mat> channels(img.channels());
    // split img:
    split(img, channels);
    // get the channels (dont forget they follow BGR order in OpenCV)
    if (show_chs)
    {
        for (int i = 0; i < img.channels(); ++i)
        {
            string ch_title = title + " [" + to_string(i) + "]";
            display_image(channels[i],ch_title);
            string image_name = "images/img_" + title + to_string(i) + ".png";
            imwrite(image_name, channels[i]);
        }
    }
    return channels;
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
 
int main( )
{
    string path = "../../../../test_images/bdrs_sally_images_20180302/images_20180302/";
    string image_name = "image_20180302_161046.359_005.png";
    //string image_name = "image_20180302_161159.774_015.png";
    string image_path = path + image_name;

    image_path = "detect_line/image_example.png";
    Mat img, ch1, ch2, ch3;
    img = imread(image_path, CV_LOAD_IMAGE_COLOR);  
    display_image(img, "img4");
    cout << "Image size: " <<  img.size() << endl;
    cout << "Image channels: " <<  img.channels() << endl;

    split_channels(img, "RGB", true);

    Mat img_gray, img_ycc, img_hsv;
    cvtColor(img, img_gray, COLOR_RGB2GRAY);
    vector<Mat> bw = split_channels(img_gray, "Gray", true);
    cvtColor(img, img_ycc, COLOR_RGB2YCrCb);
    vector<Mat> ycc = split_channels(img_ycc, "YCrCb", true);
    cvtColor(img, img_hsv, COLOR_RGB2HSV);
    vector<Mat> hsv = split_channels(img_hsv, "HSV", true);
    
    // detect line -> start using either img_gray or bw[0]
    Mat bin;
    threshold( img_gray, bin, threshold_value, max_BINARY_value,threshold_type );
    display_image(bin, "bin");
    image = img;
    blur(img_gray, img_blur, Size(3,3));
    // Create a window
    namedWindow(window_name1, 1);

    // create a toolbar
    createTrackbar("Canny threshold default", window_name1, &edgeThresh, 255, onTrackbar);
    // Show the image
    onTrackbar(0, 0);
    
    /// Create Trackbars for Thresholds
    char thresh_label[50];
    sprintf( thresh_label, "Thres: %d + input", min_threshold );
    namedWindow( probabilistic_name, WINDOW_AUTOSIZE );
    createTrackbar( thresh_label, probabilistic_name, &p_trackbar, max_trackbar, Probabilistic_Hough);
    Probabilistic_Hough(0, 0);


    // Blur images
    vector<Mat> imgs {hsv[1], ycc[1], ycc[2]};
    vector<string> title {"blur_hsv1", "blur_ycbcr1", "blur_ycbcr2"};
    vector<Mat> imgs_blur = blur_imgs(imgs, title);
    // Inversion
    imgs_blur[1] =  Scalar::all(255) - imgs_blur[1];
    display_image(imgs_blur[1],"blur_ycbcr1_inv");
	imwrite("images/img_blur_ycbcr1_inv.png", imgs_blur[1]);
	// Binarize images
    title = {"bin_hsv1", "bin_ycbcr1", "bin_ycbcr2"};
    vector<int> threshold_value {150, 140, 145};
    vector<Mat> imgs_bin = binarize_imgs(imgs_blur, title, threshold_value);

    // Images histograms
    title = {"hist_hsv1", "hist_ycbcr1", "hist_ycbcr2"};
    ////hist_imgs(imgs_blur, title); //// NOT WORKING!!!

    /*
    // Create a structuring element
    int erosion_size = 6;  
    Mat element = getStructuringElement(cv::MORPH_ELLIPSE, //cv::MORPH_DISK,
          cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
          cv::Point(erosion_size, erosion_size) );

    // Thresholding == binarize
    int threshold_value = 100;
    int threshold_type = 3;
    int const max_value = 255;
    int const max_type = 4;
    int const max_BINARY_value = 255;

    threshold( img_gray, dst2, threshold_value, max_BINARY_value,threshold_type );

    // Apply erosion or dilation on the image
    erode(dst2,dst,element);

    // dilation to the erosion
    dilate(dst,dst1,element);
    */

    // Wait for a key stroke; the same function arranges events processing
    waitKey(0);
    bool save_pictures = false;
    if(save_pictures){
        try {
            imwrite("images/img.png", img);
            imwrite("images/img_gray.png", img_gray);
            imwrite("images/img_blur.png", img_blur);
            imwrite("images/img_bin.png", bin);
        }
        catch (runtime_error& ex) {
            fprintf(stderr, "Exception converting image to PNG format: %s\n", ex.what());
            return 1;
        }

        fprintf(stdout, "Saved PNG file with alpha data.\n");                                       
    }


    //Printing white pixels
    cout << "\n\nBinary values:";
    for(int i = 0; i < 960; i+=2){
        cout << endl << "[" << i << "] ";
        for(int j = 0; j < 1280; j+=2){
            if(0!= bin.at<uchar>(i,j)) cout << bin.at<uchar>(i,j);
            else cout << " ";
        }
    }

    return 0;
}