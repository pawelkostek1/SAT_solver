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
void printFormula(Formula phi);
void printAnswer(int ans);
int UnitPropagation(Formula &phi);
int AllVariablesAssigned(Formula &phi);
assignemnt PickBranchingVariable(Formula &phi);
int ConflictAnalysis(Formula &phi);
int Backtrack(Formula &phi, int &beta);
int CDCL(Formula phi);
int DPLL(Formula phi, int decision, int level);


int main() {

	//Read the encoded Enstein's puzzle into variable phi
	Formula phi = LoadFormula();
	printFormula(phi);

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
			a = stoi(line.substr(6, 1));
			b = stoi(line.substr(8, 1));
			break;
		}
	}

	//Read the rest of the file containing encoded formula
	phi = new vector<int>[b];
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
		if (i == b) {
			break;
		}
	}

	File.close();
}

/******************************************************
* @description: Prints the formula
*
* @params: phi(Formula) -
*/
void printFormula(Formula phi) {
	cout << "Number of variables is " << a << ".\nNumber of clauses is " << b << "." << endl  << endl;
	map<int, char> variables;
	for (int i = 1; i <= a; i++) {
		variables[i] = 'A' + i - 1;
	}
	for (int i = 0; i < b; i++) {
		cout << "Clause " << i + 1 << ": ";
		for (int j = 0; j < phi[i].size(); j++) {
			if (phi[i][j] < 0) {
				cout << "NOT " << variables[-phi[i][j]];
			}
			else {
				cout << variables[phi[i][j]];
			}
			if (j < phi[i].size() - 1) {
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
int UnitPropagation(Formula &phi) {

	//If phi is is en empty clause return SAT
	//If phi is square return CONFLICT

	//Construct a graph --- do we want to create it every time a new assignement is added? Could we resuse it partially?
	int n = v.size() + 1;//No. of vertices of the graph
	Graph g(n);
	for (int i = 0; i < v.size(); i++) {
		g.addEdge(v[i].x);
	}
	//Look for pairwise clause where there is particular variable and its negation
	for (int i = 0; i < v.size(); i++) {
		v[i] -> x;
		v[i] -> val;
		for (int j = 0; j < phi->size(); j++) {
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
assignemnt PickBranchingVariable(Formula &phi) {
	//TODO
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
	if (UnitPropagation(phi, v) == CONFLICT) {
		return UNSAT;
	}
	int dl = 0;
	while (!AllVariablesAssigned(phi, v)) {
		assignemnt t = PickBranchingVariable(phi, v);
		dl++;
		v.push_back(t);
		if (UnitPropagation(phi, v) == CONFLICT) {
			int beta = ConflictAnalysis(phi, v); //Where do I declare beta? Here?
			if (beta < 0) {
				return UNSAT;
			}
			else {
				Backtrack(phi, v, beta);
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
	if (phi->size() == 0) {
		return SAT;
	}
	//TODO
}