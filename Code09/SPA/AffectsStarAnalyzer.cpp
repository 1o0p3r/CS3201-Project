#include "AffectsStarAnalyzer.h"

bool AffectsStarAnalyzer::hasResultsForArg(const int candidates, const bool isAddArg1) {
	if (isAddArg1)
		return pkbReadOnly.getAffectStarFirstLiteral(candidates).empty() ? false : true;
	else
		return pkbReadOnly.getAffectStarSecondLiteral(candidates).empty() ? false : true;
}



tuple<bool, vector<vector<string>>> AffectsStarAnalyzer::addArgTwoResult(string arg1)
{
	bool hasAffectStar = true;
	vector<int> vecOfCandidates;
	vector<string> pkbResult;
	vector<int> pkbNext;
	vector<vector<string>> AffectStarResult;
	bool isArgTypeInt = true;

	const auto synArg2Iterator = queryMap.find(arg2);

	if (synArg2Iterator != queryMap.end() && arg1 == WILDCARD_SYMBOL) {
		bool isAddArg1 = false;
		pkbResult = optimizedAddArg(synArg2Iterator, isAddArg1, isArgTypeInt);
	}
	else {
		if (arg1 == WILDCARD_SYMBOL)
			return addBothSynResult(arg1, arg2);
		else
			vecOfCandidates.push_back(stoi(arg1));
		for (int candidates : vecOfCandidates) {
			pkbNext = pkbReadOnly.getAffectStarFirstLiteral(candidates);
			pkbNext = validatePKBResultsInt(arg2Entity, pkbNext);
			for (int candidatesChosen : pkbNext) {
				pkbResult.push_back(to_string(candidatesChosen));
			}
		}
	}
	if (pkbResult.empty())
		hasAffectStar = false;
	else {
		pkbResult = removeDuplicates(pkbResult);
		pkbResult.push_back(arg2); //to denote this vector belongs to indicated synonym 
		AffectStarResult.push_back(pkbResult);
	}

	return make_tuple(hasAffectStar, AffectStarResult);
}

tuple<bool, vector<vector<string>>> AffectsStarAnalyzer::addArgOneResult(string arg2)
{
	bool hasAffectStar = true;
	vector<int> vecOfCandidates;
	vector<int> pkbPrevious;
	vector<string> pkbResult;
	vector<vector<string>> AffectStarResult;

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
			pkbPrevious = pkbReadOnly.getAffectStarSecondLiteral(candidates);
			pkbPrevious = validatePKBResultsInt(arg1Entity, pkbPrevious);
			for (int candidatesChosen : pkbPrevious) {
				pkbResult.push_back(to_string(candidatesChosen));
			}
		}
	}
	if (pkbResult.empty())
		hasAffectStar = false;
	else {
		pkbResult = removeDuplicates(pkbResult);
		pkbResult.push_back(arg1); //to denote this vector belongs to indicated synonym 
		AffectStarResult.push_back(pkbResult);
	}

	return make_tuple(hasAffectStar, AffectStarResult);
}

void AffectsStarAnalyzer::getValuesFromPKB(vector<int>& retrievedPKBValues, bool hasArg2EvalBefore, int candidates)
{
	if (!hasArg2EvalBefore) {
		retrievedPKBValues = pkbReadOnly.getAffectStarFirstLiteral(candidates);
		retrievedPKBValues = validatePKBResultsInt(arg2Entity, retrievedPKBValues);
	}
	else {
		retrievedPKBValues = pkbReadOnly.getAffectStarSecondLiteral(candidates);
		retrievedPKBValues = validatePKBResultsInt(arg1Entity, retrievedPKBValues);
	}
}

tuple<bool, vector<vector<string>>> AffectsStarAnalyzer::addBothSynResult(string arg1, string arg2)
{
	bool hasAffectStar = true;
	vector<int> vecOfCandidates;
	vector<int> retrievedPKBResults;
	vector<string> pkbResultForArg1;
	vector<string> pkbResultForArg2;
	vector<vector<string>> AffectStarResult;
	bool isSameSynonym = arg1 == arg2 ? true : false;

	bool hasArg2EvalBefore = false;
	const auto synArg1Iterator = queryMap.find(arg1);
	const auto synArg2Iterator = queryMap.find(arg2);

	if (synArg1Iterator == queryMap.end() && synArg2Iterator == queryMap.end()) {
		auto pkbAnswers = pkbReadOnly.getAffectStarTwoSynonyms();
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
		hasAffectStar = false;
	else if (isSameSynonym && !pkbResultForArg1.empty()) {
		pkbResultForArg1.push_back(arg1);
		AffectStarResult.push_back(pkbResultForArg1);
	}
	else if (!isSameSynonym) {
		if (arg1 != WILDCARD_SYMBOL && !pkbResultForArg1.empty()) {
			pkbResultForArg1.push_back(arg1);
			AffectStarResult.push_back(pkbResultForArg1);
		}
		if (arg2 != WILDCARD_SYMBOL && !pkbResultForArg2.empty()) {
			pkbResultForArg2.push_back(arg2);
			AffectStarResult.push_back(pkbResultForArg2);
		}
	}

	return make_tuple(hasAffectStar, AffectStarResult);
}

bool AffectsStarAnalyzer::checkClauseBothVariables(string arg1, string arg2)
{
	return pkbReadOnly.getAffectStarTwoLiterals(stoi(arg1), stoi(arg2));
}

bool AffectsStarAnalyzer::checkClauseVariableWild(string arg1)
{
	return pkbReadOnly.getAffectStarFirstLiteral(stoi(arg1)).empty() ? false : true;
}

bool AffectsStarAnalyzer::checkClauseWildVariable(string arg2)
{
	return pkbReadOnly.getAffectStarSecondLiteral(stoi(arg2)).empty() ? false : true;
}

bool AffectsStarAnalyzer::checkClauseBothWild()
{
	vector<int>  AffectStarArg1Candidates = pkbReadOnly.getAssign();
	for (const auto candidates : AffectStarArg1Candidates) {
		auto result = pkbReadOnly.getAffectStarFirstLiteral(candidates);
		if (!result.empty())
			return true;
	}
	return false;
}
