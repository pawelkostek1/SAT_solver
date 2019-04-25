#pragma once
#include<vector>

using namespace std;

class Variable {
    
public:
    bool locked = false;
    int literal;
    int literalId;
    int value;
    char letter;
	float activity;
    vector<int> postiveClauses;
    vector<int> negativeClauses;
    Variable(int _literalId,int _literal, int _value);
	Variable();
    ~Variable();
    void addPositiveClause(int clauseId);
    void addNegativeClause(int clauseId);
    void removePositiveClause(int clauseId);
    void removeNegativeClause(int clauseId);
};

struct ImplicationAnalysis{
    Variable target;
    vector<Variable> parents;
};
