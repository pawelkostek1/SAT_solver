#pragma once
#include<vector>
#include<string>
using namespace std;

class Variable {
    
public:
    bool locked = false;
    int literal;
    int literalId;
    int value;
    char letter;
	float activity;
    vector<string> postiveClauses;
    vector<string> negativeClauses;
    Variable(int _literalId,int _literal, int _value);
	Variable();
    ~Variable();
    void addPositiveClause(string clauseId);
    void addNegativeClause(string clauseId);
    void removePositiveClause(string clauseId);
    void removeNegativeClause(string clauseId);
};

struct ImplicationAnalysis{
    Variable target;
    vector<Variable> parents;
};
