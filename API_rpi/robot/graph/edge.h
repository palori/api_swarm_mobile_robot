#ifndef edge_h
#define edge_h

#include <iostream>
#include <math.h>

#include "node.h"

using namespace std;


class Edge
{
public:
	Edge();
	Edge(Node* node1, Node* node2, bool line, bool bidirectional, float th_w_node_1, float th_w_node_2);
	bool Connects(Node* node1, Node* node2);
	//bool get_line();
	float get_th_w(Node* node);
	void print_info();
public:
	Node* node1;
	Node* node2;
	float distance;
	bool line;
	bool bidirectional;
	float th_w_node_1;
	float th_w_node_2;
private:
	void compute_distance();

};

#endif