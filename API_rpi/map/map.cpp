#include <iostream>
#include "Point.h"
#include <vector>

using namespace std;


int main(){

	vector<Point> map;
	Point P0(0,0,0,0);
	map.push_back(P0);
	Point P1(1,1,0,0);
	map.push_back(P1);
	Point P2(2,1,1,0);
	map.push_back(P2);

	int wanted_id = 1;

	
	Point P = map.at(wanted_id);
	cout <<"wanted id: "<< P.id << endl;


}