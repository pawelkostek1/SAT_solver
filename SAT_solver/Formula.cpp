#include"Formula.h"
#include"constants.h"
#include<map>
#include<algorithm>
#include <iostream>
#include <iomanip>
#include <locale>
#include <iostream>
#include <sstream>
using namespace std;

Formula::Formula(vector<Clause> _formula, unordered_map<int,Variable> _variables)
{
    variables = _variables;
	formula = _formula;
	prevAssignedIndex = vector<int>(_variables.size(), -1);
    //initialise the unnasignedIndex
    for (int i = 1; i <= variables.size();i++){
        unassignedIndex.push_back(i);
    }
    
	//Can optimize later by sorting it while the clauses are added to the formula
	//!!!! Comment out sorting for now as it shuffles the indeces of the clauses
    //sort(formula.begin(), formula.end(), [](const Clause & a, const Clause & b){ return a.literals.size() < b.literals.size(); });
/*
<<<<<<< HEAD
=======
    for (int i = 1; i <= _numOfvar; i++) {
        cout << "Adding var: " << i <<" = "<< (char)('A' + i - 1) << "." << endl;
        addVariable(i,-1);
    }
	cout << "Formula initialized." << endl<<endl;
>>>>>>> 5f93ad95cdcd60a13382788eefa6716093dd79ec
*/
    implicationGraph = Graph();
	//removeSingleLiteralVariables();

}
Formula::Formula()
{
}
Formula::~Formula()
{
}


void Formula::removeVariable(int absLiteral){
    variables.erase(absLiteral);
    unassignedIndex.remove(absLiteral);
    assignedIndex.remove(absLiteral);
}



int Formula::getNumOfVar() {
	return this->variables.size();
}

int Formula::getNumOfClauses() {
	return this->formula.size();
}

int Formula::assignVariable(int literal, int value, int level, vector<Variable> parentVariables) {
    //if the variable already has a value that is not -1 and does not match the value
    //we want to assign there is a conflict 
    
    //variable has not been assigned a value yet or its the same value at a different level
	cout << "assignigning  variable value:" << variables[literal].value << endl;
    if(variables[literal].value == -1){
        unassignedIndex.remove(literal);
		cout << "assignedIndex before size: " << assignedIndex.size() << " ";
        assignedIndex.push_back(literal);
		cout << "assignedIndex after size: " << assignedIndex.size() << endl;
        variables[literal].value = value;
		cout << "Assigned new variable: " << literal << endl;
    }
	//if (prevAssignedIndex[literal] == -1) {
	//	prevAssignedIndex[literal] = value;
	//}
    
    //cout << "assigning value to variable: "<< literal << ", " << variables[literal].value << endl;
    //Add the node
    implicationGraph.addNode(literal, level, value, parentVariables);
	cout << "Added new node for literal: " << literal <<" with value: "<< value << " with parent size: "<< parentVariables.size() << " and node indeces: ";
	for (int i = 0; i < implicationGraph.variableIndex[literal].size(); i++)
		cout << implicationGraph.variableIndex[literal][i] << " ";
	cout << endl;
    if (variables[literal].value != value && variables[literal].value != -1){
		implicationGraph.ConflictingLiteralId = literal;
		cout << "Resulted in conflict for literal " << literal << endl;
        return CONFLICT;
    }else{
        return NOCONFLICT;
    }
    
}

void Formula::unassignVariable(int literal) {
	cout << "Unassigning literal: " <<literal<< endl;
	cout << "\tassignedIndex before size: " << assignedIndex.size() << " ";
    assignedIndex.remove(literal);
	cout << "\tassignedIndex after size: " << assignedIndex.size() << endl;
	cout << "\tunassignedIndex before size: " << unassignedIndex.size() << " ";
	if (find(unassignedIndex.begin(), unassignedIndex.end(), literal) == unassignedIndex.end())
	{
		unassignedIndex.push_back(literal);
	}
    
	cout << "\tunassignedIndex after size: " << unassignedIndex.size() << endl;
    variables[literal].value = -1;
}

bool Formula::allVariablesAssigned(){
	cout << "variables.size: " << variables.size() << " assignedIndex.size(): "<<assignedIndex.size() << " unassignedIndex.size(): " << unassignedIndex.size() << endl;
    return variables.size() == assignedIndex.size();
}

