/*************************************
  * Dijkstra’s Algorithm code in C++ *
  ************************************

Dijkstra’s algorithm is a famous algorithm that calculates the routes and
distances from a start node to all other nodes in a connected graph where all
the distances are positive.

Here is a version which I wrote in C++. My aim here was to try to create a
version that was easy to read, rather than the most efficient version. I used
the excellent Introduction to Algorithms book as a reference for my version of
Dijkstra’s. This version is implemented using STL vectors to store the edges
and nodes. I might later modify it to use adjacency lists and priority queues
to produce a more efficient implementation.

First off is my Node and Edge class. You’ll note that the objects add
themselves to the nodes and edges vectors as they are constructed. 
*/
#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

int INT_MAX = 1000;

struct orientation{			// used in edges
	float node1 = 0.0;
	float node2 = 0.0;
};

struct line{				// used in nodes
	bool is_line = false;
	int crossing = 0;
};

enum node_type{				// see how to use it
	TRANSITION,
	TASK,
	TASK_START,
	TASK_END
};

class Node
{
public:
	Node(string id, float x, float y, int type, bool is_line, int crossing);
	void print_info();
public:
	string id;
	Node* previous;
	int distanceFromStart;
	float x;
	float y;
	int type;
	bool is_line;
	int crossing;
};

class Edge
{
public:
	Edge(Node* node1, Node* node2);
	bool Connects(Node* node1, Node* node2);
public:
	Node* node1;
	Node* node2;
	float distance;
private:
	void compute_distance();

};

vector<Node*> nodes;
vector<Edge*> edges;

Node::Node(string id, float x, float y, int type, bool is_line, int crossing)
	: id(id), previous(NULL),
	distanceFromStart(INT_MAX)
{
	this->x = x;
	this->y = y;
	this->type = type;
	this->is_line = is_line;
	this->crossing = crossing;
	nodes.push_back(this);
}

void Node::print_info(){
	cout << "Node " << this->id << ":";
	cout << "\n  previous =    " << this->previous;
	cout << "\n  dist. start = " << this->distanceFromStart;
	cout << "\n  x =           " << this->x;
	cout << "\n  y =           " << this->y;
	cout << "\n  type =        " << this->type;
	cout << "\n  is_line =     " << this->is_line;
	cout << "\n  crossing =    " << this->crossing;
	cout << endl;
}


Edge::Edge(Node* node1, Node* node2)
	: node1(node1), node2(node2)
{
	compute_distance();
	edges.push_back(this);
}
bool Edge::Connects(Node* node1, Node* node2)
{
	return (
		(node1 == this->node1 &&
		node2 == this->node2) ||
		(node1 == this->node2 &&
		node2 == this->node1));
}

void Edge::compute_distance(){
	float k = 2.0;


}


/*
Next up is my code to construct a simple test graph. The starting node is
initialised with a distance of 0. Dijkstra’s algorithm is then called before
the results are printed out. Underneath the code is a diagram of what this
graph looks like. 
*/

bool Contains(vector<Node*>& nodes, Node* node);
float Distance(Node* node1, Node* node2);
vector<Node*>* AdjacentRemainingNodes(Node* node);
Node* ExtractSmallest(vector<Node*>& nodes);
void Dijkstras();
void PrintShortestRouteTo(Node* destination);

// custom
vector<Node*> store_route(Node* destination);
void print_store_route(vector<Node*> route);

// custom
void print_nodes(){
	cout << "Nodes info: " << endl;
	for (int i = 0; i < nodes.size(); ++i)
	{
		nodes.at(i)->print_info();
	}
}

// custom
Node* find_node(string node_id){
	for (int i = 0; i < nodes.size(); ++i)
	{
		if (nodes.at(i)->id ==	node_id) return nodes.at(i);
	}
	return NULL;
}

// custom
void find_route(string test_title, string node_start, string node_end){
	auto route_start = chrono::system_clock::now();

	cout << endl << test_title << endl;

	Node* start = find_node(node_start);
	Node* end = find_node(node_end);

	if (start != NULL && end != NULL){
		start->distanceFromStart = 0; // set start node
		Dijkstras();
		//PrintShortestRouteTo(end);
		vector<Node*> route = store_route(end);
		print_store_route(route);
	}
	else cout << "Start of end nodes does not exist!" << endl;
	auto route_end = chrono::system_clock::now();

	chrono::duration<double> route_elapsed = route_end - route_start;
	cout << "Route found in " << route_elapsed.count() << "s." << endl;
}

// custom
void load_map(){
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

	// Node(string id, float x, float y, int type)
	Node* a = new Node("a", 0, 0, TASK_START, true, 0);
	Node* b = new Node("b", 1, 1, TASK_END, true, 0);
	Node* c = new Node("c", 1, 2, TRANSITION, false, 0);
	Edge* e1 = new Edge(a, b, 1);
	Edge* e2 = new Edge(b, c, 2);
	auto map_end = chrono::system_clock::now();

	chrono::duration<double> map_elapsed = map_end - map_start;
	cout << "\n\nMap created in " << map_elapsed.count() << "s.\n\n";
}

