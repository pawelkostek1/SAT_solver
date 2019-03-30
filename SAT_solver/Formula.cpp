#include"Formula.h"
#include"constants.h"
#include<map>
#include<algorithm>
using namespace std;

Formula::Formula(int _numOfvar, int _numOfClauses, vector<Clause> _formula)
{

	this->numOfClauses = _numOfClauses;
	formula = _formula;
    
	//Can optimize later by sorting it while the clauses are added to the formula
    sort(formula.begin(), formula.end(), [](const Clause & a, const Clause & b){ return a.literals.size() < b.literals.size(); });
    
    for (int i = 1; i <= _numOfvar; i++) {
        cout << "adding var: " << i << endl;
        addVariable(i,-1);
    }
    implicationGraph = Graph();
}


Formula::~Formula()
{
}

void Formula::addVariable(int absLiteral,int value){
    variables[absLiteral] = Variable(absLiteral,value);
    unassignedIndex.push_back(absLiteral);
    index(absLiteral);
    numOfvar++;
}


void Formula::removeVariable(int absLiteral){
    variables.erase(absLiteral);
    unassignedIndex.remove(absLiteral);
    assignedIndex.remove(absLiteral);
    clausesIndexes.erase(absLiteral);
    numOfvar--;
}


void Formula::index(int absLiteral){
    for (int i = 0; i < getNumOfClauses(); i++) {
        auto itPos = find(formula[i].literals.begin(), formula[i].literals.end(), absLiteral);
        auto itNeg = find(formula[i].literals.begin(), formula[i].literals.end(), absLiteral*-1);
        if (itPos != formula[i].literals.end() or itNeg != formula[i].literals.end()) {
            //i represents the unique id for a clause
            if (formula[i].literals.size() == 1){
                //this is to help optmization faster find the clauses with only one literal
                //cout << "Single literal clause: " << formula[i][0] << " " << i << endl;
                clausesIndexes[absLiteral].insert(clausesIndexes[absLiteral].begin(),i);
            }else{
                clausesIndexes[absLiteral].push_back(i);
            }
        }
    }
}


int Formula::getNumOfVar() {
	return this->numOfvar;
}

int Formula::getNumOfClauses() {
	return this->numOfClauses;
}

int Formula::assignVariable(int literal, int value,int level) {
    //if the variable already has a value that is not -1 and does not match the value
    //we want to assign there is a conflict 
    if (variables[literal].value != value and variables[literal].value != -1){
        return CONFLICT;
    }
    //variable has not been assigned a value yet or its the same value at a different level
    if(variables[literal].value == -1){
        unassignedIndex.remove(literal);
        assignedIndex.push_back(literal);
        variables[literal].value = value;
    }
    //Add the
    implicationGraph.addNode(literal,level);
    return NOCONFLICT;
}

void Formula::unassignVariable(int literal) {
    assignedIndex.remove(literal);
    unassignedIndex.push_back(literal);
    variables[literal].value = -1;
}

bool Formula::allVariablesAssigned(){
    return this->numOfvar == assignedIndex.size();
}






Variable Formula::getInferred(int clauseIndex){
    int unassignedCount = 0;
    Clause clause = formula[clauseIndex];
    Variable inferredVar = Variable(0,-1);
    for (int i = 1; i <= clause.literals.size(); i++){
        int literal = clause.literals[i];
        int literalId = abs(literal);
        Variable var = variables[literalId];
        bool positive = literal > 0;
        //The intuition here is that if any literal value in the cluse is true we cannot infer any other variable
        if ((!positive and var.value == 0) or (positive and var.value == 1)){
            break;
        }else if (var.value == -1){
            //the variable is unnassigned
            unassignedCount+=1;
            //If the number of literals that are unassigned in the literals is more than 1 we cannot infer shit
            if (unassignedCount > 1){
                break;
            }else{
                //since this line only can run once and if the algorithm goes through all literals
                //variables and only finds one unassigned variable that variable will be stored in
                //the inferedVar variable.
                inferredVar.literal = literalId;
                //To make sure we infer the right value we check to see if the literal is negated or not
                //if negated the value is set to 0 else it is set to 1
                inferredVar.value = positive ? 1:0;
            }
        }
    }
    return inferredVar;
}












