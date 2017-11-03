#include "NextStarAnalyzer.h"

bool NextStarAnalyzer::hasResultsForArg(const int candidates, const bool isAddArg1) {
	if (isAddArg1)
		return pkbReadOnly.getNextStar(candidates).empty() ? false : true;
	else
		return pkbReadOnly.getPreviousStar(candidates).empty() ? false : true;
}

tuple<bool, vector<vector<string>>> NextAnalyzer::addArgTwoResult(string arg1)
{
	bool hasNext = true;
	vector<int> vecOfCandidates;
	vector<string> pkbResult;
	vector<int> pkbNext;
	vector<vector<string>> NextResult;
	
	const auto synArg2Iterator = queryMap.find(arg2);

	if (synArg2Iterator != queryMap.end() && arg1 == WILDCARD_SYMBOL) {
		bool isAddArg1 = false;
		pkbResult = optimizedAddArg(synArg2Iterator, isAddArg1);
	}
	else {
		if (arg1 == WILDCARD_SYMBOL) 
			vecOfCandidates = pkbReadOnly.getAllNext();
		else
			vecOfCandidates.push_back(stoi(arg1));
		for (int candidates : vecOfCandidates) {
			pkbNext = pkbReadOnly.getNext(candidates);
			pkbNext = validatePKBResultsInt(arg2Entity, pkbNext);
			for (int candidatesChosen : pkbNext) {
				pkbResult.push_back(to_string(candidatesChosen));
			}
		}
	}
	if (pkbResult.empty())
		hasNext = false;
	else {
		pkbResult = removeDuplicates(pkbResult);
		pkbResult.push_back(arg2); //to denote this vector belongs to indicated synonym 
		NextResult.push_back(pkbResult);
	}

	return make_tuple(hasNext, NextResult);
}

tuple<bool, vector<vector<string>>> NextAnalyzer::addArgOneResult(string arg2)
{
	bool hasNext = true;
	vector<int> vecOfCandidates;
	vector<int> pkbPrevious;
	vector<string> pkbResult;
	vector<vector<string>> NextResult;

	const auto synArg1Iterator = queryMap.find(arg1);

	if (synArg1Iterator != queryMap.end() && arg2 == WILDCARD_SYMBOL) {
		bool isAddArg1 = true;
		pkbResult = optimizedAddArg(synArg1Iterator, isAddArg1);
	}
	else {
		if (arg2 == WILDCARD_SYMBOL) // to be optimized
			vecOfCandidates = pkbReadOnly.getAllStmt();
		else
			vecOfCandidates.push_back(stoi(arg2));
		for (int candidates : vecOfCandidates) {
			pkbPrevious = pkbReadOnly.getPrevious(candidates);
			pkbPrevious = validatePKBResultsInt(arg1Entity, pkbPrevious);
			for (int candidatesChosen : pkbPrevious) {
				pkbResult.push_back(to_string(candidatesChosen));
			}
		}
	}
	if (pkbResult.empty())
		hasNext = false;
	else {
		pkbResult = removeDuplicates(pkbResult);
		pkbResult.push_back(arg1); //to denote this vector belongs to indicated synonym 
		NextResult.push_back(pkbResult);
	}

	return make_tuple(hasNext, NextResult);
}

tuple<bool, vector<vector<string>>> NextAnalyzer::addBothSynResult(string arg1, string arg2)
{
	bool hasNext = true;
	vector<int> vecOfCandidates;
	vector<int> retrievedPKBResults;
	vector<string> pkbResultForArg1;
	vector<string> pkbResultForArg2;
	vector<vector<string>> NextResult;

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
		hasNext = false;
	else {
		pkbResultForArg1.push_back(arg1);
		pkbResultForArg2.push_back(arg2); //to denote this vector belongs to indicated synonym 
		NextResult.push_back(pkbResultForArg1);
		NextResult.push_back(pkbResultForArg2);
	}

	return make_tuple(hasNext, NextResult);
}

bool NextAnalyzer::checkClauseBothVariables(string arg1, string arg2)
{
	auto pkbResult = pkbReadOnly.getNext(stoi(arg1));
	return find(pkbResult.begin(), pkbResult.end(), stoi(arg2)) == pkbResult.end() ?
		false : true;
}

bool NextAnalyzer::checkClauseVariableWild(string arg1)
{
	return pkbReadOnly.getNext(stoi(arg1)).empty() ? false : true;
}

bool NextAnalyzer::checkClauseWildVariable(string arg2)
{
	return pkbReadOnly.getPrevious(stoi(arg2)).empty() ? false : true;
}

bool NextAnalyzer::checkClauseBothWild()
{
	int MIN_STMTS_FOR_Next = 1;
	return (pkbReadOnly.getAllNext().size() < MIN_STMTS_FOR_Next) ? false : true;
}
