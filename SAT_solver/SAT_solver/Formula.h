#pragma once
#include<vector>
#include<list>
using namespace std;

//Define variables
/*
* @description: structure defining an assignement of the value to a given literal
*/
struct assignemnt {
	int x; //literal
	int val; //can be 0 or 1
};

//Intially read the data into array of vectors, we may want to consider implementing our custom object for stroing formula...Formula
class Formula {
	vector<int> * F;
	int numOfvar;
	int numOfClauses;
	list<assignemnt> v;
public:
	Formula(int _numOfvar, int _numOfClauses);
	~Formula();
	int getNumOfVar();
	int getNumOfClauses();
	list<assignemnt> getListOfAssignements();
	void getListOfUnassignVar();


};