#include "sensors.h"




#ifndef ITEM 		// defined (or not) in 'utils.h'


Sensors::Sensors(){}
Sensors::Sensors(int max_len){set_MAX_LEN(max_len);}
Sensors::~Sensors(){}


// Getters
int Sensors::get_MAX_LEN(){return MAX_LEN;}

vector<float> Sensors::get_s(){
	mtx_s.lock();
	vector<float> sm = s;
	mtx_s.unlock();
	return sm;
}
vector<float> Sensors::get_x(){
	mtx_x.lock();
	vector<float> xm = x;
	mtx_x.unlock();
	return xm;
}
vector<float> Sensors::get_y(){
	mtx_y.lock();
	vector<float> ym = y;
	mtx_y.unlock();
	return ym;
}
vector<float> Sensors::get_th(){
	mtx_th.lock();
	vector<float> thm = th;
	mtx_th.unlock();
	return thm;
}

vector<float> Sensors::get_ir1(){
	mtx_ir1.lock();
	vector<float> ir1m = ir1;
	mtx_ir1.unlock();
	return ir1m;
}
vector<float> Sensors::get_ir2(){
	mtx_ir2.lock();
	vector<float> ir2m = ir2;
	mtx_ir2.unlock();
	return ir2m;
}
vector<float> Sensors::get_obst_dist(){
	mtx_od.lock();
	vector<float> odm = obst_dist;
	mtx_od.unlock();
	return odm;
}
vector<bool> Sensors::get_obst_found(){
	mtx_of.lock();
	vector<bool> ofm = obst_found;
	mtx_of.unlock();
	return ofm;
}

vector<float> Sensors::get_gyro_x(){
	mtx_gyro.lock();
	vector<float> gyro_xm = gyro_x;
	mtx_gyro.unlock();
	return gyro_xm;
}
vector<float> Sensors::get_gyro_y(){
	mtx_gyro.lock();
	vector<float> gyro_ym = gyro_y;
	mtx_gyro.unlock();
	return gyro_ym;
}
vector<float> Sensors::get_gyro_z(){
	mtx_gyro.lock();
	vector<float> gyro_zm = gyro_z;
	mtx_gyro.unlock();
	return gyro_zm;
}

vector<float> Sensors::get_acc_x(){
	mtx_acc.lock();
	vector<float> acc_xm = acc_x;
	mtx_acc.unlock();
	return acc_xm;
}
vector<float> Sensors::get_acc_y(){
	mtx_acc.lock();
	vector<float> acc_ym = acc_y;
	mtx_acc.unlock();
	return acc_ym;
}
vector<float> Sensors::get_acc_z(){
	mtx_acc.lock();
	vector<float> acc_zm = acc_z;
	mtx_acc.unlock();
	return acc_zm;
}

vector<float> Sensors::get_comp_x(){
	mtx_comp.lock();
	vector<float> comp_xm = comp_x;
	mtx_acc.unlock();
	return comp_xm;
}
vector<float> Sensors::get_comp_y(){
	mtx_comp.lock();
	vector<float> comp_ym = comp_y;
	mtx_acc.unlock();
	return comp_ym;
}
vector<float> Sensors::get_comp_z(){
	mtx_comp.lock();
	vector<float> comp_zm = comp_z;
	mtx_acc.unlock();
	return comp_zm;
}


// Setters
void Sensors::set_MAX_LEN(int i){MAX_LEN = i;}

void Sensors::set_s(float f){
	mtx_s.lock();
	s = add2vector(s, f, get_MAX_LEN());
	mtx_s.unlock();
}
void Sensors::set_x(float f){
	mtx_x.lock();
	x = add2vector(x, f, get_MAX_LEN());
	mtx_x.unlock();
}
void Sensors::set_y(float f){
	mtx_y.lock();
	y = add2vector(y, f, get_MAX_LEN());
	mtx_y.unlock();
}
void Sensors::set_th(float f){
	mtx_th.lock();
	th = add2vector(th, f, get_MAX_LEN());
	mtx_th.unlock();
}

void Sensors::set_ir1(float f){
	mtx_ir1.lock();
	ir1 = add2vector(ir1, f, get_MAX_LEN());
	mtx_ir1.unlock();
}
void Sensors::set_ir2(float f){
	mtx_ir2.lock();
	ir2 = add2vector(ir2, f, get_MAX_LEN());
	mtx_ir2.unlock();
}
void Sensors::set_obst_dist(float f){
	mtx_od.lock();
	obst_dist = add2vector(obst_dist, f, get_MAX_LEN());
	mtx_od.unlock();
}
void Sensors::set_obst_found(bool b){
	mtx_of.lock();
	obst_found = add2vector(obst_found, b, get_MAX_LEN());
	mtx_of.unlock();
}

void Sensors::set_gyro_x(float f){
	mtx_gyro.lock();
	gyro_x = add2vector(gyro_x, f, get_MAX_LEN());
	mtx_gyro.unlock();
}
void Sensors::set_gyro_y(float f1, float f2, float f3){
	mtx_gyro.lock();
	gyro_y = add2vector(gyro_y, f2, get_MAX_LEN());
	mtx_gyro.unlock();
}
void Sensors::set_gyro_z(float f){
	mtx_gyro.lock();
	gyro_z = add2vector(gyro_z, f, get_MAX_LEN());
	mtx_gyro.unlock();
}

void Sensors::set_acc_x(float f){
	mtx_acc.lock();
	acc_x = add2vector(acc_x, f, get_MAX_LEN());
	mtx_acc.unlock();
}
void Sensors::set_acc_y(float f){
	mtx_acc.lock();
	acc_y = add2vector(acc_y, f, get_MAX_LEN());
	mtx_acc.unlock();
}
void Sensors::set_acc_z(float f){
	mtx_acc.lock();
	acc_z = add2vector(acc_z, f, get_MAX_LEN());
	mtx_acc.unlock();
}

void Sensors::set_comp_x(float f){
	mtx_comp.lock();
	comp_x = add2vector(comp_x, f, get_MAX_LEN());
	mtx_comp.unlock();
}
void Sensors::set_comp_y(float f){
	mtx_comp.lock();
	comp_y = add2vector(comp_y, f, get_MAX_LEN());
	mtx_comp.unlock();
}
void Sensors::set_comp_z(float f){
	mtx_comp.lock();
	comp_z = add2vector(comp_z, f, get_MAX_LEN());
	mtx_comp.unlock();
}
















#else  // ITEM


Sensors::Sensors(){}
Sensors::Sensors(int max_len){set_MAX_LEN(max_len);}
Sensors::~Sensors(){}



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

	this->gyro_x.set_MAX_LEN(i);
	this->gyro_y.set_MAX_LEN(i);
	this->gyro_z.set_MAX_LEN(i);

	this->acc_x.set_MAX_LEN(i);
	this->acc_y.set_MAX_LEN(i);
	this->acc_z.set_MAX_LEN(i);

	this->comp_x.set_MAX_LEN(i);
	this->comp_y.set_MAX_LEN(i);
	this->comp_z.set_MAX_LEN(i);	
}

#endif // ITEM