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

Formula::Formula(unordered_map<int,Clause> _formula, unordered_map<int,Variable> _variables)
{
    variables = _variables;
	formula = _formula;
	prevAssignedIndex = vector<int>(_variables.size(), -1);
    //initialise the unnasignedIndex
    for (int i = 1; i <= variables.size();i++){
        unassignedIndex.push_back(i);
    }
    
    implicationGraph = Graph();
	//removeSingleLiteralVariables();
}
Formula::Formula()
{
    formula = unordered_map<int,Clause>();
    variables = unordered_map<int,Variable>();
    //Not sure what this does yet
    prevAssignedIndex = vector<int>(variables.size(), -1);
    implicationGraph = Graph();
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






//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
////////////////////////////CLAUSE GETTER/SETTER//////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


void Formula::addClause(vector<int> clause,bool allowNewVariables){
    Clause c = Clause(clause);
    
    for (auto literalId:c.literalIds){
        auto it = variables.find(literalId);
        if (it == variables.end()){
            if (allowNewVariables){
                addVariable(literalId);
            }else{
                throw "NOT ALLOWED TO ADD NEW VARIABLES";
            }
        }
    }
    int cId = formula.size();
    formula[cId] = c;
   
    changeWatchedLiteral(cId, -1, 0);
    if(clause.size() > 1){
        changeWatchedLiteral(cId, -1, 1);
    }
    
}
void Formula::removeClause(int clauseId){
    Clause c = getClause(clauseId);
    changeWatchedLiteral(clauseId, c.p1, -1);
    changeWatchedLiteral(clauseId, c.p2, -1);
    formula.erase(clauseId);
    
    
}
void Formula::updateClause(int clauseId, vector<int> clause){
    Clause c = getClause(clauseId);
    changeWatchedLiteral(clauseId, c.p1, -1);
    changeWatchedLiteral(clauseId, c.p2, -1);
    formula[clauseId] = Clause(clause);
    changeWatchedLiteral(clauseId, -1, 0);
    changeWatchedLiteral(clauseId, -1, 1);
}

Clause Formula::getClause(int clauseId){
    if (formula.find(clauseId) != formula.end()){
        return formula[clauseId];
    }else{
        throw "TRYING TO ACCESS CLAUSE WITH FAULTY CLAUSE ID";
    }
}




//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////VARIABLE GETTER/SETTER//////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void Formula::addVariable(int literalId){
    variables[literalId] = Variable(literalId,literalId,-1);
    unassignedIndex.push_back(literalId);
    prevAssignedIndex.push_back(-1);
}
Variable Formula::getVariable(int literalId){
    auto const it = variables.find(literalId);
    if (it != variables.end()){
        return variables[literalId];
    }else{
        throw "TRYING TO ACCESS VARIABLE THAT IS NOT SET";
    }
}

unordered_map<int,Variable> Formula::getVariables(){
    return variables;
}



void Formula::removeClauseFromVariableIndex(int literalId,int clauseId,bool negativeIndex){
    Variable v = getVariable(literalId);
    if (negativeIndex){
        v.removeNegativeClause(clauseId);
    }else{
        v.removePositiveClause(clauseId);
    }
    variables[literalId] = v;
    
}
void Formula::addClauseToVariableIndex(int literalId,int clauseId,bool negativeIndex){
    Variable v = getVariable(literalId);
    if (negativeIndex){
        cout << "TRYING TO ADD CLAUSE " << clauseId << " TO NEGATIVE INDEX FOR VAR " << v.letter << "(" << v.literalId << ")" << endl;

        v.addNegativeClause(clauseId);
    }else{
        v.addPositiveClause(clauseId);
    }
    variables[literalId] = v;
}
void Formula::changeWatchedLiteral(int clauseId,int currentRelativeLiteralId,int newRelativeLiteralId){
    Clause c = getClause(clauseId);
    if (newRelativeLiteralId >= 0){
        int candidateLiteral = c.literals[newRelativeLiteralId];
        int candidateAbsoluteLiteralId = c.pointerToLiteralID(newRelativeLiteralId);
        
        addClauseToVariableIndex(candidateAbsoluteLiteralId,clauseId,candidateLiteral < 0);
    }
    if(currentRelativeLiteralId >= 0){
        int currentLiteral = c.literals[currentRelativeLiteralId];
        int currentAbsoluteLiteralId = c.pointerToLiteralID(currentRelativeLiteralId);
        removeClauseFromVariableIndex(currentAbsoluteLiteralId,clauseId,currentLiteral < 0);
    }
    if (currentRelativeLiteralId >= 0 && currentRelativeLiteralId >= 0){
        c.changePointer(currentRelativeLiteralId,newRelativeLiteralId);
    }
    
    formula[clauseId] = c;
    
}


