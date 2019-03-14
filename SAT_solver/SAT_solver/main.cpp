#include<iostream>
#include<fstream>
#include<list>
#include<string>
#include<sstream>
#include<vector>
#include<map>

using namespace std;

//Define constants
#define CONFLICT -1
#define UNSAT 0
#define SAT 1 

//Define variables
/*
 * @description: structure defining an assignement of the value to a given literal
 */
struct assignemnt {
	int x; //literal
	int val; //can be 0 or 1
};

//Intially read the data into array of vectors, we may want to consider implementing our custom object for stroing formula...
typedef vector<int> * formula;
/*
* @description: data structure containing the formula

class formula {
public:
	//Constructor
	formula() {
	}

	//Destructor
	~formula() {
	}

private:

};
*/

//Declare functions
void LoadFormula(int &a, int &b, formula &phi);
void printFormula(int a, int b, formula phi);
void printAnswer(int ans, list<assignemnt> v);
int UnitPropagation(formula &phi, list<assignemnt> &v);
int AllVariablesAssigned(formula &phi, list<assignemnt> &v);
assignemnt PickBranchingVariable(formula &phi, list<assignemnt> &v);
int ConflictAnalysis(formula &phi, list<assignemnt> &v);
int Backtrack(formula &phi, list<assignemnt> &v, int &beta);
int CDCL(formula phi, list<assignemnt> v);
int DPLL(formula phi, int decision, int level);


int main() {

	//Define variables
	int a, b;
	formula phi;
	list<assignemnt> v;
	
	//Read the encoded Enstein's puzzle
	LoadFormula(a, b, phi);
	printFormula(a, b, phi);

	//Solve the puzzle
	//int ans = CDCL(phi, v);

	//Print the answer
	//printAnswer(ans, v);


	while (1);
	return 0;
}

/******************************************************
* @description: Loads the formula
*
* @params: a(int) -
* @params: b(int) -
* @params: phi(formula) -
*/
void LoadFormula(int &a, int &b, formula &phi) {
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
* @params: a(int) -
* @params: b(int) -
* @params: phi(formula) -
*/
void printFormula(int a, int b, formula phi) {
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
* @params: v(assignment) -
*/
void printAnswer(int ans, list<assignemnt> v) {
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
* @params: phi(formula) -
* @params: v(assignment) -
* @ret:
*/
int UnitPropagation(formula &phi, list<assignemnt> &v) {
	//Construct a graph --- do we want to create it every time a new assignement is added? Could we resuse it partially?

	//Look for pairwise clause where there is particular variable and its negation
	for (int i = 0; i < v.size(); i++) {
		v[i]->x;
		v[i]->val;
	}
}

/******************************************************
* @description:
*
* @params: phi(formula) -
* @params: v(assignment) -
* @ret:
*/
int AllVariablesAssigned(formula &phi, list<assignemnt> &v) {
	//TODO
}

/******************************************************
* @description:
*
* @params: phi(formula) -
* @params: v(assignment) -
* @ret:
*/
assignemnt PickBranchingVariable(formula &phi, list<assignemnt> &v) {
	//TODO
}

/******************************************************
* @description:
*
* @params: phi(formula) -
* @params: v(assignment) -
* @ret:
*/
int ConflictAnalysis(formula &phi, list<assignemnt> &v) {
	//TODO
}

/******************************************************
 * @description:  
 *
 * @params: phi(formula) - 
 * @params: v(assignment) -
 * @ret: 
 */
int Backtrack(formula &phi, list<assignemnt> &v, int &beta) {
	//TODO
}

/******************************************************
* @description:
*
* @params: phi(formula) -
* @params: v(assignment) -
* @ret:

* @!!!: Is beta and dt int type?
*/
int CDCL(formula phi, list<assignemnt> v) {
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
* @params: phi(formula) -
* @params: decision(decision) -
* @params: level(int) -
* @ret:

* @!!!: Is beta and dt int type?
*/
int DPLL(formula phi, int decision, int level) {
	if (phi->size() == 0) {
		return SAT;
	}
	//TODO
}