#include"Graph.h"
#include"constants.h"
#include"Node.h"
#include<vector>
using namespace std;

Graph::Graph() {
	
}

Graph::~Graph()
{
}

int Graph::addNode(int literal,int level,vector<Variable> parentVariables) {
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
    setNodeParents(id,parentVariables);
    return id;
}
void Graph::setNodeParents(int nodeId,vector<Variable> parentVariables){
    
    Node node = nodes[nodeId];
    for (int i = 0; i < parentVariables.size(); i++){
        int parentId = parentVariables[i].literal;
        vector<int> parentNodes = variableIndex[parentId];
        for (int j = 0; j < parentNodes.size(); j++){
            int parentNodeId = parentNodes[j];
            node.addParent(parentNodeId);
        }
        
    }
}


void Graph::removeNodesByLiteralId(int literalId){
    for (int i =0; i < variableIndex[literalId].size(); i++){
        int nodeId = variableIndex[literalId][i];
        nodes.erase(nodeId);
        
    }
    //reset the variable-node Index since we removed all nodes they point to
    variableIndex[literalId] = vector<int>();
}

void Graph::printGraph(){
    for(int i = 0; i < levelIndex.size(); i++){
        //cout << "LVL" << i << endl;
        
    }
}