int Formula::assignVariable(int literalId, int literal,int value, int level, vector<int> parentLiterals) {
    //if the variable already has a value that is not -1 and does not match the value
    //we want to assign there is a conflict 
    Variable v = getVariable(literalId);
    //variable has not been assigned a value yet or its the same value at a different level
    if(v.value == -1){
        unassignedIndex.remove(literalId);
		cout << "assignedIndex before size: " << assignedIndex.size() << " ";
        assignedIndex.push_back(literalId);
		cout << "assignedIndex after size: " << assignedIndex.size() << endl;
        v.value = value;
		cout << "Assigned new variable: " << literalId << endl;
    }
	//if (prevAssignedIndex[literal] == -1) {
	//	prevAssignedIndex[literal] = value;
	//}
    
    cout << "assigning value to variable: "<< literalId << ", " << v.value << endl;
    //Add the node
    variables[literalId] = v;
    if (level > -1){
        int result = implicationGraph.addNode(literalId,literal,level, value, parentLiterals);
        implicationGraph.printGraph();
        return result;
    }else{
        return NOCONFLICT;
    }
    
}

void Formula::unassignVariable(int literalId) {
    Variable v = getVariable(literalId);
	cout << "Unassigning literal: " <<literalId<< endl;
	cout << "\tassignedIndex before size: " << assignedIndex.size() << " ";
    assignedIndex.remove(literalId);
	cout << "\tassignedIndex after size: " << assignedIndex.size() << endl;
	cout << "\tunassignedIndex before size: " << unassignedIndex.size() << " ";
	if (find(unassignedIndex.begin(), unassignedIndex.end(), literalId) == unassignedIndex.end())
	{
		unassignedIndex.push_back(literalId);
	}
    
	cout << "\tunassignedIndex after size: " << unassignedIndex.size() << endl;
    v.value = -1;
    variables[literalId] = v;
}

bool Formula::allVariablesAssigned(){
	cout << "variables.size: " << variables.size() << " assignedIndex.size(): "<<assignedIndex.size() << " unassignedIndex.size(): " << unassignedIndex.size() << endl;
    return variables.size() == assignedIndex.size();
}

