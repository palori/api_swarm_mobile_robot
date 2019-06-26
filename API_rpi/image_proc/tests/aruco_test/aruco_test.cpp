
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
#include "../../../comm/comm_rpi_2.h"

#define PI 3.14159265
#define LOCALIZE 0
#define COUPLE 1

using namespace cv;
using namespace std;

raspicam::RaspiCam_Cv Camera;
COMM_RPI cr;

vector<Vec4d> aruco_markers;
double th_x = - 25 * PI / 36;
double th_z = - PI / 2;

Vec3d tr = {0.0366 , 0.0 , 0.1163};


float markerSize = 0.02;

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
	aruco_markers.push_back(Vec4d(4,0,0,0));
	aruco_markers.push_back(Vec4d(5,0,0,0));
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

double getTheta(double Rc[3][3], double Rx[3][3], double Rz[3][3]){

	double Rtemp[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
	double R[3][3] = {{0,0,0},{0,0,0},{0,0,0}};

	for (int i=0; i<3; i++){
		for (int j=0; j<3; j++){

			Rtemp[i][j] = Rz[i][0] * Rx[0][j] + Rz[i][1] * Rx[1][j] + Rz[i][2] * Rx[2][j]; 
			if (fabs(Rtemp[i][j]) < 0.001) Rtemp[i][j] = 0;
		}

	}



	for (int i=0; i<3; i++){
		for (int j=0; j<3; j++){

			R[i][j] = Rtemp[i][0] * Rc[0][j] + Rtemp[i][1] * Rc[1][j] + Rtemp[i][2] * Rc[2][j]; 
			if (fabs(R[i][j]) < 0.001) R[i][j] = 0;
			cout << R[i][j] << " " ;
		}
		cout << endl;ž
	}

	return 0.0;
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
	cout << "pose in robot coordinates: " << endl;
	printPose(pose);

	pose[2] = getTheta(Rc,Rx,Rz);

	return pose;
}



Vec3d getDrivingParameters(int id, Vec3d pose){

	double x1 = 0;
	double y1 = 0;
	switch (id) {
		case 3:
			x1 = 0.2;
			y1 = 0.1; 
			break;
		case 4: 
			x1 = 0.15;
			y1 = 0.0;
			break;
		case 5:
			x1 = 0.2;
			y1 = -0.1;
			break;
	} 	
	double th = atan2(pose[1],pose[0]);
	double th1 = atan2(y1,x1);
	double th0 = th - th1;
	double x0 = pose[0] - cos(th0) * x1 + sin(th0) * y1;
	double y0 = pose[1] - sin(th0) * x1 - cos(th0) * y1;

	cout << "th: " << th << endl;
	cout << "th1: " << th1 << endl;
	cout << "th0: " << th0 << endl;
	cout << "x0: " << x0 << endl;
	cout << "y0: " << y0 << endl;

	Vec3d parameters;

	parameters[0] = atan2(y0,x0);
	parameters[1] = sqrt(pow(x0,2) + pow(y0,2));
	parameters[2] = -parameters[0];
	
	printPose(parameters);

	return parameters;
}


int detectAruco(int mode){

	//start time measurement
	double function_time = (double)getTickCount();

	double focal_length = 1011.454; //for camera on brown shell robot
	double dx = 640;
	double dy = 480;
	Mat cameraMatrix = (Mat_<double>(3,3) << focal_length,0,dx,0,focal_length,dy,0,0,1);
	Mat distCoeffs = (Mat_<double>(1,5) << 0.2214,-0.5639,0,0,0.448);
	vector <Vec3d> rvecs,tvecs;
	vector<int> markerIds;
	vector<vector<Point2f>> markerCorners, rejectedCandidates;
	Ptr<aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(aruco::DICT_4X4_50);
	//string name = "pics/aruco_"+to_string(i)+".png";
	//Mat inputImage = imread(name,CV_LOAD_IMAGE_GRAYSCALE);
	Mat inputImage;
	Camera.grab();
	Camera.retrieve(inputImage);
	cv::aruco::detectMarkers(inputImage, dictionary, markerCorners, markerIds);
	string msg = "@a=16,b=1,v=0.3,trn=0.3$";
	cout << "detection: " << endl;
	while (!markerIds.size()){

		cr.serial_write(msg);
		usleep(2000000);
		Camera.grab();
		Camera.retrieve(inputImage);
		cv::aruco::detectMarkers(inputImage, dictionary, markerCorners, markerIds);

	}
	if (mode == COUPLE){
		Vec3d drivingParameters = {0,0,0};
		for (int i = 0; i < markerIds.size(); i++){
			if (markerIds[i] > 2 && markerIds[i] < 6) {
				cout << "draw markers: " << endl;
				cv::aruco::drawDetectedMarkers(inputImage, markerCorners, markerIds);
				cout << "estimate pose: " << endl;
				cv::aruco::estimatePoseSingleMarkers(markerCorners,markerSize,cameraMatrix,distCoeffs,rvecs,tvecs);
				cout << "draw axis: " << endl;
				cv::aruco::drawAxis(inputImage, cameraMatrix, distCoeffs, rvecs, tvecs, 0.1); //giving exceptions
				cout << "get pose: " << endl;
				Vec3d pose = getPose(markerIds[i],rvecs[i],tvecs[i]);
				for(int j=0; j<3; j++){
					cout << rvecs[i][j] << " " ;
				}
				cout << endl;
				drivingParameters = getDrivingParameters(markerIds[i],pose);
				break;
			}
		}
		
		

		msg = "@a=16,b=1,v=0.3,trn=" + to_string(drivingParameters[0])+"$";
		//cr.serial_write(msg);
		//usleep(5000000);
		msg = "@a=15,b=1,v=0.3,fwd=" + to_string(drivingParameters[1])+"$";
		//cr.serial_write(msg);
		//usleep(5000000);
		msg = "@a=16,b=1,v=0.3,trn=" + to_string(drivingParameters[2])+"$";
		//cr.serial_write(msg);
		//usleep(5000000);

	}

	
	/*
	for (int i = 0; i < markerIds.size(); i++){
		cv::aruco::drawDetectedMarkers(inputImage, markerCorners, markerIds);
		cv::aruco::estimatePoseSingleMarkers(markerCorners,markerSize,cameraMatrix,distCoeffs,rvecs,tvecs);
		cv::aruco::drawAxis(inputImage,cameraMatrix,distCoeffs,rvecs,tvecs,0.1);
		Vec3d pose = getPose(markerIds[i],rvecs[i],tvecs[i]);	
	}*/



  	string pic_name_img = "pics/aruco_detected.png";
	imwrite(pic_name_img,inputImage);

  	function_time = ((double)getTickCount()-function_time)/getTickFrequency();
	cout << "Function time: " << function_time << endl;

	return markerIds.size();
}



void takePic(int i){
	
	Mat pic;
	Camera.grab();
	Camera.retrieve(pic);
	string pic_name = "pics/aruco_"+to_string(i)+".png";
	imwrite(pic_name, pic);
	
}



int main(){


	camera_start();
	cr.serial_open();

	detectAruco(COUPLE);
	

	cr.serial_close();
	camera_stop();		

	return 0;
}
