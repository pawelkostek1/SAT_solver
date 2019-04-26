#include<iostream>
#include<fstream>
#include<list>
#include<string>
#include<sstream>
#include<vector>
#include<map>
#include<iostream>
#include"Formula.h"
#include"Graph.h"
#include"constants.h"
#include"Clause.h"
#include"Variable.h"
#include<unordered_map>
#include<cmath>
#include<algorithm>    // std::find
#include"Verify.h"
#include <stdlib.h>

#include"curses.h"
//#include "Verify.h"
using namespace std;

//Declare functions
Formula LoadFormula(string problem);
void printAnswer(Formula &phi, int ans);
int UnitPropagation(Formula &phi, Variable branchVar,int level);
Variable RandomBranchingVariable(Formula &phi);
Variable PickBranchingVariable(Formula &phi);
int ConflictAnalysis(Formula &phi, int dl);
void Backtrack(Formula &phi, int beta);
int CDCL(Formula &phi);

int main() {
    
    //TESTING TIME
    Verify tester = Verify();
    //tester.test2WatchedIndex();
	//tester.test2WatchedLiterals();
    
	//Read the encoded Enstein's puzzle into variable phi
	//
    Formula phi = LoadFormula("puzzle5.cnf");
    //Formula phi = tester.createFormula();
    cout << "INITIAL STATE" << endl;
    phi.printClauses();
    phi.print2Watched(true);
    cout << "\n\n" << endl;
    //Solve the puzzle
    int ans = CDCL(phi);
    
    //Print the answer
    printAnswer(phi, ans);
    bool isCorrect = phi.checkSolution();
    cout << (isCorrect? "SOLUTION IS CORRECT":"SOLUTION IS WRONG!!!") << endl;
    while (1);
    return 0;
	//printAnswer(phi, SAT);
	/*
	//////////////////////////////////
	//Verify the functions before attempting to solve the puzzle
	cout << "//////////////////////////////////////////////////////" << endl
		 << "Verify the implementation of various functions." << endl;
	//Define Verify objects (currently not used) - we can use it to possibly generate the current state of exploration.
	//Verify test;

	int numOfvar = 4, numOfClauses = 4;
	unordered_map<int, Variable> variables;
	variables[1] = Variable(1, -1);
	variables[2] = Variable(2, -1);
	variables[3] = Variable(3, -1);
	variables[4] = Variable(4, -1);
	vector<Clause> F;
	F.push_back(Clause(vector<int>{-1, -2}));
	F.push_back(Clause(vector<int>{-1, 2, 3}));
	F.push_back(Clause(vector<int>{-1, -3}));
	F.push_back(Clause(vector<int>{-1, 4}));
	variables[1].addNegativeClause(0);
	variables[1].addNegativeClause(1);
	variables[2].addPositiveClause(1);
	variables[2].addNegativeClause(0);
	variables[3].addPositiveClause(1);
	variables[3].addNegativeClause(2);
	variables[4].addPositiveClause(3);
	Formula phi_test(F, variables);
	phi_test.printFormula();

	//Test Pick Branching Variable function
	cout << "*********************************************" << endl;
	cout << "Testing PickBranching function" << endl;
	//phi_test.assignVariable(1, 1, 0, vector<Variable>());
	phi_test.variables[1].activity = 1.0; // should select var 2
	phi_test.variables[3].activity = 1.0; 
	phi_test.printVariables();
	//Pick Branching Variable assumes variables are assigned certain activity levels.
	//Currently Pick Branching selects the first variable in order that has the biggest activity - is that an issue???
	//Variable with the highest activity level is assigned 
	Variable branchVar = PickBranchingVariable(phi_test);
	branchVar.value = 1;

	//phi_test.assignVariable(branchVar.literalId, branchVar.value, 1, vector<Variable>());

	cout << "The chosen variable is: " << branchVar.literalId << " with assignements set to: " << branchVar.value << " and activity level at: " << branchVar.activity << endl;

	//Test Conflict Analysis function
	cout << "*********************************************" << endl;
	cout << "Testing  ConflictAnalysis function" << endl;
	int dl = 1;

	int beta;
	//vector<Variable> parents{phi_test.variables[1], phi_test.variables[2]};
	//int implicationAssignmentResult = phi_test.assignVariable(3, 1, dl, parents);
	//if (implicationAssignmentResult == NOCONFLICT) {
	//	cout << "Succesfully resulted in noconflict." << endl;
	//}
	//parents = vector<Variable>{ phi_test.variables[1]};
	//implicationAssignmentResult = phi_test.assignVariable(3, 0, dl, parents);
	//if (implicationAssignmentResult == CONFLICT) {

	if (UnitPropagation(phi_test, branchVar, dl) == CONFLICT) {
		cout << "*********************************************" << endl;
		cout << "Succesfully resulted in conflict." << endl;
		beta = ConflictAnalysis(phi_test, dl);
	}
	cout << "Conflict Analysis suggested to backtrack to level: " << beta << "." << endl;




	//Test Backtrack function
	cout << "*********************************************" << endl;
	cout << "Testing  Backtrack function" << endl;
	Backtrack(phi_test, beta);
	phi_test.printVariables();
	//////////////////////////////////
	*/
	

	
}

