#pragma once
#include<vector>
#include"Clause.h"

using namespace std;

/*
 * @description: Graph
 */
class Clause {
    
public:
	vector<int> literals;
	vector<int> literalIds;
    int p1;
    int p2;
    Clause(vector<int> _value); //Constructor
    
	Clause(); //Constructor
    ~Clause();
    bool evaluate(int literalId, int value);
    int pointerToLiteralID(int pId);
    int whatValueMakesThisLiteralTrue(int p);
    vector<int> getParentsByPointer(int p);
	void printClause();
    void changePointer(int currentPointer,int newPointer);
};
