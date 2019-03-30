#pragma once
#include<list>
#include<tuple>
#include"Node.h"
#include<unordered_map>

#include<vector>
using namespace std;

/*
* @description: Graph
*/
class Graph {

public:
    unordered_map<int,Node> nodes;
    vector<vector<int>> levelIndex;
	Graph(); //Constructor
	~Graph();
	int addNode(int absLiteral,int level);

};

