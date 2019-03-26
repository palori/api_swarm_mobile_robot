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

/*vector<string> add2vector(vector<string> v, string new_data, int max_len);
vector<int> add2vector(vector<int> v, int new_data, int max_len);
vector<float> add2vector(vector<float> v, float new_data, int max_len);
vector<bool> add2vector(vector<bool> v, bool new_data, int max_len);
*/

/*void print_vector(vector<string> v);
void print_vector(vector<int> v);
void print_vector(vector<float> v);
void print_vector(vector<bool> v);*/

template <class T>
vector<T> add2vector(vector<T> v, T new_data, int max_len);
template <class T>
void print_vector(vector<T> v);


/* to do
time time_elapse(time t0, time t1); 
*/


#include "utils.tpp" // because some of the functions are templates!!!

#endif