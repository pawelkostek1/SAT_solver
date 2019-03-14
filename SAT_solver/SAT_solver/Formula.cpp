#include "Formula.h"



Formula::Formula(int _numOfvar, int _numOfClauses)
{
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

list<assignemnt> Formula::getListOfAssignements() {
	return this->v;
}

void Formula::getListOfUnassignVar() {
}