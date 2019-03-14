#include "Graph.h"



Graph::Graph(int _numOfvertices) {
	this->numOfvertices = _numOfvertices;
	adj = new list<Node>[numOfvertices];
}

void Graph::addEdge(int u, int v, int edge) {
	Node node(v, edge);
	adj[u].push_back(node);
}


Graph::~Graph()
{
}
