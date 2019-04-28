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
    int nodeId = literalId*(value == 0? -1:1);
    if(levelIndex.find(level) == levelIndex.end()){
        levelIndex[level] = {};
    }
    Node node ;
    if(nodes.find(literal) == nodes.end()){
        
        node = Node(literalId,literal,value,level,parentLiterals);
        
        nodes[nodeId] = node;
   
    }else{
        
        node = getNode(nodeId);
        if(node.level != level)
            node.addParents(parentLiterals);
        nodes[nodeId] = node;
    }
    
    levelIndex[level].push_back(literal);
    //cout << "NODE:"<<node.letter<<" (" << literal << ") , LEVEL: " << level << " , VALUE: " << value << ", PARENTS: ";
    for(auto const parent:parentLiterals){
        if(node.literalId == 2){
            
            
        }
        Node pNode = getNode(parent);
        //cout << pNode.letter << ",";
      
        
    }
    //cout << endl;
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
void backtrackToLevel(int level){
    
}
void Graph::resetFailedState(){
    failedState.parentNodes = {};
    
}

void Graph::printGraph(){
    //cout << "GRAPH" << endl;
    for(auto level : levelIndex)
    {
        //cout << "LVL" << level.first << endl;
        for (auto const nodeId: level.second){
            Node  node = getNode(nodeId);
            //cout << "(" << node.literalId << ") ";
            if (node.value == 0){
                //cout  << "-";
            }
            
            //cout << node.letter << ": " << node.parentNodes.size() << " -> ";
            for(int i = 0; i < node.parentNodes.size(); i++){
                
                Node pNode = getNode(node.parentNodes[i]);
                //cout << (i > 0?",":"") << (pNode.value == 0?"-":"") << pNode.letter;
            }
            //cout << endl;
        }
        
    }
    
    
    
    
}



Node Graph::getNode(int nodeId){
    if(nodes.find(nodeId) != nodes.end()){
        ////cout <<"NODE: "<< nodeId << " " << nodes[nodeId].literalId << endl;
        return nodes[nodeId];
    }else{
        printGraph();
        throw "NODE DOES NOT EXISTS";
        
    }
}



vector<int> Graph::getRoots(Node node){
    vector<int> pn = node.parentNodes;
    vector<int> rn = {};
    while(pn.size() > 0){
        int nId = pn.front();
        Node n = getNode(nId);
        pn.erase(pn.begin());
        if (n.parentNodes.size() == 0){
            if (find(rn.begin(),rn.end(),nId) == rn.end()){
                rn.push_back(nId);
            }
        }else{
            for(auto&& ppn:n.parentNodes){
                pn.push_back(ppn);
            }
            
        }
    }
    
    return rn;
}
