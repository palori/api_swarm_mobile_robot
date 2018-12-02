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
        }
    }
    return channels;
}
 
int main( )
{
    string path = "test_images/bdrs_sally_images_20180302/images_20180302/";
    string image_name = "image_20180302_161046.359_005.png";
    string image_path = path + image_name;
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
    // Thresholding == binarize
    int threshold_value = 150;
    int threshold_type = 3;
    int const max_value = 255;
    int const max_type = 4;
    int const max_BINARY_value = 255;
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
    return 0;
}