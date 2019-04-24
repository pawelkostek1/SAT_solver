#pragma once
#include<iostream>
#include<vector>
#include<list>
#include<unordered_map>
#include"Graph.h"
#include"Variable.h"
#include"Clause.h"
#include"constants.h"
#include"Variable.h"
//Intially read the data into array of vectors, we may want to consider implementing our custom object for stroing formula...Formula

class Formula {

	float decay = 0.8;
    unordered_map<int, Variable> variables;
    vector<Clause> formula;
public:
	
	
    list<int> assignedIndex;
    list<int> unassignedIndex;
	vector<int> prevAssignedIndex;
    unordered_map<int, vector<int> > clausesIndexes;
	Formula(vector<Clause> _F,unordered_map<int, Variable>);
    Formula();
	~Formula();
    Graph implicationGraph;
	int getNumOfVar();
	int getNumOfClauses();
	int assignVariable(int literal, int value,int level,vector<int> parentLiterals);
    void unassignVariable(int literal);
	bool allVariablesAssigned();
	int removeSingleLiteralVariables2();
    void index(int absLiteral);
    void printFormula();
	void printVariables();
    void printIndex();
    void removeVariable(int absLiteral);
	void print2Watched(list<int> literalIndeces);

	void bumpActivities(vector<int> learnedClauseVars);
	void decayActivities();
    
    Variable getVariable(int literalId);
    Clause getClause(int clauseId);
    unordered_map<int, Variable> getVariables();
    void removeClauseFromVariableIndex(int literalId,int clauseId,bool negativeIndex);
    void addClauseToVariableIndex(int literalId,int clauseId,bool negativeIndex);
    void changeWatchedLiteral(int clauseId,int currentRelativeLiteralId,int newRelativeLiteralId);
    void addClause(vector<int> clause,bool allowNewVariables);
    void addVariable(int literalId);
};
