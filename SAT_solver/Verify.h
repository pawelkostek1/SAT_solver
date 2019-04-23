#pragma once
#include"Formula.h"
class Verify
{

    bool testGraph();
	bool test2WatchedLiterals();
    bool testGraphAddNode();

public:
    Formula formula;
	Verify();
	~Verify();
};

