#include "NextStarAnalyzer.h"

bool NextStarAnalyzer::hasResultsForArg(const int candidates, const bool isAddArg1) {
	if (isAddArg1)
		return pkbReadOnly.getNextStarFirstLiteral(candidates).empty() ? false : true;
	else
		return pkbReadOnly.getNextStarSecondLiteral(candidates).empty() ? false : true;
}



tuple<bool, vector<vector<string>>> NextStarAnalyzer::addArgTwoResult(string arg1)
{
	bool hasNextStar = true;
	vector<int> vecOfCandidates;
	vector<string> pkbResult;
	vector<int> pkbNext;
	vector<vector<string>> NextStarResult;
	bool isArgTypeInt = true;

	const auto synArg2Iterator = queryMap.find(arg2);

	if (synArg2Iterator != queryMap.end() && arg1 == WILDCARD_SYMBOL) {
		bool isAddArg1 = false;
		pkbResult = optimizedAddArg(synArg2Iterator, isAddArg1, isArgTypeInt);
	}
	else {
		if (arg1 == WILDCARD_SYMBOL) 
			vecOfCandidates = pkbReadOnly.getAllNext();
		else
			vecOfCandidates.push_back(stoi(arg1));
		for (int candidates : vecOfCandidates) {
			pkbNext = pkbReadOnly.getNextStarFirstLiteral(candidates);
			pkbNext = validatePKBResultsInt(arg2Entity, pkbNext);
			for (int candidatesChosen : pkbNext) {
				pkbResult.push_back(to_string(candidatesChosen));
			}
		}
	}
	if (pkbResult.empty())
		hasNextStar = false;
	else {
		pkbResult = removeDuplicates(pkbResult);
		pkbResult.push_back(arg2); //to denote this vector belongs to indicated synonym 
		NextStarResult.push_back(pkbResult);
	}

	return make_tuple(hasNextStar, NextStarResult);
}

tuple<bool, vector<vector<string>>> NextStarAnalyzer::addArgOneResult(string arg2)
{
	bool hasNextStar = true;
	vector<int> vecOfCandidates;
	vector<int> pkbPrevious;
	vector<string> pkbResult;
	vector<vector<string>> NextStarResult;

	const auto synArg1Iterator = queryMap.find(arg1);

	if (synArg1Iterator != queryMap.end() && arg2 == WILDCARD_SYMBOL) {
		bool isAddArg1 = true;
		bool isArgTypeInt = true;
		pkbResult = optimizedAddArg(synArg1Iterator, isAddArg1, isArgTypeInt);
	}
	else {
		if (arg2 == WILDCARD_SYMBOL) // to be optimized
			return addBothSynResult(arg1, arg2);
		else
			vecOfCandidates.push_back(stoi(arg2));
		for (int candidates : vecOfCandidates) {
			pkbPrevious = pkbReadOnly.getNextStarSecondLiteral(candidates);
			pkbPrevious = validatePKBResultsInt(arg1Entity, pkbPrevious);
			for (int candidatesChosen : pkbPrevious) {
				pkbResult.push_back(to_string(candidatesChosen));
			}
		}
	}
	if (pkbResult.empty())
		hasNextStar = false;
	else {
		pkbResult = removeDuplicates(pkbResult);
		pkbResult.push_back(arg1); //to denote this vector belongs to indicated synonym 
		NextStarResult.push_back(pkbResult);
	}

	return make_tuple(hasNextStar, NextStarResult);
}

void NextStarAnalyzer::getValuesFromPKB(vector<int>& retrievedPKBValues, bool hasArg2EvalBefore, int candidates)
{
	if (!hasArg2EvalBefore) {
		retrievedPKBValues = pkbReadOnly.getNextStarFirstLiteral(candidates);
		retrievedPKBValues = validatePKBResultsInt(arg2Entity, retrievedPKBValues);
	}
	else {
		retrievedPKBValues = pkbReadOnly.getNextStarSecondLiteral(candidates);
		retrievedPKBValues = validatePKBResultsInt(arg1Entity, retrievedPKBValues);
	}
}

tuple<bool, vector<vector<string>>> NextStarAnalyzer::addBothSynResult(string arg1, string arg2)
{
	bool hasNextStar = true;
	vector<int> vecOfCandidates;
	vector<int> retrievedPKBResults;
	vector<string> pkbResultForArg1;
	vector<string> pkbResultForArg2;
	vector<vector<string>> NextStarResult;
	bool isSameSynonym = arg1 == arg2 ? true : false;

	bool hasArg2EvalBefore = false;
	const auto synArg1Iterator = queryMap.find(arg1);
	const auto synArg2Iterator = queryMap.find(arg2);

	if (synArg1Iterator == queryMap.end() && synArg2Iterator == queryMap.end()) {
		auto pkbAnswers = pkbReadOnly.getNextStarTwoSynonyms();
		auto validatedPKBAnswers = validatePKBResultsInt(get<ARGONE>(pkbAnswers), get<ARGTWO>(pkbAnswers));
		pkbResultForArg1 = get<ARGONE>(validatedPKBAnswers);
		pkbResultForArg2 = get<ARGTWO>(validatedPKBAnswers);

		vector<string> clauseResultForSameSynonym;
		if (isSameSynonym) {
			for (int i = 0; i < pkbResultForArg1.size(); i++) {
				if (pkbResultForArg1[i] == pkbResultForArg2[i])
					clauseResultForSameSynonym.push_back(pkbResultForArg1[i]);
			}
			pkbResultForArg1 = clauseResultForSameSynonym;
		}
	}
	else {
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
	}
	if (pkbResultForArg1.empty())
		hasNextStar = false;
	else if(isSameSynonym && !pkbResultForArg1.empty()) {
		pkbResultForArg1.push_back(arg1);
		NextStarResult.push_back(pkbResultForArg1);
	} else if(!isSameSynonym && !pkbResultForArg1.empty()) {
		if (arg1 != WILDCARD_SYMBOL) {
			pkbResultForArg1.push_back(arg1);
			NextStarResult.push_back(pkbResultForArg1);
		}
		if (arg2 != WILDCARD_SYMBOL) {
			pkbResultForArg2.push_back(arg2);
			NextStarResult.push_back(pkbResultForArg2);
		}
	}

	return make_tuple(hasNextStar, NextStarResult);
}

bool NextStarAnalyzer::checkClauseBothVariables(string arg1, string arg2)
{
	return pkbReadOnly.getNextStarTwoLiterals(stoi(arg1), stoi(arg2));
}

bool NextStarAnalyzer::checkClauseVariableWild(string arg1)
{
	return pkbReadOnly.getNextStarFirstLiteral(stoi(arg1)).empty() ? false : true;
}

bool NextStarAnalyzer::checkClauseWildVariable(string arg2)
{
	return pkbReadOnly.getNextStarSecondLiteral(stoi(arg2)).empty() ? false : true;
}

bool NextStarAnalyzer::checkClauseBothWild()
{
	int MIN_STMTS_FOR_Next = 1;
	return (pkbReadOnly.getAllNext().size() < MIN_STMTS_FOR_Next) ? false : true;
}
