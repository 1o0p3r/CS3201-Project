#include "ParentStarAnalyzer.h"

const string WILDCARD = "wildcard";

tuple<bool, vector<vector<string>>> ParentStarAnalyzer::addArgTwoResult(string arg1)
{
	return tuple<bool, vector<vector<string>>>();
}

tuple<bool, vector<vector<string>>> ParentStarAnalyzer::addArgOneResult(string arg2)
{
	return tuple<bool, vector<vector<string>>>();
}

tuple<bool, vector<vector<string>>> ParentStarAnalyzer::addBothSynResult(string arg1, string arg2)
{
	return tuple<bool, vector<vector<string>>>();
}

bool ParentStarAnalyzer::checkClauseBothVariables(string arg1, string arg2)
{
	return false;
}

bool ParentStarAnalyzer::checkClauseVariableWild(string arg1)
{
	return false;
}

bool ParentStarAnalyzer::checkClauseWildVariable(string arg2)
{
	return false;
}

bool ParentStarAnalyzer::checkClauseBothWild()
{	
	int MIN_STMTS_FOR_PARENT = 1;
	return (pkbReadOnly.getIf().size() < MIN_STMTS_FOR_PARENT &&
			pkbReadOnly.getWhile().size() < MIN_STMTS_FOR_PARENT) ? false : true;
}
