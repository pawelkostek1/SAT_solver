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
    int literal;
    int literalId;
    char letter;
	int value;
    
    int level;
    vector<int> parentNodes;
    Node(int _literalId,int _literal, int value,int level,vector<int>parentLiterals);
	Node();
    ~Node();
    void addParents(vector<int>parentLiterals);
    void removeParent(int nodeId);
    
};
