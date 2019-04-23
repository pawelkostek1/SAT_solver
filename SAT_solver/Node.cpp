#include"Node.h"
#include"constants.h"

Node::Node(int _id, int _literalId, int _value, int _level) {
    id = _id;
    literalId = _literalId;
	value = _value;
    level = _level;
    letter = 'A' + _literalId - 1;
}

Node::Node()
{
}

Node::~Node()
{
}

void Node::addParent(int nodeId){
    parentNodes.push_back(nodeId);
}

void Node::removeParent(int nodeId){
    parentNodes.remove(nodeId);
}

