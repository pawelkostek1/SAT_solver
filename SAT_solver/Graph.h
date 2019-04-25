#pragma once
#include<list>
#include<tuple>
#include"Node.h"
#include<unordered_map>
#include<vector>
#include"Variable.h"
#include<algorithm>

using namespace std;

/*
* @description: Graph
*/
class Graph {
    unordered_map<int,Node> nodes;
public:
    
    unordered_map<int,vector<int> > levelIndex;
	Graph(); //Constructor
	~Graph();
    Node failedState;
	int addNode(int literalId,int literal, int level, int value, vector<int> parentLiterals);
    void removeNodesByLiteralId(int literalId);
	int getBacktrackLevel();
    void resetFailedState();
    void printGraph();
    Node getNode(int nodeId);
    void backtrackToLevel(int level);
    vector<int> getRoots(Node node);

};

