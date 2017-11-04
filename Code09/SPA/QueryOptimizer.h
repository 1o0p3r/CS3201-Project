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
	string relation; 
	int relationTableSize;
	bool operator < (const relRanker& compareTo) const {
		return relationTableSize < compareTo.relationTableSize;
	}
};

struct clauseRanker {
	QueryElement clause; 
	int relationRank;
	int numSynonymRank;
	int designEntityRank;
	int synonymFreqRank;

	bool operator < (const clauseRanker& compareTo) const {
		if (numSynonymRank != compareTo.numSynonymRank) return numSynonymRank < compareTo.numSynonymRank;
		if(relationRank != compareTo.relationRank) return relationRank < compareTo.relationRank;
		if (designEntityRank != compareTo.designEntityRank) return designEntityRank < compareTo.designEntityRank;
		if (synonymFreqRank != compareTo.synonymFreqRank) return compareTo.synonymFreqRank < synonymFreqRank;
		return true;
	}
};


class QueryOptimizer
{
public:
	QueryOptimizer(vector<QueryElement>, vector<QueryElement>, 
			multimap<string, pair<int, int>>, multimap<string, pair<int, int>>, const PKB &);


	
	tuple<vector<QueryElement>, vector<QueryElement>> runOptimizer();

	
	

private:
	PKB pkbptr;
	vector<QueryElement> sortedNormal;
	vector<QueryElement> sortedHard;
	
	void setRankOfRelationClause();
	void setRankOfDesignEntity();
	void setFreqOfSyn();
	void runClauseRanking();


	int getRankOfClauseSynonymCount(QueryElement &clause);
	string getClauseFirstSynonym(QueryElement& clause);
	string getClauseSecondSynonym(QueryElement& clause);
	int getRankOfSynonymFreq(QueryElement& clause);
	int getRankOfRelation(QueryElement &clause);
	string getClauseFirstDesignEntity(QueryElement& clause);
	string getClauseSecondDesignEntity(QueryElement &clause);
	int getDesignEntityRank(QueryElement &clause);

	vector<QueryElement> normalClauses;
	vector<QueryElement> hardClauses;
	multimap<string, pair<int, int>> normalMMap;
	multimap<string, pair<int, int>> hardMMap;

	unordered_map<string, int> relationRankMap;
	unordered_map<string, int> designEntRankMap;
	unordered_map<string, int> synFreqMap;


};


