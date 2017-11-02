#pragma once

#include "Abstract_QA_API.h"
#include "TupleHash.h"
#include "PKB.h"
#include "QueryElement.h"
#include "Util.h"
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>
#include <tuple>
#include <unordered_set>
#include <assert.h>


using namespace std;

class suchThatAnalyzer
{
protected:
	vector<vector<vector<string>>> queryTable;
	unordered_map<string, tuple<int, int>> queryMap;
	PKB pkbReadOnly;
	string stEntity;

	string arg1;
	string arg1Type;
	string arg1Entity;

	string arg2;
	string arg2Type;
	string arg2Entity;
	bool hasSuchThatClause;
	
	const string WILDCARD_SYMBOL = "_";
	const string PROCEDURE = "procedure";
	const string PROCEDUREARG = "variable";
	const string WILDCARD = "wildcard";

	bool checkClauseTrue(string arg1, string arg2, int scenarioArg1Arg2);
	tuple<bool,vector<vector<string>>> addClauseTable(string arg1, string arg2, int scenario);
	virtual tuple<bool, vector<vector<string>>> addArgTwoResult(string arg1);
	virtual tuple<bool, vector<vector<string>>> addArgOneResult(string arg2);
	virtual tuple<bool, vector<vector<string>>> addBothSynResult(string arg1, string arg2);
	
	virtual bool checkClauseBothVariables(string arg1, string arg2);
	virtual bool checkClauseVariableWild(string arg1);
	virtual bool checkClauseWildVariable(string arg2);
	virtual bool checkClauseBothWild();

	virtual void suchThatAnalyzer::getValuesFromPKB(vector<int>& retrievedPKBValues, bool hasArg2EvalBefore, int candidates);
	void suchThatAnalyzer::getArgsPriorResults(vector<int>& vecOfCandidates, bool& hasArg2EvalBefore,
	                                           const unordered_map<string, tuple<int, int>>::iterator synArg1Iterator, const unordered_map<string, tuple<int, int>>::
	                                           iterator synArg2Iterator);
	vector<string> optimizedAddArg(const unordered_map<string, tuple<int, int>>::iterator synArgIterator, bool isAddArg1);
	virtual bool hasResultsForArg(const int candidates, const bool isAddArg1);

	vector<int> validatePKBResultsInt(string ent, vector<int> validateVec);
	vector<string> validatePKBResultsString(string ent, vector<string> validateVec);
	vector<string> removeDuplicates(vector<string> clauseResult);

	virtual vector<string> getPKBAllArgValues();

public:
	virtual ~suchThatAnalyzer() = default;
	suchThatAnalyzer(QueryElement suchThatClause, PKB pkb, vector<vector<vector<string>>> const &qTable, 
			unordered_map<string, tuple<int, int>> const &qMap);
	tuple<bool, vector<vector<string>>> solveClause();
	tuple<bool,vector<vector<string>>> solveClauseStmt();
	

	int inputHardCodeIndex;
	bool unitTestModeOn;
	vector<vector<string>> unitTestInputs;
	void setUnitTestInputs(vector<vector<string>> hcInput);
	vector<string> getUnitTestInputs();
	
};

