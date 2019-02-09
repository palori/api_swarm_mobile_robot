
#ifndef THIS_LIB_H
#define THIS_LIB_H

#define CPU_IS_RASPBERRYPI

#ifdef CPU_IS_RASPBERRYPI
	#include <string>
	#include <vector>
	//#include <iostream>
	//...
#else // CPU_IS_ARDUINO
	#include "Arduino.h"
#endif



void func1();

void func2();

void func3();

#ifdef CPU_IS_RASPBERRYPI
	char * func4(char * msg);
#else // CPU_IS_ARDUINO
	void func4(String msg);
#endif

#endif //EOF




