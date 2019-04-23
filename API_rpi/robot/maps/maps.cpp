
#include "maps.h"


Graph* map_mission_easy(string id){
	//Node(string id, float x, float y, int type, bool line, int crossing);
	Node* a = new Node("a", -0.05, 2.9, TASK_START, false, 0); 
	Node* b = new Node("b", 0.5, 2.9, TRANSITION, false, 0);
	Node* c = new Node("c", 1.65, 1.8, TRANSITION, false, 0);
	Node* cd = new Node("cd", 0.7, 1.0, TRANSITION, false, 0);
	Node* d = new Node("d", 0.7, 0.0, TRANSITION, false, 0);
	Node* e = new Node("e", 3.9, 0.0, TRANSITION, false, 0);
	Node* f = new Node("f", 4.8, 0.0, TRANSITION, false, 0);
	Node* g = new Node("g", 5.4, 0.4, TRANSITION, false, 0);
	Node* g1 = new Node("g1", 5.7, 0.65, TRANSITION, false, 0);
	Node* h = new Node("h", 5.7, 3.05, TRANSITION, false, 0);
	Node* i = new Node("i", 6.7, 3.55, TRANSITION, false, 0);		// will wait in here
	Node* j = new Node("j", 6.7, 3.55, TASK_END, false, 0);			// siren
	

	//Edge(Node* node1, Node* node2, bool bidirectional, int line, float th_w_node_1, float th_w_node_2, float distance, float vel);
	float angle;
	Edge* e1 = new Edge(a, b, false, NO_LINE, 0, 0, 0, 0.6);
	Edge* e2 = new Edge(b, c, false, NO_LINE, 0, 0, 0, 0.6);
	angle = -3 * PI / 4;
	Edge* e23 = new Edge(c, cd, false, NO_LINE, angle, 0, 3.55, 0.6);
	angle = -PI / 2;
	Edge* e3 = new Edge(cd, d, false, NO_LINE, angle, 0, 0, 0.3);
	Edge* e4 = new Edge(d, g, false, RIGHT, 0, 0, 4.5, 0.3);
	angle = PI /2;
	Edge* e5 = new Edge(g, g1, false, RIGHT, 0, 0, 1.0, 0.2);
	Edge* e7 = new Edge(g1, h, false, LEFT, angle, 0, 3.3, 0.15);
	Edge* e8 = new Edge(g1, i, false, RIGHT, angle, 0, 5.3, 0.15);
	Edge* e9 = new Edge(i, j, false, MIDDLE, angle, 0, 0.2, 0.3);

	Graph* graph = new Graph(id);
	graph->add_node(a);
	graph->add_node(b);
	graph->add_node(c);
	graph->add_node(cd);
	graph->add_node(d);
	//graph->add_node(e);
	graph->add_node(f);
	graph->add_node(g);
	graph->add_node(g1);
	graph->add_node(h);
	graph->add_node(i);
	graph->add_edge(e1);
	graph->add_edge(e2);
	graph->add_edge(e23);
	graph->add_edge(e3);
	graph->add_edge(e4);
	graph->add_edge(e5);
	//graph->add_edge(e6);
	graph->add_edge(e7);
	graph->add_edge(e8);
	graph->add_edge(e9);

	return graph;
}


