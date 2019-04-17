#include "computer_vision.h"
// global variables

double old_left = 0.0;
double old_right = 0.0;
crossing feature = NO_CROSS;
bool t_main_candidate = false;  
bool y_split_candidate = false;
bool t_left_candidate = false;
bool t_right_candidate = false;
bool cross_candidate = false;


// functions
void idle(){}
 

string follow_line(Mat img1,Side side){

	Mat img;
	cvtColor(img1,img,CV_BGR2GRAY);
		
	int lowThreshold=170; //% = 40;
	const int thres_ratio = 4;
	const int kernel_size = 3;
	const int CAM_H = 240;
	const int CAM_W = 320;
	
	//histogram stretch
	Mat img_hist;
	HistStretch(img,img_hist);
	
	//gamma mapping
	Mat img_gamma;
	float gamma = 3;	
	GammaMapping(img_hist, img_gamma, gamma);	
	
	//blurring
	Mat img_blur;
	blur(img_gamma, img_blur, Size(3,3));

	//adaptive Canny thresholding;
	Mat  img_canny, canny_edges, img_canny_crop, img_cont;
	vector<vector<Point>> contours;
	vector<vector<Point>> good_contours;
	vector<Vec4i> hierarchy;
	bool bad_threshold = true;

	// count to know that the message is different!
	int count = 0;

	while (bad_threshold){
		good_contours.clear();
		contours.clear();
		hierarchy.clear();
		
		//canny edges
		Canny(img_blur, canny_edges, lowThreshold, lowThreshold + 50 , kernel_size);
		img_canny = Scalar::all(0);
		img_gamma.copyTo(img_canny, canny_edges);

		//cropping
		img_canny_crop = img_canny.clone();
		img_canny_crop = img_canny_crop(Rect(0,CAM_H*0.5,CAM_W,CAM_H*0.5));
		
		//contours
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
		if (good_contours.size()==0) lowThreshold-=50;
		else bad_threshold = false;
		if (lowThreshold<50) bad_threshold = false;
		
	}



	//sort contours by arc length - assuming line contours are longer than noise contours
	sort(good_contours.begin(),good_contours.end(),compareContoursHeight);

	//initialize left and right contour
	vector<Point> left_contour;
	double left_cm;
	vector<Point> right_contour;
	double right_cm;

	if (good_contours.size()>0){
		feature = NO_CROSS;
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
					feature = T_MIDDLE;
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
				feature = Y;
				y_split_candidate = false;
			} else y_split_candidate = true;
		} else y_split_candidate = false;

	} else {
		feature = NO_CROSS;
		//cout << "no line found !!!"<<endl;
		left_cm=CAM_W/2;
		right_cm=CAM_W/2;
	}

	cout << " FEATURE: " << feature << endl;

	float cm = CAM_W/2;

	switch(side){

		case LEFT:
			cm = left_cm;  
			break;
		case MIDDLE:
			cm = (left_cm + right_cm)/2;
			break;
		case RIGHT:
			cm = right_cm;
			break;
	}
	
	cm -= CAM_W/2; 

	cout<<"CM: "<<cm<<endl;
	
	old_left = left_cm;
	old_right = right_cm;
	bool obstacle = false;
	if (feature != NO_CROSS) obstacle = true;

	count++;	// count to know that the message is different!
	if (count >= 10) count = 0;

	//return string composed of feature and theta for line following
	return encode_image_params(LINE,obstacle, 0.0, cm, feature, count);
}


string track_ball(Mat img){

	//convert to hsv
	Mat img_hsv;
	cvtColor(img, img_hsv, COLOR_BGR2HSV);
	int CAM_W = 320;

	//threshold for red/orange color
	Mat mask1,mask2,gray;
	inRange(img_hsv,Scalar(0,70,50),Scalar(10,255,255),mask1);
	inRange(img_hsv,Scalar(170,70,50),Scalar(180,255,255),mask2);
	gray = mask1 | mask2;

	//blurring
	GaussianBlur(gray,gray, Size(9,9),2,2);

	//find circles
	vector<Vec3f> circles;
	HoughCircles( gray, circles, CV_HOUGH_GRADIENT, 1, 30, 30, 15, 10, 50 );  // ... canny edge parameter, number of points needed, min radius, max radius 

	sort(circles.begin(),circles.end(),compareCircles);

	if (circles.size() == 0 ) 
		return encode_image_params(BALL, false, 0.0, 0.0, 0,1);
	else {
		Vec3f ball = circles[0];
		float theta = ball[0] - CAM_W / 2;
		cout << "theta: " << theta << endl; 
		return encode_image_params(BALL, true, 0.0, theta, 0,1); 
	}

}
string hole(Mat img){return "";}
string shape(Mat img){return "";}
string ArUco(Mat img){return "";}

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

bool compareContoursHeight(vector<Point> contour1, vector<Point> contour2){
	Rect rect1 = boundingRect(contour1);
	Rect rect2 = boundingRect(contour2);
	return (rect1.height > rect2.height);
}

bool compareCircles(Vec3f circle1, Vec3f circle2){
	return (circle1[1]>circle2[1]);   //the one with largest y-coordinate is the lowest in the picture and most likely to be a ball
}





// other functions needed
