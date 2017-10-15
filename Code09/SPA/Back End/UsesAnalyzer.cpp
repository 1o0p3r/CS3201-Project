#include "UsesAnalyzer.h"



tuple<bool, vector<vector<string>>> UsesAnalyzer::addArgTwoResult(string arg1)
{
	bool hasUses = true;
	vector<string> vecOfCandidates;
	vector<string> pkbResult;
	vector<string> pkbUses;
	vector<vector<string>> UsesResult;

	//semantic error check by preprocessor
	/*
	if (arg1 == WILDCARD_SYMBOL)
	return make_tuple(false, vector<vector<string>>());
	*/
	vecOfCandidates.push_back(arg1);
	for (string candidates : vecOfCandidates) {
		pkbUses = getUsesResultAddArg2(candidates, arg2Entity);
		for (string candidatesChosen : pkbUses) {
			pkbResult.push_back(candidatesChosen);
		}
	}
	if (pkbResult.empty())
		hasUses = false;
	else {
		pkbResult.push_back(arg2); //to denote this vector belongs to indicated synonym 
		UsesResult.push_back(pkbResult);
	}

	return make_tuple(hasUses, UsesResult);
}

tuple<bool, vector<vector<string>>> UsesAnalyzer::addArgOneResult(string arg2)
{
	bool hasUses = true;
	vector<string> vecOfCandidates;
	vector<string> pkbUses;
	vector<string> pkbResult;
	vector<vector<string>> UsesResult;

	if (arg2 == WILDCARD_SYMBOL)
		vecOfCandidates = pkbReadOnly.getAllVariables();
	else
		vecOfCandidates.push_back(arg2);
	for (string candidates : vecOfCandidates) {
		pkbUses = getUsesResultAddArg1(candidates, arg1Entity);
		for (string candidatesChosen : pkbUses) {
			pkbResult.push_back(candidatesChosen);
		}
	}
	if (pkbResult.empty())
		hasUses = false;
	else {
		pkbResult.push_back(arg1); //to denote this vector belongs to indicated synonym 
		UsesResult.push_back(pkbResult);
	}

	return make_tuple(hasUses, UsesResult);
}

tuple<bool, vector<vector<string>>> UsesAnalyzer::addBothSynResult(string arg1, string arg2)
{
	bool hasUses = true;
	vector<string> vecOfCandidates;
	vector<string> pkbUses;
	vector<string> pkbResultForArg1;
	vector<string> pkbResultForArg2;
	vector<vector<string>> UsesResult;

	vecOfCandidates = pkbReadOnly.getAllVariables();
	for (string candidates : vecOfCandidates) {
		pkbUses = getUsesResultAddArg1(candidates, arg1Entity);
		for (string candidatesChosen : pkbUses) {
			pkbResultForArg1.push_back(candidatesChosen);
			pkbResultForArg2.push_back(candidates);
		}
	}
	if (pkbResultForArg1.empty())
		hasUses = false;
	else {
		pkbResultForArg1.push_back(arg1);
		pkbResultForArg2.push_back(arg2); //to denote this vector belongs to indicated synonym 
		UsesResult.push_back(pkbResultForArg1);
		UsesResult.push_back(pkbResultForArg2);
	}

	return make_tuple(hasUses, UsesResult);
}

bool UsesAnalyzer::checkClauseBothVariables(string arg1, string arg2)
{	
	vector<string> pkbResult;
	if (arg1Type == PROCEDUREARG)
		pkbResult = pkbReadOnly.getProcUsedBy(arg2);
	else {
		auto pkbResultInt = pkbReadOnly.getUsedBy(arg2);
		for (int candidate : pkbResultInt)
			pkbResult.push_back(to_string(candidate));
	}
	return find(pkbResult.begin(), pkbResult.end(), arg1) == pkbResult.end() ?
		false : true;
}

bool UsesAnalyzer::checkClauseVariableWild(string arg1)
{
	if (arg1Type == PROCEDUREARG)
		return pkbReadOnly.getProcModifies(arg1).empty() ? false : true;
	return pkbReadOnly.getUses(stoi(arg1)).empty() ? false : true;
}

bool UsesAnalyzer::checkClauseWildVariable(string arg2)
{
	assert(arg1 == WILDCARD_SYMBOL);
	return pkbReadOnly.getUsedBy(arg2).empty() ? false : true;
}

bool UsesAnalyzer::checkClauseBothWild()
{
	assert(arg1 == WILDCARD_SYMBOL);
	int minNoStmtsForUses = 1;
	vector<int> allStmts = pkbReadOnly.getAllStmt();
	if (allStmts.size() < minNoStmtsForUses)
		hasSuchThatClause = false;
	return hasSuchThatClause;
}

vector<string> UsesAnalyzer::getUsesResultAddArg2(string arg1, string arg2Entity)
{
	return (arg1Type == PROCEDUREARG) ? pkbReadOnly.getProcUses(arg1) : 
			pkbReadOnly.getUses(stoi(arg1));
}

//@param: must be procedure entity for Arg1
vector<string> UsesAnalyzer::getUsesResultAddArg1(string arg2, string arg1Entity)
{
	vector<int> pkbResultInt;
	vector<string> pkbResult;
	if (arg1Entity == PROCEDURE)
		pkbResult = pkbReadOnly.getProcUsedBy(arg2);
	else {
		pkbResultInt = pkbReadOnly.getUsedBy(arg2);
		pkbResultInt = validatePKBResultsInt(arg1Entity, pkbResultInt);
		for (int entryToString : pkbResultInt)
			pkbResult.push_back(to_string(entryToString));
	}
	return pkbResult;

}

