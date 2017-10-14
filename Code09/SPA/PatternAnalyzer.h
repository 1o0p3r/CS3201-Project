#include <QueryAnalyzer.h>
#pragma once
class PatternAnalyzer
{

protected:
	string patSyn;
	string patSynEnt;
	string arg1;
	string arg1Type;
	string patExp;
	string patExpType;

public:
	PatternAnalyzer(QueryElement patternClause, PKB pkbRedOnly);
	
	virtual tuple<bool, vector<vector<string>>> solvePattern();
};

