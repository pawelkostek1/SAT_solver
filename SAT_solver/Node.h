#pragma once
#include<list>
#include<tuple>
#include<vector>
using namespace std;

/*
* @description: Node
*/
class Node {
public:
    //this is a reference to the original
    int id;
    int literalId;
    char letter;
	int value;
    
    int level;
    vector<int> parentNodes;
    Node(int _id, int _literalId, int value,int level,vector<int>parentLiterals);
	Node();
    ~Node();
    void addParents(vector<int>parentLiterals);
    void removeParent(int nodeId);
    
};
