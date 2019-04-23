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
Formula LoadFormula();
void printAnswer(Formula &phi, int ans);
int UnitPropagation(Formula &phi, Variable branchVar,int level);
Variable PickBranchingVariable(Formula &phi);
int ConflictAnalysis(Formula &phi, int dl);
void Backtrack(Formula &phi, int beta);
int CDCL(Formula &phi);

int main() {
    
    //TESTING TIME
    Verify tester = Verify();

	//tester.test2WatchedLiterals();
    
	//Read the encoded Enstein's puzzle into variable phi
	Formula phi = LoadFormula();
	
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

	//phi_test.assignVariable(branchVar.literal, branchVar.value, 1, vector<Variable>());

	cout << "The chosen variable is: " << branchVar.literal << " with assignements set to: " << branchVar.value << " and activity level at: " << branchVar.activity << endl;

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
	//Solve the puzzle
	int ans = CDCL(phi);

	

	//Print the answer
	printAnswer(phi, ans);

	while (1);
	return 0;
}

/******************************************************
* @description: Loads the formula
*
* @params: phi(Formula) -
*/
Formula LoadFormula() {
	//The following code assume .cnf input file
    int numOfvar = 0, numOfClauses = 0;
	ifstream File;
	File.open("puzzle6.cnf");
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
			numOfvar = stoi(line.substr(6, 2));
			numOfClauses = stoi(line.substr(9, 2));
			cout << "numOfvar: " << numOfvar << " numOfClauses: " << numOfClauses << endl;
			break;
		}
	}
	//Read the rest of the file containing encoded formula
	vector<Clause> phi = vector<Clause>();
    unordered_map<int,Variable> variables = unordered_map<int,Variable>();
    
	int i = 0;
	while (File) {
		int temp = 1;
		File >> temp;
        phi.push_back(Clause());
        //int count = 0;
        while (temp) {
            if (find( phi[i].literals.begin(), phi[i].literals.end(), temp) == phi[i].literals.end()){
                phi[i].literals.push_back(temp);
                phi[i].literalIds.push_back(abs(temp));
                auto literal = phi[i].literals[phi[i].literals.size()-1];
                auto it = variables.find(abs(literal));
                if (it == variables.end()){
                    variables[abs(literal)] = Variable(abs(literal),-1);
                }
            }
			File >> temp;
           // cout << literal << ":" << i << endl;
            //if (count < 2){
                
                //if (literal < 0){
                //    variables[abs(literal)].addNegativeClause(i);
                //}else{
                //    variables[abs(literal)].addPositiveClause(i);
                //}
                //count+= 1;
            //}
            
            
            
			
		}
        //cout << "clause end" << endl;
        
		i++;
		if (i == numOfClauses) {
			break;
		}
	}
    /*for(int i = 1; i <= variables.size(); i++){
        for(int j = 0; j < variables[i].negativeClauses.size(); j++){
            cout << variables[i].negativeClauses[j] << " ";
        }
        cout << " :: " << variables[i].letter << " :: ";
        for(int j = 0; j < variables[i].postiveClauses.size(); j++){
            cout << variables[i].postiveClauses[j] << " ";
        }
        cout << endl;
    }*/

	File.close();
    
	return Formula(phi,variables);
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
	phi.printVariables();
	for (auto& it: phi.variables) {
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

    if (branchVar.literal == 0){
        return phi.removeSingleLiteralVariables();
    }
	//phi.printVariables();

    vector<Variable> vars = vector<Variable>();
	vector< vector<int> > parents = vector<vector<int> >();
    vars.push_back(branchVar);
	parents.push_back(vector<int>());
	//phi.printFormula();
    while((!phi.allVariablesAssigned() && (vars.size() > 0)) || vars.size() > 0){
        //we need assign the
        
        Variable var = vars.front();
		vector<int> parent = parents.front();
		int assignmentResult = phi.assignVariable(var.literal, var.value, level, parent);
		//phi.printVariables();
		//cout << "got here" << endl;
		cout << "Considering variable: " << var.letter << endl;
		vars.erase(vars.begin(), vars.begin() + 1);
		parents.erase(parents.begin(), parents.begin() + 1);
		cout << "vars size: " << vars.size() << " parents size: " << parents.size() << endl;
		if (assignmentResult == CONFLICT) {
			//cout << "Level index: " << phi.implicationGraph.levelIndex[1].size() << endl;
			return CONFLICT;
		}
		
        //clauses that comp of assigned variable
        vector<int> clauses;
        if (var.value == 0){
            clauses = phi.variables[var.literal].postiveClauses;
        }else{
            clauses = phi.variables[var.literal].negativeClauses;
        }
		cout << "clauses size: " << clauses.size() << endl;
        for (unsigned int i=0; i< clauses.size(); i++){
            
            int clauseId = clauses.at(i);
			
            Clause *clause = &phi.formula[clauseId];
			//cout << "ID of the clause: " << clauseId << ". Size of the clause: " << clause.literals.size() << endl;
			//clause->printClause();
            //figure out which pointer you are
            int  *currentPointer = new int;
            int  *otherPointer = new int;

			//cout << "clauseId: " << clauseId << endl;
            if(var.literal == abs(clause->literals[clause->p1]))
			{
                //this means our variable is p1
                currentPointer = &clause->p1;
                otherPointer = &clause->p2;
            }else{
                //if you ended up here it means your variable pointer is p2
                currentPointer = &clause->p2;
                otherPointer = &clause->p1;
            }
			//cout << "p1: " << clause->p1 << " p2: " << clause->p2 << endl;
			//cout << "currentPointer: " << currentPointer << endl;
			//cout << "otherPointer: " << otherPointer << endl;

			//the assumption here is that the currentPointer is now false since we are only looking
            //at the compliment clauses (aka the clauses that did not satisfy the assignement just made)
			//for that reason we care only if our other pointer is true since we know the currentPointer value
			//has to be false
			if (!clause->evaluate(*otherPointer, phi.variables[clause->pointerToLiteralID(*otherPointer)].value)) {
				//cout << "1. I'm inside if statement." << endl;
				int implicatedVarId = clause->pointerToLiteralID(*otherPointer);
				int implicatedVarValue = clause->whatValueMakesThisLiteralTrue(*otherPointer);
				//cout << "Implied var: " << implicatedVarId << " with val: " << implicatedVarValue << endl;
				//clause->getParentsByPointer(*otherPointer);
				vector<int> parentLiterals(clause->getParentsByPointer(*otherPointer));
				//phi.printVariables();
				//clause->printClause();

				/*vector<Variable> parentVariables = vector<Variable>();
				for (unsigned int i = 0; i < parentLiterals.size(); i++) {
					//cout << parentLiterals[i] << " ";
					parentVariables.push_back(phi.variables[parentLiterals[i]]);
				}*/
				//cout << endl;
				//redo to make nicer
				//cout << "Implied conflict: " << implicatedVarId << " with val: " << implicatedVarValue << " with parent size: " << parentVariables.size() << endl;
				//phi.printVariables();

				vars.insert(vars.begin(), Variable(implicatedVarId, implicatedVarValue));
				parents.insert(parents.begin(), parentLiterals);
				//the other pointer is true and so we should move it to that pointers
				//return CONFLICT;
				break;
			}
            else if(phi.variables[clause->pointerToLiteralID(*otherPointer)].value == -1){
				//cout << "2. I'm inside else if statement." << endl;
                int newPointer = *currentPointer;
                for (unsigned int i = 0; i < clause->literals.size(); i++ ){
					//cout << i << endl;
                    auto literal = clause->literals[i];
                    int literalId = clause->pointerToLiteralID(i);
                    if(i != *otherPointer && i != *currentPointer && phi.variables[literalId].value == -1){
                        if (literal < 0){
                            phi.variables[literalId].addNegativeClause(clauseId);
                            var.removeNegativeClause(clauseId);
                            
                        }else{
                            phi.variables[literalId].addPositiveClause(clauseId);
                            var.removePositiveClause(clauseId);
                        }
                        newPointer = i;
						//cout << "newPointer: " << newPointer << endl;
                        break;
                    }
                    
                }
                if(*currentPointer != newPointer){
					*currentPointer = newPointer;
					//cout << "p1: " << clause->p1 << " p2: " << clause->p2 << endl;
                }else{
					//cout << "3. I'm inside else statement." << endl;
                    int implicatedVarId = clause->pointerToLiteralID(*otherPointer);
                    int implicatedVarValue = clause->whatValueMakesThisLiteralTrue(*otherPointer);
					//cout << "Implied var: " << implicatedVarId << " with val: " << implicatedVarValue << endl;
					//clause->getParentsByPointer(*otherPointer);
                    vector<int> parentLiterals(clause->getParentsByPointer(*otherPointer));
					//phi.printVariables();
					//clause->printClause();

                    /*vector<Variable> parentVariables = vector<Variable>();
                    for(unsigned int i = 0; i < parentLiterals.size(); i++){
						//cout << parentLiterals[i] << " ";
                        parentVariables.push_back(phi.variables[parentLiterals[i]]);
                    }*/
					//cout << endl;
                    //redo to make nicer
					//cout << "Implied var: " << implicatedVarId << " with val: " << implicatedVarValue << " with parent size: " << parentVariables.size() << endl;
					//phi.printVariables();
				
                    vars.push_back(Variable(implicatedVarId, implicatedVarValue));
					parents.push_back(vector<int>(parentLiterals));
                }

            }
        }
		phi.print2Watched(phi.assignedIndex);
    }
	cout << "Finished unit propagation" << endl;
    return NOCONFLICT;
}

