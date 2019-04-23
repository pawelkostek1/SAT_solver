#include "Verify.h"
#include "Formula.h"
#include<vector>
#include"Clause.h"

Verify::Verify()
{
    
    
    
}


Verify::~Verify()
{
    
}

Formula Verify::createFormula(){
    vector<Clause> clauses = vector<Clause>();
    unordered_map<int,Variable> variables = unordered_map<int,Variable>();
    variables[1] = Variable(1,-1);
    variables[2] = Variable(2,-1);
    variables[3] = Variable(3,-1);
    variables[4] = Variable(4,-1);
    variables[5] = Variable(5,-1);
    variables[6] = Variable(6,-1);
    cout << "Created variables" << endl;
    clauses.push_back(Clause({1,2,6}));
    clauses.push_back(Clause({-1,-3,5}));
    clauses.push_back(Clause({6,-5}));
    clauses.push_back(Clause({4,5,6}));
    clauses.push_back(Clause({-6,-1,-3,2}));
    clauses.push_back(Clause({1,6}));
    clauses.push_back(Clause({3,1,-6}));
    cout << "Created clauses" << endl;
    
    return Formula(clauses, variables);
}
bool Verify::testGraphAddNode(){
    Formula formula = createFormula();
    Graph graph = formula.implicationGraph;
    graph.addNode(1, 0, 1, {});
    graph.addNode(2, 0, 1, {1});
    graph.addNode(3, 1, 0, {});
   
    graph.addNode(4, 1, 0, {-5,2,-3});
    graph.addNode(5, 2, 0, {});
    graph.addNode(6, 2, 0, {1,-4,-5});
    graph.printGraph();
    return false;
}


bool Verify::testUnitPropagation(){
    Formula formula = createFormula();
    
    return false;
}





bool Verify::test2WatchedLiterals() {
    Formula formula = createFormula();
	struct state {
		int literal;
		vector<int> negativeClauses;
		vector<int> postiveClauses;
	};
	cout << "Checking the initial state" << endl;
	state initialState[6];
	initialState[0].literal = 1;
	initialState[0].negativeClauses = vector<int>{ 1,4 };
	initialState[0].postiveClauses = vector<int>{ 0,5,6 };
	initialState[1].literal = 2;
	initialState[1].negativeClauses = vector<int>();
	initialState[1].postiveClauses = vector<int>{ 0 };
	initialState[2].literal = 3;
	initialState[2].negativeClauses = vector<int>{ 1 };
	initialState[2].postiveClauses = vector<int>{ 6 };
	initialState[3].literal = 4;
	initialState[3].negativeClauses = vector<int>();
	initialState[3].postiveClauses = vector<int>{ 3 };
	initialState[4].literal = 5;
	initialState[4].negativeClauses = vector<int>{ 2 };
	initialState[4].postiveClauses = vector<int>{ 3 };
	initialState[5].literal = 6;
	initialState[5].negativeClauses = vector<int>{ 4 };
	initialState[5].postiveClauses = vector<int>{ 2,5 };
	
	formula.print2Watched(list<int>{1,2,3,4,5,6});

	bool result = true;
	for (auto it : formula.variables) {
		for (int i = 0; i < initialState[it.first - 1].negativeClauses.size(); i++) {
			cout << initialState[it.first - 1].negativeClauses[i] << " " << it.second.negativeClauses[i] << endl;
			//if(initialState[it.first - 1].negativeClauses[i] != it.second.negativeClauses[i])
				//result = false;
		}
		for (int i = 0; i < initialState[it.first - 1].postiveClauses.size(); i++) {
			cout << initialState[it.first - 1].postiveClauses[i] << " " << it.second.postiveClauses[i] << endl;
			//if (initialState[it.first - 1].postiveClauses[i] != it.second.postiveClauses[i])
				//result = false;
		}
		if (initialState[it.first - 1].negativeClauses == it.second.negativeClauses && initialState[it.first - 1].postiveClauses == it.second.postiveClauses)
			continue;
		else
			result = false;
	}
	if (result)
		cout << "Sccesfully passed on the initial state test case!" << endl;
	else
		cout << "Failed at the initial state test case." << endl;

	return result;
}
