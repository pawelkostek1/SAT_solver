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
#include"Change.h"
#include"Clause.h"
using namespace std;



//Declare functions
Formula LoadFormula();
void printAnswer(int ans);
int UnitPropagation(Formula &phi, tuple<int, int> branchVar);
int AllVariablesAssigned(Formula &phi);
Variable PickBranchingVariable(Formula phi);
int ConflictAnalysis(Formula &phi);
int Backtrack(Formula &phi, int &beta);
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

int UnitPropagation(Formula &phi, Variable branchVar,int level) {
    //Start by assigning the branchVar to the assignedVariables in phi
	if (branchVar.literal == 0) { //Base case
		//if we end up in here it means this is the first time we run this function
        cout << "Lets start by removing all single literal clauses" << endl;
        return phi.removeSingleLiteralVariables();
	}
	else { //there exist some assigned variables
        int assignmentResult = phi.assignVariable(branchVar.literal,branchVar.value,level);
        if (assignmentResult == CONFLICT){
            return CONFLICT;
        }
        vector<int> literalClauses = phi.clausesIndexes[branchVar.literal];
        for (unsigned int i = 0; i < literalClauses.size(); i++) {
            //Go through each clause and check if we can infer a variable or not
            Variable inferredVar = phi.getInferred(literalClauses[i]);
                
            if (inferredVar.literal != 0){
                //this method will assign the variable to the assignmend hasmap as well as update the
                //implication graph
                int result = UnitPropagation(phi,inferredVar,level);
                if (result == CONFLICT){
                    return CONFLICT;
                }
            }
        }
    }
    return NOCONFLICT;
}

/******************************************************
* @description:
*
 * @paramsphi phi(Formula) -
* @ret:
*/
int AllVariablesAssigned(Formula &phi) {
	//TODO
    return 1;
}

/******************************************************
* @description:
*
 * @paramsphi phi(Formula) -
* @ret:
*/
Variable PickBranchingVariable(Formula phi) {
	//TODO
    int absLiteral = *next(phi.unassignedIndex.begin(), rand() % phi.unassignedIndex.size());
    return Variable(absLiteral,rand() % 2);
}

/******************************************************
* @description:
*
 * @paramsphi phi(Formula) -
* @ret:
*/
int ConflictAnalysis(Formula &phi) {
	//TODO
    return 1;
}

/******************************************************
 * @description:
 *
 * @paramsphi: phi(Formula) -
 * @ret:
 */
int Backtrack(Formula &phi, int &beta) {
	//TODO
    return 1;
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
	if (UnitPropagation(phi,Variable(0,-1),dl) == CONFLICT) {
		return UNSAT;
	}
	while (!phi.allVariablesAssigned()) {
        phi.printFormula();
		Variable branchVar = PickBranchingVariable(phi);
        cout << "Picking A Branching variable: " << branchVar.literal << " value: " << branchVar.value << endl;
		dl++;
		if (UnitPropagation(phi, branchVar,dl) == CONFLICT) {
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
