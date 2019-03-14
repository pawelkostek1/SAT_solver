#include<iostream>
#include<fstream>
#include<list>
#include<string>
#include<sstream>
#include<vector>
#include<map>
#include"Formula.h"
#include"Graph.h"

using namespace std;

//Define constants
#define CONFLICT -1
#define UNSAT 0
#define SAT 1


//Declare functions
Formula LoadFormula();
void printFormula(Formula &phi);
void printAnswer(int ans);
int UnitPropagation(Formula &phi, int literal);
int AllVariablesAssigned(Formula &phi);
int PickBranchingVariable(Formula &phi);
int ConflictAnalysis(Formula &phi);
int Backtrack(Formula &phi, int &beta);
int CDCL(Formula phi);
int DPLL(Formula phi, int decision, int level);


int main() {

	//Read the encoded Enstein's puzzle into variable phi
	Formula phi = LoadFormula();
	printFormula(phi);
    phi.assignVariable(2,1);
	//Solve the puzzle
	//int ans = CDCL(phi);

	//Print the answer
	//printAnswer(ans);

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
	int numOfvar, numOfClauses;
	ifstream File;
	File.open("puzzle.cnf");
	if (!File) {
		cout << "Unable to open file puzzle.txt";
	}
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
	vector<int> *phi = new vector<int>[numOfClauses];
	int i = 0;
	while (File) {
		int temp = 1;
		File >> temp;
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
* @description: Prints the formula
*
* @params: phi(Formula) -
*/
void printFormula(Formula &phi) {
	cout << "Number of variables is " << phi.getNumOfVar() << ".\nNumber of clauses is " << phi.getNumOfClauses() << "." << endl  << endl;
	map<int, char> variables;
	for (int i = 1; i <= phi.getNumOfVar(); i++) {
		variables[i] = 'A' + i - 1;
	}
	for (int i = 0; i < phi.getNumOfClauses(); i++) {
		cout << "Clause " << i + 1 << ": ";
		for (unsigned int j = 0; j < phi.formula[i].size(); j++) {
			if (phi.formula[i][j] < 0) {
				cout << "NOT " << variables[-phi.formula[i][j]];
			}
			else {
				cout << variables[phi.formula[i][j]];
			}
			if (j < phi.formula[i].size() - 1) {
				cout << " OR ";
			}
		}
		cout << endl;
	}
	cout << endl;
}

/******************************************************
* @description:
*
* @params: ans(int) -
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
* @params: phi(Formula) -
* @ret:
*/
int UnitPropagation(Formula &phi, int literal) {

	if (phi.assignedIndex.size() == 0) { //Base case
		return SAT;
	}
	else { //there exist some assigned variables
		vector<int> assignementClauses = phi.clausesIndexes[literal];
		for (int i = 0; i < assignementClauses.size(); i++) {
			
			phi.assignInfered(assignementClauses[i]);
			//Need to consider case where you cannot infere anything
		}
	}
}

/******************************************************
* @description:
*
* @params: phi(Formula) -
* @ret:
*/
int AllVariablesAssigned(Formula &phi) {
	//TODO
}

/******************************************************
* @description:
*
* @params: phi(Formula) -
* @ret:
*/
int PickBranchingVariable(Formula &phi) {
	//TODO
	int literal, value;
	phi.assignVariable(literal, value);
}

/******************************************************
* @description:
*
* @params: phi(Formula) -
* @ret:
*/
int ConflictAnalysis(Formula &phi) {
	//TODO
}

/******************************************************
 * @description:
 *
 * @params: phi(Formula) -
 * @ret:
 */
int Backtrack(Formula &phi, int &beta) {
	//TODO
}

/******************************************************
* @description:
*
* @params: phi(Formula) -
* @ret:

* @!!!: Is beta and dt int type?
*/
int CDCL(Formula phi) {
	if (UnitPropagation(phi) == CONFLICT) {
		return UNSAT;
	}
	int dl = 0;
	while (!AllVariablesAssigned(phi)) {
		int literal = PickBranchingVariable(phi);
		dl++;
		if (UnitPropagation(phi, literal) == CONFLICT) {
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
int DPLL(Formula phi, int decision, int level) {
	//if (phi.F.size() == 0) {
	//	return SAT;
	//}
	//TODO
}
