
#include "dijkstra.h"


//vector<Node*> nodes;	// load each time you wan to use it, can be different
						// every time (only subsections of the graph)
						// it empties after each call of 'Dijkstra'
//vector<Edge*> edges;	// only need to be loaded/created once


Dijkstra::Dijkstra(Graph* graph){
	this->graph = graph;
}



void Dijkstra::find_route(string node_start, string node_end){
	auto route_start = chrono::system_clock::now();

	cout << endl << "Find route..." << endl;

	Node* start = graph->find_node(node_start);
	Node* end = graph->find_node(node_end);

	if (start != NULL && end != NULL){
		start->distanceFromStart = 0; // set start node
		Dijkstras();
		//PrintShortestRouteTo(end);
		vector<Node*> route = store_route(end);
		this->route = route;
		//print_route(route);
	}
	else cout << "Start of end nodes does not exist!" << endl;
	auto route_end = chrono::system_clock::now();

	chrono::duration<double> route_elapsed = route_end - route_start;
	cout << "Route found in " << route_elapsed.count() << "s." << endl;
}



// modification of 'PrintShortestRouteTo'
vector<Node*> Dijkstra::store_route(Node* destination)
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


void Dijkstra::print_route(){
	print_route(this->route);
}


void Dijkstra::print_route(vector<Node*> route)
{
	cout << "Print store route:";
	int i = 0;
	cout << "\n  Node " << i << ": " << route.at(i)->id << endl;
	Edge* edge;
	for (i = 1; i < route.size(); i++){
		edge = this->graph->find_edge(route.at(i-1), route.at(i));
		if (edge == NULL) cout << "\nEdge: No edge found.\n";
		else edge->print_info();
		cout << "\n  Node " << i << ": " << route.at(i)->id << endl;
	}
	cout << endl;
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
Node* Dijkstra::ExtractSmallest(vector<Node*>& nodes)
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
vector<Node*>* Dijkstra::AdjacentRemainingNodes(Node* node)
{
	vector<Node*>* adjacentNodes = new vector<Node*>();
	const int size = graph->edges.size();
	for(int i=0; i<size; ++i)
	{
		Edge* edge = graph->edges.at(i);
		Node* adjacent = NULL;
		if (edge->node1 == node)
		{
			adjacent = edge->node2;
		}
		else if (edge->node2 == node)
		{
			adjacent = edge->node1;
		}
		if (adjacent && Contains(graph->nodes, adjacent))
		{
			adjacentNodes->push_back(adjacent);
		}
	}
	return adjacentNodes;
}


// Return distance between two connected nodes
float Dijkstra::Distance(Node* node1, Node* node2)
{
	const int size = graph->edges.size();
	for(int i=0; i<size; ++i)
	{
		Edge* edge = graph->edges.at(i);
		if (edge->Connects(node1, node2))
		{
			return edge->distance;
		}
	}
	return 10000000; // should never happen
}


// Does the 'nodes' vector contain 'node'
bool Dijkstra::Contains(vector<Node*>& nodes, Node* node)
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
Dijkstra's algorithm

Here is the actual algorithm implementation. We remove the node with the
smallest distance from the list of nodes, and then calculate all the minimum
distances. We repeat until there are no more nodes left. 
*/
void Dijkstra::Dijkstras()
{
	while (graph->nodes.size() > 0)
	{
		Node* smallest = ExtractSmallest(graph->nodes);
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
And finally here is the code which prints out the route between our start node
‘a’ and the destination node ‘f’. It starts at the destination node and then
works backwards using the previous pointer in each node.
*//*
void Dijkstra::PrintShortestRouteTo(Node* destination)	// useless, replaced by 'store_route' & 'print_route'
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
}*/










































