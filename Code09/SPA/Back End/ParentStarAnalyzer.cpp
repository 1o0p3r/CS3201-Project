#include "ParentStarAnalyzer.h"

const string WILDCARD_SYMBOL = "_";

tuple<bool, vector<vector<string>>> ParentStarAnalyzer::addArgTwoResult(string arg1)
{
	bool hasParentStar = true;
	vector<int> vecOfCandidates;
	vector<string> pkbResult;
	vector<int> pkbParentStar;
	vector<vector<string>> parentStarResult;

	if (arg1 == WILDCARD_SYMBOL)
		vecOfCandidates = pkbReadOnly.getAllStmt();
	else
		vecOfCandidates.push_back(stoi(arg1));
	for (int candidates : vecOfCandidates) {
		pkbParentStar = pkbReadOnly.getChildStar(candidates);
		pkbParentStar = validatePKBResultsInt(arg2Entity, pkbParentStar);
		for (int candidatesChosen : pkbParentStar) {
			pkbResult.push_back(to_string(candidatesChosen));
		}
	}
	if (pkbResult.empty())
		hasParentStar = false;
	else {
		pkbResult = removeDuplicates(pkbResult);
		pkbResult.push_back(arg2); //to denote this vector belongs to indicated synonym 
		parentStarResult.push_back(pkbResult);
	}

	return make_tuple(hasParentStar, parentStarResult);
}

tuple<bool, vector<vector<string>>> ParentStarAnalyzer::addArgOneResult(string arg2)
{
	bool hasParentStar = true;
	vector<int> vecOfCandidates;
	vector<int> pkbParentStar;
	vector<string> pkbResult;
	vector<vector<string>> parentStarResult;

	if (arg2 == WILDCARD_SYMBOL)
		vecOfCandidates = pkbReadOnly.getAllStmt();
	else
		vecOfCandidates.push_back(stoi(arg2));
	for (int candidates : vecOfCandidates) {
		pkbParentStar = pkbReadOnly.getParentStar(candidates);
		pkbParentStar = validatePKBResultsInt(arg1Entity, pkbParentStar);
		for (int candidatesChosen : pkbParentStar) {
			pkbResult.push_back(to_string(candidatesChosen));
		}
	}
	if (pkbResult.empty())
		hasParentStar = false;
	else {
		pkbResult = removeDuplicates(pkbResult);
		pkbResult.push_back(arg1); //to denote this vector belongs to indicated synonym 
		parentStarResult.push_back(pkbResult);
	}

	return make_tuple(hasParentStar, parentStarResult);
}

tuple<bool, vector<vector<string>>> ParentStarAnalyzer::addBothSynResult(string arg1, string arg2)
{
	bool hasParentStar = true;
	vector<int> vecOfCandidates;
	vector<int> pkbParentStar;
	vector<string> pkbResultForArg1;
	vector<string> pkbResultForArg2;
	vector<vector<string>> parentStarResult;

	vecOfCandidates = pkbReadOnly.getAllStmt(); 
	vecOfCandidates = validatePKBResultsInt(arg1Entity, vecOfCandidates);
	for (int candidates : vecOfCandidates) {
		pkbParentStar = pkbReadOnly.getChildStar(candidates);
		pkbParentStar = validatePKBResultsInt(arg2Entity, pkbParentStar);
		for (int candidatesChosen : pkbParentStar) {
			pkbResultForArg1.push_back(to_string(candidates));
			pkbResultForArg2.push_back(to_string(candidatesChosen));
		}
	}
	if (pkbResultForArg1.empty())
		hasParentStar = false;
	else {
		pkbResultForArg1.push_back(arg1);
		pkbResultForArg2.push_back(arg2); //to denote this vector belongs to indicated synonym 
		parentStarResult.push_back(pkbResultForArg1);
		parentStarResult.push_back(pkbResultForArg2);
	}

	return make_tuple(hasParentStar, parentStarResult);
}

bool ParentStarAnalyzer::checkClauseBothVariables(string arg1, string arg2)
{
	auto pkbResult = pkbReadOnly.getChildStar(stoi(arg1));
	return find(pkbResult.begin(), pkbResult.end(), stoi(arg2)) == pkbResult.end() ?
		false : true;
}

bool ParentStarAnalyzer::checkClauseVariableWild(string arg1)
{
	return pkbReadOnly.getChildStar(stoi(arg1)).empty() ? false : true;
}

bool ParentStarAnalyzer::checkClauseWildVariable(string arg2)
{
	return pkbReadOnly.getParentStar(stoi(arg2)).empty() ? false : true;
}

bool ParentStarAnalyzer::checkClauseBothWild()
{	
	int MIN_STMTS_FOR_PARENT = 1;
	return (pkbReadOnly.getIf().size() < MIN_STMTS_FOR_PARENT &&
			pkbReadOnly.getWhile().size() < MIN_STMTS_FOR_PARENT) ? false : true;
}
