#include "FollowsAnalyzer.h"

bool FollowsAnalyzer::hasResultsForArg(const int candidates, const bool isAddArg1) {
	if (isAddArg1)
		return pkbReadOnly.getFollows(candidates).empty() ? false : true;
	else
		return pkbReadOnly.getFollowedBy(candidates).empty() ? false : true;
}

tuple<bool, vector<vector<string>>> FollowsAnalyzer::addArgTwoResult(string arg1)
{
	bool hasFollows = true;
	vector<int> vecOfCandidates;
	vector<string> pkbResult;
	vector<int> pkbFollows;
	vector<vector<string>> followsResult;

	const auto synArg2Iterator = queryMap.find(arg2);

	if (synArg2Iterator != queryMap.end() && arg1 == WILDCARD_SYMBOL) {
		bool isAddArg1 = false;
		pkbResult = optimizedAddArg(synArg2Iterator, isAddArg1, true);
	}
	else {
		if (arg1 == WILDCARD_SYMBOL)
			vecOfCandidates = pkbReadOnly.getAllStmt();
		else
			vecOfCandidates.push_back(stoi(arg1));
		for (int candidates : vecOfCandidates) {
			pkbFollows = pkbReadOnly.getFollows(candidates);
			pkbFollows = validatePKBResultsInt(arg2Entity, pkbFollows);
			if (!pkbFollows.empty())
				pkbResult.push_back(to_string(pkbFollows.front()));
		}
	}
	if (pkbResult.empty())
		hasFollows = false;
	else {
		pkbResult.push_back(arg2); //to denote this vector belongs to indicated synonym 
		followsResult.push_back(pkbResult);
	}

	return make_tuple(hasFollows, followsResult);
}

tuple<bool, vector<vector<string>>> FollowsAnalyzer::addArgOneResult(string arg2)
{
	bool hasFollows = true;
	vector<int> vecOfCandidates;
	vector<int> pkbFollows;
	vector<string> pkbResult;
	vector<vector<string>> followsResult;
	
	const auto synArg1Iterator = queryMap.find(arg1);

	if (synArg1Iterator != queryMap.end() && arg2 == WILDCARD_SYMBOL) {
		bool isAddArg1 = true;
		pkbResult = optimizedAddArg(synArg1Iterator, isAddArg1, true);
	}
	else {
		if (arg2 == WILDCARD_SYMBOL)
			vecOfCandidates = pkbReadOnly.getAllStmt();
		else
			vecOfCandidates.push_back(stoi(arg2));
		for (int candidates : vecOfCandidates) {
			pkbFollows = pkbReadOnly.getFollowedBy(candidates);
			pkbFollows = validatePKBResultsInt(arg1Entity, pkbFollows);
			if (!pkbFollows.empty())
				pkbResult.push_back(to_string(pkbFollows.front()));
		}
	}
	if (pkbResult.empty())
		hasFollows = false;
	else {
		pkbResult.push_back(arg1); //to denote this vector belongs to indicated synonym 
		followsResult.push_back(pkbResult);
	}

	return make_tuple(hasFollows, followsResult);

}

void FollowsAnalyzer::getValuesFromPKB(vector<int>& retrievedPKBValues, bool hasArg2EvalBefore, int candidates)
{
	if (!hasArg2EvalBefore) {
		retrievedPKBValues = pkbReadOnly.getFollows(candidates);
		retrievedPKBValues = validatePKBResultsInt(arg2Entity, retrievedPKBValues);
	}
	else {
		retrievedPKBValues = pkbReadOnly.getFollowedBy(candidates);
		retrievedPKBValues = validatePKBResultsInt(arg1Entity, retrievedPKBValues);
	}
}

tuple<bool, vector<vector<string>>> FollowsAnalyzer::addBothSynResult(string arg1, string arg2)
{
	bool hasFollows = true;
	vector<int> vecOfCandidates;
	vector<int> retrievedPKBResults;
	vector<string> pkbResultForArg1;
	vector<string> pkbResultForArg2;
	vector<vector<string>> followsResult;

	bool hasArg2EvalBefore = false;
	const auto synArg1Iterator = queryMap.find(arg1);
	const auto synArg2Iterator = queryMap.find(arg2);

	if (synArg1Iterator == queryMap.end() && synArg2Iterator == queryMap.end()) {
		vecOfCandidates = pkbReadOnly.getAllStmt();
		vecOfCandidates = validatePKBResultsInt(arg1Entity, vecOfCandidates);
	} else {
		getArgsPriorResults(vecOfCandidates, hasArg2EvalBefore, synArg1Iterator, synArg2Iterator);
	}
	for (int candidates : vecOfCandidates) {
		getValuesFromPKB(retrievedPKBResults, hasArg2EvalBefore, candidates);
		for (int candidatesChosen : retrievedPKBResults) {
			if (!hasArg2EvalBefore) {
				pkbResultForArg1.push_back(to_string(candidates));
				pkbResultForArg2.push_back(to_string(candidatesChosen));
			}
			else {
				pkbResultForArg1.push_back(to_string(candidatesChosen));
				pkbResultForArg2.push_back(to_string(candidates));
			}
		}
	}
	if (pkbResultForArg1.empty())
		hasFollows = false;
	else {
		pkbResultForArg1.push_back(arg1);
		pkbResultForArg2.push_back(arg2); //to denote this vector belongs to indicated synonym 
		followsResult.push_back(pkbResultForArg1);
		followsResult.push_back(pkbResultForArg2);
	}

	return make_tuple(hasFollows, followsResult);
}

bool FollowsAnalyzer::checkClauseBothVariables(string arg1, string arg2)
{
	auto pkbResult = pkbReadOnly.getFollows(stoi(arg1));
	return find(pkbResult.begin(), pkbResult.end(), stoi(arg2)) == pkbResult.end() ?
		false : true;
}

bool FollowsAnalyzer::checkClauseVariableWild(string arg1)
{
	return pkbReadOnly.getFollows(stoi(arg1)).empty() ? false : true;
}

bool FollowsAnalyzer::checkClauseWildVariable(string arg2)
{
	return pkbReadOnly.getFollowedBy(stoi(arg2)).empty() ? false : true;
}

bool FollowsAnalyzer::checkClauseBothWild()
{	
	int minNoAssignForFollows = 2;
	vector<int> allAssign = pkbReadOnly.getAssign();
	if(allAssign.size() < minNoAssignForFollows)
		hasSuchThatClause = false;
	return hasSuchThatClause;
}
