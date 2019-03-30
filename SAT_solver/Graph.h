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

public:
    unordered_map<int,Node> nodes;
    unordered_map<int,vector<int>> variableIndex;
    unordered_map<int,vector<int>> levelIndex;
	Graph(); //Constructor
	~Graph();
	int addNode(int literal,int level,vector<Variable> parentVariables);
    void setNodeParents(int nodeId,vector<Variable> parentVariables);
    void removeNodesByLiteralId(int literalId);
    void printGraph();
};