Graph* map_mission_ax(string id){
	//Node(string id, float x, float y, int type, bool line, int crossing);
	Node* h = new Node("h", 5.7, 3.05, TRANSITION, false, 0);
	Node* j = new Node("j", 5.7, 4.4, TRANSITION, false, 0);
	

	//Edge(Node* node1, Node* node2, bool bidirectional, int line, float th_w_node_1, float th_w_node_2, float distance, float vel);
	float angle;
	Edge* e1 = new Edge(h, j, false, NO_LINE, 0, 0, 1.5, 0.15);

	Graph* graph = new Graph(id);
	graph->add_node(h);
	graph->add_node(j);
	
	graph->add_edge(e1);

	return graph;
}



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
	Node* a = new Node("a", -0.05, 2.9, TASK_START, false, 0); 
	Node* b = new Node("b", 0.5, 2.9, TRANSITION, false, 0);
	Node* c = new Node("c", 1.65, 1.8, TRANSITION, false, 0);
	Node* cd = new Node("cd", 0.6, 1.0, TRANSITION, false, 0);
	Node* d = new Node("d", 0.7, 0.0, TRANSITION, false, 0);
	Node* e = new Node("e", 3.9, 0.0, TRANSITION, false, 0);
	Node* f = new Node("f", 4.8, 0.0, TRANSITION, false, 0);
	//Node* f1 = new Node("f1",4.8,0.45,TRANSITION, false, 0);
	//Node* f2 = new Node("f2",4.8,0.0, TRANSITION, false, 0);
	//Node* f3 = new Node("f3",5.6,0.2, TRANSITION, false, 0);
	//Node* f4 = new Node("f4",6.05, -0.1, TRANSITION, false, 0);
	//Node* f5 = new Node("f5",5.6,0.2,TRANSITION, false, 0);
	Node* g = new Node("g", 5.7, 0.65, TRANSITION, false, 0);
	Node* h = new Node("h", 5.7, 3.05, TRANSITION, false, 0);
	Node* i = new Node("i", 6.7, 3.55, TASK_END, false, 0);
	

	//Edge(Node* node1, Node* node2, bool bidirectional, int line, float th_w_node_1, float th_w_node_2, float distance, float vel);
	float angle;
	Edge* e1 = new Edge(a, b, false, NO_LINE, 0, 0, 0, 0.6);
	Edge* e2 = new Edge(b, c, false, NO_LINE, 0, 0, 0, 0.6);
	angle = -3 * PI / 4;
	Edge* e23 = new Edge(c, cd, false, NO_LINE, angle, 0, 3.55, 0.6);
	angle = -PI / 2;
	Edge* e3 = new Edge(cd, d, false, NO_LINE, angle, 0, 0, 0.3);
	Edge* e4 = new Edge(d, g, false, RIGHT, 0, 0, 5.5, 0.3);
	angle = PI / 2;
	//Edge* e5 = new Edge(f, f1, false, MIDDLE, angle, 0 , 0.45 , 0.15);
	//Edge* e51 = new Edge(f1, f2, false, NO_LINE, 0, 0, 0, 0.2);
	//Edge* e52 = new Edge(f2, f3, false, RIGHT, 0, 0, 1, 0.2);
	//Edge* e53 = new Edge(f3, f4, false, NO_LINE, 0, 0, 0, 0.2);
	//Edge* e54 = new Edge(f4, f5, false, NO_LINE, 0, 0, 0, 0.2);
	angle= PI /4;
	//Edge* e5 = new Edge(e, f, false, RIGHT, 0, 0, 0.8, 0.3);
	//Edge* e6 = new Edge(f5, g, false, RIGHT, angle, 0, 0.5, 0.2);
	angle = PI /2;
	Edge* e7 = new Edge(g, i, false, RIGHT, angle, 0, 3.7, 0.15);
	//Edge* e8 = new Edge(h, i, false, NO_LINE, angle, 0, 1.3, 0.3);

	Graph* graph = new Graph();
	graph->add_node(a);
	graph->add_node(b);
	graph->add_node(c);
	graph->add_node(cd);
	graph->add_node(d);
	//graph->add_node(e);
	graph->add_node(f);
	//graph->add_node(f1);
	//graph->add_node(f2);
	//graph->add_node(f3);
	//graph->add_node(f4);
	//graph->add_node(f5);
	graph->add_node(g);
	graph->add_node(h);
	graph->add_node(i);
	graph->add_edge(e1);
	graph->add_edge(e2);
	graph->add_edge(e23);
	graph->add_edge(e3);
	graph->add_edge(e4);
	//graph->add_edge(e5);
	//graph->add_edge(e51);
	//graph->add_edge(e52);
	//graph->add_edge(e53);
	//graph->add_edge(e54);
	//graph->add_edge(e6);
	graph->add_edge(e7);
	//graph->add_edge(e8);

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


