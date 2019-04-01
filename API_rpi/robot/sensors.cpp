#include "sensors.h"


Sensors::Sensors(){
	set_MAX_LEN(MAX_LEN);
	init_items();
}
Sensors::~Sensors(){}

Sensors::Sensors(int max_len){
	set_MAX_LEN(max_len);
	init_items();
}


int Sensors::get_MAX_LEN(){return MAX_LEN;}
void Sensors::set_MAX_LEN(int i){
	MAX_LEN = i;
	this->s.set_MAX_LEN(i);
	this->x.set_MAX_LEN(i);
	this->y.set_MAX_LEN(i);
	this->th.set_MAX_LEN(i);

	this->ir1.set_MAX_LEN(i);
	this->ir2.set_MAX_LEN(i);
	this->obst_dist.set_MAX_LEN(i);
	this->obst_found.set_MAX_LEN(i);

	gyro_x.set_MAX_LEN(i);
	gyro_y.set_MAX_LEN(i);
	gyro_z.set_MAX_LEN(i);

	acc_x.set_MAX_LEN(i);
	acc_y.set_MAX_LEN(i);
	acc_z.set_MAX_LEN(i);

	comp_x.set_MAX_LEN(i);
	comp_y.set_MAX_LEN(i);
	comp_z.set_MAX_LEN(i);

	obst_found_img.set_MAX_LEN(i);
	cross.set_MAX_LEN(i);
	th_t.set_MAX_LEN(i);
	obst_dist_img.set_MAX_LEN(i);
}



void Sensors::print_info(){
	cout << endl << endl << STAR_LINE << endl << endl;
	cout << "Sensors data (last)" << endl << endl;

	cout << "  Xw:     " << x.get_last_item_noMutex() << endl;
	cout << "  Yw:     " << y.get_last_item_noMutex() << endl;
	cout << "  Thw:    " << th.get_last_item_noMutex() << endl;
	cout << "  S:      " << s.get_last_item_noMutex() << endl;
	cout << endl;
	cout << "  IR1:    " << ir1.get_last_item_noMutex() << endl;
	cout << "  IR2:    " << ir2.get_last_item_noMutex() << endl;
	cout << "  OD:     " << obst_dist.get_last_item_noMutex() << endl;
	cout << "  OF:     " << obst_found.get_last_item_noMutex() << endl;
	cout << endl;
	cout << "  Gyro_x: " << gyro_x.get_last_item_noMutex() << endl;
	cout << "  Gyro_y: " << gyro_y.get_last_item_noMutex() << endl;
	cout << "  Gyro_z: " << gyro_z.get_last_item_noMutex() << endl;
	cout << endl;
	cout << "  Acc_x:  " << acc_x.get_last_item_noMutex() << endl;
	cout << "  Acc_y:  " << acc_y.get_last_item_noMutex() << endl;
	cout << "  Acc_z:  " << acc_z.get_last_item_noMutex() << endl;
	cout << endl;
	cout << "  Comp_x: " << comp_x.get_last_item_noMutex() << endl;
	cout << "  Comp_y: " << comp_y.get_last_item_noMutex() << endl;
	cout << "  Comp_z: " << comp_z.get_last_item_noMutex() << endl;
	cout << endl;
	cout << "  OF img: " << obst_found_img.get_last_item_noMutex() << endl;
	cout << "  Cross:  " << cross.get_last_item_noMutex() << endl;
	cout << "  Tht:    " << th_t.get_last_item_noMutex() << endl;
	cout << "  OD img: " << obst_dist_img.get_last_item_noMutex() << endl;
	

	cout << endl << STAR_LINE << endl << endl;
}

void Sensors::init_items(){
	
	// set name
	this->s.set_name("S");
	this->x.set_name("Xw");
	this->y.set_name("Yw");
	this->th.set_name("Thw");

	this->ir1.set_name("ir1");
	this->ir2.set_name("ir2");
	this->obst_dist.set_name("obst_dist");
	this->obst_found.set_name("obst_found");

	this->gyro_x.set_name("gyro_x");
	this->gyro_y.set_name("gyro_y");
	this->gyro_z.set_name("gyro_z");

	this->acc_x.set_name("acc_x");
	this->acc_y.set_name("acc_y");
	this->acc_z.set_name("acc_z");

	this->comp_x.set_name("comp_x");
	this->comp_y.set_name("comp_y");
	this->comp_z.set_name("comp_z");

	this->obst_found_img.set_name("obst_found_img");
	this->cross.set_name("cross");
	this->th_t.set_name("th_t");
	this->obst_dist_img.set_name("obst_found_img");


	// initialize with one item
	this->s.add_item_noMutex(0.0);
	this->x.add_item_noMutex(0.0);
	this->y.add_item_noMutex(0.0);
	this->th.add_item_noMutex(0.0);

	this->ir1.add_item_noMutex(0.0);
	this->ir2.add_item_noMutex(0.0);
	this->obst_dist.add_item_noMutex(0.0);
	this->obst_found.add_item_noMutex(false);

	this->gyro_x.add_item_noMutex(0.0);
	this->gyro_y.add_item_noMutex(0.0);
	this->gyro_z.add_item_noMutex(0.0);

	this->acc_x.add_item_noMutex(0.0);
	this->acc_y.add_item_noMutex(0.0);
	this->acc_z.add_item_noMutex(0.0);

	this->comp_x.add_item_noMutex(0.0);
	this->comp_y.add_item_noMutex(0.0);
	this->comp_z.add_item_noMutex(0.0);

	this->obst_found_img.add_item_noMutex(false);
	this->cross.add_item_noMutex(0);
	this->th_t.add_item_noMutex(0.0);
	this->obst_dist_img.add_item_noMutex(0.0);

}



Sensors & Sensors::operator=(Sensors & sens){
	set_MAX_LEN(sens.get_MAX_LEN());
	this->s = sens.s;
	this->x = sens.x;
	this->y = sens.y;
	this->th = sens.th;

	this->ir1 = sens.ir1;
	this->ir2 = sens.ir2;
	this->obst_dist = sens.obst_dist;
	this->obst_found = sens.obst_found;

	this->gyro_x = sens.gyro_x;
	this->gyro_y = sens.gyro_y;
	this->gyro_z = sens.gyro_z;

	this->acc_x = sens.acc_x;
	this->acc_y = sens.acc_y;
	this->acc_z = sens.acc_z;

	this->comp_x = sens.comp_x;
	this->comp_y = sens.comp_y;
	this->comp_z = sens.comp_z;

	this->obst_found_img = sens.obst_found_img;
	this->cross = sens.cross;
	this->th_t = sens.th_t;
	this->obst_dist_img = sens.obst_dist_img;

	return *this;
}