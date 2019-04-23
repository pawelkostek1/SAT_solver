#include"Graph.h"
#include"constants.h"
#include"Node.h"
#include<iostream>
#include<vector>
using namespace std;

Graph::Graph() {
    failedState = Node(0,0,0,0,{});
    levelIndex = {};
}

Graph::~Graph()
{
}

int Graph::addNode(int literal, int level, int value, vector<int> parentLiterals) {
    //max two nodes per literal
    //we start by checking if the node with the same value exists
    int nodeId = literal;
    if (value == 0){
        nodeId = -1*nodeId;
    }
    
    if(levelIndex.find(level) == levelIndex.end()){
        levelIndex[level] = {};
    }
    if(nodes.find(nodeId) == nodes.end()){
        nodes[nodeId] = Node(nodeId,literal,value,level,parentLiterals);
    }else{
        nodes[nodeId].addParents(parentLiterals);
        if(nodes.find(nodeId*-1) != nodes.end()){
            //found a opposite node and thus a conflict
            failedState.addParents({nodeId,nodeId*-1});
        }
    }
    levelIndex[level].push_back(nodeId);
    cout << "NODE:"<<nodes[nodeId].letter<<" (" << nodeId << ") , LEVEL: " << level << " , VALUE: " << value << ", PARENTS: ";
    for(auto const node:parentLiterals){
        cout << node << ",";
    }
    cout << endl;
    return nodeId;
}


void Graph::removeNodesByLiteralId(int literalId){
    if(nodes.find(literalId*-1) != nodes.end()){
        //found a opposite node and thus a conflict
        nodes.erase(literalId*-1);
    }
    if(nodes.find(literalId) != nodes.end()){
        //found a opposite node and thus a conflict
        nodes.erase(literalId);
    }
    
}

int Graph::backtrackToLowestLevelParent() {
    int level;
    for(auto const node:failedState.parentNodes){
        if (level == NULL){
            level = nodes[node].level;
        }else{
            if (nodes[node].level < level){
                level = nodes[node].level;
            }
        }
    }
	return level;
}

void Graph::printGraph(){
    cout << "GRAPH" << endl;
    for (auto& it: nodes){
        Node node = nodes[it.first];
        cout << node.letter << ": ";
        for(auto const& parent : node.parentNodes){
            cout << nodes[parent].letter << ",";
        }
        cout << endl;
    }
    
    
}


