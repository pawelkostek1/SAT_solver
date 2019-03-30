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
    Clause(vector<int> _value); //Constructor
	Clause(); //Constructor
    ~Clause();
};
