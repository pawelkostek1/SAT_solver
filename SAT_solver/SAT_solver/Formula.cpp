#include "Formula.h"

Formula::Formula(int _numOfvar, int _numOfClauses, vector<int> * _F)
{
	F = _F;
	this->numOfvar = _numOfvar;
	this->numOfClauses = _numOfClauses;

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
assignement Formula::assignVariable(int literal, int value){
    //check if literal is in list
    auto it = find(unassignedV.begin(), unassignedV.end(), literal);
    // remove literal from unassignedV
    unassignedV.remove(it,literal)
    //create assignement
    assignedV.push_back(assignment(literal,value))


    // add assignement to assignedV
}

boolean Formula::allVariablesAssigned(){
    return this->numOfvar == assignedV.size();
}
