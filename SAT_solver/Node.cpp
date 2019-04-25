#include"Node.h"
#include"constants.h"

Node::Node(int _literalId, int _literal,  int _value,int _level,vector<int> parentLiterals) {
    
    literalId = abs(_literalId);
    literal = _literal;
	value = _value;
    level = _level;
    letter = 'A' + _literalId - 1;
    parentNodes = parentLiterals;
}

Node::Node()
{
}

Node::~Node()
{
}

void Node::addParents(vector<int>parentLiterals){
    for(auto const l :parentLiterals){
        if(find(parentNodes.begin(), parentNodes.end(),l)==parentNodes.end()){
            parentNodes.push_back(l);
        }
    }
}

void Node::removeParent(int nodeId){
    //parentNodes.remove(nodeId);
}

