
#include "../../path_planning/dijkstra.h"


enum node_type{		// see how to use it
	TRANSITION,		// maybe goes in 'messages.h'
	TASK,
	TASK_START,
	TASK_END
};


// create a map!
// custom belong to dijkstra
Graph* map(){
	auto map_start = chrono::system_clock::now();
	/*
	Node* a = new Node("a");
	Node* b = new Node("b");
	Node* c = new Node("c");
	Node* d = new Node("d");
	Node* e = new Node("e");
	Node* f = new Node("f");
	Node* g = new Node("g");
	Edge* e1 = new Edge(a, c, 1);
	Edge* e2 = new Edge(a, d, 2);
	Edge* e3 = new Edge(b, c, 2);
	Edge* e4 = new Edge(c, d, 1);
	Edge* e5 = new Edge(b, f, 3);
	Edge* e6 = new Edge(c, e, 3);
	Edge* e7 = new Edge(e, f, 2);
	Edge* e8 = new Edge(d, g, 1);
	Edge* e9 = new Edge(g, f, 1);
	*/

	
	Node* a = new Node("a", 0, 0, TASK_START, true, 0);
	Node* b = new Node("b", 1, 1, TASK_END, true, 0);
	Node* c = new Node("c", 1, 2, TRANSITION, false, 0);
	Edge* e1 = new Edge(a, b, true, true, 90, 135);
	Edge* e2 = new Edge(b, c, true, false, 0, 0);


	Graph* graph = new Graph();
	graph->add_node(a);
	graph->add_node(b);
	graph->add_node(c);
	graph->add_edge(e1);
	graph->add_edge(e2);

	auto map_end = chrono::system_clock::now();
	
	chrono::duration<double> map_elapsed = map_end - map_start;
	cout << "\n\nMap created in " << map_elapsed.count() << "s.\n\n";
	return graph;
}



void DijkstrasTest(){
	// TEST 1
	Graph* graph = map();

	graph->print_nodes();
	graph->print_edges();


	Dijkstra dijkstra(graph);
	dijkstra.find_route("a", "c");
	dijkstra.print_route();
	

	
}






/*
 * Trying to test the code above
 */
int main(int argc, char const *argv[])
{
	DijkstrasTest();
	return 0;
}