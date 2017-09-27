#include "FollowsStarAnalyzer.h"
const string WILDCARD_SYMBOL = "_";

tuple<bool, vector<vector<string>>> FollowsStarAnalyzer::addArgTwoResult(string arg1)
{
	bool hasFollowsStar = true;
	vector<int> vecOfCandidates;
	vector<string> pkbResult;
	vector<int> pkbFollowsStar;
	vector<vector<string>> followsStarResult;

	if (arg1 == WILDCARD_SYMBOL)
		vecOfCandidates = pkbReadOnly.getAllStmt();
	else
		vecOfCandidates.push_back(stoi(arg1));
	for (int candidates : vecOfCandidates) {
		pkbFollowsStar = pkbReadOnly.getFollowsStar(candidates);
		for (int candidatesChosen : pkbFollowsStar) {
			pkbResult.push_back(to_string(candidatesChosen));
		}
	}
	if (pkbResult.empty())
		hasFollowsStar = false;
	else {
		pkbResult = removeDuplicates(pkbResult);
		pkbResult.push_back(arg1); //to denote this vector belongs to indicated synonym 
		followsStarResult.push_back(pkbResult);
	}

	return make_tuple(hasFollowsStar, followsStarResult);
}

tuple<bool, vector<vector<string>>> FollowsStarAnalyzer::addArgOneResult(string arg2)
{
	bool hasFollowsStar = true;
	vector<int> vecOfCandidates;
	vector<int> pkbFollowsStar;
	vector<string> pkbResult;
	vector<vector<string>> followsStarResult;

	if (arg2 == WILDCARD_SYMBOL)
		vecOfCandidates = pkbReadOnly.getAllStmt();
	else
		vecOfCandidates.push_back(stoi(arg2));
	for (int candidates : vecOfCandidates) {
		pkbFollowsStar = pkbReadOnly.getFollowedByStar(candidates);
		for (int candidatesChosen : pkbFollowsStar) {
			pkbResult.push_back(to_string(candidatesChosen));
		}
	}
	if (pkbResult.empty())
		hasFollowsStar = false;
	else {
		pkbResult = removeDuplicates(pkbResult);
		pkbResult.push_back(arg2); //to denote this vector belongs to indicated synonym 
		followsStarResult.push_back(pkbResult);
	}

	return make_tuple(hasFollowsStar, followsStarResult);
}

tuple<bool, vector<vector<string>>> FollowsStarAnalyzer::addBothSynResult(string arg1, string arg2)
{
	bool hasFollowsStar = true;
	vector<int> vecOfCandidates;
	vector<int> pkbFollowsStar;
	vector<string> pkbResultForArg1;
	vector<string> pkbResultForArg2;
	vector<vector<string>> followsStarResult;

	vecOfCandidates = pkbReadOnly.getAllStmt();
	for (int candidates : vecOfCandidates) {
		pkbFollowsStar = pkbReadOnly.getFollowsStar(candidates);
		for (int candidatesChosen : pkbFollowsStar) {
			pkbResultForArg1.push_back(to_string(candidates));
			pkbResultForArg2.push_back(to_string(candidatesChosen));
		}
		
	}
	if (pkbResultForArg1.empty())
		hasFollowsStar = false;
	else {
		pkbResultForArg1.push_back(arg1);
		pkbResultForArg2.push_back(arg2); //to denote this vector belongs to indicated synonym 
		followsStarResult.push_back(pkbResultForArg1);
		followsStarResult.push_back(pkbResultForArg2);
	}

	return make_tuple(hasFollowsStar, followsStarResult);
}

bool FollowsStarAnalyzer::checkClauseBothVariables(string arg1, string arg2)
{
	auto pkbResult = pkbReadOnly.getFollowsStar(stoi(arg1));
	return find(pkbResult.begin(), pkbResult.end(), stoi(arg2)) == pkbResult.end() ?
		false : true;
}

bool FollowsStarAnalyzer::checkClauseVariableWild(string arg1)
{
	return pkbReadOnly.getFollowsStar(stoi(arg1)).empty() ? false : true;
}

bool FollowsStarAnalyzer::checkClauseWildVariable(string arg2)
{
	return pkbReadOnly.getFollowedByStar(stoi(arg2)).empty() ? false : true;
}

bool FollowsStarAnalyzer::checkClauseBothWild()
{
	int minNoAssignForFollows = 2;
	vector<int> allAssign = pkbReadOnly.getAssign();
	if (allAssign.size() < minNoAssignForFollows)
		hasSuchThatClause = false;
	return hasSuchThatClause;
}
