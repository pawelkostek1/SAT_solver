#include "Clause.h"
#include<cmath>
Clause::Clause(vector<int> _value) {
    literals = _value;
    p1 = 0;
    p2 = 1;
    
}
bool Clause::evaluate(int p, int value){
    if (literals[p] < 0 && value == 0){
        return true;
    }
    if (literals[p] > 0 && value == 1){
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



