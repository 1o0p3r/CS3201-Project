#pragma once
#include "QueryElement.h"
#include <vector>
#include <unordered_map>
#include <queue>
#include "Abstract_QA_API.h"
#include "PKB.h"
#include <functional>

using namespace std;

struct relRanker {
	string relation; int relationTableSize;
	bool operator < (const relRanker& compareTo) const {
		return relationTableSize < compareTo.relationTableSize;
	}
};


class QueryOptimizer
{
public:
	QueryOptimizer(vector<QueryElement>, vector<QueryElement>, 
			multimap<string, pair<int, int>>, multimap<string, pair<int, int>>, const PKB &);


	
	void runOptimizer();

	static int getRankOfClauseSynonymCount(QueryElement &clause);
	static int getRankOfRelationClause(QueryElement &clause);

private:
	PKB pkbptr;
	
	void setRankOfRelationClause();
	void setRankOfDesignEntity();
	void setFreqOfSyn();

	vector<QueryElement> normalClauses;
	vector<QueryElement> hardClauses;
	multimap<string, pair<int, int>> normalMMap;
	multimap<string, pair<int, int>> hardMMap;

	unordered_map<string, int> relRankMap;
	unordered_map<string, int> designRankMap;
	unordered_map<string, int> synFreqMap;

	bool myCompare(const QueryElement& clause1, const QueryElement& clause2);
	bool compareRel(tuple<int, string> clause1, tuple<int, string> clause2);

	priority_queue<QueryElement, vector<QueryElement>, less<vector<QueryElement>::value_type>> normalQueue;
	priority_queue<QueryElement, vector<QueryElement>, less<vector<QueryElement>::value_type>> hardQueue;
};


