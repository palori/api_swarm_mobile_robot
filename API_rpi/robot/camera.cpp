
#include "camera.h"


Camera::Camera(){}
Camera::~Camera(){}

Camera::Camera(int image_height, int image_width){
	set_image_height(image_height);
	set_image_width(image_width);
}

// Getters
//raspicam::RaspiCam_Cv get_camera();
double Camera::get_image_format(){return image_format;}
int Camera::get_image_height(){return image_height;}
int Camera::get_image_width(){return image_width;}
int Camera::get_focal_length(){return focal_length;}


// Setters
void Camera::set_image_format(double d){image_format = d;}
void Camera::set_image_height(int i){image_height = i;}
void Camera::set_image_width(int i){image_width = i;}
void Camera::set_focal_length(int i){focal_length = i;}



int Camera::init(){
	camera.set( CV_CAP_PROP_FORMAT, get_image_format() );
	camera.set(CV_CAP_PROP_FRAME_HEIGHT, get_image_height());
	camera.set(CV_CAP_PROP_FRAME_WIDTH, get_image_width());
	//Camera.set(CV_CAP_PROP_EXPOSURE, 100);
}

int Camera::start(){
	//Open camera
	cout<<"Opening Camera..."<<endl;
	if (!Camera.open()) {cerr<<"Error opening the camera"<<endl;return -1;}
	//Camera.set(CV_CAP_PROP_EXPOSURE, 100);
	usleep(2000000);
	return 0;	
}

void Camera::stop(){
	cout<<"Stop camera..."<<endl;
	Camera.release();
}

Mat Camera::take_picture(){
	Mat img;
	Camera.grab();
	Camera.retrieve (img);
	return img;
}