int Formula::removeSingleLiteralVariables2(){
    list<int> removedClauses = list<int>();
    unordered_map<int, int> singleLiteralVariableSign = unordered_map<int, int>();
    vector<int> variablesWithOneLiteral = vector<int>();
    unsigned int approvedClauses = 0;
    int i = 0;
    while(approvedClauses < formula.size()){
        //if a clause has one literal add it to the variablesWithOneLiteral vector
        print2Watched(true);
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
                    assignVariable(abs(clause[0]), clause[0], 1, -1, {});
				}
				else {
					assignVariable(abs(clause[0]), clause[0], 0, -1, {});
				}
            }
            
            variablesWithOneLiteral.push_back(clause[0]);
            approvedClauses = 0;
            //remove the formula
            removeClause(i);
            //formula.erase(formula.begin()+i);
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
                    removeClause(i);
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
                    if(clause.size()!=0){
                        updateClause(i, clause);
                    }else{
                        removeClause(i);
                    }
                    
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
    
    /*for(int unsigned i = 0; i < formula.size(); i++){
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
    }*/
    printVariables();
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
void Formula::printClauses(){
    int i = 0;
    for (auto clauseIt : formula) {
        Clause clause = clauseIt.second;
        cout << clauseIt.first << " -> ";
        for (unsigned int j = 0; j < clause.literals.size(); j++) {
            
            if (clause.literals[j] < 0) {
                cout << "NOT " << variables[abs(clause.literals[j])].letter;
                cout << "(" << variables[abs(clause.literals[j])].value << ")" << (clause.p1 == j || clause.p2 == j? "*":"");
            }
            else {
                cout << variables[clause.literals[j]].letter;
                cout << "(" << variables[abs(clause.literals[j])].value << ")" << (clause.p1 == j || clause.p2 == j? "*":"");
            }
            
            if (j < clause.literals.size() - 1) {
                cout << " OR ";
            }
           
        }
         cout << endl;
        i++;
    }
}
void Formula::print2Watched(bool verbose) {
	cout << "|-------------------------------------------------|-----------|------------------------------------------------|" << endl;

		for (auto& it : variables) {
			
			for (int i = 0; i < max(it.second.postiveClauses.size(), it.second.negativeClauses.size()); i++) {
				if (i<it.second.postiveClauses.size()) {
					ostringstream s;
					Clause clause = formula[it.second.postiveClauses[i]];
					cout << left << setw(50);
                    s << "| " << it.second.postiveClauses[i] << " -> ";
                    if (verbose){
                        for (unsigned int j = 0; j < clause.literals.size(); j++) {
                            if (clause.literals[j] < 0) {
                                s << "NOT " << variables[abs(clause.literals[j])].letter;
                                s << "(" << variables[abs(clause.literals[j])].value << ")" << (clause.p1 == j || clause.p2 == j? "*":"");
                            }
                            else {
                                s << variables[clause.literals[j]].letter;
                                s << "(" << variables[abs(clause.literals[j])].value << ")" << (clause.p1 == j || clause.p2 == j? "*":"");
                            }
                            
                            if (j < clause.literals.size() - 1) {
                                s << " OR ";
                            }
                            
                        }
                    }else{
                        s << it.second.postiveClauses[i];
                    }
					
					
					cout << s.str();
				}
				else {
					ostringstream s;
					cout << left << setw(50);
					s << "| ";
					cout << s.str();
				}
				if (i == 0) {
                    string value = (it.second.value == 1? " = TRUE ":(it.second.value == 0?"= FALSE ":" = ---- "));
                    cout << "| "<< it.second.letter << value << " |";
                }
				else {
					cout << "|           |";
				}
				if (i<it.second.negativeClauses.size()) {
					ostringstream s;
					Clause clause = formula[it.second.negativeClauses[i]];
					cout << right << setw(50);
					
                    if (verbose){
                        for (unsigned int j = 0; j < clause.literals.size(); j++) {
                            
                            if (clause.literals[j] < 0) {
                                s << "NOT " << variables[abs(clause.literals[j])].letter;
                                s << "(" << variables[abs(clause.literals[j])].value << ")" << (clause.p1 == j || clause.p2 == j? "*":"");
                            }
                            else {
                                s << variables[clause.literals[j]].letter;
                                s << "(" << variables[abs(clause.literals[j])].value << ")" << (clause.p1 == j || clause.p2 == j? "*":"");
                            }
                            
                            if (j < clause.literals.size() - 1) {
                                s << " OR ";
                            }
                            
                        }
                        s << " <- " << it.second.negativeClauses[i];
                    }else{
                       s << it.second.negativeClauses[i];
                    }
					
					//s << "(" << variables[it].negativeClauses[i] << ")";
					s << "|\n";
					
					cout << s.str();
				}
				else {
					ostringstream s;
					cout << right << setw(50);
					s << "|\n";
					cout << s.str();
				}
			}
			cout << "|-------------------------------------------------|-----------|------------------------------------------------|" << endl;
		}
		cout << "|-------------------------------------------------|-----------|------------------------------------------------|" << endl;
}
