
#ifndef dijkstra_h
#define dijkstra_h

#include <iostream>
//#include <vector>
#include <chrono>

#include "../graph/graph.h"

using namespace std;

class Dijkstra
{
public:
	Dijkstra(Graph* graph);
	
	void find_route(string node_start, string node_end);
	vector<Node*> store_route(Node* destination);
	void print_route();
	void print_route(vector<Node*> route);
public:
	Graph* graph;
	vector<Node*> route;
private:
	Node* ExtractSmallest(vector<Node*>& nodes);
	bool Contains(vector<Node*>& nodes, Node* node);
	float Distance(Node* node1, Node* node2);
	vector<Node*>* AdjacentRemainingNodes(Node* node);
	
	void Dijkstras();
	//void PrintShortestRouteTo(Node* destination);	// useless, replaced by 'store_route' & 'print_store_route'

};

#endif