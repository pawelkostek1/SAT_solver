#include "Graph.h"



Graph::Graph(int V) {
	this->V = V;
	adj = new list<Node>[V];
}

void Graph::addEdge(int u, int v, int edge) {
	Node node(v, edge);
	adj[u].push_back(node);
}


Graph::~Graph()
{
}
