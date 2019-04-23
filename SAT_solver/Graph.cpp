#include"Graph.h"
#include"constants.h"
#include"Node.h"
#include<iostream>
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
	cout << "levelIndex size: " << levelIndex.size() << endl;
    if(levelIndex.find(level) == levelIndex.end()){
        levelIndex[level] = vector<int>();
    }
	//cout << "================================" << endl;
    if(variableIndex.find(literal) == variableIndex.end()){
        variableIndex[literal] = vector<int>();
    }


    levelIndex[level].push_back(literal);
    
	cout << "levelIndex size: " << levelIndex[level].size() << endl;
	//cout << "variableIndex before adding: ";
	//for (int i = 0; i < variableIndex[literal].size(); i++)
	//	cout << variableIndex[literal][i] << " ";
	//cout << endl;
    variableIndex[literal].push_back(id);
	//cout << "variableIndex after adding: ";
	//for (int i = 0; i < variableIndex[literal].size(); i++)
	//	cout << variableIndex[literal][i] << " ";
	//cwout << endl;
	//cout << "================================" << endl;
    setNodeParents(id,parentVariables);
    return id;
}
void Graph::setNodeParents(int nodeId,vector<Variable> parentVariables){
    
    for (unsigned int i = 0; i < parentVariables.size(); i++){
        int parentId = parentVariables[i].literal;
        vector<int> parentNodes = variableIndex[parentId];
        for (unsigned int j = 0; j < parentNodes.size(); j++){
            int parentNodeId = parentNodes[j];
			nodes[nodeId].addParent(parentNodeId);
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

	cout << "Removed node for literalID: " << literalId << " and node indeces: ";
	for (int i = 0; i < variableIndex[literalId].size(); i++)
		cout << variableIndex[literalId][i] << " ";
	cout << endl;
}

int Graph::backtrackToLowestLevelParent(int parentId, int maxLevel) {
	int level = maxLevel;
	int lowestLevelParentId = parentId;
	int prevLowestLevelParentId = lowestLevelParentId;
	list<int> listOfParents = nodes[parentId].parentNodes;
	cout << "Size of listOfParents = " << listOfParents.size()<<endl;
	//Backtrack to the parent with the lowest level
	//Runs until the list of parents is empty - the root node was reached
	while (listOfParents.size() != 0) {
		cout << "Printing parents for node: " << nodes[parentId].literalId << endl;
		prevLowestLevelParentId = lowestLevelParentId;
		for (auto const& parent : listOfParents) {
			if (nodes[parent].level < level) {
				level = nodes[parent].level;
				
				lowestLevelParentId = nodes[parent].id;
			}
			cout << nodes[parent].literalId<<endl;
		}
		if (lowestLevelParentId == prevLowestLevelParentId)
			break;
		else
			listOfParents = nodes[lowestLevelParentId].parentNodes;
	}
	return level;
}

void Graph::printGraph(){
    cout << "GRAPH" << endl;
    for(unsigned int i = 0; i < levelIndex.size(); i++){
        cout << "LVL" << i << endl;
        vector<int> index = levelIndex[i];
        for(unsigned int j = 0; j < levelIndex.size(); j++){
            Node node = nodes[j];
            if (j == 0){
                cout << node.letter << " : ";
            }else{
                cout << node.letter << "( " ;
               for (auto const& parent : node.parentNodes) {
                    cout << nodes[parent].letter << ",";
                }
                cout << " )" << endl;
            }
            
        }
    }
}


