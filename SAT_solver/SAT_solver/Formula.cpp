#include "Formula.h"

using namespace std;


Formula::Formula(int _numOfvar, int _numOfClauses, vector<int> * _formula)
{

	this->numOfvar = _numOfvar;
	this->numOfClauses = _numOfClauses;
	formula = _formula;
	for (int i = 1; i <= numOfvar; i++) {
		assignments[i] = -1;
		unassignedIndex.push_back(i);
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

void Formula::assignVariable(int literal, int value) {
	unassignedIndex.remove(literal);
	assignedIndex.push_back(literal);

	assignments[literal] = value;
	cout << "Assigned variables:" << endl;
	for (list<int>::iterator it = assignedIndex.begin(); it != assignedIndex.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl;
	cout << "Unassigned variables:" << endl;
	for (list<int>::iterator it = unassignedIndex.begin(); it != unassignedIndex.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl;
	cout << "Assignements:" << endl;
	for (unsigned int i = 1; i <= assignments.size(); i++) {
		cout << i << ": " << assignments[i] << endl;
	}
}

void Formula::unassignVariable(int literal) {
    assignedIndex.remove(literal);
    unassignedIndex.push_back(literal);
    assignments[literal] = -1;
}

bool Formula::allVariablesAssigned(){
    return this->numOfvar == assignedIndex.size();
}
