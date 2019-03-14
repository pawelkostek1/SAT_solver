#pragma once
#include<list>

using namespace std;

/*
* @description: Node
*/
class Node {
	int v;
	int edge;
public:
	Node(int _v, int _e) {
		v = _v;
		edge = _e;
	}
	int getV() {
		return v;
	}
	int getEdge() {
		return edge;
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
	Graph(int _numOfvertices); //Constructor
	~Graph();

	void addEdge(int u, int v, int edge);

};

