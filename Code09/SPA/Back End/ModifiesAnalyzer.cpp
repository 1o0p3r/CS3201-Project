#include "ModifiesAnalyzer.h"

const string PROCEDURE = "procedure";

tuple<bool, vector<vector<string>>> ModifiesAnalyzer::addArgTwoResult(string arg1)
{
	bool hasModifies = true;
	vector<string> vecOfCandidates;
	vector<string> pkbResult;
	vector<string> pkbModifies;
	vector<vector<string>> modifiesResult;


	vecOfCandidates.push_back(arg1);
	for (string candidates : vecOfCandidates) {
		pkbModifies = getModifiesResultAddArg2(candidates);
		for (string candidatesChosen : pkbModifies) {
			pkbResult.push_back(candidatesChosen);
		}
	}
	if (pkbResult.empty())
		hasModifies = false;
	else {
		pkbResult = removeDuplicates(pkbResult);
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
		vecOfCandidates = pkbReadOnly.getAllModifiedVariables();
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
		pkbResult = removeDuplicates(pkbResult);
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

	bool hasArg1EvalBefore = false;
	const auto synArg1Iterator = queryMap.find(arg1);
	const auto synArg2Iterator = queryMap.find(arg2);

	if (synArg1Iterator == queryMap.end() && synArg2Iterator == queryMap.end()) {
		vecOfCandidates = pkbReadOnly.getAllModifiedVariables();;
	}  else
		getArgsPriorStringResults(vecOfCandidates, hasArg1EvalBefore, synArg2Iterator, synArg1Iterator);

	if (unitTestModeOn) {
		vecOfCandidates = unitTestInputs[inputHardCodeIndex];
		inputHardCodeIndex++;
	}

	for (string candidates : vecOfCandidates) {
		if (!hasArg1EvalBefore)
			pkbModifies = getModifiesResultAddArg1(candidates, arg1Entity);
		else
			pkbModifies = getModifiesResultAddArg2(candidates);
		for (string candidatesChosen : pkbModifies) {
			if (!hasArg1EvalBefore) {
				pkbResultForArg1.push_back(candidatesChosen);
				pkbResultForArg2.push_back(candidates);
			} else {
				pkbResultForArg1.push_back(candidates);
				pkbResultForArg2.push_back(candidatesChosen);
			}
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
	vector<string> pkbResult;
	if (unitTestModeOn) {
		return(unitTestInputs[inputHardCodeIndex].size());
	}
	if (arg1Type == PROCEDUREARG)
		pkbResult = pkbReadOnly.getProcModifiedBy(arg2);
	else {
		auto pkbResultInt = pkbReadOnly.getModifiedBy(arg2);
		for (int candidate : pkbResultInt)
			pkbResult.push_back(to_string(candidate));
	}
	return find(pkbResult.begin(), pkbResult.end(), arg1) == pkbResult.end() ?
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

vector<string> ModifiesAnalyzer::getModifiesResultAddArg2(string arg1)
{	
	vector<string> pkbResult;
	if (unitTestModeOn) {
		auto result = (unitTestInputs[inputHardCodeIndex]);
		inputHardCodeIndex++;
		return result;
	}
	if (arg1Type == PROCEDUREARG)
		return pkbReadOnly.getProcModifies(arg1);
	else {
		pkbResult = pkbReadOnly.getModifies(stoi(arg1));
	}
	return pkbResult;
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
		pkbResultInt = validatePKBResultsInt(arg1Entity, pkbResultInt);
		for (int entryToString : pkbResultInt)
			pkbResult.push_back(to_string(entryToString));
	}
	return pkbResult;
	
}