/******************************************************
* @description: Loads the formula
*
* @params: phi(Formula) -
*/
Formula LoadFormula(string problem) {
	//The following code assume .cnf input file
    int numOfvar = 0, numOfClauses = 0;
	ifstream File;
	File.open(problem);
	if (!File) {
		cout << "Unable to open file puzzle.cnf" << endl;
	}
    cout << "File was loaded into memory" << endl;
	string line;
	while (getline(File, line)) {
		//read the file line by line
		if (line[0] == 'c') {
			//cout << line.substr(2, line.length() - 2) << endl;
		}
		else if (line[0] == 'p') {
			//cout << line.substr(2, line.length() - 2) << endl;
			if (line.substr(2, 3) != "cnf") {
				cout << "Error: Provided file is not in CNF format." << line.substr(2, 3) << endl;
			}
			//numOfvar = stoi(line.substr(6, 2));
			//numOfClauses = stoi(line.substr(9, 2));
            //cout << "numOfvar: " << numOfvar << " numOfClauses: " << numOfClauses << endl;
            vector<string> parts = {};
            istringstream iss(line);
            string s;
            while ( getline( iss, s, ' ' ) ) {
                parts.push_back(s.c_str());
            }
			numOfvar = stoi(parts[2]);
			numOfClauses = stoi(parts[3]);
			
			break;
		}
	}
	//Read the rest of the file containing encoded formula
	
    Formula formula = Formula();
	int i = 0;
	while (File) {
		int temp = 1;
		File >> temp;
        //int count = 0;
        vector<int> clause = {};
        while (temp) {
            if (find(clause.begin(), clause.end(), temp) == clause.end()){
                clause.push_back(temp);
            }
			File >> temp;
		}
        formula.addClause(clause,true);
        //cout << "clause end" << endl;
        
		i++;
		if (i == numOfClauses) {
			break;
		}
	}

	File.close();
    
	return formula;
}


/******************************************************
* @description:
*
 * @params ans(int) -
*/
void printAnswer(Formula &phi, int ans) {
	cout << "The solver produced following output: ";
	if (ans == SAT) {
		cout << "SAT" << endl;
	}
	else if (ans == UNSAT) {
		cout << "UNSAT" << endl;
	}
	cout << "The following assignments were found:"<<endl;
	//phi.printVariables();
	for (auto& it: phi.getVariables()) {
		cout << it.second.letter << "(" << it.second.value << ") ";
	}
	//cout << endl << phi.variables.size();
}

