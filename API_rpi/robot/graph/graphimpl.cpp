/*
* File: graphimpl.cpp
* -------------------
* This file provides the private implementation for the Graph
* class. As is typical for layered abstractions built on top
* of other classes, the implementations of these methods tend
* to be one or two lines long and require no detailed commentary.
*/
#ifdef _graph_h
#include "genlib.h"
/*
* Implementation notes: NodeCompare, ArcCompare
* ---------------------------------------------
* These generic functions compare nodes and arcs by comparing
* the node names alphabetically.
*/
template <typename NodeType>
int NodeCompare(NodeType *n1, NodeType *n2) {
if (n1->name == n2->name) return 0;
return (n1->name < n2->name) ? -1 : +1;
}
template <typename NodeType,typename ArcType>
int ArcCompare(ArcType *a1, ArcType *a2) {
NodeType *n1 = a1->start;
NodeType *n2 = a2->start;
int cmp = NodeCompare(n1, n2);
if (cmp != 0) return cmp;
n1 = a1->finish;
n2 = a2->finish;
return NodeCompare(n1, n2);
}
/*
* Implementation notes: Constructor
* ---------------------------------
* The constructor requires no code in its function body, but
* does use the initialization list to set up the nodes and
* arc sets with the appropriate comparison functions.
*/
template <typename NodeType,typename ArcType>
Graph<NodeType,ArcType>::Graph() : nodes(NodeCompare<NodeType>),
arcs(ArcCompare<NodeType,ArcType>) {
/* Empty */
}
template <typename NodeType,typename ArcType>
Graph<NodeType,ArcType>::~Graph() {
clear();
}


/////////////////
// end  page 1 //
/////////////////


template <typename NodeType,typename ArcType>
void Graph<NodeType,ArcType>::clear() {
foreach (NodeType *node in nodes) {
delete node;
}
foreach (ArcType *arc in arcs) {
delete arc;
}
arcs.clear();
nodes.clear();
nodeMap.clear();
}
template <typename NodeType,typename ArcType>
NodeType *Graph<NodeType,ArcType>::addNode(NodeType *node) {
nodes.add(node);
nodeMap[node->name] = node;
return node;
}
template <typename NodeType,typename ArcType>
NodeType *Graph<NodeType,ArcType>::addNode(string name) {
NodeType *node = new NodeType();
node->arcs = Set<ArcType *>(ArcCompare<NodeType,ArcType>);
node->name = name;
return addNode(node);
}
template <typename NodeType,typename ArcType>
ArcType *Graph<NodeType,ArcType>::addArc(ArcType *arc) {
arc->start->arcs.add(arc);
arcs.add(arc);
return arc;
}
template <typename NodeType,typename ArcType>
ArcType *Graph<NodeType,ArcType>::addArc(NodeType *n1, NodeType *n2) {
ArcType *arc = new ArcType();
arc->start = n1;
arc->finish = n2;
arc->cost = 1;
return addArc(arc);
}
template <typename NodeType,typename ArcType>
ArcType *Graph<NodeType,ArcType>::addArc(string s1, string s2) {
return addArc(getNode(s1), getNode(s2));
}
template <typename NodeType,typename ArcType>
bool Graph<NodeType,ArcType>::isConnected(NodeType *n1, NodeType *n2) {
foreach (ArcType *arc in n1->arcs) {
if (arc->finish == n2) return true;
}
return false;
}


/////////////////
// end  page 2 //
/////////////////


template <typename NodeType,typename ArcType>
bool Graph<NodeType,ArcType>::isConnected(string s1, string s2) {
return isConnected(getNode(s1), getNode(s2));
}
template <typename NodeType,typename ArcType>
NodeType *Graph<NodeType,ArcType>::getNode(string name) {
if (nodeMap.containsKey(name)) return nodeMap.get(name);
return NULL;
}
template <typename NodeType,typename ArcType>
Set<NodeType *> & Graph<NodeType,ArcType>::getNodeSet() {
return nodes;
}
template <typename NodeType,typename ArcType>
Set<ArcType *> & Graph<NodeType,ArcType>::getArcSet() {
return arcs;
}
template <typename NodeType,typename ArcType>
Set<ArcType *> & Graph<NodeType,ArcType>::getArcSet(NodeType *node) {
return node->arcs;
}
#endif


/////////////////
// end  page 3 //
/////////////////