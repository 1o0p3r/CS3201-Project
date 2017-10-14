#include "PatternAnalyzer.h"

PatternAnalyzer::PatternAnalyzer(QueryElement patternClause, PKB pkbRedOnly) {
	patSyn = patternClause.getPatternSynonym();
	patSynEnt = patternClause.getPatternEntity();
	arg1 = patternClause.getPatternArg1();
	arg1Type = patternClause.getPatternArg1Type();
	patExp = patternClause.getPatternArg2();
	patExpType = patternClause.getPatternArg2Type();
}

tuple<bool, vector<vector<string>>> PatternAnalyzer::solvePattern()
{
	tuple<vector<string>, vector<string>> evaluatedPatResult;
	vector<string> validatedPatResult;
	vector<vector<string>> patternAssignResult;
}

