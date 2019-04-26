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
    int totalClauseCounter = 0;
    unordered_map<int, Variable> variables;
    unordered_map<int,Clause> formula;
    unordered_map<int,Clause> originalFormula;
public:
	
	
    list<int> assignedIndex;
    list<int> unassignedIndex;
	vector<int> prevAssignedIndex;
    unordered_map<int, vector<int> > clausesIndexes;
	Formula(unordered_map<int,Clause> _F,unordered_map<int, Variable>);
    Formula();
	~Formula();
    Graph implicationGraph;
	int getNumOfVar();
	int getNumOfClauses();
	int assignVariable(int literalId,int literal, int value,int level,vector<int> parentLiterals);
    void unassignVariable(int literal);
	bool allVariablesAssigned();
	int removeSingleLiteralVariables2();
    void index(int absLiteral);
    void printFormula();
	void printVariables();
    void printIndex();
    void removeVariable(int absLiteral);
	void print2Watched(bool verbose);
    void printClauses();
	void bumpActivities(vector<int> learnedClauseVars);
	void decayActivities();
    
    Variable getVariable(int literalId);
    Clause getClause(int clauseId);
    bool needsSingleRemovalPass = true;
    unordered_map<int, Variable> getVariables();
    void removeClauseFromVariableIndex(int literalId,int clauseId,bool negativeIndex);
    void addClauseToVariableIndex(int literalId,int clauseId,bool negativeIndex);
    void changeWatchedLiteral(int clauseId,int currentRelativeLiteralId,int newRelativeLiteralId);
    int addClause(vector<int> clause,bool allowNewVariables);
    void addVariable(int literalId);
    void removeClause(int clauseId);
    //void updateClause(int clauseId,vector<int> clause);
    bool checkSolution();
    void printClause(int clauseId);
    void removeClauseLiteral(int clauseId, int pRemove);
};
