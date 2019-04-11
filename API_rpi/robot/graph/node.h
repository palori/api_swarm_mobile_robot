#ifndef node_h
#define node_h

#include <iostream>

using namespace std;


class Node
{
public:
	Node(string id, float x, float y, int type, bool line, int crossing);
	void print_info();
	void reset();
public:
	int INT_MAX = 1000;
	
	string id;
	Node* previous;
	int distanceFromStart;
	float x;
	float y;
	int type;
	bool line;
	int crossing;
};

#endif