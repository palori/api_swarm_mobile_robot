
#include "image_analysis.h"

Image_analysis::Image_analysis(){}

Image_analysis::~Image_analysis(){}

Image_analysis::Image_analysis(int port_image, int port_task){		// add other input arguments
	//Publisher pub(port_image);
	//Subscriber subs(port_task, "localhost");


	
	// ...
	this->port_image.set(port_image);
	this->port_task.set(port_task);
	//this->image_height.set(image_height);
	//this->image_width.set(image_width);

	cout << endl << "image params:" << endl;
	cout << "  - port img:   " << this->port_image.get() << endl;
	cout << "  - port task:  " << this->port_task.get() << endl;
	//cout << "  - img heigth: " << this->image_height.get() << endl;
	//cout << "  - img with:   " << this->image_width.get() << endl;

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
		//tsk = str2int(new_task);
		//this->tasks.add_item(tsk);
		decode_task(new_task,this->tasks);
	}
}


void Image_analysis::send_data(){
	//message.set("Can I publish now???");// data should be encoded somewhere else (delete this line)
	
	pub_image.publish(this->message.get()); 
}


void Image_analysis::take_picture(){
	Camera cam;
	cam.init();
	cam.start();

	int old_task = -1, task = -1;
	Mat pic;


	while (true){
		task = tasks.get_last_item();
		if (old_task != task){
			cam.stop();
			if (task == LINE){
				cam.set_image_format(CV_8UC1);
				cam.set_image_height(240);
				cam.set_image_width(320);
			}
			else if (task == BALL){
				cam.set_image_format(CV_8UC3);
				cam.set_image_height(240);
				cam.set_image_width(320);
			}
			else if (task == HOLE){
				cam.set_image_format(CV_8UC1);
				cam.set_image_height(240);
				cam.set_image_width(320);
			}
			else if (task == SHAPE){
				cam.set_image_format(CV_8UC3);
				cam.set_image_height(240);
				cam.set_image_width(320);
			}
			else if (task == ARUCO){
				cam.set_image_format(CV_8UC1);
				cam.set_image_height(960);
				cam.set_image_width(1280);
			}
			else{
				
			}
			cam.init();
			old_task = task;
			cam.start();
		}
		else {
			pic = cam.take_picture();
			picture.set(pic);
		}
	}
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




void Image_analysis::run(){

	std::thread subscribe_new_task(&Image_analysis::get_new_task, this);
	std::thread subscribe_pictures(&Image_analysis::take_picture, this);
	string data = "";
	int task;

	//float theta = 0; // only for testing
	while(true){
		// if or switch to run the according task

		cout << "    Task: " << task << endl;
		task = tasks.get_last_item();

		if (task == LINE) data = follow_line(picture.get(),MIDDLE);
		else if (task == BALL) data = ball(picture.get());
		else if (task == HOLE) data = hole(picture.get());
		else if (task == SHAPE) data = shape(picture.get());
		else if (task == ARUCO) data = ArUco(picture.get());




		// update 'data' in the running task
		// encode params in a string
		//theta++;
		//data = encode_image_params(10, 1, 0.67, theta, 3);
		//data = "@of_i=1,cr=2,tht=40,od_i=0.23$";
		this->message.set(data);

		int millis_sleep = 5000;
		this_thread::sleep_for(chrono::milliseconds(millis_sleep));

		// at the end send the data to the robot
		send_data();
	}

}