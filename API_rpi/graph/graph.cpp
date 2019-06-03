
#include "graph.h"


Graph::Graph(){
	/*Node* a = new Node("default", -1, -1, 0, true, 0);
	add_node(a);
	print_nodes();*/
	this->id = "";
}

Graph::Graph(string id){
	this->id = id;
}


void Graph::add_node(Node* node){
	nodes.push_back(node);
}


void Graph::add_edge(Edge* edge){
	edges.push_back(edge);
}


void Graph::reset_nodes(){
	for (int i = 0; i < nodes.size(); i++){
		nodes.at(i)->reset();
	}
}


void Graph::print_nodes(){
	cout << "Nodes info: " << endl;
	for (int i = 0; i < nodes.size(); i++){
		nodes.at(i)->print_info();
	}
}


void Graph::print_edges(){
	cout << "Edges info: " << endl;
	for (int i = 0; i < edges.size(); i++){
		edges.at(i)->print_info();
	}
}


Node* Graph::find_node(string node_id){
	for (int i = 0; i < nodes.size(); i++){
		if (nodes.at(i)->id ==	node_id) return nodes.at(i);
	}
	return NULL;
}


Edge* Graph::find_edge(Node* node_start, Node* node_end){
	//cout << "find_edge..." << endl;
	Edge* edge;
	for (int i = 0; i < edges.size(); i++){
		edge = edges.at(i);
		if (edge->Connects(node_start, node_end)) return edge;
	}
	return NULL;
}