/******************************************************
* @description:
*
* @params: phi(Formula) -
* @ret:
*/
int UnitPropagation(Formula &phi, Variable branchVar,int level) {
    cout << "INSIDE UNITPROP" << endl;
    if (branchVar.literalId == 0){
        return phi.removeSingleLiteralVariables2();
    }
	//phi.printVariables();

    vector<Variable> vars = vector<Variable>();
	vector< vector<int> > parents = vector<vector<int> >();
	vector<int> varsId = vector<int>();
	
    varsId.push_back(branchVar.literalId);
    vars.push_back(branchVar);
    
	parents.push_back(vector<int>());
	//phi.printFormula();
    while((!phi.allVariablesAssigned() && (vars.size() > 0)) || vars.size() > 0){
        //we need assign the
       
        Variable var = vars.front();
		vector<int> parent = parents.front();
        if(var.literalId == 0 || var.literal > 10000){
            cout << "STOP" << endl;
        }
		int assignmentResult = phi.assignVariable(var.literalId,var.literal, var.value, level, parent);
        
		//phi.printVariables();
		//cout << "got here" << endl;
        cout << "Considering variable: " << var.literalId << endl;
		cout << "Considering variable: " << var.letter << endl;
		//varsId.erase(varsId.begin(), varsId.begin() + 1);
		vars.erase(vars.begin(), vars.begin() + 1);
		parents.erase(parents.begin(), parents.begin() + 1);
		cout << "vars size: " << vars.size() << " parents size: " << parents.size() << endl;
		if (assignmentResult == CONFLICT) {
			//cout << "Level index: " << phi.implicationGraph.levelIndex[1].size() << endl;
			return CONFLICT;
		}
        
		/////////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////THIS VARIABLE IS A COPY TO THE REAL ONE INSIDE FORMULA////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////
        if(var.literalId == 0 || var.literalId > 10000){
            cout << "STOP" << endl;
        }
        
        
        
        Variable assignedVar = phi.getVariable(var.literalId);
        
        //clauses that comp of assigned variable
        vector<int> clauses;
        if (assignedVar.value == 0){
            clauses = assignedVar.postiveClauses;
        }else{
            clauses = assignedVar.negativeClauses;
        }
		cout << "clauses size: " << clauses.size() << endl;
        for (unsigned int i=0; i< clauses.size(); i++){
            
            int clauseId = clauses[i];
			
            Clause clause = phi.getClause(clauseId);
            if (clause.evaluateAll(phi.getVariables())){
                continue;
            }
			//cout << "ID of the clause: " << clauseId << ". Size of the clause: " << clause.literals.size() << endl;
			//clause->printClause();
            //figure out which pointer you are
            int currentPointer = NULL;
            int otherPointer = NULL;

			//cout << "clauseId: " << clauseId << endl;
            if(var.literalId == clause.pointerToLiteralID(clause.p1))
			{
                //this means our variable is p1
                currentPointer = clause.p1;
                otherPointer = clause.p2;
            }else{
                //if you ended up here it means your variable pointer is p2
                currentPointer = clause.p2;
                otherPointer = clause.p1;
            }
			//cout << "p1: " << clause->p1 << " p2: " << clause->p2 << endl;
			//cout << "currentPointer: " << currentPointer << endl;
			//cout << "otherPointer: " << otherPointer << endl;

			//the assumption here is that the currentPointer is now false since we are only looking
            //at the compliment clauses (aka the clauses that did not satisfy the assignement just made)
			//for that reason we care only if our other pointer is true since we know the currentPointer value
			//has to be false
            
            ////////////////////////////////////////////////////////////
            /////////////////// ACCSSING VARIABLE COPY /////////////////
            ////////////////////////////////////////////////////////////
            if(clause.pointerToLiteralID(otherPointer) == 0 || clause.pointerToLiteralID(otherPointer) > 100000 || clause.pointerToLiteralID(currentPointer) == 0 || clause.pointerToLiteralID(currentPointer) > 100000){
                cout << "STOP" << endl;
            }
            
            Variable otherPointerVariable = phi.getVariable(clause.pointerToLiteralID(otherPointer));
            bool otherPointerEval = clause.evaluate(otherPointer, otherPointerVariable.value);
            Variable currentPointerVariable = phi.getVariable(clause.pointerToLiteralID(currentPointer));
            bool currentPointerEval = clause.evaluate(currentPointer, currentPointerVariable.value);
			
            if (!otherPointerEval) {
				//cout << "1. I'm inside if statement." << endl;
				int implicatedLiteralId = clause.pointerToLiteralID(otherPointer);
                int implicatedVarLiteral =clause.literals[otherPointer];
				int implicatedVarValue = clause.whatValueMakesThisLiteralTrue(otherPointer);
				//cout << "Implied var: " << implicatedVarId << " with val: " << implicatedVarValue << endl;
				//clause->getParentsByPointer(*otherPointer);
				vector<int> parentLiterals(clause.getParentsByPointer(otherPointer));
               
				vars.push_back( Variable(implicatedLiteralId,implicatedVarLiteral, implicatedVarValue));
				parents.push_back( parentLiterals);
                
                cout << "//////////////////////// IMPLICATION ABOUT TO HAPPEN ////////////////////////" << endl;
                phi.printClause(clauseId);
                cout << "//////////////////////// IMPLICATION ABOUT TO HAPPEN ////////////////////////" << endl;
                if(clauseId == 398 || clauseId == 617){
                    
                    
                    
                    
                }
				//the other pointer is true and so we should move it to that pointers
				//return CONFLICT;
				break;
			}
            else if(otherPointerVariable.value == -1){
				
                int newPointer = currentPointer;
                for (unsigned int i = 0; i < clause.literals.size(); i++ ){
					//cout << i << endl;
                    ////////////////////////////////////////////////////////////
                    /////////////////// ACCSSING VARIABLE COPY /////////////////
                    ////////////////////////////////////////////////////////////
                    if(clause.pointerToLiteralID(i) == 0 || clause.pointerToLiteralID(i) > 100000){
                        cout << "STOP" << endl;
                    }
                    
                    Variable candidatePointerVariable = phi.getVariable(clause.pointerToLiteralID(i));
                    if(i != otherPointer && i != currentPointer && candidatePointerVariable.value == -1){
                        newPointer = i;
                        break;
                    }
                }
                
                if(currentPointer != newPointer){
                    phi.changeWatchedLiteral(clauseId, currentPointer, newPointer);
					//cout << "p1: " << clause->p1 << " p2: " << clause->p2 << endl;
                }else{
                    
					//cout << "3. I'm inside else statement." << endl;
                    int implicatedLiteralId = clause.pointerToLiteralID(otherPointer);
                    int implicatedVarLiteral =clause.literals[otherPointer];
                    int implicatedVarValue = clause.whatValueMakesThisLiteralTrue(otherPointer);
					//cout << "Implied var: " << implicatedVarId << " with val: " << implicatedVarValue << endl;
					//clause->getParentsByPointer(*otherPointer);
                    vector<int> parentLiterals(clause.getParentsByPointer(otherPointer));
                    cout << "//////////////////////// IMPLICATION ABOUT TO HAPPEN ////////////////////////" << endl;
                    phi.printClause(clauseId);
                    cout << "//////////////////////// IMPLICATION ABOUT TO HAPPEN ////////////////////////" << endl;
                    cout << endl;
                    
                    if(clauseId == 398 || clauseId == 617){
                        
                        
                        
                        
                    }
                    
                    
                    
                    
                    vars.push_back(Variable(implicatedLiteralId,implicatedVarLiteral, implicatedVarValue));
                    parents.push_back(parentLiterals);
                    
                }

            }
        }
		phi.print2Watched(true);
    }
    
	cout << "Finished unit propagation" << endl;
    return NOCONFLICT;
}
Variable RandomBranchingVariable(Formula &phi) {
	int brachingVar = 1 + rand() % phi.getNumOfVar();
	cout << "pick branching value: " << phi.prevAssignedIndex[brachingVar] << endl;
	if (phi.prevAssignedIndex[brachingVar] == 0)
		return Variable(brachingVar,brachingVar, 1);
	else if (phi.prevAssignedIndex[brachingVar] == 1)
		return Variable(brachingVar,brachingVar*-1, 0);
    else{
        int value = rand() % 2;
        if (value == 1){
            return Variable(brachingVar,brachingVar,value);
        }else{
            return Variable(brachingVar,brachingVar*-1,value);
        }
    }
}
/******************************************************
* @description: Function that implements heuristics how variables are being picked for further search
*				using unit propagation. Currently the function implements Variable State Decaying Sum (VSIDS).
*
* @params: phi(Formula) - the formula at the current state of exploration (might be modified accordingly to reflect the exporation state)
* @ret: Variable - returns Variable object that represents next variable to be selected
*/
Variable PickBranchingVariable(Formula &phi) {
    cout << "INSIDE PICK BRANCHNG" << endl;
    //int absLiteral = *next(phi.unassignedIndex.begin(), rand() % phi.unassignedIndex.size());
    //return Variable(absLiteral,rand() % 1);
    
    float highestActivity = -1.0; //you have to init this to under 0.0 since your activity scores in each var is 0.0
    
	int brachingVar = -1;
	//Use Variable State Independent Decaying Sum (VSIDS) to pick branching varible
    //important branch only on variables not already assigned
	for (auto& it : phi.unassignedIndex) {
        ////////////////////////////////////////////////////////////
        /////////////////// ACCSSING VARIABLE COPY /////////////////
        ////////////////////////////////////////////////////////////
        Variable unnassignedCandidateVariable = phi.getVariable(it);
        
		float currentActivity = unnassignedCandidateVariable.activity;
		if (highestActivity < currentActivity) {
			highestActivity = currentActivity;
			brachingVar = it;
		}
	}
	if(phi.prevAssignedIndex[brachingVar] == 0)
		
        return Variable(brachingVar,brachingVar, 1);
    
	else if (phi.prevAssignedIndex[brachingVar] == 1)
        
		return Variable(brachingVar,brachingVar*-1, 0);
    else{
        
        int value = rand() % 2;
        if (value == 1){
            return Variable(brachingVar,brachingVar,value);
        }else{
            return Variable(brachingVar,brachingVar*-1,value);
        }
    }
}

