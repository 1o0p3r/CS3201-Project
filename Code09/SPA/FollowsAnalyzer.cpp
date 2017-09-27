#include "FollowsAnalyzer.h"

tuple<bool, vector<vector<string>>> FollowsAnalyzer::addArgTwoResult(string arg1)
{
	return tuple<bool, vector<vector<string>>>();
}

tuple<bool, vector<vector<string>>> FollowsAnalyzer::addArgOneResult(string arg2)
{
	return tuple<bool, vector<vector<string>>>();
}

tuple<bool, vector<vector<string>>> FollowsAnalyzer::addBothSynResult(string arg1, string arg2)
{
	return tuple<bool, vector<vector<string>>>();
}

bool FollowsAnalyzer::checkClauseBothVariables(string arg1, string arg2)
{
	return false;
}

bool FollowsAnalyzer::checkClauseVariableWild(string arg1)
{
	return false;
}

bool FollowsAnalyzer::checkClauseWildVariable(string arg2)
{
	return false;
}

bool FollowsAnalyzer::checkClauseBothWild()
{	
	int minNoAssignForFollows = 2;
	vector<int> allAssign = pkbReadOnly.getAssign();
	if(allAssign.size() < minNoAssignForFollows)
		hasSuchThatClause = false;
	return hasSuchThatClause;
}
