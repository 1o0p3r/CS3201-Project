#include <QueryAnalyzer.h>
#include <Abstract_QA_API.h>

#pragma once
class PatternAnalyzer
{

protected:

	PKB pkbPtr;

	string patSyn;
	string patSynEnt;
	string arg1;
	string arg1Type;
	string patExp;
	string patExpType;

	static void initMapPatternExpType();
	bool hasPatternClause;
	
	tuple<vector<string>,vector<string>> solvePatSynAssign(string arg1, string patExp, string patExpType, string patSyn);
	vector<string> validatedPatSynAssign(string arg1, string patExp, string patExpType, string patSyn);
	vector<int> getPKBPatternIfWhile(string candidates);
	void getPatSynIfWhile(vector<string> candidateList, vector<string> *entityVector, 
			vector<string> *variableVector, bool *hasPattern);
	tuple<vector<string>, vector<string>> solvePatSynIfWhile(string arg1, string patSyn, string patSynEnt);
	vector<string> validatedSynIfWhile(string arg1, string patSyn, string patSynEnt);

		
public:
	
	PatternAnalyzer(QueryElement patternClause, PKB &pkbRedOnly);
	tuple<bool, vector<vector<string>>> solvePatternAssign();
	tuple<bool, vector<vector<string>>> solvePatternIfWhile();
};

