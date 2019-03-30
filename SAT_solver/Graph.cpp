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
    nodes[id] = (Node(id,literal,level));
    if(levelIndex.find(level) != levelIndex.end()){
        levelIndex[level] = vector<int>();
    }
    if(variableIndex.find(level) != variableIndex.end()){
        variableIndex[literal] = vector<int>();
    }
    levelIndex[level].push_back(literal);
    variableIndex[literal].push_back(id);
    return id;
}
void Graph::setNodeParent(int nodeId,list<Variable> parentNodes){
    Node node = nodes[nodeId];
    for (int i = 0; i < parentNodes.size(); i++){
        node.addParent(<#int nodeId#>)
    }
}



