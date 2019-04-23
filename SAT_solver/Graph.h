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
    unordered_map<int,vector<int> > levelIndex;
	int ConflictingLiteralId;
	Graph(); //Constructor
	~Graph();
    Node failedState;
	int addNode(int literal, int level, int value, vector<int> parentLiterals);
    void removeNodesByLiteralId(int literalId);
	int backtrackToLowestLevelParent();
    void printGraph();
};

