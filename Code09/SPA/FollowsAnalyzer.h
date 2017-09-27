#pragma once

#include "QueryElement.h"
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>

using namespace std;
class FollowsAnalyzer
{
public:
	QueryElement follows;

	FollowsAnalyzer(QueryElement followClause);
	vector<vector<string>> solveFollows();

private:
	

};

