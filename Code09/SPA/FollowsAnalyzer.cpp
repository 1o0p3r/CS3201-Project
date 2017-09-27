#include "FollowsAnalyzer.h"

const string WILDCARD_SYMBOL = "_";

tuple<bool, vector<vector<string>>> FollowsAnalyzer::addArgTwoResult(string arg1)
{
	bool hasFollows = true;
	vector<int> vecOfCandidates;
	vector<string> pkbResult;
	vector<int> pkbFollows;
	vector<vector<string>> followsResult;

	if (arg1 == WILDCARD_SYMBOL)
		vecOfCandidates = pkbReadOnly.getAllStmt();
	else
		vecOfCandidates.push_back(stoi(arg1));
	for (int candidates : vecOfCandidates) {
		pkbFollows = pkbReadOnly.getFollows(candidates);
		if(!pkbFollows.empty())
			pkbResult.push_back(to_string(pkbFollows.front()));
	}
	if (pkbResult.empty())
		hasFollows = false;
	else {
		pkbResult.push_back(arg1); //to denote this vector belongs to indicated synonym 
		followsResult.push_back(pkbResult);
	}

	return make_tuple(hasFollows, followsResult);
}

tuple<bool, vector<vector<string>>> FollowsAnalyzer::addArgOneResult(string arg2)
{
	bool hasFollows = true;
	vector<int> vecOfCandidates;
	vector<int> pkbFollows;
	vector<string> pkbResult;
	vector<vector<string>> followsResult;
	
	if (arg2 == WILDCARD_SYMBOL)
		vecOfCandidates = pkbReadOnly.getAllStmt();
	else
		vecOfCandidates.push_back(stoi(arg2));
	for (int candidates : vecOfCandidates) {
		pkbFollows = pkbReadOnly.getFollowedBy(candidates);
		if (!pkbFollows.empty())
			pkbResult.push_back(to_string(pkbFollows.front()));
	}
		if (pkbResult.empty())
		hasFollows = false;
	else {
		pkbResult.push_back(arg2); //to denote this vector belongs to indicated synonym 
		followsResult.push_back(pkbResult);
	}

	return make_tuple(hasFollows, followsResult);

}

tuple<bool, vector<vector<string>>> FollowsAnalyzer::addBothSynResult(string arg1, string arg2)
{
	bool hasFollows = true;
	vector<int> vecOfCandidates;
	vector<int> pkbFollows;
	vector<string> pkbResultForArg1;
	vector<string> pkbResultForArg2;
	vector<vector<string>> followsResult;

	vecOfCandidates = pkbReadOnly.getAllStmt();
	for (int candidates : vecOfCandidates) {
		pkbFollows = pkbReadOnly.getFollows(candidates);
		if (!pkbFollows.empty()) {
			pkbResultForArg1.push_back(to_string(candidates));
			pkbResultForArg2.push_back(to_string(pkbFollows.front()));
		}
	}
	if (pkbResultForArg1.empty())
		hasFollows = false;
	else {
		pkbResultForArg1.push_back(arg1);
		pkbResultForArg2.push_back(arg2); //to denote this vector belongs to indicated synonym 
		followsResult.push_back(pkbResultForArg1);
		followsResult.push_back(pkbResultForArg2);
	}

	return make_tuple(hasFollows, followsResult);
}

bool FollowsAnalyzer::checkClauseBothVariables(string arg1, string arg2)
{
	auto pkbResult = pkbReadOnly.getFollows(stoi(arg1));
	return find(pkbResult.begin(), pkbResult.end(), stoi(arg2)) == pkbResult.end() ?
		false : true;
}

bool FollowsAnalyzer::checkClauseVariableWild(string arg1)
{
	return pkbReadOnly.getFollows(stoi(arg1)).empty() ? false : true;
}

bool FollowsAnalyzer::checkClauseWildVariable(string arg2)
{
	return pkbReadOnly.getFollowedBy(stoi(arg2)).empty() ? false : true;
}

bool FollowsAnalyzer::checkClauseBothWild()
{	
	int minNoAssignForFollows = 2;
	vector<int> allAssign = pkbReadOnly.getAssign();
	if(allAssign.size() < minNoAssignForFollows)
		hasSuchThatClause = false;
	return hasSuchThatClause;
}