/******************************************************
* @description:
*
* @params phi(Formula) -
* @ret:
*/
int ConflictAnalysis(Formula &phi, int dl) {
    cout << "INSIDE CONFLICT ANALYSSI" << endl;
	int level = dl;

	//Perform decay of the activities
	phi.decayActivities();

	//Learn the clause
	/*
	You use the graph to go back to all the closest parents of the conflict, this gives you a learned clause.
	You add the clause to the formula and then you use the graph to find the level before the one that corresponds to the earliest root
	that resulted in the conflict.

	We also need to consider the case where the conflict can no longer be resolved, in which case we return UNSAT.
	*/
    //TODO
	vector<int> conflictingNodeIndeces = phi.implicationGraph.failedState.parentNodes;
	cout << "conflictingNodeIndeces: ";
	for (unsigned int i = 0; i < conflictingNodeIndeces.size(); i++) {
		cout << conflictingNodeIndeces[i] << " ";
	}
	cout << endl;
	int conflictingNodeIndecesPos = NULL;
	int conflictingNodeIndecesNeg = NULL;
   
	for (unsigned int i = 0; i < conflictingNodeIndeces.size(); i++) {
        
		if (phi.implicationGraph.getNode(conflictingNodeIndeces[i]).value == 1)
			conflictingNodeIndecesPos = conflictingNodeIndeces[i];
		else
			conflictingNodeIndecesNeg = conflictingNodeIndeces[i];
	}
    if (conflictingNodeIndecesPos == NULL || conflictingNodeIndecesPos == NULL){
        cout << "CONFILICT WITHOUT FAILED STATE PARENTS NODES!!!" << endl;
        return level;
    }else{
        cout << "conflicts: " << conflictingNodeIndecesPos << " , " << conflictingNodeIndecesNeg << endl;
    }
	//Go through all the combinations of pairs of conflicted nodes
	//For each pair concatenate the parents of the nodes to form a new clause
	//Add the learned clause to the original formula
	//int lowestLevelParentNodeId;
	
    vector<int> clause;
    vector<int> parentPos = phi.implicationGraph.getNode(conflictingNodeIndecesPos).parentNodes;
    vector<int> parentNeg = phi.implicationGraph.getNode(conflictingNodeIndecesNeg).parentNodes;
    
    cout << "parentPos size: " << parentPos.size() << " parentNeg size: " << parentNeg.size() <<"  ["<< conflictingNodeIndecesPos <<"  "<<conflictingNodeIndecesNeg<<"]"<<endl;
    auto rootNodes = phi.implicationGraph.getRoots(phi.implicationGraph.failedState);
    for (auto const& root : rootNodes) {
        Node n = phi.implicationGraph.getNode(root);
        clause.push_back(-n.literal); //learn the opposite value than what was initially assigned
    }
    /*for (auto const& parent : parentPos) {
        
        
        Node n = phi.implicationGraph.getNode(parent);
        cout << "ParentLiteralId: " << n.literalId << " n.value: " << n.value << endl;
        //The literalId is absolute so we need to use id which basically just is the literal
        if (n.value && (find(clause.begin(), clause.end(), -n.literal) == clause.end()))
            clause.push_back(-n.literal); //learn the opposite value than what was initially assigned
        else if (!n.value && (find(clause.begin(), clause.end(), n.literal) == clause.end()))
            clause.push_back(n.literal);
        cout << "n.level" << n.level << endl;
        //Overwrite the parent with the lowest level found
        if (level > n.level) {
            level = n.level;
            cout << "n.level" << n.level << endl;
            lowestLevelParentNodeId = n.literal;
        }
    }
    for (auto parent : parentNeg) {
       
        Node n = phi.implicationGraph.getNode(parent);
        cout << "n.literalId: " << n.literalId << " n.value: " << n.value << endl;
        //The literalId is absolute so we need to use id which basically just is the literal
        if (n.value && (find(clause.begin(), clause.end(), -n.literal) == clause.end()))
            clause.push_back(-n.literal); //learn the opposite value than what was initially assigned
        else if (!n.value && (find(clause.begin(), clause.end(), n.literal) == clause.end()))
            clause.push_back(n.literal);
        cout << "n.level: " << n.level << endl;
        //Overwrite the parent with the lowest level found
        if (level > n.level) {
            level = n.level;
            cout << "n.level" << n.level << endl;
            lowestLevelParentNodeId = n.literal;
        }
    }*/
    
    int cId = phi.addClause(clause,false);
    cout << "LEARNED CLAUSE: ";
    phi.getClause(cId).printClause();
    cout << endl;
    if (clause.size() == 1){
        phi.needsSingleRemovalPass = true;
        level = 0;
    }else{
        //TODO does this effect the structure??
        phi.bumpActivities(clause);
        //while (1);
        level = phi.implicationGraph.getBacktrackLevel()-1;
    
    }
    //int level2 = phi.implicationGraph.getBacktrackLevel();
    
    
	return level; //return one level below of the corresponding lowest level root node

}

