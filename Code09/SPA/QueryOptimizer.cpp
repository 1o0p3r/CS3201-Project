#include "QueryOptimizer.h"




QueryOptimizer::QueryOptimizer(vector<QueryElement> normalElement, vector<QueryElement> hardElement,
		multimap<string, pair<int, int>> normalMap, multimap<string, pair<int, int>> hardMap, const PKB &pkb) {
	normalClauses = normalElement;
	hardClauses = hardElement;
	normalMMap = normalMap;
	hardMMap = hardMap;
	pkbptr = pkb;

	
}

int QueryOptimizer::getRankOfClauseSynonymCount(QueryElement &clause) {
	int count = 0;
	if (clause.getPatternArg1Type() == SYNONYM) { count++; }
	if (clause.getPatternSynonym() == SYNONYM) { count++; }
	if (clause.getSuchThatArg1Type() == SYNONYM) { count++; }
	if (clause.getSuchThatArg2Type() == SYNONYM) { count++; }
	
	if (count == 2) { return 3; }
	if (count == 1) { return 2; }
	return 1;
}



void QueryOptimizer::setRankOfRelationClause() {
	vector<relRanker> pqRel;
	relRanker follows, followsT, modifies,modifiesProc, usesProc, uses, parent, parentT, next,
			patternA, calls, callsT, patternI, patternW;
	
	follows.relation = "follows";
	follows.relationTableSize = pkbptr.getFollowsCount();
	pqRel.push_back(follows);
	
	followsT.relation = "followsT";
	followsT.relationTableSize = pkbptr.getFollowStarCount();
	pqRel.push_back(followsT);
	
	modifies.relation = "modifies";
	modifies.relationTableSize = pkbptr.getModifyCount();
	pqRel.push_back(modifies);

	modifiesProc.relation = "modifiesProc";
	modifiesProc.relationTableSize = pkbptr.getProcModifyCount();
	pqRel.push_back(modifiesProc);

	usesProc.relation = "usesProc";
	usesProc.relationTableSize = pkbptr.getProcUseCount();
	pqRel.push_back(usesProc);

	uses.relation = "uses";
	uses.relationTableSize = pkbptr.getUseCount();
	pqRel.push_back(uses);

	parent.relation = "parent";
	parent.relationTableSize = pkbptr.getParentCount();
	pqRel.push_back(parent);

	parentT.relation = "parentT";
	parentT.relationTableSize = pkbptr.getParentStarCount();
	pqRel.push_back(parentT);
	
	next.relation = "next";
	next.relationTableSize = pkbptr.getNextCount();
	pqRel.push_back(next);
	
	patternA.relation = "patternA";
	patternA.relationTableSize = pkbptr.getAssign().size();
	pqRel.push_back(patternA);

	patternI.relation = "patternI";
	patternI.relationTableSize = pkbptr.getIf().size();
	pqRel.push_back(patternI);

	patternW.relation = "patternW";
	patternW.relationTableSize = pkbptr.getWhile().size();
	pqRel.push_back(patternW);

	calls.relation = "calls";
	calls.relationTableSize = pkbptr.getCall().size();
	pqRel.push_back(calls);

	callsT.relation = "callsT";
	callsT.relationTableSize = pkbptr.getCall().size();
	pqRel.push_back(callsT);

	sort(pqRel.begin(), pqRel.end());
	int i = 1;
	for(const auto candidate: pqRel) {
		relRankMap.insert(make_pair(candidate.relation, i));
		i++;
	}
	
}

void QueryOptimizer::setRankOfDesignEntity()
{
	vector<string> designEntity= { "stmt","assign","while","variable","constant","prog_line","if","procedure","call","stmtLst"};
	vector<unsigned int> designEntitySize = { pkbptr.getAllStmt().size(), pkbptr.getAssign().size(), pkbptr.getWhile().size(), 
		pkbptr.getAllVariables().size(), pkbptr.getAllConstants().size(),pkbptr.getAllStmt().size(),pkbptr.getIf().size(),pkbptr.getAllProcedures().size(),
		pkbptr.getAllCalls().size(), pkbptr.getStatementList().size() };
	vector<relRanker> designRank;

	int i = 0;
	for (const auto de : designEntity) {
		relRanker candidate;
		candidate.relation = de;
		candidate.relationTableSize = designEntitySize[i];
		designRank.push_back(candidate);
		i++;
	}
	sort(designRank.begin(), designRank.end());

	int j = 1;
	for (const auto candidate : designRank) {
		designRankMap.insert(make_pair(candidate.relation, i));
		j++;
	}
}

struct sort_Freq {
	bool operator()(const tuple<string, int> &left, const tuple<string, int> &right) {
		return get<1>(left) < get<1>(right);
	}
};

void QueryOptimizer::setFreqOfSyn() {
	vector<tuple<string, int>> synCount;
	for (auto iter = normalMMap.begin(); iter != normalMMap.end();) {
		string key = iter->first;
		int freqCount = 0;
		do {
			freqCount++;
			iter++;
		} while (iter != normalMMap.end() && key == iter->first);
		synCount.push_back(make_tuple(key, freqCount));
	}
	sort(synCount.begin(), synCount.end(), sort_Freq());

	int j = 1;
	for (const auto candidate : synCount) {
		synFreqMap.insert(make_pair(get<0>(candidate), j));
		j++;
	}

}

void QueryOptimizer::runOptimizer() {
	
	setRankOfRelationClause();
	setRankOfDesignEntity();
	setFreqOfSyn();
}


int QueryOptimizer::getRankOfRelationClause(QueryElement &clause) {
	return 1;
}

bool QueryOptimizer::compareRel(tuple<int, string> clause1, tuple<int, string> clause2)
{
}


bool myCompare(QueryElement clause1, QueryElement clause2) {
	int countSynRankClause1 = QueryOptimizer::getRankOfClauseSynonymCount(clause1);
	int countSynRankClause2 = QueryOptimizer::getRankOfClauseSynonymCount(clause2);

	return true;
}

bool compareRel(tuple<int,string>, tuple<int,string> ) {
	
	
	return true;
}


