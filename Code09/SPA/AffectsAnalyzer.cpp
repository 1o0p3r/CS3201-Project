#include "AffectsAnalyzer.h"

bool AffectsAnalyzer::hasResultsForArg(const int candidates, const bool isAddArg1) {
	if (isAddArg1)
		return pkbReadOnly.getAffectsFirstLiteral(candidates).empty() ? false : true;
	else
		return pkbReadOnly.getAffectsSecondLiteral(candidates).empty() ? false : true;
}



tuple<bool, vector<vector<string>>> AffectsAnalyzer::addArgTwoResult(string arg1)
{
	bool hasNext = true;
	vector<int> vecOfCandidates;
	vector<string> pkbResult;
	vector<int> pkbNext;
	vector<vector<string>> AffectsResult;
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
			pkbNext = pkbReadOnly.getAffectsFirstLiteral(candidates);
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
		AffectsResult.push_back(pkbResult);
	}

	return make_tuple(hasNext, AffectsResult);
}

tuple<bool, vector<vector<string>>> AffectsAnalyzer::addArgOneResult(string arg2)
{
	bool hasNext = true;
	vector<int> vecOfCandidates;
	vector<int> pkbPrevious;
	vector<string> pkbResult;
	vector<vector<string>> AffectsResult;

	const auto synArg1Iterator = queryMap.find(arg1);

	if (synArg1Iterator != queryMap.end() && arg2 == WILDCARD_SYMBOL) {
		bool isAddArg1 = true;
		bool isArgTypeInt = true;
		pkbResult = optimizedAddArg(synArg1Iterator, isAddArg1, isArgTypeInt);
	}
	else {
		if (arg2 == WILDCARD_SYMBOL) // to be optimized
			vecOfCandidates = pkbReadOnly.getAllStmt();
		else
			vecOfCandidates.push_back(stoi(arg2));
		for (int candidates : vecOfCandidates) {
			pkbPrevious = pkbReadOnly.getAffectsSecondLiteral(candidates);
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
		AffectsResult.push_back(pkbResult);
	}

	return make_tuple(hasNext, AffectsResult);
}

void AffectsAnalyzer::getValuesFromPKB(vector<int>& retrievedPKBValues, bool hasArg2EvalBefore, int candidates)
{
	if (!hasArg2EvalBefore) {
		retrievedPKBValues = pkbReadOnly.getAffectsFirstLiteral(candidates);
		retrievedPKBValues = validatePKBResultsInt(arg2Entity, retrievedPKBValues);
	}
	else {
		retrievedPKBValues = pkbReadOnly.getAffectsSecondLiteral(candidates);
		retrievedPKBValues = validatePKBResultsInt(arg1Entity, retrievedPKBValues);
	}
}

tuple<bool, vector<vector<string>>> AffectsAnalyzer::addBothSynResult(string arg1, string arg2)
{
	bool hasNext = true;
	vector<int> vecOfCandidates;
	vector<int> retrievedPKBResults;
	vector<string> pkbResultForArg1;
	vector<string> pkbResultForArg2;
	vector<vector<string>> AffectsResult;

	bool hasArg2EvalBefore = false;
	const auto synArg1Iterator = queryMap.find(arg1);
	const auto synArg2Iterator = queryMap.find(arg2);

	if (synArg1Iterator == queryMap.end() && synArg2Iterator == queryMap.end()) {
		auto pkbAnswers = pkbReadOnly.getAffectsTwoSynonyms();
		auto validatedPKBAnswers = validatePKBResultsInt(get<ARGONE>(pkbAnswers), get<ARGTWO>(pkbAnswers));
		pkbResultForArg1 = get<ARGONE>(validatedPKBAnswers);
		pkbResultForArg2 = get<ARGTWO>(validatedPKBAnswers);

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
		hasNext = false;
	else {
		pkbResultForArg1.push_back(arg1);
		pkbResultForArg2.push_back(arg2); //to denote this vector belongs to indicated synonym 
		AffectsResult.push_back(pkbResultForArg1);
		AffectsResult.push_back(pkbResultForArg2);
	}

	return make_tuple(hasNext, AffectsResult);
}

bool AffectsAnalyzer::checkClauseBothVariables(string arg1, string arg2)
{
	return pkbReadOnly.getAffectsTwoLiterals(stoi(arg1), stoi(arg2));
}

bool AffectsAnalyzer::checkClauseVariableWild(string arg1)
{
	return pkbReadOnly.getAffectsFirstLiteral(stoi(arg1)).empty() ? false : true;
}

bool AffectsAnalyzer::checkClauseWildVariable(string arg2)
{
	return pkbReadOnly.getAffectsSecondLiteral(stoi(arg2)).empty() ? false : true;
}

bool AffectsAnalyzer::checkClauseBothWild()
{
	vector<int>  affectsArg1Candidates = pkbReadOnly.getAssign();
	for(const auto candidates: affectsArg1Candidates) {
		auto result = pkbReadOnly.getAffectsFirstLiteral(candidates);
		if (!result.empty())
			return true;
	}
	return false;
}