int Formula::removeSingleLiteralVariables(){
    cout << "hello";
    list<int> removedClauses = list<int>();
    unordered_map<int, int> singleLiteralVariableSign = unordered_map<int, int>();
    vector<int> variablesWithOneLiteral = vector<int>();
    unsigned int approvedClauses = 0;
    int i = 0;
    while(approvedClauses < formula.size()){
        //if a clause has one literal add it to the variablesWithOneLiteral vector
        printFormula();
        vector<int> clause = formula[i].literals;
        cout << "Checking clause: " << i << endl;
        if(clause.size() == 1){
            //Check weather this literal already has a single literal of the complimentary sort
            if (singleLiteralVariableSign[abs(clause[0])] and singleLiteralVariableSign[abs(clause[0])] == -1*clause[0]){
                return CONFLICT;
            }else if(!singleLiteralVariableSign[abs(clause[0])]){
                //the sign is the same and thus we can proceed
                singleLiteralVariableSign[abs(clause[0])] = clause[0];
                cout << clause[0];
            }
            
            variablesWithOneLiteral.push_back(clause[0]);
            approvedClauses = 0;
            //remove the formula
            formula.erase(formula.begin()+i);
            numOfClauses-=1;
            cout << "Formula size after deletion: " << formula.size() << endl;
            //since we removed a clause we let i be the same until the next loop
        }else{
            //for a clause that has more than one literal we check it for single literal matches
            int incrementI = 1;
            int incrementA = 1;
            cout << "Lets see if we can find the variables in the single literal bucket" << variablesWithOneLiteral.size() << endl;
            for(int unsigned j = 0; j < variablesWithOneLiteral.size(); j++){
                int literal = variablesWithOneLiteral[j];
                int compLiteral = literal*-1;
                cout << "variable to test: " << literal << "complement: " << compLiteral << endl;
                auto compPosition = find(clause.begin(), clause.end(), compLiteral);
                auto normalPosition = find(clause.begin(), clause.end(), literal);
                if (normalPosition != clause.end()){
                    //Delete the clause all together because it has already been satisified by the single literal
                    //assignement
                    formula.erase(formula.begin()+i);
                    numOfClauses-=1;
                    //since we again removed a clause we do not modify i
                    incrementI = 0;
                    //since we do not know if the clause we erased was contributed to the approvedCount
                    //We decrement the approvedCounter
                    incrementA = approvedClauses*-1;
                    break;
                }
                else if (compPosition != clause.end()){
                    //Delete the complement of the literal
                    clause.erase(compPosition);
                    formula[i] = clause;
                    incrementI = 0;
                    incrementA = approvedClauses*-1;
                    
                }else{
                    //the literal is not in the clause
                    //and thus we have no beef with it
                    
                }
            }
            i+=incrementI;
            approvedClauses+=incrementA;
        }
        
        //Make sure we reset the i variable so we only exit the while loop
        //when all variablesWithOneLiteral have been
        if(approvedClauses < formula.size() and i == formula.size()-1){
            i = 0;
        }
        
        
        
    }
    //remove each variable that had a single literal clause from any index or assignment list
    for (unsigned int i = 0; i < variablesWithOneLiteral.size(); i++){
        removeVariable(abs(variablesWithOneLiteral[variablesWithOneLiteral[i]]));
    }
    
    for(int unsigned i = 0; i < formula.size(); i++){
        vector<int> clause = formula[i].literals;
        cout << "ClauseId: " << i << endl;
        for(int unsigned j = 0; j < clause.size(); j++){
            cout << "\tLiteral: " << clause[j] << endl;
        }
    }
    
    return NOCONFLICT;
}



void Formula::printFormula() {
    cout << "Number of variables is " << getNumOfVar() << ".\nNumber of clauses is " << getNumOfClauses() << "." << endl  << endl;
    for (int i = 0; i < getNumOfClauses(); i++) {
        cout << "Clause " << i + 1 << ": ";
        for (unsigned int j = 0; j < formula[i].literals.size(); j++) {
            if (formula[i].literals[j] < 0) {
                cout << "NOT " << variables[abs(formula[i].literals[j])].letter;
            }
            else {
                cout << variables[abs(formula[i].literals[j])].letter;
            }
            if (j < formula[i].literals.size() - 1) {
                cout << " OR ";
            }
        }
        cout << endl;
    }
    cout << endl;
}
