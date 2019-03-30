#pragma once
#include<iostream>
#include<vector>
#include<list>
#include<unordered_map>
#include"Graph.h"
#include"Change.h"
#include"Clause.h"
//Intially read the data into array of vectors, we may want to consider implementing our custom object for stroing formula...Formula


class Formula {
	int numOfvar;
	int numOfClauses;
	float decay = 0.8;

public:
	vector<Clause> formula;
	unordered_map<int, Variable> variables;
    list<int> assignedIndex;
    list<int> unassignedIndex;

    unordered_map<int, vector<int> > clausesIndexes;
	Formula(int _numOfvar, int _numOfClauses, vector<Clause> _F);
	~Formula();
    Graph implicationGraph;
	int getNumOfVar();
	int getNumOfClauses();
	int assignVariable(int literal, int value,int level);
    void unassignVariable(int literal);
	bool allVariablesAssigned();
	Variable getInferred(int clauseIndex);
    int removeSingleLiteralVariables();
    void index(int absLiteral);
    void printFormula();
    void addVariable(int absLiteral,int value);
    void removeVariable(int absLiteral);

	void bumpActivities(vector<int> learnedClauseVars);
	void decayActivities();
};
