#pragma once
#include "SuchThatAnalyzer.h"
class CallsAnalyzer :
	public suchThatAnalyzer
{
public:
	CallsAnalyzer(QueryElement suchThatClause, PKB pkbReadOnly, vector<vector<vector<string>>> const &qTable,
		unordered_map<string, tuple<int, int>> const &qMap) :
		suchThatAnalyzer(suchThatClause, pkbReadOnly, qTable, qMap)
	{
	}

protected:
	tuple<bool, vector<vector<string>>> addArgTwoResult(string arg1) override;
	tuple<bool, vector<vector<string>>> addArgOneResult(string arg2) override;
	tuple<bool, vector<vector<string>>> addBothSynResult(string arg1, string arg2) override;

	bool hasResultsForArg(string candidates, bool isAddArg1) override;
	void getValuesFromPKB(vector<string>& pkbParent, bool hasArg2EvalBefore, string candidates) override;

	bool checkClauseBothVariables(string arg1, string arg2) override;
	bool checkClauseVariableWild(string arg1) override;
	bool checkClauseWildVariable(string arg2) override;
	bool checkClauseBothWild() override;

	vector<string> getPKBAllArgValues() override;
};
