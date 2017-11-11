#pragma once
#include "QaConstants.h"

#include <unordered_map>
#include <tuple>
#include <vector>
#include "QueryElement.h"


class AnalyzerFactory
{
public:

	//tuple<bool, vector<vector<string>>> clauseResult;

	AnalyzerFactory();
	tuple<bool, vector<vector<string>>> getResults(QueryElement clause, string clauseRel);

private:
	unordered_map<string, int> clauseTypeMap;

	void initClauseTypeMap();
};

