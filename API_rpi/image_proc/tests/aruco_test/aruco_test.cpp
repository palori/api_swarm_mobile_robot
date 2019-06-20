
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/aruco.hpp"

#include "opencv2/core/core.hpp"

//#include "opencv2/core/utility.hpp"
//#include "opencv2/imgcodecs.hpp"
#include <raspicam/raspicam_cv.h>

#include <ctime>
#include <unistd.h>
#include <iostream>
//#include <stdlib.h>
#include <stdio.h>
#include <cmath>

#include <string.h>
#include <string>
#include <vector>

#include <signal.h> //deprecated, see csignal
//#include <csignal>

#define PI 3.14159265



using namespace cv;
using namespace std;

raspicam::RaspiCam_Cv Camera;

vector<Vec4d> aruco_markers;
double th_x = - 25 * PI / 36;
double th_z = - PI / 2; 
Vec3d tr = {0.0366 , 0.0 , 0.1263};

float markerSize = 0.03;
int camera_aruco_init(){
	Camera.set( CV_CAP_PROP_FORMAT, CV_8UC1 );
	Camera.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	Camera.set(CV_CAP_PROP_FRAME_HEIGHT, 960);
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


void initializeMarkers (){

	aruco_markers.push_back(Vec4d(3,0,0,0));


}

Vec3d getMarkerPose(int id){

	Vec3d markerPose = {0,0,0};
	for (unsigned int i=0;i < aruco_markers.size();i++){
		if (aruco_markers[i][0] == id) {
			markerPose[0]=aruco_markers[i][1];
			markerPose[1]=aruco_markers[i][2];
			markerPose[2]=aruco_markers[i][3];

			cout << "MATCH!" << endl;
		}
	}

	return markerPose;
}

Vec3d transform(double R[3][3], Vec3d t, Vec3d pose){   // from smaller to bigger coordinate system

		Vec3d newPose = {0,0,0};

		for (unsigned int i=0;i<3;i++){
			newPose[i] = R[i][0] * pose[0] + R[i][1] * pose[1] + R[i][2] * pose[2] + t[i];
		}

	return newPose;
}

void printPose(Vec3d pose){

	cout << "New pose: ";

	for(unsigned int i=0;i<3;i++){
			cout << pose[i] << " ";
	}
	cout << endl;

}

Vec3d getPose(int id, Vec3d r, Vec3d t){

	Vec3d markerPose = getMarkerPose(id);

	double x_m = markerPose[0];
	double y_m = markerPose[1];
	double z_m = markerPose[2];

	double Rc[3][3];
	Mat Rcam;
	Rodrigues(r,Rcam);

	//cout << "Rcam: " << Rcam << endl;

	//cout << "Rc: " << endl;
	for (int i=0;i<3;i++){
		for (int j=0;j<3;j++){
			
			Rc[i][j] = Rcam.at<double>(i,j);
			//cout << Rc[i][j] << " ";
		}
		cout << endl;
	}

	Vec3d pose = transform(Rc, t, markerPose);
	//cout << "pose in camera coordinate system: " << endl;
	//printPose(pose);

	double Rx[3][3] = {{1,0,0},{0,cos(th_x),-sin(th_x)},{0,sin(th_x),cos(th_x)}};
	pose = transform(Rx, {0,0,0}, pose);
	//cout << "pose after x rotation: " << endl;
	//printPose(pose);


	double Rz[3][3] = {{cos(th_z),-sin(th_z),0},{sin(th_z),cos(th_z),0},{0,0,1}};
	pose = transform(Rz, tr , pose);
	cout << "pose after z rotation: " << endl;
	printPose(pose);

	return pose;
}



void detectAruco(int i){

	double focal_length = 1011.454; //for camera on brown shell robot
	double dx = 640;
	double dy = 480;
	Mat cameraMatrix = (Mat_<double>(3,3) << focal_length,0,dx,0,focal_length,dy,0,0,1);
	Mat distCoeffs = (Mat_<double>(1,5) << 0.2214,-0.5639,0,0,0.448);
	vector <Vec3d> rvecs,tvecs;
	//string name = "pics/aruco_"+to_string(i)+".png";
	//Mat inputImage = imread(name,CV_LOAD_IMAGE_GRAYSCALE);
	Mat inputImage;
	Camera.grab();
	Camera.retrieve(inputImage);

	vector<int> markerIds;
	vector<vector<Point2f>> markerCorners, rejectedCandidates;
	Ptr<aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(aruco::DICT_4X4_50);
	cv::aruco::detectMarkers(inputImage, dictionary, markerCorners, markerIds);
	if (markerIds.size()>0){
		cv::aruco::drawDetectedMarkers(inputImage, markerCorners, markerIds);
		cv::aruco::estimatePoseSingleMarkers(markerCorners,markerSize,cameraMatrix,distCoeffs,rvecs,tvecs);
		cv::aruco::drawAxis(inputImage,cameraMatrix,distCoeffs,rvecs,tvecs,0.1);
		Vec3d pose = getPose(markerIds[0],rvecs[0],tvecs[0]);	
	}
	/*if (rvecs.size()){
		cout << "rvecs: ";
		for(unsigned int i=0;i<3;i++){

				cout << rvecs[0][i] << " ";
		}
		cout << endl;
	}
	if (tvecs.size()){
		cout << "tvecs: ";
		for(unsigned int i=0;i<3;i++){

				cout << tvecs[0][i] << " ";
		}
		cout << endl;
	}*/
	//string window_name = "ARUCO_"+to_string(i);
  	//namedWindow( window_name, CV_WINDOW_AUTOSIZE );
  	//imshow( window_name , inputImage);
  	//string pic_name_img = "pics/aruco_detected_"+to_string(i)+".png";
	//imwrite(pic_name_img,inputImage);
  	//waitKey(0);

}

void takePic(int i){
	
	Mat pic;
	Camera.grab();
	Camera.retrieve(pic);
	string pic_name = "pics/aruco_"+to_string(i)+".png";
	imwrite(pic_name, pic);
	
}



int main(){

	
	//initializeMarkers();
	camera_start();
	//cout << "shape_color: " << endl << shape_color() << endl;
	//cout << "ARUCO DETECTION: "  << endl;
	int k=0;
	while (k<20){
		cout << to_string(k) << ". try: " << endl;
		detectAruco(k);
		usleep(5000000);
		k++;
	}
	camera_stop();		

	return 0;
}
