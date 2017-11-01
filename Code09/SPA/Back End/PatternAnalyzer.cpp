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

enum patternAssignType
{
	literalWild, literalSubstring, literalExact,
	synWild, synSubstring, synExact,
	wildWild, wildSubstring, wildExact
};

const int IF = 0;
const int WHILE = 1;
const int GETSTMTS = 0;
const int GETVARS = 1;
const string SUBSTRING = "substring";
const string EXACT = "exact";

PatternAnalyzer::PatternAnalyzer(QueryElement patternClause, PKB &pkbRedOnly) {
	//pattern patSyn(arg1,patExp)
	pkbPtr = pkbRedOnly;
	patSyn = patternClause.getPatternSynonym();
	patSynEnt = patternClause.getPatternEntity();
	arg1 = patternClause.getPatternArg1();
	arg1Type = patternClause.getPatternArg1Type();
	patExp = Util::trimLead(patternClause.getPatternArg2());
	patExpType = patternClause.getPatternArg2Type();
	hasPatternClause = true;
	initMapPatternExpType();
}



tuple<bool, vector<vector<string>>> PatternAnalyzer::solvePatternAssign()
{
	/* pattern assign (arg1, arg2)
	 * case 0 : literal,wildcard
	 *		1 : literal,substring
	 *		2 : literal,exact
	 *		3 : synonym,wildcard
	 *		4 : synonym,substring
	 *		5 : synonym, exact
	 *		6 : wildcard,wildcard
	 *		7 : wildcard,substring
	 *		8 : wildcard,exact
	 */

	tuple<vector<string>, vector<string>> evaluatedPatResult;
	vector<string> validatedPatResult;
	vector<vector<string>> patternResult;
	int scenario = 0;

	tuple<vector<string>, vector<string>> pkbTupleResult;
	tuple<bool, vector<vector<string>>> result;
	vector<string> patSynResult;
	vector<string> patVarResult;

	if(arg1Type == SYNONYM) {scenario += 3;}
	else if (arg1Type == WILDCARD) { scenario += 6; }
	if (patExpType == SUBSTRING) {scenario += 1; }
	else if (patExpType == EXACT) { scenario += 2; }

	switch(scenario) {
		case literalWild: case literalSubstring: case literalExact:
			patSynResult = getAssignPatLiteral();
			break;
		case synWild: case synSubstring: case synExact:
			pkbTupleResult = getAssignPatSyn();
			patSynResult = get<GETSTMTS>(pkbTupleResult);
			patVarResult = get<GETVARS>(pkbTupleResult);
			break;
		case wildWild:
			patSynResult = Util::convertIntToString(pkbPtr.getAssign());
			patSynResult.push_back(patSyn);
			break;
		case wildSubstring:
		case wildExact:
			pkbTupleResult = getAssignPatSyn();
			patSynResult = get<GETSTMTS>(pkbTupleResult);
			break;
	}

	//if (arg1Type == SYNONYM) {
	//	evaluatedPatResult = solvePatSynAssign(arg1, patExp, patExpType, patSyn);
	//	if (!get<ARGONE>(evaluatedPatResult).empty()) {
	//		patternResult.push_back(get<ARGONE>(evaluatedPatResult));
	//		patternResult.push_back(get<ARGTWO>(evaluatedPatResult));
	//	} 
	//}
	//else {
	//	validatedPatResult = validatedPatSynAssign(arg1, patExp, patExpType, patSyn);
	//	if (!validatedPatResult.empty())
	//		patternResult.push_back(validatedPatResult);
	//}
	if (!patSynResult.empty())
		patternResult.push_back(patSynResult);
	if (!patVarResult.empty())
		patternResult.push_back(patVarResult);
		
	return make_tuple(hasPatternClause,patternResult);
}

vector<string> PatternAnalyzer::getAssignPatLiteral() {
	bool containsPattern = false;
	vector<string> stringResult;
	vector<int> result;
	if (patExpType == EXACT)
		result = pkbPtr.getPatternVariableExpression(arg1, patExp);
	else if (patExpType == SUBSTRING)
		result = pkbPtr.getPatternVariableExpressionSubstring(arg1, patExp);
	else
		result = get<GETSTMTS>(pkbPtr.getPatternVariable(arg1));
	if (!result.empty()) {
		stringResult = Util::convertIntToString(result);
		stringResult.push_back(patSyn); //for mapping
		containsPattern = true;
	}
	hasPatternClause = containsPattern;
	return stringResult;
}

