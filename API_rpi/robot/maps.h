#ifndef maps_h
#define maps_h

enum node_type{		// see how to use it
	TRANSITION,		// maybe goes in 'messages.h'
	TASK,
	TASK_START,
	TASK_END
};


// create a test map!
Graph* map_test(){
	
	Node* a = new Node("a", 0, 0, TRANSITION, FALSE, 0);
	Node* b = new Node("b", 1, 1, TRANSITION, FALSE, 0);
	Edge* e1 = new Edge(a, b, true, false, 0, 0);


	Graph* graph = new Graph();
	graph->add_node(a);
	graph->add_node(b);
	graph->add_edge(e1);

	return graph;
}