void DijkstrasTest()
{
	// TEST 1
	//print_nodes();
	load_map();
	print_nodes();
	find_route("test 1", "a", "c");
	//print_nodes();

	// TEST 2
	//print_nodes();
	//load_map();
	//print_nodes();
	//find_route("test 2", "a", "f");
	//print_nodes();
	
}



/*
Dijkstra's algorithm

Here is the actual algorithm implementation. We remove the node with the
smallest distance from the list of nodes, and then calculate all the minimum
distances. We repeat until there are no more nodes left. 
*/

void Dijkstras()
{
	while (nodes.size() > 0)
	{
		Node* smallest = ExtractSmallest(nodes);
		vector<Node*>* adjacentNodes =
			AdjacentRemainingNodes(smallest);
		const int size = adjacentNodes->size();
		for (int i=0; i<size; ++i)
		{
			Node* adjacent = adjacentNodes->at(i);
			float distance = Distance(smallest, adjacent) +
				smallest->distanceFromStart;
			if (distance < adjacent->distanceFromStart)
			{
				adjacent->distanceFromStart = distance;
				adjacent->previous = smallest;
			}
		}
		delete adjacentNodes;
	}
}

/*
Here are the supporting functions. The first function removes and returns the
node with the smallest distance from the list.

The next returns a new vector containing all the nodes which are adjacent to
the node that we pass in.

The third returns the distance value of an edge which directly connects the
two nodes.

The final one checks if a node is in the list of nodes.
*/

// Find the node with the smallest distance,
// remove it, and return it.
Node* ExtractSmallest(vector<Node*>& nodes)
{
	int size = nodes.size();
	if (size == 0) return NULL;
	int smallestPosition = 0;
	Node* smallest = nodes.at(0);
	for (int i=1; i<size; ++i)
	{
		Node* current = nodes.at(i);
		if (current->distanceFromStart <
			smallest->distanceFromStart)
		{
			smallest = current;
			smallestPosition = i;
		}
	}
	nodes.erase(nodes.begin() + smallestPosition);
	return smallest;
}
// Return all nodes adjacent to 'node' which are still
// in the 'nodes' collection.
vector<Node*>* AdjacentRemainingNodes(Node* node)
{
	vector<Node*>* adjacentNodes = new vector<Node*>();
	const int size = edges.size();
	for(int i=0; i<size; ++i)
	{
		Edge* edge = edges.at(i);
		Node* adjacent = NULL;
		if (edge->node1 == node)
		{
			adjacent = edge->node2;
		}
		else if (edge->node2 == node)
		{
			adjacent = edge->node1;
		}
		if (adjacent && Contains(nodes, adjacent))
		{
			adjacentNodes->push_back(adjacent);
		}
	}
	return adjacentNodes;
}
// Return distance between two connected nodes
float Distance(Node* node1, Node* node2)
{
	const int size = edges.size();
	for(int i=0; i<size; ++i)
	{
		Edge* edge = edges.at(i);
		if (edge->Connects(node1, node2))
		{
			return edge->distance;
		}
	}
	return -1; // should never happen
}
// Does the 'nodes' vector contain 'node'
bool Contains(vector<Node*>& nodes, Node* node)
{
	const int size = nodes.size();
	for(int i=0; i<size; ++i)
	{
		if (node == nodes.at(i))
		{
			return true;
		}
	}
	return false;
}

/*
And finally here is the code which prints out the route between our start node
‘a’ and the destination node ‘f’. It starts at the destination node and then
works backwards using the previous pointer in each node.
*/

void PrintShortestRouteTo(Node* destination)	// useless, replaced by 'store_route' & 'print_store_route'
{
	Node* previous = destination;
	cout << "Distance from start: "
		<< destination->distanceFromStart << endl;
	while (previous)
	{
		cout << previous->id << " ";
		previous = previous->previous;
	}
	cout << endl;
}


// modification of 'PrintShortestRouteTo'
vector<Node*> store_route(Node* destination)
{
	cout << "store_route..." << endl;
	vector<Node*> route;
	vector<Node*>::iterator it;

	Node* node = destination;
	route.push_back(destination);
	
	while (node)
	{
		it = route.begin();
		it = route.insert(it, node);
		node = node->previous;
	}
	route.pop_back();
	return route;
}

void print_store_route(vector<Node*> route)
{
	cout << "Print store route:";
	for (int i = 0; i < route.size(); ++i)
	{
		cout << " " << route.at(i)->id;
	}
	cout << endl;
}





/*
 * Trying to test the code above
 */
int main(int argc, char const *argv[])
{
	DijkstrasTest();
	return 0;
}