/******************************************************
* @description: Function that implements heuristics how variables are being picked for further search
*				using unit propagation. Currently the function implements Variable State Decaying Sum (VSIDS).
*
* @params: phi(Formula) - the formula at the current state of exploration (might be modified accordingly to reflect the exporation state)
* @ret: Variable - returns Variable object that represents next variable to be selected
*/
Variable PickBranchingVariable(Formula &phi) {
    
    //int absLiteral = *next(phi.unassignedIndex.begin(), rand() % phi.unassignedIndex.size());
    //return Variable(absLiteral,rand() % 1);
    
    float highestActivity = -1.0; //you have to init this to under 0.0 since your activity scores in each var is 0.0
    
	int brachingVar = -1;
	//Use Variable State Independent Decaying Sum (VSIDS) to pick branching varible
    //important branch only on variables not already assigned
	for (auto& it : phi.unassignedIndex) {
		float currentActivity = phi.variables[it].activity;
		if (highestActivity < currentActivity) {
			highestActivity = currentActivity;
			brachingVar = it;
		}
	}
	cout << "pick branching value: " << phi.prevAssignedIndex[brachingVar] << endl;
	if(phi.prevAssignedIndex[brachingVar] == 0)
		return Variable(brachingVar, 1);
	else if (phi.prevAssignedIndex[brachingVar] == 1)
		return Variable(brachingVar, 0);
	else
		return Variable(brachingVar, rand() % 2); //Not sure how do you decide which value to set??? Currently, it is assigned randomly.
}

