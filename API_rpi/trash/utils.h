#ifndef utils_rpi_h
#define utils_rpi_h

#include <iostream>
#include <string>
#include <string.h> //<cstring>
#include <string>
#include <vector>

using namespace std;


#define ITEM 1			// to compile some files using 'item.h'
						// e.g: robot_params, sensors...


const float BIG_INT = 989898;
const float BIG_FLOAT = 191919.191919;

//int str2int(string s);

float str2float(string s);

#endif