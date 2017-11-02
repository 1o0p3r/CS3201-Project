#include "ParentAnalyzer.h"
#include "Util.h"

bool ParentAnalyzer::hasResultsForArg(const int candidates, const bool isAddArg1) {
	if (isAddArg1)
		return pkbReadOnly.getChild(candidates).empty() ? false : true;
	else
		return pkbReadOnly.getParent(candidates).empty() ? false : true;
}

tuple<bool, vector<vector<string>>> ParentAnalyzer::addArgTwoResult(string arg1)
{
	bool hasParent = true;
	vector<int> vecOfCandidates;
	vector<string> pkbResult;
	vector<int> pkbChild;
	vector<vector<string>> parentResult;

	const auto synArg2Iterator = queryMap.find(arg2);
	
	if (synArg2Iterator != queryMap.end() && arg1 == WILDCARD_SYMBOL) {
		bool isAddArg1 = false;
		pkbResult = optimizedAddArg(synArg2Iterator, isAddArg1);
	}
	else {
		if (arg1 == WILDCARD_SYMBOL) {
			vecOfCandidates = pkbReadOnly.getWhile();
			vector<int> temp = pkbReadOnly.getIf();
			vecOfCandidates.insert(vecOfCandidates.end(), temp.begin(), temp.end());
		}
		else
			vecOfCandidates.push_back(stoi(arg1));
		for (int candidates : vecOfCandidates) {
			pkbChild = pkbReadOnly.getChild(candidates);
			pkbChild = validatePKBResultsInt(arg2Entity, pkbChild);
			for (int candidatesChosen : pkbChild) {
				pkbResult.push_back(to_string(candidatesChosen));
			}
		}
	}
	if (pkbResult.empty())
		hasParent = false;
	else {
		pkbResult = removeDuplicates(pkbResult);
		pkbResult.push_back(arg2); //to denote this vector belongs to indicated synonym 
		parentResult.push_back(pkbResult);
	}

	return make_tuple(hasParent, parentResult);
}

tuple<bool, vector<vector<string>>> ParentAnalyzer::addArgOneResult(string arg2)
{
	bool hasParent = true;
	vector<int> vecOfCandidates;
	vector<int> pkbParent;
	vector<string> pkbResult;
	vector<vector<string>> parentResult;
	bool hasArg1EvalBefore = false;
	const auto synArg1Iterator = queryMap.find(arg1);

	if (synArg1Iterator != queryMap.end() && arg2 == WILDCARD_SYMBOL) {
		bool isAddArg1 = true;
		pkbResult = optimizedAddArg(synArg1Iterator, isAddArg1);
	}
	else {
		if (arg2 == WILDCARD_SYMBOL) {
			vecOfCandidates = pkbReadOnly.getAllStmt();
		}
		else
			vecOfCandidates.push_back(stoi(arg2));
		for (int candidates : vecOfCandidates) {
			pkbParent = pkbReadOnly.getParent(candidates);
			pkbParent = validatePKBResultsInt(arg1Entity, pkbParent);
			for (int candidatesChosen : pkbParent) {
				pkbResult.push_back(to_string(candidatesChosen));
			}
		}
	}
	if (pkbResult.empty())
		hasParent = false;
	else {
		pkbResult = removeDuplicates(pkbResult);
		pkbResult.push_back(arg1); //to denote this vector belongs to indicated synonym 
		parentResult.push_back(pkbResult);
	}

	return make_tuple(hasParent, parentResult);
}

void ParentAnalyzer::getValuesFromPKB(vector<int>& retrievedPKBValues, bool hasArg2EvalBefore, int candidates)
{
	if (!hasArg2EvalBefore) {
		retrievedPKBValues = pkbReadOnly.getChild(candidates);
		retrievedPKBValues = validatePKBResultsInt(arg2Entity, retrievedPKBValues);
	} else {
		retrievedPKBValues = pkbReadOnly.getParent(candidates);
		retrievedPKBValues = validatePKBResultsInt(arg1Entity, retrievedPKBValues);
	}
}

tuple<bool, vector<vector<string>>> ParentAnalyzer::addBothSynResult(string arg1, string arg2)
{
	bool hasParent = true;
	vector<int> vecOfCandidates;
	vector<int> retrievedPKBResults;
	vector<string> pkbResultForArg1;
	vector<string> pkbResultForArg2;
	vector<vector<string>> parentResult;
	
	bool hasArg2EvalBefore = false;
	const auto synArg1Iterator = queryMap.find(arg1);
	const auto synArg2Iterator = queryMap.find(arg2);

	if (synArg1Iterator == queryMap.end() && synArg2Iterator == queryMap.end()) {
		vecOfCandidates = pkbReadOnly.getWhile();
		vector<int> temp = pkbReadOnly.getIf();
		vecOfCandidates.insert(vecOfCandidates.end(), temp.begin(), temp.end());
		vecOfCandidates = validatePKBResultsInt(arg1Entity, vecOfCandidates);
	} else 
		getArgsPriorResults(vecOfCandidates, hasArg2EvalBefore, synArg1Iterator, synArg2Iterator);

	for (int candidates : vecOfCandidates) {
		getValuesFromPKB(retrievedPKBResults, hasArg2EvalBefore, candidates);
		for (int candidatesChosen : retrievedPKBResults) {
			if (!hasArg2EvalBefore) {
				pkbResultForArg1.push_back(to_string(candidates));
				pkbResultForArg2.push_back(to_string(candidatesChosen));
			} else {
				pkbResultForArg1.push_back(to_string(candidatesChosen));
				pkbResultForArg2.push_back(to_string(candidates));
			}
		}
	}
	if (pkbResultForArg1.empty())
		hasParent = false;
	else {
		pkbResultForArg1.push_back(arg1);
		pkbResultForArg2.push_back(arg2); //to denote this vector belongs to indicated synonym 
		parentResult.push_back(pkbResultForArg1);
		parentResult.push_back(pkbResultForArg2);
	}

	return make_tuple(hasParent, parentResult);
}

bool ParentAnalyzer::checkClauseBothVariables(string arg1, string arg2)
{
	auto pkbResult = pkbReadOnly.getChild(stoi(arg1));
	return find(pkbResult.begin(), pkbResult.end(), stoi(arg2)) == pkbResult.end() ?
		false : true;
}

bool ParentAnalyzer::checkClauseVariableWild(string arg1)
{
	return pkbReadOnly.getChild(stoi(arg1)).empty() ? false : true;
}

bool ParentAnalyzer::checkClauseWildVariable(string arg2)
{
	return pkbReadOnly.getParent(stoi(arg2)).empty() ? false : true;
}

bool ParentAnalyzer::checkClauseBothWild()
{
	int MIN_STMTS_FOR_PARENT = 1;
	return (pkbReadOnly.getIf().size()+pkbReadOnly.getWhile().size() < MIN_STMTS_FOR_PARENT) ? false : true;
}
