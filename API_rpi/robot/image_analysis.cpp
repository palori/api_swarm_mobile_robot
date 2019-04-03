
#include "image_analysis.h"

Image_analysis::Image_analysis(){}

Image_analysis::~Image_analysis(){}

Image_analysis::Image_analysis(int port_image, int port_task, int image_height, int image_width){		// add other input arguments
	//Publisher pub(port_image);
	//Subscriber subs(port_task, "localhost");

	// camera params...
	//Camera cam(image_format, image_height, image_width);		//comment to compile when NO Raspi

	// ...
	this->port_image.set(port_image);
	this->port_task.set(port_task);
	this->image_height.set(image_height);
	this->image_width.set(image_width);

	cout << endl << "image params:" << endl;
	cout << "  - port img:   " << this->port_image.get() << endl;
	cout << "  - port task:  " << this->port_task.get() << endl;
	cout << "  - img heigth: " << this->image_height.get() << endl;
	cout << "  - img with:   " << this->image_width.get() << endl;

	// initialization
	this->tasks.add_item(0);
	this->message.set("Can I publish now???");// data should be encoded somewhere else (delete this line)
	

	pub_image.set_port(this->port_image.get());
	pub_image.setup();

}




// Getters
//string Image_analysis::get_message(){return message;}	// might not need to be an attr
//int Image_analysis::get_task(){return messa;}
//Mat Image_analysis::get_picture(){return picture;}

// Setters
//void Image_analysis::set_message(string s){message = s;}		// might not need to be an attr








void Image_analysis::get_new_task(){
	Subscriber subs(port_task.get(), "localhost");
	string new_task;
	int tsk;
	while(true){
		new_task = subs.listen();
		// some decoding ??
		tsk = str2int(new_task);
		this->tasks.add_item(tsk);
	}
}


void Image_analysis::send_data(){
	//message.set("Can I publish now???");// data should be encoded somewhere else (delete this line)
	
	pub_image.publish(this->message.get()); 
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

	std::thread subscribe_new_task(&Image_analysis::get_new_task, this);
	string data = "";


	while(true){
		// if or switch to run the according task
		cout << "    Task: " << tasks.get_last_item() << endl;

		// update 'data' in the running task
		// encode params in a string
		this->message.set("");
		// at the end send the data to the robot
		send_data();
	}

}