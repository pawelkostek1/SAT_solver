#include "Graph.h"
#include "constants.h"
#include"Node.h"
Graph::Graph() {
	
}

Graph::~Graph()
{
}

int Graph::addNode(int literal,int level) {
    int id = int(nodes.size());
    nodes[literal] = (Node(id,literal,level));
    return id;
}


