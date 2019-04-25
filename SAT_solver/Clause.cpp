#include "Clause.h"
#include "Formula.h"
#include<iostream>
#include<cmath>
Clause::Clause(vector<int> _value) {
    //literals = _value;
	copy(_value.begin(), _value.end(), back_inserter(literals));
	for (int i = 0; i < _value.size(); i++) {
		literalIds.push_back(abs(_value[i]));
		//cout << _value[i]<<" ";
	}
	//cout << endl;

    p1 = literalIds.size() > 0? 0:-1;
    p2 = literalIds.size() > 1? 1:-1;
    
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


bool Clause::evaluateAll(unordered_map<int,Variable> assignments){
    for (int i = 0; i < literals.size(); i++){
        int value = assignments[literalIds[i]].value;
        if(evaluate(i,value)){
            return value != -1;
        }
       
    }
    return false;
}
int Clause::pointerToLiteralID(int p){
    return literalIds[p];
	//for (auto&& it : literalIds) {
	//	cout << it << " ";
	//}
	//cout << endl;
}
int Clause::whatValueMakesThisLiteralTrue(int p){
    if (literals[p] < 0){
        return 0;
    }else{
        return 1;
    }
}
void Clause::changePointer(int currentPointer,int newPointer){
    if (p1 == currentPointer){
        p1 = newPointer;
    }else if (p2 == currentPointer){
        p2 = newPointer;
    }else{
        throw "TIRED TO REASIGN POINTER WHICH IS NOT SET";
    }
}
vector<int> Clause::getParentsByPointer(int p){
    vector<int> parents = literals;
	//std::cout << "1. Parent size: " << parents.size() << endl;
	//for (auto&& it : literalIds) {
	//	cout << it << " ";
	//}
	//cout << endl;
	//cout << p << endl;
    parents.erase(parents.begin()+p);
    //for(int i = 0; i < parents.size(); i++){
    //    parents[i] = -1*parents[i];
    //}
	//std::cout << "2. Parent size: " << parents.size() << endl;
	//for (auto&& it : parents) {
	//	cout << it << " ";
	//}
	//cout << endl;
    return parents;
  
}
Clause::~Clause()
{
}

Clause::Clause()
{
	p1 = 0;
	p2 = 1;
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
	//std::cout << std::endl;
}



