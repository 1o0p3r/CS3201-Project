#pragma once


#include "PKB.h"
#include "QueryElement.h"
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>
#include <tuple>
#include <unordered_set>

using namespace std;

class suchThatAnalyzer
{
protected:
	PKB pkbReadOnly;
	string stEntity;

	string arg1;
	string arg1Type;
	string arg1Entity;

	string arg2;
	string arg2Type;
	string arg2Entity;

	bool hasSuchThatClause;
	
	bool checkClauseTrue(string arg1, string arg2, int scenarioArg1Arg2);
	tuple<bool,vector<vector<string>>> addClauseTable(string arg1, string arg2, int scenario);
	tuple<bool, vector<vector<string>>> addArgTwoResult(string arg1);
	tuple<bool, vector<vector<string>>> addArgOneResult(string arg2);
	tuple<bool, vector<vector<string>>> addBothSynResult(string arg1, string arg2);
	
	bool checkClauseBothVariables(string arg1, string arg2);
	bool checkClauseVariableWild(string arg1);
	bool checkClauseWildVariable(string arg2);
	bool checkClauseBothWild();

public:
	suchThatAnalyzer(QueryElement suchThatClause, PKB pkb);
	tuple<bool,vector<vector<string>>> solveClause();
};

