#include "sensors.h"

using namespace std;


Sensors::Sensors();
Sensors::~Sensors();

// Attributes
float s;
float x;
float y;
float th;

float ir1;
float ir2;
float obst_dist;
bool obst_found;

float gyro[3];
float acc[3];
float comp[3];


// Mutex (for critical sections, reading and writing attributes from different threads)
mutex mtx_s;
mutex mtx_x;
mutex mtx_y;
mutex mtx_th;

mutex mtx_ir1;
mutex mtx_ir2;
mutex mtx_od;
mutex mtx_of;

mutex mtx_gyro;
mutex mtx_acc;
mutex mtx_comp;


// Getters
float Sensors::get_s(){
	mtx_s.lock();
	float sm = s;
	mtx_s.unlock();
	return sm;
}
float Sensors::get_x(){
	mtx_x.lock();
	float xm = x;
	mtx_x.unlock();
	return xm;
}
float Sensors::get_y(){
	mtx_y.lock();
	float ym = y;
	mtx_y.unlock();
	return ym;
}
float Sensors::get_th(){
	mtx_th.lock();
	float thm = th;
	mtx_th.unlock();
	return thm;
}

float Sensors::get_ir1(){
	mtx_ir1.lock();
	float ir1m = ir1;
	mtx_ir1.unlock();
	return ir1m;
}
float Sensors::get_ir2(){
	mtx_ir2.lock();
	float ir2m = ir2;
	mtx_ir2.unlock();
	return ir2m;
}
float Sensors::get_obst_dist(){
	mtx_od.lock();
	float odm = obst_dist;
	mtx_od.unlock();
	return odm;
}
bool Sensors::get_obst_found(){
	mtx_of.lock();
	bool ofm = obst_found;
	mtx_of.unlock();
	return ofm;
}

/*float Sensors::get_gyro();				// maybe with pointers???
float Sensors::get_acc();
float Sensors::get_comp();*/


// Setters
void Sensors::set_s(float f){
	mtx_s.lock();
	s = f;
	mtx_s.unlock();
}
void Sensors::set_x(float f){
	mtx_x.lock();
	x = f;
	mtx_x.unlock();
}
void Sensors::set_y(float f){
	mtx_y.lock();
	y = f;
	mtx_y.unlock();
}
void Sensors::set_th(float f){
	mtx_th.lock();
	th = f;
	mtx_th.unlock();
}

void Sensors::set_ir1(float f){
	mtx_ir1.lock();
	ir1 = f;
	mtx_ir1.unlock();
}
void Sensors::set_ir2(float f){
	mtx_ir2.lock();
	ir2 = f;
	mtx_ir2.unlock();
}
void Sensors::set_obst_dist(float f){
	mtx_od.lock();
	obst_dist = f;
	mtx_od.unlock();
}
void Sensors::set_obst_found(bool b){
	mtx_of.lock();
	obst_found = f;
	mtx_of.unlock();
}

void Sensors::set_gyro(float f1, float f2, float f3){
	mtx_gyro.lock();
	gyro = {f1, f2, f3};
	mtx_gyro.unlock();
}
void Sensors::set_acc(float f1, float f2, float f3){
	mtx_acc.lock();
	acc = {f1, f2, f3};
	mtx_acc.unlock();
}
void Sensors::set_comp(float f1, float f2, float f3){
	mtx_comp.lock();
	comp = {f1, f2, f3};
	mtx_comp.unlock();
}