ImplicationAnalysis Formula::setInferredVariable(int clauseIndex){
    Clause clause = formula[clauseIndex];

    int unassignedCount = 0;
    Variable inferredVar = Variable(0,-1);
    vector<Variable> parentVars = vector<Variable>();
    for (unsigned int i = 1; i <= clause.literals.size(); i++){
        int literal = clause.literals[i];
        int literalId = abs(literal);
        Variable var = variables[literalId];
        bool positive = literal > 0;
        //The intuition here is that if any literal value in the cluse is true we cannot infer any other variable
        if ((!positive && var.value == 0) || (positive && var.value == 1)){
            break;
        }else if (var.value != -1){
            parentVars.push_back(variables[literalId]);
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
    ImplicationAnalysis result = ImplicationAnalysis();
    result.target = inferredVar;
    result.parents = parentVars;
    return result;
}
int Formula::removeSingleLiteralVariables() {
	while(1){
		vector<int> variablesWithOneLiteral = vector<int>();
		unordered_map<int, int> singleLiteralVariableSign = unordered_map<int, int>();
		//Find all the single literals
		for (int i = 0; i < formula.size(); i++) {
			vector<int> clause = formula[i].literals;
			if (clause.size() == 1) {
				auto conflictSingleLiteral = find(variablesWithOneLiteral.begin(), variablesWithOneLiteral.end(), -1*clause[0]);
				if (conflictSingleLiteral != variablesWithOneLiteral.end()) {
					return CONFLICT;
				}
				else {
					variablesWithOneLiteral.push_back(clause[0]);
					formula.erase(formula.begin() + i);
					//Assign the single iteral to a corresponding value
					if (clause[0] > 0) {
						variables[abs(clause[0])].value = 1;
						assignedIndex.push_back(abs(clause[0]));
						unassignedIndex.remove(abs(clause[0]));
					}
					else {
						variables[abs(clause[0])].value = 0;
						assignedIndex.push_back(abs(clause[0]));
						unassignedIndex.remove(abs(clause[0]));
					}
				}
			}
		}
		if (variablesWithOneLiteral.size() == 0)
			break;
		//For all single literals remove the clause all together if it is satisified or remove the complement
		for (int j = 0; j < variablesWithOneLiteral.size(); j++) {
			//cout << "Considering single literal: " << variablesWithOneLiteral[j] << endl;
			for (int i = 0; i < formula.size(); i++) {
				vector<int> clause = formula[i].literals;
				int literal = variablesWithOneLiteral[j];
				int compLiteral = literal * -1;
				//cout << "variable to test: " << literal << "complement: " << compLiteral << endl;
				auto compPosition = find(clause.begin(), clause.end(), compLiteral);
				auto normalPosition = find(clause.begin(), clause.end(), literal);
				if (normalPosition != clause.end()) {
					formula.erase(formula.begin() + i);
				}
				else if (compPosition != clause.end()) {
					clause.erase(compPosition);
					//cout << "Clause size: " << clause.size() << endl;
					if (clause.size() != 0)
						formula[i] = Clause(clause);
					else
						return CONFLICT;
				}
			}
			//printFormula();
		}
	}
	for (int unsigned i = 0; i < formula.size(); i++) {
		vector<int> clause = formula[i].literals;
		cout << "ClauseId: " << i << endl;
		formula[i].printClause();
		int count = 0;
		for (int unsigned j = 0; j < clause.size(); j++) {
			cout << "\tLiteral: " << clause[j] << endl;
			if (count < 2) {
				if (clause[j] < 0) {
					cout << "\tNegative clause: " << i << " " << endl;
					//formula[i].printClause();
					variables[abs(clause[j])].addNegativeClause(i);
				}
				else {
					cout << "\tPositive clause: " << i << " " << endl;
					//formula[i].printClause();
					variables[abs(clause[j])].addPositiveClause(i);
				}
				count += 1;
			}
		}
	}

	return NOCONFLICT;
}
int Formula::removeSingleLiteralVariables2(){

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
            if (singleLiteralVariableSign[abs(clause[0])] && singleLiteralVariableSign[abs(clause[0])] == -1*clause[0]){
                return CONFLICT;
            }else if(!singleLiteralVariableSign[abs(clause[0])]){
                //the sign is the same and thus we can proceed
                singleLiteralVariableSign[abs(clause[0])] = clause[0];
                cout << clause[0];
				//Assign the variable accordingly

				if (clause[0] > 0) {
					variables[abs(clause[0])].value = 1;
					assignedIndex.push_back(abs(clause[0]));
				}
				else {
					variables[abs(clause[0])].value = 0;
					assignedIndex.push_back(abs(clause[0]));
				}
            }
            
            variablesWithOneLiteral.push_back(clause[0]);
            approvedClauses = 0;
            //remove the formula
            formula.erase(formula.begin()+i);
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
					cout << "Clause size: " << clause.size() << endl;
					if(clause.size()!=0)
						formula[i] = Clause(clause);
					else
						formula.erase(formula.begin() + i);
					//if (literal < 0) {
					//	auto it = find(variables[abs(literal)].negativeClauses.begin(), variables[abs(literal)].negativeClauses.end(), i);
					//	variables[abs(literal)].negativeClauses.erase(it);
					//}
					//else {
					//	auto it = find(variables[abs(literal)].postiveClauses.begin(), variables[abs(literal)].postiveClauses.end(), i);
					//
					//	variables[abs(literal)].postiveClauses.erase(it);
					//}

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
		printVariables();
        //Make sure we reset the i variable so we only exit the while loop
        //when all variablesWithOneLiteral have been
        if(approvedClauses < formula.size() && i == formula.size()-1){
            i = 0;
        }  
    }
    
    //remove each variable that had a single literal clause from any index or assignment list apart from the original variables list
    //for (unsigned int i = 0; i < variablesWithOneLiteral.size(); i++){
    //    cout << "remove: " << abs(variablesWithOneLiteral[i]) << endl;
    //    removeVariable(abs(variablesWithOneLiteral[i]));
    //}
    
    for(int unsigned i = 0; i < formula.size(); i++){
        vector<int> clause = formula[i].literals;
        cout << "ClauseId: " << i << endl;
		int count = 0;
        for(int unsigned j = 0; j < clause.size(); j++){
            cout << "\tLiteral: " << clause[j] << endl;
			if (count < 2){
				if (clause[j] < 0){
					cout <<"Negative clause: "<< clause[j] << " "<< endl;
					formula[i].printClause();
					variables[abs(clause[j])].addNegativeClause(i);
				}else{
					cout << "Positive clause: " << clause[j] << " " << endl;
					formula[i].printClause();
					variables[abs(clause[j])].addPositiveClause(i);
				}
				count+= 1;
			}
        }
    }
    
    return NOCONFLICT;
}

void Formula::printFormula() {
    //cout << "Number of variables is " << getNumOfVar() << ".\nNumber of clauses is " << getNumOfClauses() << "." << endl  << endl;
    for (int i = 0; i < getNumOfClauses(); i++) {
        cout << "Clause " << i + 1 << ": ";
        for (unsigned int j = 0; j < formula[i].literals.size(); j++) {
            if (formula[i].literals[j] < 0) {
                cout << "NOT " << variables[abs(formula[i].literals[j])].letter;
            }
            else {
                cout << variables[abs(formula[i].literals[j])].letter;
            }
            cout << "(" << variables[abs(formula[i].literals[j])].value<< ")";
            if (j < formula[i].literals.size() - 1) {
                cout << " OR ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

void Formula::printVariables() {
	for (auto& it : variables) {
		cout << "Variable: " << it.second.letter<<"("<<it.first<<") was assigned: " << it.second.value << " and has activity level set to: " << it.second.activity << endl;
	}
	cout << endl;
}

void Formula::printIndex(){
    cout << "UNASSIGNED VARIABLES" << endl;
    for (auto& it : unassignedIndex)
        cout << variables[it].literal << endl;
    cout << "ASSIGNED VARIABLES" << endl;
    for (auto& it : assignedIndex)
        cout << variables[it].literal << endl;
	cout << endl;
}

void Formula::bumpActivities(vector<int> learnedClause) {
	for (unsigned int i = 0; i < learnedClause.size(); i++) {
		variables[abs(learnedClause[i])].activity += 1.0;
	}
}

void Formula::decayActivities() {
	for (auto& it : variables) {
		it.second.activity *= this->decay;
	}
}

void Formula::print2Watched(list<int> literalIndeces) {
	cout << "|---------------------------------------|---|--------------------------------------|" << endl;

		for (auto& it : literalIndeces) {
			
			for (int i = 0; i < max(variables[it].postiveClauses.size(), variables[it].negativeClauses.size()); i++) {
				if (i<variables[it].postiveClauses.size()) {
					ostringstream s;
					Clause clause = formula[variables[it].postiveClauses[i]];
					cout << left << setw(40);
					s << "| ";

					for (unsigned int j = 0; j < clause.literals.size(); j++) {
						if (clause.literals[j] < 0) {
							s << "NOT " << variables[abs(clause.literals[j])].letter;
						}
						else {
							s << variables[clause.literals[j]].letter;
						}

						if (j < clause.literals.size() - 1) {
							s << " OR ";
						}
					}
					s << "(" << variables[it].postiveClauses[i] << ")";
					cout << s.str();
				}
				else {
					ostringstream s;
					cout << left << setw(40);
					s << "| ";
					cout << s.str();
				}
				if (i == 0) {
					cout << "| "<< variables[it].letter << " |";;
				}
				else {
					cout << "|   |";
				}
				if (i<variables[it].negativeClauses.size()) {
					ostringstream s;
					Clause clause = formula[variables[it].negativeClauses[i]];
					cout << right << setw(40);
					

					for (unsigned int j = 0; j < clause.literals.size(); j++) {
						if (clause.literals[j] < 0) {
							s << "NOT " << variables[abs(clause.literals[j])].letter;
						}
						else {
							s << variables[clause.literals[j]].letter;
						}

						if (j < clause.literals.size() - 1) {
							s << " OR ";
						}
					}
					s << "(" << variables[it].negativeClauses[i] << ")";
					s << "|\n";
					
					cout << s.str();
				}
				else {
					ostringstream s;
					cout << right << setw(40);
					s << "|\n";
					cout << s.str();
				}
			}
			cout << "|---------------------------------------|---|--------------------------------------|" << endl;
		}
		cout << "|---------------------------------------|---|--------------------------------------|" << endl;
}