/******************************************************
 * @description:
 *
 * @params: phi(Formula) -
 * @ret:
 */
void Backtrack(Formula &phi, int beta) {
    cout << "INSIDE BACKPROP" << endl;
	/* phi stores a map of levels and asignments that were done at a this level.
	   Using beta that is the level we want to back track to we use the aformentioned data structure to 
	   unassign all the variables that were set up until the given level beta.
	*/
	int initialSize = phi.implicationGraph.levelIndex.size();
	cout << "initialSize: " << initialSize << "beta: " << beta << endl;
	for (int i = initialSize; i > beta; i--) {
		vector<int> levelIndexList = phi.implicationGraph.levelIndex[i];
		cout << "levelIndexList size: " << levelIndexList.size() << endl;
		for (unsigned int j = 0; j < levelIndexList.size(); j++) {
			phi.unassignVariable(abs(levelIndexList[j]));
			phi.implicationGraph.removeNodesByLiteralId(abs(levelIndexList[j]));
		}
        phi.implicationGraph.levelIndex.erase(i);

        
	}
    phi.implicationGraph.resetFailedState();
    if(phi.needsSingleRemovalPass){
        phi.print2Watched(true);
        phi.removeSingleLiteralVariables2();
        phi.print2Watched(true);
    }
    phi.implicationGraph.printGraph();
    
}

/******************************************************
* @description:
*
* @params phi(Formula) -
* @ret:
*/
int CDCL(Formula &phi) {
    int dl = 0;
	if (UnitPropagation(phi,Variable(0,0,-1),dl) == CONFLICT) {
		return UNSAT;
	}
	//phi.printFormula();
	//phi.printIndex();
	while (!phi.allVariablesAssigned()) {
		Variable branchVar = PickBranchingVariable(phi);
		cout << "***********************************************" << endl;
        cout << "Picking a branching variable: " << branchVar.letter << " value: " << branchVar.value << endl;

		dl++;
		if (UnitPropagation(phi, branchVar, dl) == CONFLICT) {
            cout << "CONFLICT WAS DETECTED" << endl;
			int beta = ConflictAnalysis(phi, dl);
			cout << "Conflict Analysis suggested to backtrack to level: " << beta << "." ;
			if (beta < 0) {
				return UNSAT;
			}
			else {
				//phi.printFormula();
				phi.implicationGraph.printGraph();
				Backtrack(phi, beta);
				phi.implicationGraph.printGraph();
				dl = beta;
				//phi.printFormula();
			}
		}
		//phi.printVariables();
		//phi.printIndex();
	}
	return SAT;
}

