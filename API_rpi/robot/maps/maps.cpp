
#include "maps.h"


// create a test map!
/*Graph* map_test(){
	
	Node* a = new Node("a", 0, 0, TRANSITION, false, 0);
	Node* b = new Node("b", 1, 1, TRANSITION, false, 0);
	Edge* e1 = new Edge(a, b, true, false, 0, 0);


	Graph* graph = new Graph();
	graph->add_node(a);
	graph->add_node(b);
	graph->add_edge(e1);

	return graph;
}*/


Graph* map_mission0(){
	
	Node* a = new Node("a", 0.0, 3.0, TASK_START, false, 0);
	Node* b = new Node("b", 1.0, 0.0, TRANSITION, false, 0);
	//Node* c = new Node("c", 6.3, 0.5, TRANSITION, false, 0);
	//Node* d = new Node("d", 7.5, 3.2, TRANSITION, false, 0);

	Edge* e1 = new Edge(a, b, false, NO_LINE, 0, 0, 5, 0.3);
	//Edge* e2 = new Edge(b, c, false, RIGHT, 0, 0, );
	//Edge* e3 = new Edge(c, d, false, RIGHT, 1.57, 0);

	Graph* graph = new Graph();
	graph->add_node(a);
	graph->add_node(b);
	//graph->add_node(c);
	//graph->add_node(d);
	graph->add_edge(e1);
	//graph->add_edge(e2);
	//graph->add_edge(e3);

	return graph;
}