Graph* map_mission1(){
	
	//Node(string id, float x, float y, int type, bool line, int crossing);
	Node* a = new Node("a", -0.2, 2.9, TASK_START, false, 0);
	Node* b = new Node("b", 0.5, 2.9, TRANSITION, false, 0);
	Node* c = new Node("c", 1.65, 1.8, TRANSITION, false, 0);
	Node* cd = new Node("cd", 0.6, 0.8, TRANSITION, false, 0);

	//new nodes
	//Node* l = new Node("l", 1.8, 0.8, TRANSITION, false, 0);
	Node* l = new Node("l", 2.0, 0.6, TRANSITION, false, 0);
	//Node* l4 = new Node("l4", 1.5, 2.2, TRANSITION, false, 0);
	Node* l1 = new Node("l1", 1.8, 1.8, TRANSITION, false, 0);
	Node* l2 = new Node("l2", 1.8, 2.8, TRANSITION, false, 0);
	Node* l3 = new Node("l3", 1.7, 3.8, TRANSITION, false, 0);
	

	Node* m = new Node("m", 1.7, 4.15, TRANSITION, false, 0);
	Node* n = new Node("n", 2.3, 4.15, TRANSITION, false, 0);
	Node* o = new Node("o", 2.3, 4.6, TRANSITION, false, 0);
	Node* p = new Node("p", 0.45, 4.6, TRANSITION, false, 0);

	//Node* pq = new Node("pq", 0.65, 4.6, TRANSITION, false, 0);	// backwards

	Node* q = new Node("q", 0.4, 5.1, TRANSITION, false, 0);
	Node* r = new Node("r", 0.85, 5.85, TRANSITION, false, 0);
	

	//Edge(Node* node1, Node* node2, bool bidirectional, int line, float th_w_node_1, float th_w_node_2, float distance, float vel);
	float angle;
	Edge* e1 = new Edge(a, b, false, NO_LINE, 0, 0, 0, 0.6);
	Edge* e2 = new Edge(b, c, false, NO_LINE, 0, 0, 0, 0.6);
	Edge* e23 = new Edge(c, cd, false, NO_LINE, 0, 0, 0, 0.6);

	Edge* e3 = new Edge(cd, l, false, NO_LINE, 0, 0, 0, 0.4);

	Edge* e3l1 = new Edge(l, l1, false, NO_LINE, 0, 0, 0, 0.4);
	Edge* e3l2 = new Edge(l1, l2, false, NO_LINE, 0, 0, 0, 0.4);
	Edge* e3l3 = new Edge(l2, l3, false, NO_LINE, 0, 0, 0, 0.4);
	//Edge* e3l4 = new Edge(l, l4, false, NO_LINE, 0, 0, 0, 0.4);

	Edge* e4 = new Edge(l3, m, false, NO_LINE, 0, 0, 0, 0.4);
	
	angle = PI/2;
	//Edge* e4 = new Edge(l4, m, false, MIDDLE, angle, 0, 2.5, 0.3);

	Edge* e5 = new Edge(m, n, false, NO_LINE, 0, 0, 0, 0.4);
	Edge* e6 = new Edge(n, o, false, NO_LINE, 0, 0, 0, 0.4);
	Edge* e7 = new Edge(o, p, false, NO_LINE, 0, 0, 0, 0.6);

	//Edge* e8 = new Edge(p, pq, false, NO_LINE, 0, 0, 0, -0.4);	//backwards
	//Edge* e9 = new Edge(pq, q, false, NO_LINE, 0, 0, 0, 0.4);

	Edge* e8 = new Edge(p, q, false, NO_LINE, 0, 0, 0, 0.4);

	Edge* e10 = new Edge(q, r, false, NO_LINE, 0, 0, 0, 0.4);


	Graph* graph = new Graph();
	graph->add_node(a);
	graph->add_node(b);
	graph->add_node(c);
	graph->add_node(cd);
	
	graph->add_node(l);

	graph->add_node(l1);
	graph->add_node(l2);
	graph->add_node(l3);
	//graph->add_node(l4);

	graph->add_node(m);
	graph->add_node(n);
	graph->add_node(o);
	graph->add_node(p);

	//graph->add_node(pq);

	graph->add_node(q);
	graph->add_node(r);

	graph->add_edge(e1);
	graph->add_edge(e2);
	graph->add_edge(e23);

	graph->add_edge(e3);

	graph->add_edge(e3l1);
	graph->add_edge(e3l2);
	graph->add_edge(e3l3);
	//graph->add_edge(e3l4);

	graph->add_edge(e4);
	graph->add_edge(e5);
	graph->add_edge(e6);
	graph->add_edge(e7);

	graph->add_edge(e8);
	//graph->add_edge(e9);
	graph->add_edge(e10);

	return graph;
}