/******************************************************
* @description:
*
* @params phi(Formula) -
* @ret:
*/
int ConflictAnalysis(Formula &phi, int dl) {

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
	int conflictingNodeIndecesPos;
	int conflictingNodeIndecesNeg;
	for (unsigned int i = 0; i < conflictingNodeIndeces.size(); i++) {
		if (phi.implicationGraph.nodes[conflictingNodeIndeces[i]].value == 1)
			conflictingNodeIndecesPos = conflictingNodeIndeces[i];
		else
			conflictingNodeIndecesNeg=conflictingNodeIndeces[i];
	}
	
	//Go through all the combinations of pairs of conflicted nodes
	//For each pair concatenate the parents of the nodes to form a new clause
	//Add the learned clause to the original formula
	int lowestLevelParentNodeId;
	
			vector<int> clause;
			vector<int> parentPos = phi.implicationGraph.nodes[conflictingNodeIndecesPos].parentNodes;
			vector<int> parentNeg = phi.implicationGraph.nodes[conflictingNodeIndecesNeg].parentNodes;
			cout << "parentPos size: " << parentPos.size() << " parentNeg size: " << parentNeg.size() << endl;
			for (auto const& parent : parentPos) {
				int parentId = phi.implicationGraph.nodes[parent].id;
				int parentLiteralId = phi.implicationGraph.nodes[parent].literalId;
				int parentValue = phi.implicationGraph.nodes[parent].value;
				int parentLevel = phi.implicationGraph.nodes[parent].level;
				cout << "parentLiteralId: " << parentLiteralId << " parentValue: " << parentValue << endl;
				if (parentValue && (find(clause.begin(), clause.end(), -parentLiteralId) == clause.end()))
					clause.push_back(-parentLiteralId); //learn the opposite value than what was initially assigned
				else if (!parentValue && (find(clause.begin(), clause.end(), parentLiteralId) == clause.end()))
					clause.push_back(parentLiteralId);
				cout << "parentLevel" << parentLevel << endl;
				//Overwrite the parent with the lowest level found
				if (level > parentLevel) {
					level = parentLevel;
					cout << "parentLevel" << parentLevel << endl;
					lowestLevelParentNodeId = parentId;
				}
			}
			for (auto const& parent : parentNeg) {
				int parentId = phi.implicationGraph.nodes[parent].id;
				int parentLiteralId = phi.implicationGraph.nodes[parent].literalId;
				int parentValue = phi.implicationGraph.nodes[parent].value;
				int parentLevel = phi.implicationGraph.nodes[parent].level;
				cout << "parentLiteralId: " << parentLiteralId << " parentValue: " << parentValue << endl;
				if (parentValue && (find(clause.begin(), clause.end(), -parentLiteralId) == clause.end()))
					clause.push_back(-parentLiteralId); //learn the opposite value than what was initially assigned
				else if (!parentValue && (find(clause.begin(), clause.end(), parentLiteralId) == clause.end()))
					clause.push_back(parentLiteralId);
				cout << "parentLevel" << parentLevel << endl;
				//Overwrite the parent with the lowest level found
				if (level > parentLevel) {
					level = parentLevel;
					cout << "parentLevel" << parentLevel << endl;
					lowestLevelParentNodeId = parentId;
				}
			}
			phi.formula.push_back(Clause(clause));
			phi.bumpActivities(clause);
			cout << "Learned clause: ";
			for (unsigned int i = 0; i < clause.size(); i++) {
				if (clause[i] < 0)
					cout << "NOT ";
				cout << phi.variables[abs(clause[i])].letter << " ";
				if (i < clause.size() - 1)
					cout << "OR ";
			}
			cout << endl;

	//while (1);
    level = phi.implicationGraph.backtrackToLowestLevelParent();
	return level; //return one level below of the corresponding lowest level root node

}

