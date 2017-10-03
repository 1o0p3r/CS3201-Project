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
	virtual tuple<bool, vector<vector<string>>> addArgTwoResult(string arg1);
	virtual tuple<bool, vector<vector<string>>> addArgOneResult(string arg2);
	virtual tuple<bool, vector<vector<string>>> addBothSynResult(string arg1, string arg2);
	
	virtual bool checkClauseBothVariables(string arg1, string arg2);
	virtual bool checkClauseVariableWild(string arg1);
	virtual bool checkClauseWildVariable(string arg2);
	virtual bool checkClauseBothWild();

	vector<string> removeDuplicates(vector<string> clauseResult);

public:
	suchThatAnalyzer(QueryElement suchThatClause, PKB pkb);
	tuple<bool, vector<vector<string>>> solveClause();
	tuple<bool, vector<vector<string>>> solveClauseProc();
	tuple<bool,vector<vector<string>>> solveClauseStmt();
};

