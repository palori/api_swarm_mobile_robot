#include "../utils.h"
#include <iostream>
#include <vector>
#include "../comm_rpi_2.h"
#include "../sensors.h"

//#import "graph.h"

using namespace std;

void test_split_string(){
	
	vector<string> words;
	cout << "words out: " << words.size() << endl;
	words = split_str("x=1,y=3", "=,");
	cout << "words out: " << words.size() << endl;
	cout << words.size() << " words:" << endl;
	for (uint i=0; i<words.size(); i++){
		cout << words.at(i) << endl;
	}
	
}

void test_msg2sensorData(){
	string msg = "@x=1,y=2$";
	Sensors sens;
	COMM_RPI cr;
	cr.msg2sensorData(msg, sens);
}



void test_add2vector_print_vector(){
	vector<string> v;
	int max_len = 2;
	v = add2vector(v, "holaw", max_len);
	print_vector(v);
	v = add2vector(v, "adeu", max_len);
	print_vector(v);
	v = add2vector(v, "fins aviat", max_len);
	print_vector(v);

	vector<int> v10;
	v10 = add2vector(v10, 4, max_len);
	print_vector(v10);
	v10 = add2vector(v10, 5, max_len);
	print_vector(v10);
	v10 = add2vector(v10, 6, max_len);
	print_vector(v10);

	vector<float> v1;
	v1 = add2vector(v1, 1.0, max_len);
	print_vector(v1);
	v1 = add2vector(v1, 2.0, max_len);
	print_vector(v1);
	v1 = add2vector(v1, 3.0, max_len);
	print_vector(v1);

	vector<bool> v2;
	v2 = add2vector(v2, true, max_len);
	print_vector(v2);
	v2 = add2vector(v2, false, max_len);
	print_vector(v2);
	v2 = add2vector(v2, true, max_len);
	print_vector(v2);
	
}



//  compile from terminal!!!!!!!!!!!
//  g++ -O -W -Wall -pedantic -o test_utils -p ../utils.cpp ../comm_rpi_2.cpp test_utils.cpp ../sensors.cpp
//  or make (now working!)

int main(int argc, char const *argv[])
{
	test_split_string();
	//test_msg2sensorData();
	test_add2vector_print_vector();
	return 0;
}

