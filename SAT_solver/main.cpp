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
#include "Verify.h"
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
    
	//Read the encoded Enstein's puzzle into variable phi
	//Formula phi = LoadFormula();
	
	//printAnswer(phi, SAT);

	//////////////////////////////////
	//Verify the functions before attempting to solve the puzzle
	cout << "//////////////////////////////////////////////////////" << endl
		 << "Verify the implementation of various functions." << endl;
	//Define Verify objects (currently not used) - we can use it to possibly generate the current state of exploration.
	Verify test;

	int numOfvar = 4, numOfClauses = 4;
	vector<Clause> F;
	F.push_back(Clause(vector<int>{-1, -2}));
	F.push_back(Clause(vector<int>{-1, 2, 3}));
	F.push_back(Clause(vector<int>{-1, -3}));
	F.push_back(Clause(vector<int>{-1, 4}));
	Formula phi_test(numOfvar, numOfClauses, F);
	phi_test.printFormula();

	//Test Pick Branching Variable function
	cout << "*********************************************" << endl;
	cout << "Testing PickBranching function" << endl;
	phi_test.assignVariable(1, 1, 0, vector<Variable>());
	phi_test.variables[2].activity = 1.0; // should select var 2
	phi_test.variables[3].activity = 1.0; 
	phi_test.printVariables();
	//Pick Branching Variable assumes variables are assigned certain activity levels.
	//Currently Pick Branching selects the first variable in order that has the biggest activity - is that an issue???
	//Variable with the highest activity level is assigned 
	Variable branchVar = PickBranchingVariable(phi_test);
	branchVar.value = 0;

	//phi_test.assignVariable(branchVar.literal, branchVar.value, 1, vector<Variable>());

	cout << "The chosen variable is: " << branchVar.literal << " with assignements set to: " << branchVar.value << " and activity level at: " << branchVar.activity << endl;

	//Test Conflict Analysis function
	cout << "*********************************************" << endl;
	cout << "Testing  ConflictAnalysis function" << endl;
	int dl = 2;

	int beta;
	//vector<Variable> parents{phi_test.variables[1], phi_test.variables[2]};
	//int implicationAssignmentResult = phi_test.assignVariable(3, 1, dl, parents);
	//if (implicationAssignmentResult == NOCONFLICT) {
	//	cout << "Succesfully resulted in noconflict." << endl;
	//}
	//parents = vector<Variable>{ phi_test.variables[1]};
	//implicationAssignmentResult = phi_test.assignVariable(3, 0, dl, parents);
	//if (implicationAssignmentResult == CONFLICT) {

	phi_test.printVariables();
	phi_test.printFormula();
	if (UnitPropagation(phi_test, branchVar, dl) == CONFLICT) {
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
	
	//Solve the puzzle
	//int ans = CDCL(phi);

	//Print the answer
	//printAnswer(phi, ans);

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
	File.open("puzzle.cnf");
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
			numOfvar = stoi(line.substr(6, 1));
			numOfClauses = stoi(line.substr(8, 1));
			break;
		}
	}
	//Read the rest of the file containing encoded formula
	vector<Clause> phi = vector<Clause>();
	int i = 0;
	while (File) {
		int temp = 1;
		File >> temp;
        phi.push_back(Clause());
		while (temp) {
			phi[i].literals.push_back(temp);
			File >> temp;
		}
		/*
		for (int j = 0; j < phi[i].size(); ++j) {
			cout << phi[i][j] << ' ';
		}
		cout << endl;
		*/
		i++;
		if (i == numOfClauses) {
			break;
		}
	}

	File.close();
    
	return Formula(numOfvar, numOfClauses, phi);
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
	for (auto& it: phi.variables) {
		cout << it.second.letter << "(" << it.second.value << ") ";
	}
	cout << endl << phi.variables.size();
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
    int assignmentResult = phi.assignVariable(branchVar.literal,branchVar.value,level,vector<Variable>());
	cout << "got here" << endl;
    if (assignmentResult == CONFLICT){
        return CONFLICT;
    }
    vector<Variable> vars = vector<Variable>();
    vars.push_back(branchVar);
	phi.printFormula();
    while(!phi.allVariablesAssigned() || vars.size() > 0){
        //we need assign the
        Variable var = vars.front();
		cout << "Considering variable: " << var.letter << endl;
        vars.erase(vars.begin(), vars.begin()+1);
        
        vector<int> varClauses = phi.clausesIndexes[var.literal];
		

		cout << endl;
        for (unsigned int i = 0; i < varClauses.size(); i++) {
			cout << "Considering clause: "<< phi.formula[varClauses[i]].literals.size()<<endl;
			for (int j = 0; j < phi.formula[varClauses[i]].literals.size(); j++) {
				cout << phi.formula[varClauses[i]].literals[j] << " ";
			}
			cout << endl;
            //Go through each clause and check if we can infer a variable or not
            ImplicationAnalysis result = phi.setInferredVariable(varClauses[i]);
            if (result.target.literal != 0){
                //
                int implicationAssignmentResult = phi.assignVariable(result.target.literal, result.target.value, level, result.parents);
				cout << "even got here" << endl;
                if(implicationAssignmentResult == CONFLICT){
					cout << "honestly even got here" << endl;
                    return CONFLICT;
                }
                vars.push_back(result.target);
				cout << "Inferred var: " << result.target.literal << endl;
            }
        }
		
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
	vector<int> conflictingNodeIndeces = phi.implicationGraph.variableIndex[phi.implicationGraph.ConflictingLiteralId];
	cout << "conflictingNodeIndeces: ";
	for (int i = 0; i < conflictingNodeIndeces.size(); i++) {
		cout << conflictingNodeIndeces[i] << " ";
	}
	cout << endl;
	vector<int> conflictingNodeIndecesPos;
	vector<int> conflictingNodeIndecesNeg;
	for (unsigned int i = 0; i < conflictingNodeIndeces.size(); i++) {
		if (phi.implicationGraph.nodes[conflictingNodeIndeces[i]].value == 1)
			conflictingNodeIndecesPos.push_back(conflictingNodeIndeces[i]);
		else
			conflictingNodeIndecesNeg.push_back(conflictingNodeIndeces[i]);
	}
	cout << "conflictingNodeIndecesPos: ";
	for (int i = 0; i < conflictingNodeIndecesPos.size(); i++) {
		cout << conflictingNodeIndecesPos[i] << " ";
	}
	cout << endl;
	cout << "conflictingNodeIndecesNeg: ";
	for (int i = 0; i < conflictingNodeIndecesNeg.size(); i++) {
		cout << conflictingNodeIndecesNeg[i] << " ";
	}
	cout << endl;
	//Go through all the combinations of pairs of conflicted nodes
	//For each pair concatenate the parents of the nodes to form a new clause
	//Add the learned clause to the original formula
	int lowestLevelParentNodeId;
	for (unsigned int i = 0; i < conflictingNodeIndecesPos.size(); i++) {
		for (unsigned int j = 0; j < conflictingNodeIndecesNeg.size(); j++) {
			vector<int> clause;
			list<int> parentPos = phi.implicationGraph.nodes[conflictingNodeIndecesPos[i]].parentNodes;
			list<int> parentNeg = phi.implicationGraph.nodes[conflictingNodeIndecesNeg[j]].parentNodes;
			for (auto const& parent : parentPos) {
				int parentId = phi.implicationGraph.nodes[parent].id;
				int parentLiteralId = phi.implicationGraph.nodes[parent].literalId;
				int parentValue = phi.implicationGraph.nodes[parent].value;
				int parentLevel = phi.implicationGraph.nodes[parent].value;
				cout << "parentLiteralId: " << parentLiteralId << endl;
				if (parentValue && (find(clause.begin(), clause.end(), parentLiteralId) == clause.end()))
					clause.push_back(parentLiteralId);
				else if (!parentValue && (find(clause.begin(), clause.end(), -parentLiteralId) == clause.end()))
					clause.push_back(-parentLiteralId);

				//Overwrite the parent with the lowest level found
				if (level > parentLevel) {
					level = parentLevel;
					lowestLevelParentNodeId = parentId;
				}
			}
			for (auto const& parent : parentNeg) {
				int parentId = phi.implicationGraph.nodes[parent].id;
				int parentLiteralId = phi.implicationGraph.nodes[parent].literalId;
				int parentValue = phi.implicationGraph.nodes[parent].value;
				int parentLevel = phi.implicationGraph.nodes[parent].value;
				cout << "parentLiteralId: " << parentLiteralId << endl;
				if (parentValue && (find(clause.begin(), clause.end(), parentLiteralId) == clause.end()))
					clause.push_back(parentLiteralId);
				else if (!parentValue && (find(clause.begin(), clause.end(), -parentLiteralId) == clause.end()))
					clause.push_back(-parentLiteralId);

				//Overwrite the parent with the lowest level found
				if (level > parentLevel) {
					level = parentLevel;
					lowestLevelParentNodeId = parentId;
				}
			}
			phi.formula.push_back(Clause(clause));
			phi.bumpActivities(clause);
			cout << "Learned clause: ";
			for (int i = 0; i < clause.size(); i++) {
				cout << clause[i] << " ";
			}
			cout << endl;
		}
	}

	level = phi.implicationGraph.backtrackToLowestLevelParent(lowestLevelParentNodeId, level);
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
	int initialSize = phi.implicationGraph.levelIndex.size() - 1;
	for (int i = initialSize; i >= beta; i--) {
		vector<int> levelIndexList = phi.implicationGraph.levelIndex[i];
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
	while (!phi.allVariablesAssigned()) {
		Variable branchVar = PickBranchingVariable(phi);
		cout << "***********************************************" << endl;
        cout << "Picking a branching variable: " << branchVar.letter << " value: " << branchVar.value << endl;

		dl++;
		if (UnitPropagation(phi, branchVar,dl) == CONFLICT) {
            cout << "CONFLICT WAS DETECTED" << endl;
			int beta = ConflictAnalysis(phi, dl);
			if (beta < 0) {
				return UNSAT;
			}
			else {
				Backtrack(phi, beta);
				dl = beta;
			}
		}
		phi.printFormula();
		phi.printIndex();
	}
	return SAT;
}

