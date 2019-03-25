#ifndef utils_rpi_h
#define utils_rpi_h

#include <iostream>
#include <string>
#include <string.h> //<cstring>
#include <string>
#include <vector>
#include <mutex>

using namespace std;

const float BIG_INT = 989898;
const float BIG_FLOAT = 191919.191919;

int str2int(string s);
float str2float(string s);

vector<string> split_str(string msg, string delimiters);

//template <typename type_ident>
//vector<type_ident> add2vector(vector<type_ident> v, type_ident new_data, int max_len);
vector<string> add2vector(vector<string> v, string new_data, int max_len);
vector<int> add2vector(vector<int> v, int new_data, int max_len);
vector<float> add2vector(vector<float> v, float new_data, int max_len);
vector<bool> add2vector(vector<bool> v, bool new_data, int max_len);


void print_vector(vector<string> v);
void print_vector(vector<int> v);
void print_vector(vector<float> v);
void print_vector(vector<bool> v);


/* to do
time time_elapse(time t0, time t1); 
*/
#endif