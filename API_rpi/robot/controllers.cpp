#include "controllers.h"

Controllers::Controllers(){
	set_MAX_LEN(MAX_LEN);
	init_items();
}
Controllers::~Controllers(){}


// Getters
int Controllers::get_MAX_LEN(){return MAX_LEN;}




void Controllers::set_MAX_LEN(int i){
	MAX_LEN = i;
	this->reset_enc.set_MAX_LEN(i);
	this->stop.set_MAX_LEN(i);
	this->avoid_obst.set_MAX_LEN(i);
	this->obst_dist.set_MAX_LEN(i);

	this->ir_on.set_MAX_LEN(i);
	this->ir_send.set_MAX_LEN(i);

	this->imu_on.set_MAX_LEN(i);
	this->imu_gyro_send.set_MAX_LEN(i);
	this->imu_acc_send.set_MAX_LEN(i);
	this->imu_comp_send.set_MAX_LEN(i);

	this->motors_on.set_MAX_LEN(i);
	this->vel.set_MAX_LEN(i);

	this->m1_kp.set_MAX_LEN(i);
	this->m1_ki.set_MAX_LEN(i);
	this->m2_kp.set_MAX_LEN(i);
	this->m2_ki.set_MAX_LEN(i);
	this->th_kp.set_MAX_LEN(i);
	this->th_ki.set_MAX_LEN(i);

	this->fwd_dist.set_MAX_LEN(i);
	this->trn_deg.set_MAX_LEN(i);
	this->trn_r.set_MAX_LEN(i);
	this->s.set_MAX_LEN(i);

	this->x_t.set_MAX_LEN(i);
	this->y_t.set_MAX_LEN(i);
	this->th_t.set_MAX_LEN(i);
}



void Controllers::print_info(){
	cout << endl << endl << STAR_LINE << endl << endl;
	cout << "Controllers params. (last)" << endl << endl;

	cout << "  reset_enc:     " << reset_enc.get_last_item_noMutex() << endl;
	cout << "  stop:          " << stop.get_last_item_noMutex() << endl;
	cout << "  avoid_obst:    " << avoid_obst.get_last_item_noMutex() << endl;
	cout << "  obst_dist:     " << obst_dist.get_last_item_noMutex() << endl;
	cout << endl;
	cout << "  ir_on:         " << ir_on.get_last_item_noMutex() << endl;
	cout << "  ir_send:       " << ir_send.get_last_item_noMutex() << endl;
	cout << endl;
	cout << "  imu_on:        " << imu_on.get_last_item_noMutex() << endl;
	cout << "  imu_gyro_send: " << imu_gyro_send.get_last_item_noMutex() << endl;
	cout << "  imu_acc_send:  " << imu_acc_send.get_last_item_noMutex() << endl;
	cout << "  imu_comp_send: " << imu_comp_send.get_last_item_noMutex() << endl;
	cout << endl;
	cout << "  motors_on:     " << motors_on.get_last_item_noMutex() << endl;
	cout << "  vel:           " << vel.get_last_item_noMutex() << endl;
	cout << endl;
	cout << "  m1_kp:         " << m1_kp.get_last_item_noMutex() << endl;
	cout << "  m1_ki:         " << m1_ki.get_last_item_noMutex() << endl;
	cout << "  m2_kp:         " << m2_kp.get_last_item_noMutex() << endl;
	cout << "  m2_ki:         " << m2_ki.get_last_item_noMutex() << endl;
	cout << "  th_kp:         " << th_kp.get_last_item_noMutex() << endl;
	cout << "  th_ki:         " << th_ki.get_last_item_noMutex() << endl;
	cout << endl;
	cout << "  fwd_dist:      " << fwd_dist.get_last_item_noMutex() << endl;
	cout << "  trn_deg:       " << trn_deg.get_last_item_noMutex() << endl;
	cout << "  trn_r:         " << trn_r.get_last_item_noMutex() << endl;
	cout << "  s:             " << s.get_last_item_noMutex() << endl;
	cout << endl;
	cout << "  Xt:            " << x_t.get_last_item_noMutex() << endl;
	cout << "  Yt:            " << y_t.get_last_item_noMutex() << endl;
	cout << "  Tht:           " << th_t.get_last_item_noMutex() << endl;

	cout << endl << STAR_LINE << endl << endl;
}

void Controllers::init_items(){
	
	// set name
	this->reset_enc.set_name("reset_enc");
	this->stop.set_name("stop");
	this->avoid_obst.set_name("avoid_obst");
	this->obst_dist.set_name("obst_dist");

	this->ir_on.set_name("ir_on");
	this->ir_send.set_name("ir_send");

	this->imu_on.set_name("imu_on");
	this->imu_gyro_send.set_name("imu_gyro_send");
	this->imu_acc_send.set_name("imu_acc_send");
	this->imu_comp_send.set_name("imu_comp_send");

	this->motors_on.set_name("motors_on");
	this->vel.set_name("vel");

	this->m1_kp.set_name("m1_kp");
	this->m1_ki.set_name("m1_ki");
	this->m2_kp.set_name("m2_kp");
	this->m2_ki.set_name("m2_ki");
	this->th_kp.set_name("th_kp");
	this->th_ki.set_name("th_ki");

	this->fwd_dist.set_name("fwd_dist");
	this->trn_deg.set_name("trn_deg");
	this->trn_r.set_name("trn_r");
	this->s.set_name("S");

	this->x_t.set_name("Xt");
	this->y_t.set_name("Yt");
	this->th_t.set_name("Tht");



	// initialize with one item
	this->reset_enc.add_item_noMutex(false);
	this->stop.add_item_noMutex(false);
	this->avoid_obst.add_item_noMutex(false);
	this->obst_dist.add_item_noMutex(0.0);

	this->ir_on.add_item_noMutex(false);
	this->ir_send.add_item_noMutex(false);

	this->imu_on.add_item_noMutex(false);
	this->imu_gyro_send.add_item_noMutex(false);
	this->imu_acc_send.add_item_noMutex(false);
	this->imu_comp_send.add_item_noMutex(false);

	this->motors_on.add_item_noMutex(false);
	this->vel.add_item_noMutex(0.0);

	this->m1_kp.add_item_noMutex(0.0);
	this->m1_ki.add_item_noMutex(0.0);
	this->m2_kp.add_item_noMutex(0.0);
	this->m2_ki.add_item_noMutex(0.0);
	this->th_kp.add_item_noMutex(0.0);
	this->th_ki.add_item_noMutex(0.0);

	this->fwd_dist.add_item_noMutex(0.0);
	this->trn_deg.add_item_noMutex(0.0);
	this->trn_r.add_item_noMutex(0.0);
	this->s.add_item_noMutex(0.0);

	this->x_t.add_item_noMutex(0.0);
	this->y_t.add_item_noMutex(0.0);
	this->th_t.add_item_noMutex(0.0);

}
