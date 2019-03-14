#pragma once
#include<iostream>
#include<vector>
#include<list>
#include<unordered_map>
#include"Graph.h"

//Intially read the data into array of vectors, we may want to consider implementing our custom object for stroing formula...Formula
class Formula {
	int numOfvar;
	int numOfClauses;

public:
	vector<int> * formula;
	unordered_map<int, int> assignments;
    list<int> assignedIndex;
    list<int> unassignedIndex;
	Formula(int _numOfvar, int _numOfClauses, vector<int> * _F);
	~Formula();
    //Graph implicationGraph;
	int getNumOfVar();
	int getNumOfClauses();
	void assignVariable(int literal, int value);
    void unassignVariable(int literal);
	bool allVariablesAssigned();
};