tuple<vector<string>, vector<string>> PatternAnalyzer::getAssignPatSyn() {
	bool containsPattern = false;
	vector<string> stmtResult;
	vector<string> varResult;
	vector<int> result;

	if (patExpType == EXACT) {
		auto pkbResultTuple = pkbPtr.getPatternExpression(patExp);
		result = get<GETSTMTS>(pkbResultTuple);
		stmtResult = Util::convertIntToString(result);
		varResult = get<GETVARS>(pkbResultTuple);
	}
	else if (patExpType == SUBSTRING) {
		auto pkbResultTuple = pkbPtr.getPatternExpressionSubstring(patExp);
		result = get<GETSTMTS>(pkbResultTuple);
		stmtResult = Util::convertIntToString(result);
		varResult = get<GETVARS>(pkbResultTuple);
	}
	else { //wildcard
		vector<int> candidateList = pkbPtr.getAssign();
		for(auto candidate : candidateList) {
			vector<string> pkbResult = pkbPtr.getModifies(candidate);
			for(auto var: pkbResult) {
				stmtResult.push_back(to_string(candidate));
				varResult.push_back(var);
			}
		}
	}
		
	if (!stmtResult.empty()) {
		containsPattern = true;
		stmtResult.push_back(patSyn);
		varResult.push_back(arg1);
	}
	hasPatternClause = containsPattern;
	return make_tuple(stmtResult,varResult);
}

tuple<vector<string>, vector<string>> PatternAnalyzer::solvePatSynAssign(string arg1,
	string patExp, string patType, string patSyn) {
	vector<tuple<int, string>> pkbPatResult;
	bool containsPattern = false;
	vector<string> entityVector;
	vector<string> variableVector;
	//for (string candidates : pkbPtr.getAllVariables()) {
	//	pkbPatResult = pkbPtr.getPattern(candidates);
	//	for (tuple<int, string> evalPattExpression : pkbPatResult) {
	//		switch (mapPatternExpType[patType]) {
	//		case _wildcard_:
	//			entityVector.push_back(to_string(get<0>(evalPattExpression)));
	//			variableVector.push_back(candidates);
	//			containsPattern = true;
	//			break;
	//		case substring:
	//			if (get<1>(evalPattExpression).find(patExp) != string::npos) {
	//				entityVector.push_back(to_string(get<0>(evalPattExpression)));
	//				variableVector.push_back(candidates);
	//				containsPattern = true;
	//			}
	//			break;
	//		case exact:
	//			if (get<1>(evalPattExpression) == patExp) {
	//				entityVector.push_back(to_string(get<0>(evalPattExpression)));
	//				variableVector.push_back(candidates);
	//				containsPattern = true;
	//			}
	//			break;
	//		}
	//	}
	//}
	if (containsPattern) {
		entityVector.push_back(patSyn);
		variableVector.push_back(arg1);
	}
	hasPatternClause = containsPattern;
	return make_tuple(entityVector, variableVector);
}

vector<string> PatternAnalyzer::validatedPatSynAssign(string arg1, string patExp, string patExpType, string patSyn)
{
	vector<int> pkbStmtResult;
	vector<string> listOfCandidates;
	vector<string> result;
	bool containsPattern = false;
	vector<int> entityVector;
	
	listOfCandidates.push_back(arg1);
	for (string candidates : listOfCandidates) {
		pkbStmtResult = get<GETSTMTS>(pkbPtr.getPatternVariable(candidates));
		if (entityVector.empty()) {
			entityVector = pkbStmtResult;
		}
		else
			entityVector = intersectionT(pkbStmtResult, entityVector);
	}
	if (!entityVector.empty()) { //removing duplicates
		entityVector = Util::removeDuplicates(entityVector);
		for (auto candidate : entityVector)
			result.push_back(to_string(candidate));
		result.push_back(patSyn);
	}
	hasPatternClause = containsPattern;
	return result;
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

