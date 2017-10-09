#include "ModifiesAnalyzer.h"

const string WILDCARD_SYMBOL = "_";
const string PROCEDURE = "procedure";

tuple<bool, vector<vector<string>>> ModifiesAnalyzer::addArgTwoResult(string arg1)
{
	bool hasModifies = true;
	vector<string> vecOfCandidates;
	vector<string> pkbResult;
	vector<string> pkbModifies;
	vector<vector<string>> modifiesResult;

	//semantic error check by preprocessor
	/*
	if (arg1 == WILDCARD_SYMBOL)
		return make_tuple(false, vector<vector<string>>());
	*/
	vecOfCandidates.push_back(arg1);
	for (string candidates : vecOfCandidates) {
		pkbModifies = getModifiesResultAddArg2(candidates, arg2Entity);
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
	vector<string> pkbModifies;
	vector<string> pkbResult;
	vector<vector<string>> modifiesResult;

	if (arg2 == WILDCARD_SYMBOL) {
		vecOfCandidates = pkbReadOnly.getAllVariables();
		if (unitTestModeOn) {
			vecOfCandidates = unitTestInputs[inputHardCodeIndex];
			inputHardCodeIndex++;
		}
	}
	else
		vecOfCandidates.push_back(arg2);
	for (string candidates : vecOfCandidates) {
		pkbModifies = getModifiesResultAddArg1(candidates, arg1Entity);
		for (string candidatesChosen : pkbModifies) {
			pkbResult.push_back(candidatesChosen);
		}
	}
	if (pkbResult.empty())
		hasModifies = false;
	else {
		pkbResult.push_back(arg1); //to denote this vector belongs to indicated synonym 
		modifiesResult.push_back(pkbResult);
	}

	return make_tuple(hasModifies, modifiesResult);
}

tuple<bool, vector<vector<string>>> ModifiesAnalyzer::addBothSynResult(string arg1, string arg2)
{
	bool hasModifies = true;
	vector<string> vecOfCandidates;
	vector<string> pkbModifies;
	vector<string> pkbResultForArg1;
	vector<string> pkbResultForArg2;
	vector<vector<string>> modifiesResult;

	vecOfCandidates = pkbReadOnly.getAllVariables();
	if (unitTestModeOn) {
		vecOfCandidates = unitTestInputs[inputHardCodeIndex];
		inputHardCodeIndex++;
	}
	for (string candidates : vecOfCandidates) {
		pkbModifies = getModifiesResultAddArg1(candidates,arg1Entity);
		for (string candidatesChosen : pkbModifies) {
			pkbResultForArg1.push_back(candidatesChosen);
			pkbResultForArg2.push_back(candidates);
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
	if (unitTestModeOn) {
		return(unitTestInputs[inputHardCodeIndex].size());
	}
	auto pkbResult = pkbReadOnly.getModifies(stoi(arg1));
	return find(pkbResult.begin(), pkbResult.end(), arg2) == pkbResult.end() ?
		false : true;
}

bool ModifiesAnalyzer::checkClauseVariableWild(string arg1)
{
	if (unitTestModeOn) {
		return(unitTestInputs[inputHardCodeIndex].size());
	}
	if (arg1Type == PROCEDUREARG)
		return pkbReadOnly.getProcModifies(arg1).empty() ? false : true;
	return pkbReadOnly.getModifies(stoi(arg1)).empty() ? false : true;
}

bool ModifiesAnalyzer::checkClauseWildVariable(string arg2)
{	
	assert(arg1 == WILDCARD_SYMBOL);
	if (unitTestModeOn) {
		return(unitTestInputs[inputHardCodeIndex].size());
	}
	return pkbReadOnly.getModifiedBy(arg2).empty() ? false : true;
}

bool ModifiesAnalyzer::checkClauseBothWild()
{
	if (unitTestModeOn) {
		return(unitTestInputs[inputHardCodeIndex].size());
	}
	int minNoStmtsForModifies = 1;
	vector<int> allStmts = pkbReadOnly.getAllStmt();
	if(allStmts.size() < minNoStmtsForModifies)
		hasSuchThatClause = false;
	return hasSuchThatClause;
}

vector<string> ModifiesAnalyzer::getModifiesResultAddArg2(string arg1, string arg2Entity)
{	
	if (unitTestModeOn) {
		auto result = (unitTestInputs[inputHardCodeIndex]);
		inputHardCodeIndex++;
		return result;
	}
	return (arg1Type == PROCEDUREARG) ? pkbReadOnly.getProcModifies(arg1):
			pkbReadOnly.getModifies(stoi(arg1));
}

//@param: must be procedure entity for Arg1
vector<string> ModifiesAnalyzer::getModifiesResultAddArg1(string arg2, string arg1Entity)
{
	vector<int> pkbResultInt;
	vector<string> pkbResult;

	if (unitTestModeOn) {
		auto result = (unitTestInputs[inputHardCodeIndex]);
		inputHardCodeIndex++;
		return result;
	}
	if(arg1Entity == PROCEDURE)
		pkbResult = pkbReadOnly.getProcModifiedBy(arg2);
	else {
		pkbResultInt = pkbReadOnly.getModifiedBy(arg2);
		for (int entryToString : pkbResultInt)
			pkbResult.push_back(to_string(entryToString));
	}
	return pkbResult;
	
}
