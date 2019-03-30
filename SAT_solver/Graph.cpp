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

int Graph::addNode(int literal, int level, int value, vector<Variable> parentVariables) {
    int id = int(nodes.size());
    nodes[id] = (Node(id,literal,value,level));
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
    for (unsigned int i = 0; i < parentVariables.size(); i++){
        int parentId = parentVariables[i].literal;
        vector<int> parentNodes = variableIndex[parentId];
        for (unsigned int j = 0; j < parentNodes.size(); j++){
            int parentNodeId = parentNodes[j];
            node.addParent(parentNodeId);
        }
        
    }
}

void Graph::removeNodesByLiteralId(int literalId){
    for (unsigned int i =0; i < variableIndex[literalId].size(); i++){
        int nodeId = variableIndex[literalId][i];
        nodes.erase(nodeId);
        
    }
    //reset the variable-node Index since we removed all nodes they point to
    variableIndex[literalId] = vector<int>();
}

int Graph::backtrackToLowestLevelParent(int parentId, int maxLevel) {
	int level = maxLevel;
	int lowestLevelParentId = parentId;
	list<int> listOfParents = nodes[parentId].parentNodes;
	//Backtrack to the parent with the lowest level
	//Runs until the list of parents is empty - the root node was reached
	while (listOfParents.size() != 0) {
		for (auto const& parent : listOfParents) {
			if (nodes[parent].level < level) {
				level = nodes[parent].level;
				lowestLevelParentId = nodes[parent].id;
			}
		}
		listOfParents = nodes[lowestLevelParentId].parentNodes;
	}
	return level;
}

void Graph::printGraph(){
    for(unsigned int i = 0; i < levelIndex.size(); i++){
        //cout << "LVL" << i << endl;
        
    }
}


