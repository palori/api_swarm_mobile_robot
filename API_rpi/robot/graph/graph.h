#ifndef graph_h
#define graph_h

#include <iostream>
#include <vector>

#include "node.h"
#include "edge.h"

using namespace std;


class Graph
{
public:
	Graph();
	void add_node(Node* node);
	void add_edge(Edge* edge);

	void reset_nodes();

	void print_nodes();
	void print_edges();

	Node* find_node(string node_id);
	Edge* find_edge(Node* node_start, Node* node_end);

public:
	vector<Node*> nodes;
	vector<Edge*> edges;
};

#endif