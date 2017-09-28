#include "ModifiesAnalyzer.h"


const string WILDCARD_SYMBOL = "_";

tuple<bool, vector<vector<string>>> ModifiesAnalyzer::addArgTwoResult(string arg1)
{
	bool hasModifies = true;
	vector<int> vecOfCandidates;
	vector<string> pkbResult;
	vector<string> pkbModifies;
	vector<vector<string>> modifiesResult;

	if (arg1 == WILDCARD_SYMBOL)
		vecOfCandidates = pkbReadOnly.getAllStmt();
	else
		vecOfCandidates.push_back(stoi(arg1));
	for (int candidates : vecOfCandidates) {
		pkbModifies = pkbReadOnly.getModifies(candidates);
		for (string candidatesChosen : pkbModifies) {
			pkbResult.push_back(candidatesChosen);
		}
	}
	if (pkbResult.empty())
		hasModifies = false;
	else {
		pkbResult.push_back(arg2); //to denote this vector belongs to indicated synonym 
		modifiesResult.push_back(pkbResult);
	}

	return make_tuple(hasModifies, modifiesResult);
}

tuple<bool, vector<vector<string>>> ModifiesAnalyzer::addArgOneResult(string arg2)
{
	bool hasModifies = true;
	vector<string> vecOfCandidates;
	vector<int> pkbModifies;
	vector<string> pkbResult;
	vector<vector<string>> modifiesResult;

	if (arg2 == WILDCARD_SYMBOL)
		vecOfCandidates = pkbReadOnly.getAllVariables();
	else
		vecOfCandidates.push_back(arg2);
	for (string candidates : vecOfCandidates) {
		pkbModifies = pkbReadOnly.getModifiedBy(candidates);
		for (int candidatesChosen : pkbModifies) {
			pkbResult.push_back(to_string(candidatesChosen));
		}
	}
	if (pkbResult.empty())
		hasModifies = false;
	else {
		pkbResult.push_back(arg2); //to denote this vector belongs to indicated synonym 
		modifiesResult.push_back(pkbResult);
	}

	return make_tuple(hasModifies, modifiesResult);
}

tuple<bool, vector<vector<string>>> ModifiesAnalyzer::addBothSynResult(string arg1, string arg2)
{
	bool hasModifies = true;
	vector<int> vecOfCandidates;
	vector<string> pkbModifies;
	vector<string> pkbResultForArg1;
	vector<string> pkbResultForArg2;
	vector<vector<string>> modifiesResult;

	vecOfCandidates = pkbReadOnly.getAllStmt();
	for (int candidates : vecOfCandidates) {
		pkbModifies = pkbReadOnly.getModifies(candidates);
		for (string candidatesChosen : pkbModifies) {
			pkbResultForArg1.push_back(to_string(candidates));
			pkbResultForArg2.push_back(candidatesChosen);
		}
	}
	if (pkbResultForArg1.empty())
		hasModifies = false;
	else {
		pkbResultForArg1.push_back(arg1);
		pkbResultForArg2.push_back(arg2); //to denote this vector belongs to indicated synonym 
		modifiesResult.push_back(pkbResultForArg1);
		modifiesResult.push_back(pkbResultForArg2);
	}

	return make_tuple(hasModifies, modifiesResult);
}

bool ModifiesAnalyzer::checkClauseBothVariables(string arg1, string arg2)
{
	auto pkbResult = pkbReadOnly.getModifies(stoi(arg1));
	return find(pkbResult.begin(), pkbResult.end(), arg2) == pkbResult.end() ?
		false : true;
}

bool ModifiesAnalyzer::checkClauseVariableWild(string arg1)
{
	return pkbReadOnly.getModifies(stoi(arg1)).empty() ? false : true;
}

bool ModifiesAnalyzer::checkClauseWildVariable(string arg2)
{
	return pkbReadOnly.getModifiedBy(arg2).empty() ? false : true;
}

bool ModifiesAnalyzer::checkClauseBothWild()
{
	int minNoStmtsForModifies = 1;
	vector<int> allStmts = pkbReadOnly.getAllStmt();
	if(allStmts.size() < minNoStmtsForModifies)
		hasSuchThatClause = false;
	return hasSuchThatClause;
}
