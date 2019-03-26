#include "controllers.h"

Controllers::Controllers(){}
Controllers::~Controllers(){}


// Getters
int Controllers::get_MAX_LEN(){return MAX_LEN;}

vector<float> Controllers::get_s(){
	mtx_s.lock();
	vector<float> sm = s;
	mtx_s.unlock();
	return sm;
}
vector<float> Controllers::get_th(){
	mtx_th.lock();
	vector<float> thm = th;
	mtx_th.unlock();
	return thm;
}



// Setters
void Controllers::set_MAX_LEN(int i){MAX_LEN = i;}

void Controllers::set_s(float f){
	mtx_s.lock();
	s = add2vector(s, f, get_MAX_LEN());
	mtx_s.unlock();
}
void Controllers::set_th(float f){
	mtx_th.lock();
	th = add2vector(th, f, get_MAX_LEN());
	mtx_th.unlock();
}
