#include "Formula.h"

using namespace std;


Formula::Formula(int _numOfvar, int _numOfClauses, vector<int> * _formula)
{

	this->numOfvar = _numOfvar;
	this->numOfClauses = _numOfClauses;
	formula = _formula;
	for (int i = 1; i <= numOfvar; i++) {
		assignments[i] = -1;
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
    unassignedIndex.remove(literal);
    assignedIndex.push_back(literal);
	assignments[literal] = value;
	for (std::list<int>::iterator it = assignedIndex.begin(); it != assignedIndex.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl;
	for (std::list<int>::iterator it = unassignedIndex.begin(); it != unassignedIndex.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl;
	for (int i = 1; i <= assignments.size(); i++) {
		cout << i << ": " << assignments[i] << endl;
	}
}

bool Formula::allVariablesAssigned(){
    return this->numOfvar == assignedIndex.size();
}
