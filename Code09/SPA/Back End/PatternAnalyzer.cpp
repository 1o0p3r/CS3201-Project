#include "PatternAnalyzer.h"

enum patternExpType
{
	undefinedExpression, exact, substring, _wildcard_
};
map<string, patternExpType> mapPatternExpType;

void PatternAnalyzer::initMapPatternExpType() {
	mapPatternExpType["exact"] = exact;
	mapPatternExpType["substring"] = substring;
	mapPatternExpType["wildcard"] = _wildcard_;
}

const int IF = 0;
const int WHILE = 1;

PatternAnalyzer::PatternAnalyzer(QueryElement patternClause, PKB &pkbRedOnly) {
	pkbPtr = pkbRedOnly;
	patSyn = patternClause.getPatternSynonym();
	patSynEnt = patternClause.getPatternEntity();
	arg1 = patternClause.getPatternArg1();
	arg1Type = patternClause.getPatternArg1Type();
	patExp = patternClause.getPatternArg2();
	patExpType = patternClause.getPatternArg2Type();
	hasPatternClause = true;
	initMapPatternExpType();
}

tuple<bool, vector<vector<string>>> PatternAnalyzer::solvePatternAssign()
{
	tuple<vector<string>, vector<string>> evaluatedPatResult;
	vector<string> validatedPatResult;
	vector<vector<string>> patternResult;
	if (arg1Type == SYNONYM) {
		evaluatedPatResult = solvePatSynAssign(arg1, patExp, patExpType, patSyn);
		if (!get<ARGONE>(evaluatedPatResult).empty()) {
			patternResult.push_back(get<ARGONE>(evaluatedPatResult));
			patternResult.push_back(get<ARGTWO>(evaluatedPatResult));
		} 
	}
	else {
		validatedPatResult = validatedPatSynAssign(arg1, patExp, patExpType, patSyn);
		if (!validatedPatResult.empty())
			patternResult.push_back(validatedPatResult);
	}
	if (patternResult.empty())
		hasPatternClause = false;
	return make_tuple(hasPatternClause,patternResult);

}

tuple<vector<string>, vector<string>> PatternAnalyzer::solvePatSynAssign(string arg1,
	string patExp, string patType, string patSyn) {
	vector<tuple<int, string>> pkbPatResult;
	bool containsPattern = false;
	vector<string> entityVector;
	vector<string> variableVector;
	for (string candidates : pkbPtr.getAllVariables()) {
		pkbPatResult = pkbPtr.getPattern(candidates);
		for (tuple<int, string> evalPattExpression : pkbPatResult) {
			switch (mapPatternExpType[patType]) {
			case _wildcard_:
				entityVector.push_back(to_string(get<0>(evalPattExpression)));
				variableVector.push_back(candidates);
				containsPattern = true;
				break;
			case substring:
				if (get<1>(evalPattExpression).find(patExp) != string::npos) {
					entityVector.push_back(to_string(get<0>(evalPattExpression)));
					variableVector.push_back(candidates);
					containsPattern = true;
				}
				break;
			case exact:
				if (get<1>(evalPattExpression) == patExp) {
					entityVector.push_back(to_string(get<0>(evalPattExpression)));
					variableVector.push_back(candidates);
					containsPattern = true;
				}
				break;
			}
		}
	}
	if (containsPattern) {
		entityVector.push_back(patSyn);
		variableVector.push_back(arg1);
	}
	hasPatternClause = containsPattern;
	return make_tuple(entityVector, variableVector);
}