/******************************************************
 * @description:
 *
 * @params: phi(Formula) -
 * @ret:
 */
void Backtrack(Formula &phi, int beta) {
	/* phi stores a map of levels and asignments that were done at a this level.
	   Using beta that is the level we want to back track to we use the aformentioned data structure to 
	   unassign all the variables that were set up until the given level beta.
	*/
	int initialSize = phi.implicationGraph.levelIndex.size()-1;
	cout << "initialSize: " << initialSize << "beta: " << beta << endl;
	for (int i = initialSize; i > beta; i--) {
		vector<int> levelIndexList = phi.implicationGraph.levelIndex[i];
		cout << "levelIndexList size: " << levelIndexList.size() << endl;
		phi.implicationGraph.levelIndex.erase(i);
		for (unsigned int j = 0; j < levelIndexList.size(); j++) {
			phi.unassignVariable(levelIndexList[j]);
			phi.implicationGraph.removeNodesByLiteralId(levelIndexList[j]);
		}
	}
}

/******************************************************
* @description:
*
* @params phi(Formula) -
* @ret:
*/
int CDCL(Formula &phi) {
    int dl = 0;
	if (UnitPropagation(phi,Variable(0,-1),dl) == CONFLICT) {
		return UNSAT;
	}
	phi.printFormula();
	phi.printIndex();
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
				
				dl = beta;
				//phi.printFormula();
			}
		}
		phi.printVariables();
		phi.printIndex();
	}
	return SAT;
}

