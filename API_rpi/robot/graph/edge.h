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
	Edge(Node* node1, Node* node2, bool bidirectional, int line, float th_w_node_1, float th_w_node_2, float distance, float vel);
	bool Connects(Node* node1, Node* node2);
	//bool get_line();
	float get_th_w(Node* node);
	void print_info();
	void compute_distance();
public:
	Node* node1;
	Node* node2;
	bool bidirectional;
	int line;
	float th_w_node_1;
	float th_w_node_2;
	float distance;
	float vel;

};

#endif