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

int Graph::addNode(int literalId,int literal, int level, int value, vector<int> parentLiterals) {
    //max two nodes per literal
    //we start by checking if the node with the same value exists
    int nodeId = literal;
   
    
    if(levelIndex.find(level) == levelIndex.end()){
        levelIndex[level] = {};
    }
    if(nodes.find(nodeId) == nodes.end()){
        
        Node n = Node(nodeId,literal,value,level,parentLiterals);
        nodes[nodeId] = n;
   
    }else{
        
        nodes[nodeId].addParents(parentLiterals);
        
    }
    
    levelIndex[level].push_back(nodeId);
    cout << "NODE:"<<nodes[nodeId].letter<<" (" << nodeId << ") , LEVEL: " << level << " , VALUE: " << value << ", PARENTS: ";
    for(auto const node:parentLiterals){
        cout << nodes[node].letter << ",";
    }
    cout << endl;
    if(nodes.find(nodeId*-1) != nodes.end()){
        //found a opposite node and thus a conflict
        failedState.addParents({nodeId,nodeId*-1});
        return CONFLICT;
    }else{
        return NOCONFLICT;
    }
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

int Graph::getBacktrackLevel() {
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

void Graph::resetFailedState(){
    failedState.parentNodes = {};
}

void Graph::printGraph(){
    cout << "GRAPH" << endl;
    unordered_map<int, Node>::iterator it = nodes.begin();
    while(it != nodes.end())
    {
        cout << it->second.letter << ": ";
        for(auto const parent : it->second.parentNodes){
            cout << nodes[parent].letter << ",";
        }
        cout << endl;
        it++;
    }
    
    
    
    
}



Node Graph::getNode(int nodeId){
    if(nodes.find(nodeId) != nodes.end()){
        cout <<"NODE: "<< nodeId << " " << nodes[nodeId].literalId << endl;
        return nodes[nodeId];
    }else{
        throw "NODE DOES NOT EXISTS";
    }
}
