#pragma once
#include<list>
#include<tuple>
using namespace std;



/*
* @description: Node
*/
class Node {
	int value;
    int absLiteral;
    int depth;
    int type;
    list<Node> parentNodes;
    
public:
	Node(tuple<int,int> _variable,int _type,int _depth, list<Node> _parentNodes) {
		value = get<1>(_variable);
        absLiteral = get<0>(_variable);
		parentNodes = _parentNodes;
        depth = _depth;
        type = _type;
	}
	int getV() {
		return value;
	}
};

/*
* @description: Graph
*/
class Graph {
	int numOfvertices; //No. of vertices

	//Pointer to an array containng adjacency lists
	list<Node> *adj;

public:
	Graph(); //Constructor
	~Graph();
	void addNode(list<Node> parentNodes,int literal,bool implication);

};

