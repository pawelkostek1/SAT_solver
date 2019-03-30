#pragma once
#include<vector>

using namespace std;

class Variable {
    
public:
    int literal;
    int value;
    char letter;
	float activity;
    Variable(int _literal, int _value);
	Variable();
    ~Variable();
};

struct ImplicationAnalysis{
    Variable target;
    vector<Variable> parents;
};
