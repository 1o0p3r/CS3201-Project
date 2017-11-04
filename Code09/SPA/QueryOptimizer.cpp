#include "QueryOptimizer.h"




QueryOptimizer::QueryOptimizer(const vector<QueryElement> normalElement, const vector<QueryElement> hardElement,
		const multimap<string, pair<int, int>> normalMap, const multimap<string, pair<int, int>> hardMap, const PKB &pkb) {
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
	relRanker follows, followsT, modifies, uses, parent, parentT, next,
			patternA, calls, callsT, patternI, patternW;
	
	follows.relation = "Follows";
	follows.relationTableSize = pkbptr.getFollowsCount();
	pqRel.push_back(follows);
	
	followsT.relation = "Follows*";
	followsT.relationTableSize = pkbptr.getFollowStarCount();
	pqRel.push_back(followsT);
	
	modifies.relation = "Modifies";
	modifies.relationTableSize = pkbptr.getModifyCount() + pkbptr.getProcModifyCount();;
	pqRel.push_back(modifies);

	uses.relation = "Uses";
	uses.relationTableSize = pkbptr.getUseCount();
	pqRel.push_back(uses);

	parent.relation = "Parent";
	parent.relationTableSize = pkbptr.getParentCount();
	pqRel.push_back(parent);

	parentT.relation = "Parent*";
	parentT.relationTableSize = pkbptr.getParentStarCount();
	pqRel.push_back(parentT);
	
	next.relation = "Next";
	next.relationTableSize = pkbptr.getNextCount();
	pqRel.push_back(next);
	
	patternA.relation = "pattern";
	patternA.relationTableSize = pkbptr.getAssign().empty() ? 0: pkbptr.getAssign().size() 
								+ pkbptr.getIf().empty() ? 0: pkbptr.getIf().size() 
								+ pkbptr.getWhile().empty() ? 0:pkbptr.getWhile().size();
	pqRel.push_back(patternA);
	
	calls.relation = "Calls";
	calls.relationTableSize = pkbptr.getCall().empty() ? 0: pkbptr.getCall().size();
	pqRel.push_back(calls);

	callsT.relation = "Calls*";
	callsT.relationTableSize = pkbptr.getCall().empty() ? 0 : pkbptr.getCall().size();
	pqRel.push_back(callsT);

	sort(pqRel.begin(), pqRel.end());
	int i = 1;
	for(const auto candidate: pqRel) {
		relationRankMap.insert(make_pair(candidate.relation, i));
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
		designEntRankMap.insert(make_pair(candidate.relation, j));
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

void QueryOptimizer::runClauseRanking() {
	vector<clauseRanker> normalClauseRankVector;
	vector<clauseRanker> hardClauseRankVector;
	
	for (auto &candidate : normalClauses) {
		clauseRanker queryObj;
		queryObj.relationRank = getRankOfRelation(candidate);
		queryObj.designEntityRank = getDesignEntityRank(candidate);
		queryObj.numSynonymRank = getRankOfClauseSynonymCount(candidate);
		queryObj.synonymFreqRank = getRankOfSynonymFreq(candidate);
		normalClauseRankVector.push_back(queryObj);
	}

	for (auto &candidate : hardClauses) {
		clauseRanker queryObj;
		queryObj.relationRank = getRankOfRelation(candidate);
		queryObj.designEntityRank = getDesignEntityRank(candidate);
		queryObj.numSynonymRank = getRankOfClauseSynonymCount(candidate);
		queryObj.synonymFreqRank = getRankOfSynonymFreq(candidate);
		hardClauseRankVector.push_back(queryObj);
	}

	sort(normalClauseRankVector.begin(), normalClauseRankVector.end());
	sort(hardClauseRankVector.begin(), hardClauseRankVector.end());

	for(const auto &candidate: normalClauseRankVector) {
		sortedNormal.push_back(candidate.clause);
	}

	for (const auto &candidate : hardClauseRankVector) {
		sortedHard.push_back(candidate.clause);
	}
	
}

string QueryOptimizer::getClauseFirstSynonym(QueryElement& clause) {
	if (!clause.getPatternSynonym().empty()) return clause.getPatternSynonym();
	if (!clause.getSuchThatArg1().empty()) return clause.getSuchThatArg1();
	return EMPTY_STRING;
}

string QueryOptimizer::getClauseSecondSynonym(QueryElement& clause) {
	if (!clause.getPatternArg1().empty()) return clause.getPatternArg1();
	if (!clause.getSuchThatArg2().empty()) return clause.getSuchThatArg2();
	return EMPTY_STRING;
}

int QueryOptimizer::getRankOfSynonymFreq(QueryElement& clause) {
	string firstSynonym = getClauseFirstSynonym(clause);
	string secondSynonym = getClauseSecondSynonym(clause);
	int rankOfFirstSynonym;
	int rankOfSecondSynonym;

	unordered_map<string, int>::iterator iter = synFreqMap.find(firstSynonym);
	if (!firstSynonym.empty() && iter != synFreqMap.end())  rankOfFirstSynonym = iter->second;
	else rankOfFirstSynonym = LOWEST_RANK; //for wildcards

	iter = synFreqMap.find(secondSynonym);
	if (!secondSynonym.empty() && iter != synFreqMap.end())  rankOfSecondSynonym = iter->second;
	else rankOfSecondSynonym = LOWEST_RANK; //for wildcards
	return rankOfFirstSynonym + rankOfSecondSynonym;

}

int QueryOptimizer::getRankOfRelation(QueryElement &clause) {
	string relation = clause.getSuchThatRel();
	unordered_map<string, int>::iterator iter = relationRankMap.find(relation);
	if (!relation.empty() && iter != relationRankMap.end()) return iter->second;
	else {
		relation = clause.getClauseType();
		iter = relationRankMap.find(relation);
		if (iter != relationRankMap.end()) return iter->second;
		return INT_MAX;
	}
}

string QueryOptimizer::getClauseFirstDesignEntity(QueryElement& clause) {
	if (!clause.getPatternEntity().empty()) return clause.getPatternEntity();
	if (!clause.getSuchThatArg1Entity().empty()) return clause.getSuchThatArg1Entity();
	return EMPTY_STRING;
}

string QueryOptimizer::getClauseSecondDesignEntity(QueryElement& clause) {
	if (!clause.getPatternArg1Ent().empty()) return clause.getPatternArg1Ent();
	if (!clause.getSuchThatArg2Entity().empty()) return clause.getSuchThatArg2Entity();
	return EMPTY_STRING;
}

int QueryOptimizer::getDesignEntityRank(QueryElement& clause) {
	string firstDesignEntity = getClauseFirstDesignEntity(clause);
	string secondDesignEntity = getClauseSecondDesignEntity(clause);
	int rankOfFirstDesignEntity;
	int rankOfSecondDesignEntity;

	unordered_map<string, int>::iterator iter = designEntRankMap.find(firstDesignEntity);
	if (!firstDesignEntity.empty() && iter != designEntRankMap.end()) rankOfFirstDesignEntity = iter->second;
	else rankOfFirstDesignEntity = LOWEST_RANK; //for wildcards

	iter = designEntRankMap.find(secondDesignEntity);
	if (!secondDesignEntity.empty() && iter != designEntRankMap.end()) rankOfSecondDesignEntity = iter->second;
	else rankOfSecondDesignEntity = LOWEST_RANK; //for wildcards
	return rankOfFirstDesignEntity * rankOfSecondDesignEntity;
	
}

tuple<vector<QueryElement>,vector<QueryElement>> QueryOptimizer::runOptimizer() {
	
	setRankOfRelationClause();
	setRankOfDesignEntity();
	setFreqOfSyn();
	runClauseRanking();
	return make_tuple(sortedNormal, sortedHard);

}






