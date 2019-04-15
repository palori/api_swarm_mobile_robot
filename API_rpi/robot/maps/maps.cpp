
#include "maps.h"


// create a test map!
Graph* map_test(){
	
	Node* a = new Node("a", 0, 0, TRANSITION, false, 0);
	Node* b = new Node("b", 1, 1, TRANSITION, false, 0);
	Edge* e1 = new Edge(a, b, true, false, 0, 0);


	Graph* graph = new Graph();
	graph->add_node(a);
	graph->add_node(b);
	graph->add_edge(e1);

	return graph;
}