vector<string> PatternAnalyzer::validatedPatSynAssign(string arg1, string patExp, string patExpType, string patSyn)
{
	vector<tuple<int, string>> pkbPatResult;
	bool containsPattern = false;
	vector<string> entityVector;
	unordered_set<string> shortlisted; // for removing duplicates
	vector<string> listOfCandidates;
	if (arg1 == WILDCARD_SYMBOL)
		listOfCandidates = pkbPtr.getAllVariables();
	else
		listOfCandidates.push_back(arg1);
	for (string candidates : listOfCandidates) {
		pkbPatResult = pkbPtr.getPattern(candidates);
		for (tuple<int, string> evalPattExpression : pkbPatResult) {
			switch (mapPatternExpType[patExpType]) {
			case _wildcard_:
				shortlisted.insert(to_string(get<0>(evalPattExpression)));
				containsPattern = true;
				break;
			case substring:
				if (get<1>(evalPattExpression).find(patExp) != string::npos) {
					shortlisted.insert(to_string(get<0>(evalPattExpression)));
					containsPattern = true;
				}
				break;
			case exact:
				if (get<1>(evalPattExpression) == patExp) {
					shortlisted.insert(to_string(get<0>(evalPattExpression)));
					containsPattern = true;
				}
				break;
			}
		}
	}
	if (!shortlisted.empty()) { //removing duplicates
		entityVector.assign(shortlisted.begin(), shortlisted.end());
		entityVector.push_back(patSyn);
	}
	hasPatternClause = containsPattern;
	return entityVector;
}

tuple<bool, vector<vector<string>>> PatternAnalyzer::solvePatternIfWhile()
{
	tuple<vector<string>, vector<string>> evaluatedPatResult;
	vector<string> validatedPatResult;
	vector<vector<string>> patternResult;

	if (arg1Type == SYNONYM) {
		evaluatedPatResult = solvePatSynIfWhile(arg1, patSyn, patSynEnt);
		if (!get<ARGONE>(evaluatedPatResult).empty()) {
			patternResult.push_back(get<ARGONE>(evaluatedPatResult));
			patternResult.push_back(get<ARGTWO>(evaluatedPatResult));
		}
	}
	else {
		validatedPatResult = validatedSynIfWhile(arg1, patSyn, patSynEnt);
		if (!validatedPatResult.empty())
			patternResult.push_back(validatedPatResult);
	}
	if (patternResult.empty())
		hasPatternClause = false;
	return make_tuple(hasPatternClause, patternResult);

}

vector<int> PatternAnalyzer::getPKBPatternIfWhile(string candidates)
{
	if(patSynEnt == "if") return pkbPtr.getPatternIf(candidates);
	if (patSynEnt == "while") return pkbPtr.getPatternWhile(candidates);
	return{};
}

void PatternAnalyzer::getPatSynIfWhile( vector<string> candidateList,
	vector<string> *entityVector, vector<string> *variableVector, bool *hasPattern)
{
	vector<int> statementResult;
	for (string candidates : candidateList) {
		statementResult = getPKBPatternIfWhile(candidates);
		for (auto elements : statementResult) {
			entityVector->push_back(to_string(elements));
			variableVector->push_back(candidates);
		}
	}
	if (!entityVector->empty())
		*hasPattern = true;
}

tuple<vector<string>, vector<string>> PatternAnalyzer::solvePatSynIfWhile(string arg1, string patSyn, string patSynEnt)
{
	vector<tuple<int, string>> pkbPatResult;
	bool containsPattern = false;
	vector<string> entityVector;
	vector<string> variableVector;
	vector<int> statementResult;
	getPatSynIfWhile(pkbPtr.getAllVariables(),&entityVector, &variableVector, &containsPattern);	
	
	if (containsPattern) {
		entityVector.push_back(patSyn);
		variableVector.push_back(arg1);
	}
	hasPatternClause = containsPattern;
	return make_tuple(entityVector, variableVector);
}

vector<string> PatternAnalyzer::validatedSynIfWhile(string arg1, string patSyn, string patSynEnt)
{
	vector<tuple<int, string>> pkbPatResult;
	bool containsPattern = false;
	vector<string> entityVector;
	vector<string> variableVector;
	vector<int> statementResult;
	if(arg1 != WILDCARD_SYMBOL)
		getPatSynIfWhile({ arg1 }, &entityVector, &variableVector, &containsPattern);
	else if (patSynEnt == "while") statementResult = pkbPtr.getWhile();
	else if (patSynEnt == "if") statementResult = pkbPtr.getIf();

	if(!statementResult.empty())
	{
		for (int element : statementResult)
			entityVector.push_back(to_string(element));
		containsPattern = true;
	}

	if (containsPattern) {
		entityVector.push_back(patSyn);
		variableVector.push_back(arg1);
	}
	hasPatternClause = containsPattern;
	return entityVector;
}

