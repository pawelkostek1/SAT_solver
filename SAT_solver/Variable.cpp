
#include "Variable.h"
#include "vector"

Variable::Variable(int _literalId,int _literal,int _value) {
    literalId = _literalId;
    literal = _literal;
    value = _value;
    letter = 'A' + _literalId - 1;
	activity = 0.0;
    
}
void Variable::addPositiveClause(string clauseId){
    auto it = find(postiveClauses.begin(),postiveClauses.end(),clauseId);
    if(it == postiveClauses.end()){
        postiveClauses.push_back(clauseId);
    }else{
        ////cout << "";
        ////cout << "TRYING TO ADD CLAUSE " << clauseId << " TO NEGATIVE INDEX FOR VAR " << letter << "(" << literalId << ")" << endl;
        throw "ERROR CLAUSE ALREADY EXISTS IN POSTIVE INDEX";
    }
}
void Variable::addNegativeClause(string clauseId){
    auto it = find(negativeClauses.begin(),negativeClauses.end(),clauseId);
    if(it == negativeClauses.end()){
        negativeClauses.push_back(clauseId);
    }else{
        ////cout << "";
        throw "ERROR CLAUSE ALREADY EXISTS IN NEGATIVE INDEX";
     }
    
    
}
void Variable::removeNegativeClause(string clauseId){
    auto it = find(negativeClauses.begin(),negativeClauses.end(),clauseId);
    if(it != negativeClauses.end()){
        
        negativeClauses.erase(it);
    }else{
        ////cout << "";
        ////cout << "TRYING TO ADD CLAUSE " << clauseId << " TO NEGATIVE INDEX FOR VAR " << letter << "(" << literalId << ")" << endl;
        throw "ERROR CLAUSE NOT EXISTS IN NEGATIVE INDEX";
    }
}
void Variable::removePositiveClause(string clauseId){
    auto it = find(postiveClauses.begin(),postiveClauses.end(),clauseId);
    if(it != postiveClauses.end()){
        postiveClauses.erase(it);
    }else{
        ////cout << "";
        ////cout << "TRYING TO ADD CLAUSE " << clauseId << " TO NEGATIVE INDEX FOR VAR " << letter << "(" << literalId << ")" << endl;
        throw "ERROR CLAUSE NOT EXISTS IN POSTIVE INDEX";
    }
}
Variable::Variable()
{
}

Variable::~Variable()
{
}





