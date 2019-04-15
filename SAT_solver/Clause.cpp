#include "Clause.h"
#include "Formula.h"
#include<iostream>
#include<cmath>
Clause::Clause(vector<int> _value) {
    literals = _value;
	for (int i = 0; i < _value.size(); i++)
		literalIds.push_back(abs(_value[i]));
    p1 = 0;
    p2 = 1;
    
}
bool Clause::evaluate(int p, int value){
    if (literals[p] < 0 && (value == 0 || value == -1)){
        return true;
    }
    if (literals[p] > 0 && (value == 1 || value == -1)){
        return true;
    }
    return false;
}
int Clause::pointerToLiteralID(int p){
    return literalIds[p];
}
int Clause::whatValueMakesThisLiteralTrue(int p){
    if (literals[p] < 0){
        return 0;
    }else{
        return 1;
    }
}

vector<int> Clause::getParentsByPointer(int p){
    vector<int> parents = literalIds;
    parents.erase(literalIds.begin()+p);
    return parents;
  
}
Clause::~Clause()
{
}

Clause::Clause()
{
}

void Clause::printClause()
{
	std::cout << "Clause: ";
	for (unsigned int j = 0; j < literals.size(); j++) {
		if (literals[j] < 0) {
			std::cout << "NOT " << abs(literals[j]);
		}
		else {
			std::cout << literals[j];
		}
		
		if (j < literals.size() - 1) {
			std::cout << " OR ";
		}
	}
	std::cout << std::endl;
}



