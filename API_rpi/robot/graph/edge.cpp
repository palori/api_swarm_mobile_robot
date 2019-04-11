
#include "edge.h"

Edge::Edge(){}

Edge::Edge(Node* node1, Node* node2, bool line, bool bidirectional, float th_w_node_1, float th_w_node_2)
	//: node1(node1), node2(node2)
{
	this->node1 = node1;
	this->node2 = node2;

	this->line = line;
	this->bidirectional = bidirectional;

	// orientation
	if (this->line){
		this->th_w_node_1 = th_w_node_1;
		if (this->bidirectional) this->th_w_node_2 = th_w_node_2;
		else this->th_w_node_1 = NULL;
	}
	else{
		this->th_w_node_1 = NULL;
		this->th_w_node_2 = NULL;
	}
	
	compute_distance();
}


bool Edge::Connects(Node* node1, Node* node2){
	bool connection = (node1 == this->node1 && node2 == this->node2);
	if (bidirectional) connection = (connection || (node1 == this->node2 && node2 == this->node1));
	return connection;
}


float Edge::get_th_w(Node* node){
	float th_w = NULL;
	if (node == this->node1) th_w = this->th_w_node_1;
	else if (node == this->node2) th_w = this->th_w_node_2;
	return th_w;
}


void Edge::print_info(){
	cout << "Edge:";
	cout << "\n  node1_id =      " << this->node1->id;
	cout << "\n  node2_id =      " << this->node2->id;
	cout << "\n  line =          " << this->line;
	cout << "\n  bidirectional = " << this->bidirectional;
	cout << "\n  th_w_node_1 =   " << this->th_w_node_1;
	cout << "\n  th_w_node_2 =   " << this->th_w_node_2;
	cout << endl;
}


void Edge::compute_distance(){
	float k = 2.0;
	float delta_x = node2->x - node1->x;
	float delta_y = node2->y - node1->y;

	distance = sqrt(delta_x*delta_x + delta_y*delta_y);

	if (line) distance = k*distance;
}