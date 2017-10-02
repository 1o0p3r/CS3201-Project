#include "CallsAnalyzer.h"
const string WILDCARD_SYMBOL = "_";

tuple<bool, vector<vector<string>>> CallsAnalyzer::addArgTwoResult(string arg1)
{
	bool hasCalls = true;
	vector<string> vecOfCandidates;
	vector<string> pkbResult;
	vector<string> pkbCalls;
	vector<vector<string>> callsResult;

	if (arg1 == WILDCARD_SYMBOL)
		vecOfCandidates = pkbReadOnly.getCall();
	else
		vecOfCandidates.push_back(arg1);
	for (string candidates : vecOfCandidates) {
		pkbCalls = pkbReadOnly.getCalls(candidates);
		for (string candidatesChosen : pkbCalls) {
			pkbResult.push_back(candidatesChosen);
		}
	}
	if (pkbResult.empty())
		hasCalls = false;
	else {
		pkbResult = removeDuplicates(pkbResult);
		pkbResult.push_back(arg2); //to denote this vector belongs to indicated synonym 
		callsResult.push_back(pkbResult);
	}

	return make_tuple(hasCalls, callsResult);
}

tuple<bool, vector<vector<string>>> CallsAnalyzer::addArgOneResult(string arg2)
{
	bool hasCalls = true;
	vector<string> vecOfCandidates;
	vector<string> pkbCalls;
	vector<string> pkbResult;
	vector<vector<string>> callsResult;

	if (arg2 == WILDCARD_SYMBOL)
		vecOfCandidates = pkbReadOnly.getCall();
	else
		vecOfCandidates.push_back(arg2);
	for (string candidates : vecOfCandidates) {
		pkbCalls = pkbReadOnly.getCalledBy(candidates);
		for (string candidatesChosen : pkbCalls) {
			pkbResult.push_back(candidatesChosen);
		}
	}
	if (pkbResult.empty())
		hasCalls = false;
	else {
		pkbResult = removeDuplicates(pkbResult);
		pkbResult.push_back(arg1); //to denote this vector belongs to indicated synonym 
		callsResult.push_back(pkbResult);
	}

	return make_tuple(hasCalls, callsResult);
}

tuple<bool, vector<vector<string>>> CallsAnalyzer::addBothSynResult(string arg1, string arg2)
{
	bool hasCalls = true;
	vector<string> vecOfCandidates;
	vector<string> pkbCalls;
	vector<string> pkbResultForArg1;
	vector<string> pkbResultForArg2;
	vector<vector<string>> callsResult;

	vecOfCandidates = pkbReadOnly.getCall();
	for (string candidates : vecOfCandidates) {
		pkbCalls = pkbReadOnly.getCalls(candidates);
		for (string candidatesChosen : pkbCalls) {
			pkbResultForArg1.push_back(candidates);
			pkbResultForArg2.push_back(candidatesChosen);
		}
	}
	if (pkbResultForArg1.empty())
		hasCalls = false;
	else {
		pkbResultForArg1.push_back(arg1);
		pkbResultForArg2.push_back(arg2); //to denote this vector belongs to indicated synonym 
		callsResult.push_back(pkbResultForArg1);
		callsResult.push_back(pkbResultForArg2);
	}

	return make_tuple(hasCalls, callsResult);
}

bool CallsAnalyzer::checkClauseBothVariables(string arg1, string arg2)
{
	auto pkbResult = pkbReadOnly.getCalls(arg1);
	return find(pkbResult.begin(), pkbResult.end(), arg2) == pkbResult.end() ?
		false : true;
}

bool CallsAnalyzer::checkClauseVariableWild(string arg1)
{
	return pkbReadOnly.getCalls(arg1).empty() ? false : true;
}

bool CallsAnalyzer::checkClauseWildVariable(string arg2)
{
	return pkbReadOnly.getCalledBy(arg2).empty() ? false : true;
}

bool CallsAnalyzer::checkClauseBothWild()
{
	int minCallStmts = 1;
	vector<int> allCalls = pkbReadOnly.getCall();
	if (allCalls.size() < minCallStmts)
		hasSuchThatClause = false;
	return hasSuchThatClause;
}

