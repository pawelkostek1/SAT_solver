#pragma once
#include<list>
#include<tuple>
#include"Node.h"
#include<unordered_map>
using namespace std;

/*
* @description: Graph
*/
class Graph {

public:
    unordered_map<int,Node> nodes;
    unordered_map<int,int> levelIndex;
	Graph(); //Constructor
	~Graph();
	int addNode(int absLiteral,int level);

};

