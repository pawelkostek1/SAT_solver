#pragma once
#include<list>
#include<tuple>
using namespace std;



/*
* @description: Node
*/
class Node {
public:
    int id;
    //this is a reference to the original
    int literalId;
    int level;
    list<int> parentNodes;
    Node(int _id, int _literalId,int _level);
    ~Node();
    void addParent(int nodeId);
    void removeParent(int nodeId);
};
