#include "FollowsStarAnalyzer.h"

bool FollowsStarAnalyzer::hasResultsForArg(const int candidates, const bool isAddArg1) {
	if (isAddArg1)
		return pkbReadOnly.getFollowsStar(candidates).empty() ? false : true;
	else
		return pkbReadOnly.getFollowedByStar(candidates).empty() ? false : true;
}

tuple<bool, vector<vector<string>>> FollowsStarAnalyzer::addArgTwoResult(string arg1)
{
	bool hasFollowsStar = true;
	vector<int> vecOfCandidates;
	vector<string> pkbResult;
	vector<int> pkbFollowsStar;
	vector<vector<string>> followsStarResult;

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
			pkbFollowsStar = pkbReadOnly.getFollowsStar(candidates);
			pkbFollowsStar = validatePKBResultsInt(arg2Entity, pkbFollowsStar);
			for (int candidatesChosen : pkbFollowsStar) {
				pkbResult.push_back(to_string(candidatesChosen));
			}
		}
	}
	if (pkbResult.empty())
		hasFollowsStar = false;
	else {
		pkbResult = removeDuplicates(pkbResult);
		pkbResult.push_back(arg2); //to denote this vector belongs to indicated synonym 
		followsStarResult.push_back(pkbResult);
	}

	return make_tuple(hasFollowsStar, followsStarResult);
}

tuple<bool, vector<vector<string>>> FollowsStarAnalyzer::addArgOneResult(string arg2)
{
	bool hasFollowsStar = true;
	vector<int> vecOfCandidates;
	vector<int> pkbFollowsStar;
	vector<string> pkbResult;
	vector<vector<string>> followsStarResult;

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
			pkbFollowsStar = pkbReadOnly.getFollowedByStar(candidates);
			pkbFollowsStar = validatePKBResultsInt(arg1Entity, pkbFollowsStar);
			for (int candidatesChosen : pkbFollowsStar) {
				pkbResult.push_back(to_string(candidatesChosen));
			}
		}
	}
	if (pkbResult.empty())
		hasFollowsStar = false;
	else {
		pkbResult = removeDuplicates(pkbResult);
		pkbResult.push_back(arg1); //to denote this vector belongs to indicated synonym 
		followsStarResult.push_back(pkbResult);
	}

	return make_tuple(hasFollowsStar, followsStarResult);
}

void FollowsStarAnalyzer::getValuesFromPKB(vector<int>& retrievedPKBValues, bool hasArg2EvalBefore, int candidates)
{
	if (!hasArg2EvalBefore) {
		retrievedPKBValues = pkbReadOnly.getFollowsStar(candidates);
		retrievedPKBValues = validatePKBResultsInt(arg2Entity, retrievedPKBValues);
	}
	else {
		retrievedPKBValues = pkbReadOnly.getFollowedByStar(candidates);
		retrievedPKBValues = validatePKBResultsInt(arg1Entity, retrievedPKBValues);
	}
}

tuple<bool, vector<vector<string>>> FollowsStarAnalyzer::addBothSynResult(string arg1, string arg2)
{
	bool hasFollowsStar = true;
	vector<int> vecOfCandidates;
	vector<int> retrievedPKBResults;
	vector<string> pkbResultForArg1;
	vector<string> pkbResultForArg2;
	vector<vector<string>> followsStarResult;

	bool hasArg2EvalBefore = false;
	const auto synArg1Iterator = queryMap.find(arg1);
	const auto synArg2Iterator = queryMap.find(arg2);

	if (synArg1Iterator == queryMap.end() && synArg2Iterator == queryMap.end()) {
	vecOfCandidates = pkbReadOnly.getAllStmt();
	vecOfCandidates = validatePKBResultsInt(arg1Entity, vecOfCandidates);
	} else
		getArgsPriorResults(vecOfCandidates, hasArg2EvalBefore, synArg1Iterator, synArg2Iterator);
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
		hasFollowsStar = false;
	else {
		pkbResultForArg1.push_back(arg1);
		pkbResultForArg2.push_back(arg2); //to denote this vector belongs to indicated synonym 
		followsStarResult.push_back(pkbResultForArg1);
		followsStarResult.push_back(pkbResultForArg2);
	}

	return make_tuple(hasFollowsStar, followsStarResult);
}

bool FollowsStarAnalyzer::checkClauseBothVariables(string arg1, string arg2)
{
	auto pkbResult = pkbReadOnly.getFollowsStar(stoi(arg1));
	return find(pkbResult.begin(), pkbResult.end(), stoi(arg2)) == pkbResult.end() ?
		false : true;
}

bool FollowsStarAnalyzer::checkClauseVariableWild(string arg1)
{
	return pkbReadOnly.getFollowsStar(stoi(arg1)).empty() ? false : true;
}

bool FollowsStarAnalyzer::checkClauseWildVariable(string arg2)
{
	return pkbReadOnly.getFollowedByStar(stoi(arg2)).empty() ? false : true;
}

bool FollowsStarAnalyzer::checkClauseBothWild()
{
	int minNoAssignForFollows = 2;
	vector<int> allAssign = pkbReadOnly.getAssign();
	if (allAssign.size() < minNoAssignForFollows)
		hasSuchThatClause = false;
	return hasSuchThatClause;
}
