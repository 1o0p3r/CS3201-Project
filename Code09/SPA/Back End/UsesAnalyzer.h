#pragma once
#include "SuchThatAnalyzer.h"
class UsesAnalyzer :
	public suchThatAnalyzer
{
public:
	UsesAnalyzer(QueryElement suchThatClause, PKB pkbReadOnly, vector<vector<vector<string>>> const &qTable,
		unordered_map<string, tuple<int, int>> const &qMap) :
		suchThatAnalyzer(suchThatClause, pkbReadOnly, qTable, qMap)
	{
	}

protected:
	tuple<bool, vector<vector<string>>> addArgTwoResult(string arg1) override;
	tuple<bool, vector<vector<string>>> addArgOneResult(string arg2) override;
	tuple<bool, vector<vector<string>>> addBothSynResult(string arg1, string arg2) override;

	bool checkClauseBothVariables(string arg1, string arg2) override;
	bool checkClauseVariableWild(string arg1) override;
	bool checkClauseWildVariable(string arg2) override;
	bool checkClauseBothWild() override;

private:
	vector<string> getUsesResultAddArg2(string arg1, string arg2Entity);
	vector<string> getUsesResultAddArg1(string arg2, string arg1Entity);

};

