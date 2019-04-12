#include "Variable.h"
#include "vector"
Variable::Variable(int _literal,int _value) {
    literal = _literal;
    value = _value;
    letter = 'A' + _literal - 1;
	activity = 0.0;
    
}
void Variable::addPositiveClause(int clauseId){
    postiveClauses.push_back(clauseId);
}
void Variable::addNegativeClause(int clauseId){
    negativeClauses.push_back(clauseId);
}
void Variable::removeNegativeClause(int clauseId){
    auto it = find(negativeClauses.begin(),negativeClauses.end(),clauseId);
    if(it != negativeClauses.end()){
        negativeClauses.erase(it);
    }
}
void Variable::removePositiveClause(int clauseId){
    auto it = find(postiveClauses.begin(),postiveClauses.end(),clauseId);
    if(it != postiveClauses.end()){
        postiveClauses.erase(it);
    }
}
Variable::Variable()
{
}

Variable::~Variable()
{
}



