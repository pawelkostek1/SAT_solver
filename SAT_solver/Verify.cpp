#include "Verify.h"
#include "Formula.h"
#include<vector>
#include"Clause.h"

Verify::Verify()
{
    vector<Clause> clauses = vector<Clause>();
    unordered_map<int,Variable> variables = unordered_map<int,Variable>();
    variables[1] = Variable(1,-1);
    variables[2] = Variable(2,-1);
    variables[3] = Variable(3,-1);
    variables[4] = Variable(4,-1);
    variables[5] = Variable(5,-1);
    variables[6] = Variable(6,-1);
    cout << "Created variables" << endl;
    clauses.push_back(Clause({1,2,6}));
    clauses.push_back(Clause({-1,-3,5}));
    clauses.push_back(Clause({6,-5}));
    clauses.push_back(Clause({4,5,6}));
    clauses.push_back(Clause({-6,-1,-3,2}));
    clauses.push_back(Clause({1,6}));
    clauses.push_back(Clause({3,1,-6}));
     cout << "Created clauses" << endl;
    Formula formula = Formula(clauses, variables);
    formula.printFormula();
}


Verify::~Verify()
{
    
}


bool Verify::testGraph(){
    
    return false;
}

bool Verify::test2WatchedLiterals() {
	cout << "Checking the intilial state" << endl;


	return false;
}
