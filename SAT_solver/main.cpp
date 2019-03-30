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
	Formula phi = LoadFormula();

	printAnswer(phi, SAT);
	
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
    if (assignmentResult == CONFLICT){
        return CONFLICT;
    }
    vector<Variable> vars = vector<Variable>();
    vars.push_back(branchVar);

    while(!phi.allVariablesAssigned() and vars.size() > 0){
        //we need assign the
        Variable var = vars.front();
        vars.erase(vars.begin(), vars.begin()+1);
        
        vector<int> varClauses = phi.clausesIndexes[var.literal];
        for (unsigned int i = 0; i < varClauses.size(); i++) {
            //Go through each clause and check if we can infer a variable or not
            ImplicationAnalysis result = phi.setInferredVariable(varClauses[i]);
            if (result.target.literal != 0){
                //
                int implicationAssignmentResult = phi.assignVariable(result.target.literal, result.target.value, level, result.parents);
                if(implicationAssignmentResult == CONFLICT){
                    return CONFLICT;
                }
                vars.push_back(result.target);
            }
        }
    }
    return NOCONFLICT;
}

/******************************************************
* @description:
*
* @params phi(Formula) -
* @ret:
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
	vector<int> conflictingNodeIndecesPos;
	vector<int> conflictingNodeIndecesNeg;
	for (int i = 0; i < conflictingNodeIndeces.size(); i++) {
		if (phi.implicationGraph.nodes[conflictingNodeIndeces[i]].value == 1)
			conflictingNodeIndecesPos.push_back(conflictingNodeIndeces[i]);
		else
			conflictingNodeIndecesNeg.push_back(conflictingNodeIndeces[i]);
	}
	//Go through all the combinations of pairs of conflicted nodes
	//For each pair concatenate the parents of the nodes to form a new clause
	//Add the learned clause to the original formula
	//!!!Currently not checking for reapeted literals
	int lowestLevelParentNodeId;
	for (int i = 0; i < conflictingNodeIndecesPos.size(); i++) {
		for (int j = 0; j < conflictingNodeIndecesNeg.size(); j++) {
			vector<int> clause;
			list<int> parentPos = phi.implicationGraph.nodes[conflictingNodeIndecesPos[i]].parentNodes;
			list<int> parentNeg = phi.implicationGraph.nodes[conflictingNodeIndecesNeg[j]].parentNodes;
			for (auto const& parent : parentPos) {
				int parentId = phi.implicationGraph.nodes[parent].id;
				int parentLiteralId = phi.implicationGraph.nodes[parent].literalId;
				int parentValue = phi.implicationGraph.nodes[parent].value;
				int parentLevel = phi.implicationGraph.nodes[parent].value;
				if (parentValue)
					clause.push_back(parentLiteralId);
				else
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
				if (parentValue)
					clause.push_back(parentLiteralId);
				else
					clause.push_back(-parentLiteralId);

				//Overwrite the parent with the lowest level found
				if (level > parentLevel) {
					level = parentLevel;
					lowestLevelParentNodeId = parentId;
				}
			}
			phi.formula.push_back(Clause(clause));
		}
	}

	level = phi.implicationGraph.backtrackToLowestLevelParent(lowestLevelParentNodeId, dl);
	return level - 1; //return one level below of the corresponding lowest level root node
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
	for (int i = initialSize; i > beta; i--) {
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

