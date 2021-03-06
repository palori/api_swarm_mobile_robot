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
int threshold_value = 100;//150;
int threshold_type = 3;
int const max_value = 255;
int const max_type = 4;
int const max_BINARY_value = 255;
int lowThreshold = 0;
const int ratio = 3;
const int kernel_size = 3;

/////////////////////////////////
bool is_color_edge = false;
int edgeThresh = 110;
Mat cedge;
const char* window_name1 = "Edge map : Canny default (Sobel gradient)";



Mat img, img_blur, img_th, img_canny;

void display_image(Mat img, string title)
{
    namedWindow( title, WINDOW_AUTOSIZE );  
    imshow( title, img );
}

static void onTrackbar(int, void*)
{
    //blur(gray, blurImage, Size(3,3));
    // Run the edge detector on grayscale
    Canny(img_blur, img_canny, edgeThresh, edgeThresh*3, 3);
    cedge = Scalar::all(0);
    if (is_color_edge)
    {
        image.copyTo(cedge, img_canny);
        imshow(window_name1, cedge);
    }
    else
    {
        imshow(window_name1, edge1);
    }
    //imwrite("images/img_edge.png", edge1);
}


int main( ){

    string image_path = "pic.jpg"; 
    // image_path = "image_example.png";
	img = imread(image_path, CV_LOAD_IMAGE_COLOR); 



	


    
    //img = imread(image_path, CV_LOAD_IMAGE_COLOR);  
    cout << "Image size: " <<  img.size() << endl;
    cout << "Image channels: " <<  img.channels() << endl;

    //Mat img_gray, img_blur, img_th, img_canny;
 
    cvtColor(img, img_gray, COLOR_RGB2GRAY);
	blur(img_gray, img_blur, Size(5,5));
    //threshold(img_blur, img_th, threshold_value, max_BINARY_value,threshold_type);
	//adaptiveThreshold(img_blur, img_th, max_BINARY_value, ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 5, 5);
    //Canny(img_th, img_canny, lowThreshold, lowThreshold * ratio, kernel_size);
    

    // Create a window
    namedWindow(window_name1, 1);
    // create a toolbar
    createTrackbar("Canny threshold default", window_name1, &edgeThresh, 255, onTrackbar);
    // Show the image
    onTrackbar(0, 0);


    display_image(img, "img");
	display_image(img_gray, "img_gray");
	display_image(img_blur, "img_blur");
    //display_image(img_th, "img_th");
    display_image(img_canny,"img_canny");

    // Wait for a key stroke; the same function arranges events processing
    waitKey(0);


    imwrite("pic_gray.jpg",img_gray);
    imwrite("pic_blur.jpg",img_blur);
    imwrite("pic_th.jpg",img_th);
    imwrite("pic_canny.jpg",img_canny);

	//Printing white pixels
	/*cout << "\n\nBinary values:";
	for(int i = 0; i < 960; i++){
		cout << endl << "[" << i << "] ";
	    for(int j = 0; j < 1280; j++){
	        //if(255 == bin.at<uchar>(i,j))
	        cout << bin.at<uchar>(i,j) << " ";
	    }
	}*/


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
