#pragma once
#include<iostream>
#include<vector>
#include<list>
#include<unordered_map>
#include"Graph.h"
#include"Variable.h"
//Intially read the data into array of vectors, we may want to consider implementing our custom object for stroing formula...Formula


class Formula {
	int numOfvar;
	int numOfClauses;

public:
	vector<vector<int> > formula;
	unordered_map<int, Variable> variables;
    list<int> assignedIndex;
    list<int> unassignedIndex;

    unordered_map<int, vector<int> > clausesIndexes;
	Formula(int _numOfvar, int _numOfClauses, vector<vector<int> > _F);
	~Formula();
    Graph implicationGraph;
	int getNumOfVar();
	int getNumOfClauses();
	void assignVariable(int literal, int value,bool implicated);
    void unassignVariable(int literal);
	bool allVariablesAssigned();
	tuple<int,int> getInfered(int clauseIndex);
    int removeSingleLiteralVariables();
    void index(int absLiteral);
    void printFormula();
    void addVariable(int absLiteral,int value);
    void removeVariable(int absLiteral);
};
