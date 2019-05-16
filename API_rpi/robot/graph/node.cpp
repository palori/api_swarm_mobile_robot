
#include "node.h"

Node::Node(){}

Node::Node(string id, float x, float y, int type, bool line, int crossing)
	//: id(id), previous(NULL),
	//distanceFromStart(INT_MAX)
{	
	this->id = id;
	this->previous = NULL;
	reset();
	this->x = x;
	this->y = y;
	this->type = type;
	this->line = line;
	this->crossing = crossing;
}

void Node::print_info(){
	cout << "Node " << this->id << ":";
	cout << "\n  previous =    " << this->previous;
	cout << "\n  dist. start = " << this->distanceFromStart;
	cout << "\n  x =           " << this->x;
	cout << "\n  y =           " << this->y;
	cout << "\n  type =        " << this->type;
	cout << "\n  line =        " << this->line;
	cout << "\n  crossing =    " << this->crossing;
	cout << endl;
}


void Node::reset(){
	this->distanceFromStart = INT_MAX;
}