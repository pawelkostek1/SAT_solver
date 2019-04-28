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

Formula::Formula(unordered_map<string,Clause> _formula, unordered_map<int,Variable> _variables)
{
    variables = _variables;
    originalFormula = _formula;
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
    formula = unordered_map<string,Clause>();
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


string Formula::addClause(vector<int> clause,bool allowNewVariables){
    vector<int> cleanedClause = vector<int>();
    for(auto literal:clause){
        auto it = find(cleanedClause.begin(), cleanedClause.end(), literal);
        auto complementIt = find(clause.begin(),clause.end(),literal*-1);
        if (complementIt == clause.end() && it == cleanedClause.end()){
            cleanedClause.push_back(literal);
        }
    }
    Clause c = Clause(cleanedClause);
    
    if (cleanedClause.size() == 0){
        //cout << "EMPYT CLAAAUUUSEEE" << endl;
        return "";
        
    }
    
    
    //Check for duplication
    auto existingDuplicate = formula.find(c.id);
    if(existingDuplicate != formula.end()){
        //duplicate found
        return existingDuplicate->second.id;
    }
  
    
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
    
    
    formula[c.id] = c;
    if(allowNewVariables){
        originalFormula[c.id] = c;
    }
    
    changeWatchedLiteral(c.id, -1, 0);
    
    if(c.literals.size() > 1){
        changeWatchedLiteral(c.id, -1, 1);
    }
    //cout << "ADDED C -> " << c.id << endl;
    totalClauseCounter++;
    return c.id;
}
void Formula::removeClause(string clauseId){
    //cout << "REMOVING C -> " << clauseId << "...";
    Clause c = getClause(clauseId);
    
    changeWatchedLiteral(clauseId, c.p1, -1);
    changeWatchedLiteral(clauseId, c.p2, -1);
    
    formula.erase(clauseId);
    //cout << "DONE" << endl;
    
}
void Formula::removeClauseLiteral(string clauseId, int pRemove){
    //cout << "UPDATING C -> " << clauseId << "...";
    
    
    Clause c = getClause(clauseId);
    //cout << " FOUND CLAUSE: ";
    printClause(clauseId);
    
    
    bool needToShiftPointer = pRemove == c.p1 || pRemove == c.p2;
    if(needToShiftPointer){
        int cp = pRemove == c.p2? c.p2:c.p1;
        int op = pRemove == c.p2? c.p1:c.p2;
        int np = -1;
        int npV = -1;
        for(int i = 0; i < c.literalIds.size();i++){
            Variable v = getVariable(c.literalIds[i]);
            bool isOtherP = i == op;
            bool isCurrentP = i == cp;
            bool isUnset = v.value == -1;
            bool isNew = np == -1 && npV == -1;
            //exit loop if the var is the current other pointer
            if (isOtherP || isCurrentP)
                continue;
            if(isNew){
                np = i;
                npV = v.value;
                if(v.value == -1)
                    break;
            }else{
                if(v.value >= 0 && npV >= 0){
                    np = i;
                    npV = v.value;
                }else if(v.value == -1){
                    np = i;
                    npV = v.value;
                    break;
                }
                
            }
        }
        
        //cout << "gonna reassign: " << cp << " to " << np << endl;
        printClause(clauseId);
        
        changeWatchedLiteral(clauseId, cp, np);
    }
    
    formula[clauseId].removeLiteral(pRemove);
    printClause(clauseId);
    
  
    
}

Clause Formula::getClause(string clauseId){
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



void Formula::removeClauseFromVariableIndex(int literalId,string clauseId,bool negativeIndex){
    Variable v = getVariable(literalId);
    if (negativeIndex){
        v.removeNegativeClause(clauseId);
    }else{
        v.removePositiveClause(clauseId);
    }
    variables[literalId] = v;
    
}
void Formula::addClauseToVariableIndex(int literalId,string clauseId,bool negativeIndex){
    Variable v = getVariable(literalId);
    if (negativeIndex){
        ////cout << "TRYING TO ADD CLAUSE " << clauseId << " TO NEGATIVE INDEX FOR VAR " << v.letter << "(" << v.literalId << ")" << endl;

        v.addNegativeClause(clauseId);
    }else{
        v.addPositiveClause(clauseId);
    }
    variables[literalId] = v;
}




void Formula::changeWatchedLiteral(string clauseId,int currentRelativeLiteralId,int newRelativeLiteralId){
    Clause c = getClause(clauseId);
    
    
    if(currentRelativeLiteralId >= 0){
        
        int currentLiteral = c.literals[currentRelativeLiteralId];
        int currentAbsoluteLiteralId = c.pointerToLiteralID(currentRelativeLiteralId);
        removeClauseFromVariableIndex(currentAbsoluteLiteralId,clauseId,currentLiteral < 0);
        
    }
    
    if (newRelativeLiteralId >= 0){
        int candidateLiteral = c.literals[newRelativeLiteralId];
        int candidateAbsoluteLiteralId = c.pointerToLiteralID(newRelativeLiteralId);
        addClauseToVariableIndex(candidateAbsoluteLiteralId,clauseId,candidateLiteral < 0);
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
		////cout << "assignedIndex before size: " << assignedIndex.size() << " ";
        assignedIndex.push_back(literalId);
		////cout << "assignedIndex after size: " << assignedIndex.size() << endl;
        v.value = value;
		////cout << "Assigned new variable: " << literalId << endl;
    }
	//if (prevAssignedIndex[literal] == -1) {
	//	prevAssignedIndex[literal] = value;
	//}
    
    ////cout << "assigning value to variable: "<< literalId << ", " << v.value << endl;
    //Add the node
    variables[literalId] = v;
    if (level > -1){
        vector<int> parentNodeIds = {};
        for(int i = 0; i < parentLiterals.size(); i++){
            
            Variable parentVar = getVariable(abs(parentLiterals[i]));
            parentNodeIds.push_back(parentVar.literalId*(parentVar.value == 0? -1:1));
        }
        
        int result = implicationGraph.addNode(literalId,literal,level, value, parentNodeIds);
        
        return result;
    }else{
        return NOCONFLICT;
    }
    
}

void Formula::unassignVariable(int literalId) {
    Variable v = getVariable(literalId);
	////cout << "Unassigning literal: " <<literalId<< endl;
	////cout << "\tassignedIndex before size: " << assignedIndex.size() << endl;
    assignedIndex.remove(literalId);
	////cout << "\tassignedIndex after size: " << assignedIndex.size() << endl;
	////cout << "\tunassignedIndex before size: " << unassignedIndex.size() << " ";
	if (find(unassignedIndex.begin(), unassignedIndex.end(), literalId) == unassignedIndex.end())
	{
		unassignedIndex.push_back(literalId);
	}
    
	////cout << "\tunassignedIndex after size: " << unassignedIndex.size() << endl;
    variables[literalId].value = -1;
}

bool Formula::allVariablesAssigned(){
	//cout << "variables.size: " << variables.size() << " assignedIndex.size(): "<<assignedIndex.size() << " unassignedIndex.size(): " << unassignedIndex.size() << endl;
    return variables.size() == assignedIndex.size();
}

int Formula::removeSingleLiteralVariables2() {
    //Find all the single literals
    vector<string> clauses = vector<string>();
    for(auto const& imap: formula){
        clauses.push_back(imap.first);
        //cout << imap.first << ",";
    }
    while(1){
        vector<int> variablesWithOneLiteral = vector<int>();
        unordered_map<int, int> singleLiteralVariableSign = unordered_map<int, int>();
        
        
        for (int i = 0; i < clauses.size(); i++) {
            vector<int> clause = getClause(clauses[i]).literals;
            /*//cout << "print cluase ";
            getClause(clauses[i]).printClause();
            //cout << endl;*/
            //if clause.id == "10 or -6 or"
            if (clause.size() == 1) {
                auto conflictSingleLiteral = find(variablesWithOneLiteral.begin(), variablesWithOneLiteral.end(), -1*clause[0]);
                if (conflictSingleLiteral != variablesWithOneLiteral.end()) {
                    return CONFLICT;
                }
                else {
                    
                    variablesWithOneLiteral.push_back(clause[0]);
                    removeClause(clauses[i]);
                    clauses.erase(clauses.begin()+i);
                    //cout << clauses[i] << endl;
                    //print2Watched(true);
                    i--;
                    //Assign the single iteral to a corresponding value
                    if (clause[0] > 0) {
                        assignVariable(abs(clause[0]), clause[0], 1, -1, {});
                    }
                    else {
                        assignVariable(abs(clause[0]), clause[0], 0, -1, {});
                    }
                }
            }
        }
        if (variablesWithOneLiteral.size() == 0)
            break;
        //For all single literals remove the clause all together if it is satisified or remove the complement
        for (int j = 0; j < variablesWithOneLiteral.size(); j++){
            ////cout << "Considering single literal: " << variablesWithOneLiteral[j] << endl;
            for (int i = 0; i < clauses.size(); i++) {
                vector<int> clause = getClause(clauses[i]).literals;
                int literal = variablesWithOneLiteral[j];
                int compLiteral = literal * -1;
                ////cout << "variable to test: " << literal << "complement: " << compLiteral << endl;
                auto compPosition = find(clause.begin(), clause.end(), compLiteral);
                auto normalPosition = find(clause.begin(), clause.end(), literal);
                if (normalPosition != clause.end()) {
                    removeClause(clauses[i]);
                    clauses.erase(clauses.begin()+i);
                    //cout << clauses[i] << endl;
                    //print2Watched(true);

                    i--;
                }
                else if (compPosition != clause.end()) {
                    ////cout << "Clause size: " << clause.size() << endl;
                    if (clause.size() != 0){
                        removeClauseLiteral(clauses[i], compPosition - clause.begin());
                        i--;
                    }else{
                        return CONFLICT;
                    }
                }
            }
            //printFormula();
        }
    }
    //cout << endl;
    //cout << endl;
    //cout << "|--------------------------------------------------------------------------------------------------------------|" << endl;
    //cout << "|                                                                                                              |" << endl;
    //cout << "|                                      FINISHED SINGLE LITERAL REMOVAL                                         |" << endl;
    //cout << "|                                                                                                              |" << endl;
    //cout << "|--------------------------------------------------------------------------------------------------------------|" << endl;
    //cout << endl;
    //cout << endl;
    needsSingleRemovalPass = false;
    return NOCONFLICT;
}

int Formula::tryReassigningToEmpty(int currentPointer,string clauseId){
    Clause clause = getClause(clauseId);
    int newPointer = currentPointer;
    for (unsigned int i = 0; i < clause.literals.size(); i++ ){
        ////cout << i << endl;
        ////////////////////////////////////////////////////////////
        /////////////////// ACCSSING VARIABLE COPY /////////////////
        ////////////////////////////////////////////////////////////
        if(clause.pointerToLiteralID(i) == 0 || clause.pointerToLiteralID(i) > 100000){
            //cout << "STOP" << endl;
        }
        
        Variable candidatePointerVariable = getVariable(clause.pointerToLiteralID(i));
        if(i != clause.p1 && i != clause.p2 && candidatePointerVariable.value == -1){
            newPointer = i;
            break;
        }
    }
    
    if(currentPointer != newPointer){
        changeWatchedLiteral(clause.id, currentPointer, newPointer);
        ////cout << "p1: " << clause->p1 << " p2: " << clause->p2 << endl;
        return newPointer;
    }else{
        return -1;
    }
}

/*

int Formula::removeSingleLiteralVariables2(){
    list<int> removedClauses = list<int>();
    unordered_map<int, int> singleLiteralVariableSign = unordered_map<int, int>();
    vector<int> variablesWithOneLiteral = vector<int>();
    vector<int> clauses = vector<int>();
    for(auto const& imap: formula){
        clauses.push_back(imap.first);
    }
    unsigned int approvedClauses = 0;
    int i = 0;
    while(approvedClauses < clauses.size()){
        //if a clause has one literal add it to the variablesWithOneLiteral vector
        //print2Watched(true);
        vector<int> clause = formula[clauses[i]].literals;
        //cout << "Checking clause: " << i << endl;
        if(clause.size() == 1){
            //Check weather this literal already has a single literal of the complimentary sort
            if (singleLiteralVariableSign[abs(clause[0])] && singleLiteralVariableSign[abs(clause[0])] == -1*clause[0]){
                return CONFLICT;
            }else if(!singleLiteralVariableSign[abs(clause[0])]){
                //the sign is the same and thus we can proceed
                singleLiteralVariableSign[abs(clause[0])] = clause[0];
                //cout << clause[0];
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
            removeClause(clauses[i]);
            //formula.erase(formula.begin()+i);
            //since we removed a clause we let i be the same until the next loop
        }else{
            //for a clause that has more than one literal we check it for single literal matches
            int incrementI = 1;
            int incrementA = 1;
            for(int unsigned j = 0; j < variablesWithOneLiteral.size(); j++){
                int literal = variablesWithOneLiteral[j];
                int compLiteral = literal*-1;
                auto compPosition = find(clause.begin(), clause.end(), compLiteral);
                auto normalPosition = find(clause.begin(), clause.end(), literal);
                if (normalPosition != clause.end()){
                    //Delete the clause all together because it has already been satisified by the single literal
                    //assignement
                    removeClause(clauses[i]);
                    clauses.erase(clauses.begin() + i);
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
                    if(clause.size()!=0){
                        updateClause(clauses[i], clause);
                        
                    }else{
                        removeClause(clauses[i]);
                        clauses.erase(clauses.begin() + i);
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
        if(approvedClauses < clauses.size() && i == clauses.size()-1){
            i = 0;
        }
        
    }
    
    //remove each variable that had a single literal clause from any index or assignment list apart from the original variables list
    //for (unsigned int i = 0; i < variablesWithOneLiteral.size(); i++){
    //    //cout << "remove: " << abs(variablesWithOneLiteral[i]) << endl;
    //    removeVariable(abs(variablesWithOneLiteral[i]));
    //}
    
    /*for(int unsigned i = 0; i < formula.size(); i++){
        vector<int> clause = formula[i].literals;
        //cout << "ClauseId: " << i << endl;
		int count = 0;
        for(int unsigned j = 0; j < clause.size(); j++){
            //cout << "\tLiteral: " << clause[j] << endl;
			if (count < 2){
				if (clause[j] < 0){
					//cout <<"Negative clause: "<< clause[j] << " "<< endl;
					formula[i].printClause();
					variables[abs(clause[j])].addNegativeClause(i);
				}else{
					//cout << "Positive clause: " << clause[j] << " " << endl;
					formula[i].printClause();
					variables[abs(clause[j])].addPositiveClause(i);
				}
				count+= 1;
			}
        }
 return NOCONFLICT;
 }

    }*/
    

    


bool Formula::checkSolution(){
    formula = originalFormula;
    for(auto clause:originalFormula){
        if(!clause.second.evaluateAll(variables)){
            //cout << " FAILED CLAUSE ID:" << clause.first ;
            printClause(clause.first);
            return false;
        }
    }
    return true;
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
        printClause(clauseIt.first);
        i++;
    }
}
void Formula::printClause(string clauseId){
    Clause clause = getClause(clauseId);
    //cout << clauseId << " -> ";
    for (unsigned int j = 0; j < clause.literals.size(); j++) {
        
        if (clause.literals[j] < 0) {
            //cout << "NOT " << variables[abs(clause.literals[j])].letter;
            //cout << "(" << variables[abs(clause.literals[j])].value << ")" << (clause.p1 == j || clause.p2 == j? "*":"");
        }
        else {
            //cout << variables[clause.literals[j]].letter;
            //cout << "(" << variables[abs(clause.literals[j])].value << ")" << (clause.p1 == j || clause.p2 == j? "*":"");
        }
        
        if (j < clause.literals.size() - 1) {
            //cout << " OR ";
        }
        
    }
    //cout << "  SIZE=" << clause.literals.size() << endl;
    //cout << endl;
}
void Formula::print2Watched(bool verbose) {
	//cout << "|-------------------------------------------------|-----------|------------------------------------------------|" << endl;

		for (auto& it : variables) {
			
			for (int i = 0; i < max(it.second.postiveClauses.size()+1, it.second.negativeClauses.size()+1); i++) {
				if (i<it.second.postiveClauses.size()) {
					ostringstream s;
					Clause clause = formula[it.second.postiveClauses[i]];
					//cout << left << setw(50);
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
					
					
					//cout << s.str();
				}
				else {
					ostringstream s;
					//cout << left << setw(50);
					s << "| ";
					//cout << s.str();
				}
				if (i == 0) {
                    string value = (it.second.value == 1? " = TRUE ":(it.second.value == 0?"= FALSE ":" = ---- "));
                    //cout << "| "<< it.second.letter << value << " |";
                }
				else {
					//cout << "|           |";
				}
				if (i<it.second.negativeClauses.size()) {
					ostringstream s;
					Clause clause = formula[it.second.negativeClauses[i]];
					//cout << right << setw(50);
					
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
					
					//cout << s.str();
				}
				else {
					ostringstream s;
					//cout << right << setw(50);
					s << "|\n";
					//cout << s.str();
				}
			}
			//cout << "|-------------------------------------------------|-----------|------------------------------------------------|" << endl;
		}
		//cout << "|-------------------------------------------------|-----------|------------------------------------------------|" << endl;
}