Graph* map_mission2(){
	
	//Node(string id, float x, float y, int type, bool line, int crossing);
	Node* a = new Node("a", -0.35, 2.9, TASK_START, false, 0);
	Node* b = new Node("b", 0.5, 2.9, TRANSITION, false, 0);
	Node* c = new Node("c", 1.65, 1.8, TRANSITION, false, 0);
	Node* cd = new Node("cd", 0.6, 0.8, TRANSITION, false, 0);

	//new nodes
	Node* l = new Node("l", 1.8, 0.8, TRANSITION, false, 0);

	Node* l1 = new Node("l1", 1.8, 1.8, TRANSITION, false, 0);
	Node* l2 = new Node("l2", 1.8, 2.8, TRANSITION, false, 0);
	Node* l3 = new Node("l3", 1.7, 3.8, TRANSITION, false, 0);

	Node* m = new Node("m", 1.7, 4.15, TRANSITION, false, 0);

	Node* a1 = new Node("a1", 1.1, 4.15, TRANSITION, false, 0);
	Node* a2 = new Node("a2", 1.1, 4.6, TRANSITION, false, 0);
	Node* a2c = new Node("a2c", 1.1, 4.6, TRANSITION, false, 0);
	Node* a3 = new Node("a3", 1.3, 4.6, TRANSITION, false, 0);
	Node* a4 = new Node("a4", 1.1, 5.1, TRANSITION, false, 0);
	Node* a5 = new Node("a5", 2.2, 5.1, TRANSITION, false, 0);
	Node* a6 = new Node("a6", 2.2, 4.6, TRANSITION, false, 0);
	Node* a6c = new Node("a6c", 2.2, 4.6, TRANSITION, false, 0);
	Node* a7 = new Node("a7", 2.0, 4.6, TRANSITION, false, 0);
	Node* a8 = new Node("a8", 2.7, 4.6, TRANSITION, false, 0);
	Node* a81 = new Node("a81", 3.65, 4.6, TRANSITION, false, 0);
	Node* a82 = new Node("a82", 4.4, 4.6, TRANSITION, false, 0);
	Node* a9 = new Node("a9", 5.5, 4.6, TRANSITION, false, 0);

	

	//Edge(Node* node1, Node* node2, bool bidirectional, int line, float th_w_node_1, float th_w_node_2, float distance, float vel);
	//float angle;
	Edge* e1 = new Edge(a, b, false, NO_LINE, 0, 0, 0, 0.6);
	Edge* e2 = new Edge(b, c, false, NO_LINE, 0, 0, 0, 0.6);
	Edge* e23 = new Edge(c, cd, false, NO_LINE, 0, 0, 0, 0.6);

	Edge* e3 = new Edge(cd, l, false, NO_LINE, 0, 0, 0, 0.6);
	Edge* e3l1 = new Edge(l, l1, false, NO_LINE, 0, 0, 0, 0.4);
	Edge* e3l2 = new Edge(l1, l2, false, NO_LINE, 0, 0, 0, 0.4);
	Edge* e3l3 = new Edge(l2, l3, false, NO_LINE, 0, 0, 0, 0.4);


	Edge* e4 = new Edge(l3, m, false, NO_LINE, 0, 0, 0, 0.4);


	Edge* e5 = new Edge(m, a1, false, NO_LINE, 0, 0, 0, 0.4);
	Edge* e6 = new Edge(a1, a2, false, NO_LINE, 0, 0, 0, 0.4);
	Edge* e7 = new Edge(a2, a3, false, NO_LINE, 0, 0, 0, 0.4);
	Edge* e8 = new Edge(a3, a2c, false, NO_LINE, 0, 0, 0, -0.4);	//backwards

	Edge* e9 = new Edge(a2c, a4, false, NO_LINE, 0, 0, 0, 0.4);
	Edge* e10 = new Edge(a4, a5, false, NO_LINE, 0, 0, 0, 0.4);
	Edge* e11 = new Edge(a5, a6, false, NO_LINE, 0, 0, 0, 0.4);
	Edge* e12 = new Edge(a6, a7, false, NO_LINE, 0, 0, 0, 0.4);
	Edge* e13 = new Edge(a7, a6c, false, NO_LINE, 0, 0, 0, -0.4); // backwards

	Edge* e14 = new Edge(a6c, a8, false, NO_LINE, 0, 0, 0, 0.4);
	Edge* e15 = new Edge(a8, a81, false, NO_LINE, 0, 0, 0, 0.4);
	Edge* e16 = new Edge(a81, a82, false, NO_LINE, 0, 0, 0, 0.4);
	Edge* e17 = new Edge(a82, a9, false, NO_LINE, 0, 0, 0, 0.4);

	Graph* graph = new Graph();
	graph->add_node(a);
	graph->add_node(b);
	graph->add_node(c);
	graph->add_node(cd);
	
	graph->add_node(l);

	graph->add_node(l1);
	graph->add_node(l2);
	graph->add_node(l3);

	graph->add_node(m);
	graph->add_node(a1);
	graph->add_node(a2);
	graph->add_node(a2c);
	graph->add_node(a3);
	graph->add_node(a4);
	graph->add_node(a5);
	graph->add_node(a6);
	graph->add_node(a6c);
	graph->add_node(a7);
	graph->add_node(a8);
	graph->add_node(a81);
	graph->add_node(a82);
	graph->add_node(a9);

	graph->add_edge(e1);
	graph->add_edge(e2);
	graph->add_edge(e23);

	graph->add_edge(e3);

	graph->add_edge(e3l1);
	graph->add_edge(e3l2);
	graph->add_edge(e3l3);

	graph->add_edge(e4);
	graph->add_edge(e5);
	graph->add_edge(e6);
	graph->add_edge(e7);

	graph->add_edge(e8);
	graph->add_edge(e9);
	graph->add_edge(e10);
	graph->add_edge(e11);
	graph->add_edge(e12);
	graph->add_edge(e13);
	graph->add_edge(e14);
	graph->add_edge(e15);
	graph->add_edge(e16);
	graph->add_edge(e17);

	return graph;
}




