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
using namespace std;



//Declare functions
Formula LoadFormula();
void printAnswer(int ans);
int UnitPropagation(Formula &phi, tuple<int, int> branchVar);
tuple<int,int> PickBranchingVariable(Formula phi);
int ConflictAnalysis(Formula &phi);
void Backtrack(Formula &phi, int &beta);
int CDCL(Formula phi);
//int DPLL(Formula phi, int decision, int level);


int main() {

	//Read the encoded Enstein's puzzle into variable phi
	Formula phi = LoadFormula();
    phi.printFormula();
    
	//phi.assignVariable(2,1,false);
	
	//Solve the puzzle
	int ans = CDCL(phi);

	//Print the answer
	printAnswer(ans);

	while (1);
	return 0;
}

/******************************************************
* @description: Loads the formula
*
* @paramsphi: phi(Formula) -
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
	vector<vector<int> > phi = vector<vector<int> >();
	int i = 0;
	while (File) {
		int temp = 1;
		File >> temp;
        phi.push_back(vector<int>());
		while (temp) {
			phi[i].push_back(temp);
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
 * @paramsans ans(int) -
*/
void printAnswer(int ans) {
	cout << "The solver produced following output: ";
	if (ans == SAT) {
		cout << "SAT" << endl;
	}
	else if (ans == UNSAT) {
		cout << "UNSAT" << endl;
	}
	//TODO
}

/******************************************************
* @description:
*
* @paramsphi: phi(Formula) -
* @ret:
*/

int UnitPropagation(Formula &phi, tuple<int,int> branchVar) {
    //Start by assigning the branchVar to the assignedVariables in phi
	if (get<0>(branchVar) == 0) { //Base case
		//if we end up in here it means this is the first time we run this function
        cout << "Lets start by removing all single literal clauses" << endl;
        return phi.removeSingleLiteralVariables();
	}
	else { //there exist some assigned variables
        phi.assignVariable(get<0>(branchVar), get<1>(branchVar),false);
        vector<int> assignementClauses = phi.clausesIndexes[get<0>(branchVar)];
        list<tuple<int,int>> inferedList = list<tuple<int,int>>();
		for (unsigned int i = 0; i < assignementClauses.size(); i++) {
            //Go through each clause and check if we can infer a variable or not
			
			tuple<int,int> inferedVar = phi.getInfered(assignementClauses[i]);
            if (get<0>(inferedVar) != 0){
                //this method will assign the variable to the assignmend hasmap as well as update the
                //implication graph
                phi.assignVariable(get<0>(inferedVar), get<1>(inferedVar),true); 
            }
			
			
		}
	}
    return CONFLICT;
}

/******************************************************
* @description:
*
 * @paramsphi phi(Formula) -
* @ret:
*/
tuple<int,int> PickBranchingVariable(Formula phi) {

    //int absLiteral = *next(phi.unassignedIndex.begin(), rand() % phi.unassignedIndex.size());
    //return tuple<int,int>(absLiteral,rand() % 1);

	//Use Variable State Independent Decaying Sum (VSIDS) to pick branching varible
	return tuple<int, int>(0, 0);

}

/******************************************************
* @description:
*
 * @paramsphi phi(Formula) -
* @ret:
*/
int ConflictAnalysis(Formula &phi) {
	
	int level = 0;

	//Perform decay of the activities
	phi.decayActivities();

	//Learn the clause
	/*
		You use the graph to go back to all the closest parents of the conflict, this gives you a learned clause.
		You add the clause to the formula and then you use the graph to find the level that corresponds to the earliest root 
		that resulted in the conflict.

		We also need to consider the case where the conflict can no longer be resolved, in which case we return UNSAT.
	*/
	
    return level;
}

/******************************************************
 * @description:
 *
 * @paramsphi: phi(Formula) -
 * @ret:
 */
void Backtrack(Formula &phi, int &beta) {
	/* phi stores a map of levels and asignments that were done at a this level.
	   Using beta that is the level we want to back track to we use the aformentioned data structure to 
	   unassign all the variables that were set up until the given level beta.
	*/
}

/******************************************************
* @description:
*
 * @paramsphi phi(Formula) -
* @ret:

* @!!!: Is beta and dt int type?
*/
int CDCL(Formula phi) {
    int dl = 0;
	if (UnitPropagation(phi,tuple<int,int>(0,-1)) == CONFLICT) {
		return UNSAT;
	}
	while (!phi.allVariablesAssigned()) {
        phi.printFormula();
		tuple<int,int> branchVar = PickBranchingVariable(phi);
        cout << "Picking A Branching variable: " << get<0>(branchVar) << " value: " << get<1>(branchVar) << endl;
		dl++;
		if (UnitPropagation(phi, branchVar) == CONFLICT) {
			int beta = ConflictAnalysis(phi); //Where do I declare beta? Here?
			if (beta < 0) {
				return UNSAT;
			}
			else {
				Backtrack(phi, beta);
				dl = beta;
			}
		}
	}
	return SAT;
}

/******************************************************
* @description:
*
* @params: phi(Formula) -
* @params: decision(decision) -
* @params: level(int) -
* @ret:

* @!!!: Is beta and dt int type?
*/
/*int DPLL(Formula phi, int decision, int level) {
	//if (phi.F.size() == 0) {
	//	return SAT;
	//}
	//TODO
}*/
