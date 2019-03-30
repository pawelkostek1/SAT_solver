#include "Graph.h"
#include "constants.h"
#include"Node.h"
#include<vector>

Graph::Graph() {
	
}

Graph::~Graph()
{
}

int Graph::addNode(int literal,int level) {
    int id = int(nodes.size());
    nodes[literal] = (Node(id,literal,level));
    if(levelIndex.size() < level){
        levelIndex.push_back(vector<int>());
    }
    levelIndex[level].push_back(literal);
    return id;
}


