#include "Formula.h"

Formula::Formula(int _numOfvar, int _numOfClauses, vector<int> * _formula)
{

	this->numOfvar = _numOfvar;
	this->numOfClauses = _numOfClauses;
	formula = _formula;
	for (int i = 1; i <= numOfvar; i++) {
		assignements[i] = -1;
	}
	//implicationGraph = new Graph();
}


Formula::~Formula()
{
}

int Formula::getNumOfVar() {
	return this->numOfvar;
}

int Formula::getNumOfClauses() {
	return this->numOfClauses;
}
void Formula::assignVariable(int literal, int value){
    auto it = find(unassignedIndex.begin(),unassignedIndex.end(),literal);
    unassignedIndex.remove(it);
    assignedIndex.push_back(literal);
    assignments[literal] = value;
    cout << assignedIndex << assignments << unassignedIndex;
}

bool Formula::allVariablesAssigned(){
    return this->numOfvar == assignedV.size();
}
