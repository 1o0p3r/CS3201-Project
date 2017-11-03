#include "CallsStarAnalyzer.h"

bool CallsStarAnalyzer::hasResultsForArg(const string candidates, const bool isAddArg1) {
	if (isAddArg1)
		return pkbReadOnly.getCallsStar(candidates).empty() ? false : true;
	else
		return pkbReadOnly.getCalledByStar(candidates).empty() ? false : true;
}

tuple<bool, vector<vector<string>>> CallsStarAnalyzer::addArgTwoResult(string arg1)
{
	bool hasCallsStar = true;
	vector<string> vecOfCandidates;
	vector<string> pkbResult;
	vector<string> pkbCallsStar;
	vector<vector<string>> callsResult;

	const auto synArg2Iterator = queryMap.find(arg2);

	if (synArg2Iterator != queryMap.end() && arg1 == WILDCARD_SYMBOL) {
		bool isAddArg1 = false;
		pkbResult = optimizedAddArg(synArg2Iterator, isAddArg1, true);
	} else {
		if (arg1 == WILDCARD_SYMBOL)
			pkbResult = unitTestModeOn ? getUnitTestInputs() : getPKBAllArgValues();
		else {
			vecOfCandidates.push_back(arg1);
			for (string candidates : vecOfCandidates) {
				pkbCallsStar = unitTestModeOn ? getUnitTestInputs() : pkbReadOnly.getCallsStar(candidates);
				for (string candidatesChosen : pkbCallsStar) {
					pkbResult.push_back(candidatesChosen);
				}
			}
		}
	}
	if (pkbResult.empty())
		hasCallsStar = false;
	else {
		pkbResult = removeDuplicates(pkbResult);
		pkbResult.push_back(arg2); //to denote this vector belongs to indicated synonym 
		callsResult.push_back(pkbResult);
	}

	return make_tuple(hasCallsStar, callsResult);
}

tuple<bool, vector<vector<string>>> CallsStarAnalyzer::addArgOneResult(string arg2)
{
	bool hasCallsStar = true;
	vector<string> vecOfCandidates;
	vector<string> pkbCallsStar;
	vector<string> pkbResult;
	vector<vector<string>> callsResult;

	const auto synArg1Iterator = queryMap.find(arg1);
	
	if (synArg1Iterator != queryMap.end() && arg2 == WILDCARD_SYMBOL) {
		bool isAddArg1 = true;
		pkbResult = optimizedAddArg(synArg1Iterator, isAddArg1, true);
	} else {
		if (arg2 == WILDCARD_SYMBOL)
			vecOfCandidates = unitTestModeOn ? getUnitTestInputs() : pkbReadOnly.getAllCalls();
		else
			vecOfCandidates.push_back(arg2);
		for (string candidates : vecOfCandidates) {
			pkbCallsStar = unitTestModeOn ? getUnitTestInputs() : pkbReadOnly.getCalledBy(candidates);
			for (string candidatesChosen : pkbCallsStar) {
				pkbResult.push_back(candidatesChosen);
			}
		}
	}
	if (pkbResult.empty())
		hasCallsStar = false;
	else {
		pkbResult = removeDuplicates(pkbResult);
		pkbResult.push_back(arg1); //to denote this vector belongs to indicated synonym 
		callsResult.push_back(pkbResult);
	}

	return make_tuple(hasCallsStar, callsResult);
}

void CallsStarAnalyzer::getValuesFromPKB(vector<string>& retrievedPKBValues, bool hasArg2EvalBefore, string candidates)
{
	if (!hasArg2EvalBefore) {
		retrievedPKBValues = pkbReadOnly.getCallsStar(candidates);
		retrievedPKBValues = validatePKBResultsString(arg2Entity, retrievedPKBValues);
	}
	else {
		retrievedPKBValues = pkbReadOnly.getCalledByStar(candidates);
		retrievedPKBValues = validatePKBResultsString(arg1Entity, retrievedPKBValues);
	}
}

tuple<bool, vector<vector<string>>> CallsStarAnalyzer::addBothSynResult(string arg1, string arg2)
{
	bool hasCallsStar = true;
	vector<string> vecOfCandidates;
	vector<string> retrievedPKBResults;
	vector<string> pkbResultForArg1;
	vector<string> pkbResultForArg2;
	vector<vector<string>> callsResult;

	bool hasArg2EvalBefore = false;
	const auto synArg1Iterator = queryMap.find(arg1);
	const auto synArg2Iterator = queryMap.find(arg2);

	if (synArg1Iterator == queryMap.end() && synArg2Iterator == queryMap.end()) {
		vecOfCandidates = unitTestModeOn ? getUnitTestInputs() : pkbReadOnly.getAllCalls();
		hasArg2EvalBefore = true;
	} else 
		getArgsPriorStringResults(vecOfCandidates, hasArg2EvalBefore, synArg1Iterator, synArg2Iterator);

	for (string candidates : vecOfCandidates) {
		if (unitTestModeOn)
			retrievedPKBResults = getUnitTestInputs();
		getValuesFromPKB(retrievedPKBResults, hasArg2EvalBefore, candidates);
		for (string candidatesChosen : retrievedPKBResults) {
			if (!hasArg2EvalBefore) {
				pkbResultForArg1.push_back(candidates);
				pkbResultForArg2.push_back(candidatesChosen);
			}
			else {
				pkbResultForArg1.push_back(candidatesChosen);
				pkbResultForArg2.push_back(candidates);
			}
		}
	}
	if (pkbResultForArg1.empty())
		hasCallsStar = false;
	else {
		pkbResultForArg1.push_back(arg1);
		pkbResultForArg2.push_back(arg2); //to denote this vector belongs to indicated synonym 
		callsResult.push_back(pkbResultForArg1);
		callsResult.push_back(pkbResultForArg2);
	}

	return make_tuple(hasCallsStar, callsResult);
}

bool CallsStarAnalyzer::checkClauseBothVariables(string arg1, string arg2)
{

	auto pkbResult = pkbReadOnly.getCallsStar(arg1);
	if (unitTestModeOn)
		return getUnitTestInputs().empty() ? false : true;
	return find(pkbResult.begin(), pkbResult.end(), arg2) == pkbResult.end() ?
		false : true;
}

bool CallsStarAnalyzer::checkClauseVariableWild(string arg1)
{
	if (unitTestModeOn)
		return getUnitTestInputs().empty() ? false : true;
	return pkbReadOnly.getCallsStar(arg1).empty() ? false : true;
}

bool CallsStarAnalyzer::checkClauseWildVariable(string arg2)
{
	if (unitTestModeOn)
		return getUnitTestInputs().empty() ? false : true;
	return pkbReadOnly.getCalledBy(arg2).empty() ? false : true;
}

bool CallsStarAnalyzer::checkClauseBothWild()
{
	if (unitTestModeOn)
		return getUnitTestInputs().empty() ? false : true;

	int minCallStmts = 1;
	int allCallsStar = getPKBAllArgValues().size();
	if (allCallsStar < minCallStmts)
		hasSuchThatClause = false;
	return hasSuchThatClause;
}

vector<string> CallsStarAnalyzer::getPKBAllArgValues()
{	//returns all procedure calledBy another procedure
	return pkbReadOnly.getAllCalls();
}
