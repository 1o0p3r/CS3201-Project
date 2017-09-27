#include "FollowsAnalyzer.h"




FollowsAnalyzer::FollowsAnalyzer(QueryElement followsClause)
{
	follows = followsClause;
}

vector<vector<string>> FollowsAnalyzer::solveFollows()
{
	vector<vector<string>> answer;
	string arg1 = follows.getSuchThatArg1();
	string arg2 = follows.getSuchThatArg2();
	return answer;
}





