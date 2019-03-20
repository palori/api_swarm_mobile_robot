
#include "image_analysis.h"

Image_analysis::Image_analysis(){}

Image_analysis::~Image_analysis(){}

Image_analysis::Image_analysis(int publisher_port, int subscriber_port, double image_format, int image_height, int image_width){		// add other input arguments
	Publisher pub(publisher_port);
	Subscriber subs(subscriber_port, "localhost");

	// camera params...
	Camera cam(image_format, image_height, image_width);

	// ...
}




// Getters
string Image_analysis::get_message(){return message;}	// might not need to be an attr
int Image_analysis::get_task(){
	mtx_task.lock();
	int taskm = task;
	mtx_task.unlock();
	return taskm;
}
int Image_analysis::get_old_task(){return old_task;}


// Setters
void Image_analysis::set_message(string s){message = s;}		// might not need to be an attr
void Image_analysis::set_task(int i){
	mtx_task.lock();
	task = i;
	mtx_task.unlock();
}
void Image_analysis::set_old_task(int i){task = i;}








void Image_analysis::get_new_task(){

	string new_task;
	int tsk;
	while(true){
		new_task = subs.listen();
		// some decoding ??
		tsk = str2int(new_task);
		set_task(tsk);
	}
}


void Image_analysis::send_data(){
	set_message("Can I publish now???");// data should be encoded somewhere else (delete this line)
	pub.publish(get_message()); 
}







/*
	at the beginning of each task evaluate:
void task(){

	// initialize camera if new task
	if(get_task() != get_old_task()){
		// close camera and open again with params for this task
		cam.stop();
		cam.set_image_format(double d);
		cam.set_image_height(int i);
		cam.set_image_width(int i);
		cam.set_focal_length(int i);
		cam.init();
		cam.start();
		set_old_task(get_task());
	}

	// do the actual task
}
*/

void Image_analysis::idle(){}
void Image_analysis::follow_line(){}
void Image_analysis::crossing(){}		// ?? maybe not needed, integrated in 'follow_line'
void Image_analysis::ball(){}
void Image_analysis::shape(){}
void Image_analysis::obstacle(){}		// ?? maybe not needed
void Image_analysis::ArUco(){}












void Image_analysis::run(){

	std::thread subscribe_new_task(get_new_task);
	string data = "";

	while(true){
		// if or switch to run the according task


		// update 'data' in the running task
		// encode params in a string
		set_message("");
		// at the end send the data to the robot
		send_data();
	}

}