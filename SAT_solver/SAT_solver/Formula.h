#pragma once
#include<vector>
#include<list>
#include"Graph.h"
using namespace std;

//Define variables
/*
* @description: structure defining an assignement of the value to a given literal
*/
struct assignemnt {
	int x; //literal
	int val; //can be 0 or 1
};

//Intially read the data into array of vectors, we may want to consider implementing our custom object for stroing formula...Formula
class Formula {
	int numOfvar;
	int numOfClauses;
public:
	vector<int> * formula;
	list<assignemnt> assignedV;
    list<int> unassignedV;
	Formula(int _numOfvar, int _numOfClauses, vector<int> * _F);
	~Formula();
    Graph implicationGraph;
	int getNumOfVar();
	int getNumOfClauses();
    int assignVariable(int literal,int value);
    boolean allVariablesAssigned();
};
