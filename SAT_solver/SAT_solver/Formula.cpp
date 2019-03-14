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

