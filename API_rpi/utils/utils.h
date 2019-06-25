#ifndef utils_rpi_h
#define utils_rpi_h

#include <iostream>
#include <string>
#include <string.h> //<cstring>
#include <sstream>
#include <vector>
#include <mutex>

using namespace std;

const float BIG_INT = 989898;
const float BIG_FLOAT = 191919.191919;
const string STAR_LINE = "***************************************";

int str2int(string s);
float str2float(string s);

vector<string> split_str(string msg, string delimiters);


template<typename T>	// convert any type to string
string xtos(T x){		// (because its a template it cannot be in the cpp)
	stringstream s;
	s << x;
	return s.str();
}




#endif