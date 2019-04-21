
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


Graph* map_mission0_test(){
	
	Node* a = new Node("a", 0.0, 2.9, TASK_START, false, 0);
	Node* ab = new Node("ab", 2.08, 2.47, TRANSITION, false, 0); 
	Node* b = new Node("b", 0.7, 0.0, TRANSITION, false, 0);
	//Node* c = new Node("c", 6.3, 0.5, TRANSITION, false, 0);
	//Node* d = new Node("d", 7.5, 3.2, TRANSITION, false, 0);

	Edge* e1 = new Edge(a, ab, false, MIDDLE, 0, 0, 2.2, 0.3); 
	Edge* e2 = new Edge(ab, b, false, LEFT, -1.85, 0, 3.8, 0.3);
	//Edge* e3 = new Edge(c, d, false, RIGHT, 1.57, 0);

	Graph* graph = new Graph();
	graph->add_node(a);
	graph->add_node(b);
	graph->add_node(ab);
	//graph->add_node(d);
	graph->add_edge(e1);
	graph->add_edge(e2);
	//graph->add_edge(e3);

	return graph;
}


Graph* map_mission0(){
	
	//Node(string id, float x, float y, int type, bool line, int crossing);
	Node* a = new Node("a", 0.0, 2.9, TASK_START, false, 0); 
	Node* b = new Node("b", 0.5, 2.9, TRANSITION, false, 0);
	Node* c = new Node("c", 1.65, 1.8, TRANSITION, false, 0);
	Node* d = new Node("d", 0.7, 0.0, TRANSITION, false, 0);
	Node* e = new Node("e", 3.9, 0.0, TRANSITION, false, 0);
	Node* f = new Node("f", 4.8, -0.1, TRANSITION, false, 0);
	Node* g = new Node("g", 5.7, 0.65, TRANSITION, false, 0);
	Node* h = new Node("h", 5.7, 3.05, TRANSITION, false, 0);
	Node* i = new Node("i", 6.7, 3.55, TASK_END, false, 0);
	

	//Edge(Node* node1, Node* node2, bool bidirectional, int line, float th_w_node_1, float th_w_node_2, float distance, float vel);
	float angle;
	Edge* e1 = new Edge(a, b, false, NO_LINE, 0, 0, 0, 0.6);
	Edge* e2 = new Edge(b, c, false, NO_LINE, 0, 0, 0, 0.6);
	angle = -3 * PI / 4;
	Edge* e3 = new Edge(c, d, false, LEFT, angle, 0, 2.75, 0.3);
	Edge* e4 = new Edge(d, f, false, RIGHT, 0, 0, 4.1, 0.3);
	//Edge* e5 = new Edge(e, f, false, RIGHT, 0, 0, 0.8, 0.3);
	Edge* e6 = new Edge(f, g, false, RIGHT, 0, 0, 1.4, 0.3);
	angle = PI / 2;
	Edge* e7 = new Edge(g, h, false, RIGHT, angle, 0, 2.4, 0.3);
	Edge* e8 = new Edge(h, i, false, RIGHT, angle, 0, 1.3, 0.3);

	Graph* graph = new Graph();
	graph->add_node(a);
	graph->add_node(b);
	graph->add_node(c);
	graph->add_node(d);
	//graph->add_node(e);
	graph->add_node(f);
	graph->add_node(g);
	graph->add_node(h);
	graph->add_node(i);
	graph->add_edge(e1);
	graph->add_edge(e2);
	graph->add_edge(e3);
	graph->add_edge(e4);
	//graph->add_edge(e5);
	graph->add_edge(e6);
	graph->add_edge(e7);
	graph->add_edge(e8);

	return graph;
}



Graph* map_mission007(){
	
	//Node(string id, float x, float y, int type, bool line, int crossing);
	Node* a = new Node("a", 0.0, 2.9, TASK_START, false, 0); 
	Node* b = new Node("i", 0.5, 2.9, TRANSITION, false, 0);
	

	//Edge(Node* node1, Node* node2, bool bidirectional, int line, float th_w_node_1, float th_w_node_2, float distance, float vel);
	float angle;
	Edge* e1 = new Edge(a, b, false, NO_LINE, 0, 0, 0, 0.6);
	

	Graph* graph = new Graph();
	graph->add_node(a);
	graph->add_node(b);
	graph->add_edge(e1);
	

	return graph;
}
