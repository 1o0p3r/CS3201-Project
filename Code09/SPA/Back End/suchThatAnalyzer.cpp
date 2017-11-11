#include "SuchThatAnalyzer.h"



enum clauseCases 
{
	varVar, varSyn, varWild, synVar, synSyn, synWild,
	wildVar, wildSyn, wildWild
};

suchThatAnalyzer::suchThatAnalyzer(QueryElement stClause, PKB pkb, 
		vector<vector<vector<string>>> const &qTable, unordered_map<string, tuple<int, int>> const &qMap)
{ 
	pkbReadOnly = pkb;
	queryTable = qTable;
	queryMap = qMap;

	stEntity = stClause.getSuchThatRel();
	arg1 = stClause.getSuchThatArg1();
	arg1Entity = stClause.getSuchThatArg1Entity();
	arg1Type = stClause.getSuchThatArg1Type();
	
	arg2 = stClause.getSuchThatArg2();
	arg2Entity = stClause.getSuchThatArg2Entity();
	arg2Type = stClause.getSuchThatArg2Type();

	hasSuchThatClause = true;
	unitTestModeOn = false;
}

tuple<bool, vector<vector<string>>> suchThatAnalyzer::solveClause()
{
	/* arg1, arg2  = case
	* stmtVersion = syn, wildcard, var 9 cases, syn , var, wilcard
	* procVersion = syn, wildcard, varInt, varString 12 cases
	*/
	tuple<bool,vector<vector<string>>> stResult;
	stResult = solveClauseStmt();
	return stResult;

}

tuple<bool,vector<vector<string>>> suchThatAnalyzer::solveClauseStmt() 
{	/*Cases
	Arg1 Arg2
	int int
	int syn
	int wild
	syn int
	syn syn
	syn	wild
	wild int
	wild syn
	wild wild
	*/
	int scenarioArg1Arg2;
	tuple<bool,vector<vector<string>>> clauseResult;
	vector<vector<string>> emptyVec;

	scenarioArg1Arg2 = 0; //base case
	if (arg1Type == "synonym") { scenarioArg1Arg2 += 3; }
	else if (arg1Type == WILDCARD) { scenarioArg1Arg2 += 6; }
	if (arg2Type == "synonym") { scenarioArg1Arg2 += 1; }
	else if (arg2Type == WILDCARD) { scenarioArg1Arg2 += 2; }

	switch (scenarioArg1Arg2) {
	case varVar:
		hasSuchThatClause = checkClauseTrue(arg1, arg2, scenarioArg1Arg2);
		clauseResult = make_tuple(hasSuchThatClause, emptyVec);
		break;
	case varSyn:
		clauseResult = addClauseTable(arg1, arg2, scenarioArg1Arg2);		
		break;
	case varWild:
		hasSuchThatClause = checkClauseTrue(arg1, arg2, scenarioArg1Arg2);
		clauseResult = make_tuple(hasSuchThatClause, emptyVec);
		break;
	case synVar:
		clauseResult = addClauseTable(arg1, arg2, scenarioArg1Arg2);
		break;
	case synSyn:
		clauseResult = addClauseTable(arg1, arg2, scenarioArg1Arg2);
		break;
	case synWild:
		clauseResult = addClauseTable(arg1, arg2, scenarioArg1Arg2);
		break;
	case wildVar:
		hasSuchThatClause = checkClauseTrue(arg1, arg2, scenarioArg1Arg2);
		clauseResult = make_tuple(hasSuchThatClause, emptyVec);
		break;
	case wildSyn:
		clauseResult = addClauseTable(arg1, arg2, scenarioArg1Arg2);
		break;
	case wildWild:
		hasSuchThatClause = checkClauseTrue(arg1, arg2, scenarioArg1Arg2);
		clauseResult = make_tuple(hasSuchThatClause, emptyVec);
		break;
	}
	return clauseResult;
}

bool suchThatAnalyzer::checkClauseTrue(string arg1, string arg2, int scenarioArg1Arg2)
{	//overwrite methods in switch in subclass
	vector<string> result;
	bool boolResult;
	switch (scenarioArg1Arg2) {
	case varVar:
		boolResult = checkClauseBothVariables(arg1,arg2);
		break;
	case varWild:
		boolResult = checkClauseVariableWild(arg1);
		break;
	case wildVar:
		boolResult = checkClauseWildVariable(arg2);
		break;
	case wildWild:
		boolResult = checkClauseBothWild();
		break;
	}
	return boolResult;
}

tuple<bool, vector<vector<string>>> suchThatAnalyzer::addClauseTable(string arg1, string arg2, int scenarioArg1Arg2)
{
	//overwrite this method in child class
	tuple<bool,vector<vector<string>>> clauseResult;

	switch (scenarioArg1Arg2) {
	case varSyn:
		clauseResult = addArgTwoResult(arg1);
		break;
	case synVar:
		clauseResult = addArgOneResult(arg2);
		break;
	case synSyn:
		clauseResult = addBothSynResult(arg1, arg2);
		break;
	case synWild:
		clauseResult = addArgOneResult(arg2);
		break;
	case wildSyn:
		clauseResult = addArgTwoResult(arg1);
		break;
	}
	return clauseResult;
}

tuple<bool, vector<vector<string>>> suchThatAnalyzer::addArgTwoResult(string arg1)
{
	//overwrite this method in child class

	return tuple<bool, vector<vector<string>>>();
}

tuple<bool, vector<vector<string>>> suchThatAnalyzer::addArgOneResult(string arg2)
{
	//overwrite this method in child class

	return tuple<bool, vector<vector<string>>>();
}

tuple<bool, vector<vector<string>>> suchThatAnalyzer::addBothSynResult(string arg1, string arg2)
{
	//overwrite this method in child class

	return tuple<bool, vector<vector<string>>>();
}

bool suchThatAnalyzer::checkClauseBothVariables(string arg1, string arg2)
{
	//overwrite this method in child class
	return false;
}

bool suchThatAnalyzer::checkClauseVariableWild(string arg1)
{
	//overwrite this method in child class
	return false;
}

bool suchThatAnalyzer::checkClauseWildVariable(string arg2)
{
	//overwrite this method in child class
	return false;
}

bool suchThatAnalyzer::checkClauseBothWild()
{
	//overwrite this method in child class
	return false;
}

vector<string> suchThatAnalyzer::optimizedAddArg(const unordered_map<string, tuple<int, int>>::iterator synArgIterator,
                                                 const bool isAddArg1, const bool isArgTypeInt) {
	vector<string> pkbResult;
	tuple<int, int> synLocation = synArgIterator->second;
	auto qTableResult = queryTable[get<TABLELOC>(synLocation)][get<SYNVECLOC>(synLocation)];
	qTableResult.pop_back(); // remove mapping indicator
	qTableResult = removeDuplicates(qTableResult);
	if (isArgTypeInt) {
		vector<int> vecOfCandidates = Util::convertStringToInt(qTableResult);
		for (int candidates : vecOfCandidates) {
			if (hasResultsForArg(candidates, isAddArg1)) {
				pkbResult.push_back(to_string(candidates));
			}
		}
	} else {
		for (const string candidates : qTableResult) {
			if (hasResultsForArg(candidates, isAddArg1)) {
				pkbResult.push_back(candidates);
			}
		}
	}
	return pkbResult;
}

void suchThatAnalyzer::getArgsPriorResults(vector<int>& vecOfCandidates, bool& hasArg2EvalBefore,
                                           const unordered_map<string, tuple<int, int>>::iterator synArg1Iterator, const unordered_map<string, tuple<int, int>>::
                                           iterator synArg2Iterator)
{
	tuple<int, int> synLocation1;
	tuple<int, int> synLocation2;
	vector<string> qTableResult;
	bool hasArg1EvalBefore = false;
	if (synArg1Iterator != queryMap.end()) {
		synLocation1 = synArg1Iterator->second;
		hasArg1EvalBefore = true;
	}
	if (synArg2Iterator != queryMap.end()) {
		synLocation2 = synArg2Iterator->second;
		hasArg2EvalBefore = true;
	}

	if (hasArg1EvalBefore && hasArg2EvalBefore) {
		if (queryTable[get<TABLELOC>(synLocation1)][get<SYNVECLOC>(synLocation1)].size() >
			queryTable[get<TABLELOC>(synLocation2)][get<SYNVECLOC>(synLocation2)].size())
			qTableResult = queryTable[get<TABLELOC>(synLocation2)][get<SYNVECLOC>(synLocation2)];
		else {
			qTableResult = queryTable[get<TABLELOC>(synLocation1)][get<SYNVECLOC>(synLocation1)];
			hasArg2EvalBefore = false;
		}
	}
	else if (hasArg1EvalBefore) {
		qTableResult = queryTable[get<TABLELOC>(synLocation1)][get<SYNVECLOC>(synLocation1)];
	}
	else if (hasArg2EvalBefore) {
		qTableResult = queryTable[get<TABLELOC>(synLocation2)][get<SYNVECLOC>(synLocation2)];
	}
	if (!qTableResult.empty()) {
		qTableResult.pop_back(); // remove mapping indicator
		qTableResult = removeDuplicates(qTableResult);
		vecOfCandidates = Util::convertStringToInt(qTableResult);
	}
}

void suchThatAnalyzer::getArgsPriorStringResults(vector<string>& vecOfCandidates, bool& hasArg2EvalBefore,
	const unordered_map<string, tuple<int, int>>::iterator synArg1Iterator, const unordered_map<string, tuple<int, int>>::
	iterator synArg2Iterator)
{
	tuple<int, int> synLocation1;
	tuple<int, int> synLocation2;
	vector<string> qTableResult;
	bool hasArg1EvalBefore = false;
	if (synArg1Iterator != queryMap.end()) {
		synLocation1 = synArg1Iterator->second;
		hasArg1EvalBefore = true;
	}
	if (synArg2Iterator != queryMap.end()) {
		synLocation2 = synArg2Iterator->second;
		hasArg2EvalBefore = true;
	}

	if (hasArg1EvalBefore && hasArg2EvalBefore) {
		if (queryTable[get<TABLELOC>(synLocation1)][get<SYNVECLOC>(synLocation1)].size() >
			queryTable[get<TABLELOC>(synLocation2)][get<SYNVECLOC>(synLocation2)].size())
			qTableResult = queryTable[get<TABLELOC>(synLocation2)][get<SYNVECLOC>(synLocation2)];
		else {
			qTableResult = queryTable[get<TABLELOC>(synLocation1)][get<SYNVECLOC>(synLocation1)];
			hasArg2EvalBefore = false;
		}
	}
	else if (hasArg1EvalBefore) {
		qTableResult = queryTable[get<TABLELOC>(synLocation1)][get<SYNVECLOC>(synLocation1)];
	}
	else if (hasArg2EvalBefore) {
		qTableResult = queryTable[get<TABLELOC>(synLocation2)][get<SYNVECLOC>(synLocation2)];
	}
	if (!qTableResult.empty()) {
		qTableResult.pop_back(); // remove mapping indicator
		vecOfCandidates = removeDuplicates(qTableResult);
		
	}
}

void suchThatAnalyzer::getValuesFromPKB(vector<int>& retrievedPKBValues, bool hasArg2EvalBefore, int candidates)
{
	//overwrite in child
}

void suchThatAnalyzer::getValuesFromPKB(vector<string>& retrievedPKBValues, bool hasArg2EvalBefore, string candidates)
{
	//overwrite in child
}

bool suchThatAnalyzer::hasResultsForArg(const int candidates, const bool isAddArg1)
{
	//overwrite this method in child class
	return false;
}

bool suchThatAnalyzer::hasResultsForArg(const string candidates, const bool isAddArg1)
{
	//overwrite this method in child class
	return false;
}

vector<int> suchThatAnalyzer::validatePKBResultsInt(string ent, vector<int> validateVec)
{
	vector<int> pkbResult;

	if(ent == "stmt" || ent == "prog_line")  pkbResult = pkbReadOnly.getAllStmt();
	else if (ent == "assign") pkbResult = pkbReadOnly.getAssign();
	else if (ent == "while") pkbResult = pkbReadOnly.getWhile();
	else if (ent == "if") pkbResult = pkbReadOnly.getIf();
	else if (ent == "call") pkbResult = pkbReadOnly.getCall();
	
	if (ent == "constant") {
		vector<string> pkbStringResult = pkbReadOnly.getAllConstants();
		for (auto entry : pkbStringResult)
			pkbResult.push_back(stoi(entry));
	}
	return intersectionT(pkbResult, validateVec);
	
}

vector<string> suchThatAnalyzer::validatePKBResultsString(string ent, vector<string> validateVec)
{	
	vector<string> pkbResult;
	if(ent == "variable")  
		pkbResult = pkbReadOnly.getAllVariables();
	else 
		pkbResult = pkbReadOnly.getAllProcedures();
	
	return intersectionT(pkbResult, validateVec);
}



vector<string> suchThatAnalyzer::removeDuplicates(vector<string> clauseResult) {
	unordered_set<string> shortlisted;
	vector<string> answer;
	for (string entry : clauseResult) {
		shortlisted.insert(entry);
	}

	if (!shortlisted.empty()) { //removing duplicates and sort
		answer.assign(shortlisted.begin(), shortlisted.end());
		sort(answer.begin(), answer.end());
	}

	return answer;
}

vector<string> suchThatAnalyzer::getPKBAllArgValues()
{	//override in child function
	return vector<string>();
}


void suchThatAnalyzer::setUnitTestInputs(vector<vector<string>> hcInput)
{
	unitTestModeOn = true;
	unitTestInputs = hcInput;
	inputHardCodeIndex = 0;
}

vector<string> suchThatAnalyzer::getUnitTestInputs()
{
	vector<string> result = unitTestInputs[inputHardCodeIndex];
	inputHardCodeIndex++;
	return result;
}

vector<string> suchThatAnalyzer::getArgEntityResults(const string& argEnt) {
	vector<string> pkbResult;
	
	if (argEnt == "stmt" || argEnt == "prog_line")  pkbResult = Util::convertIntToString(pkbReadOnly.getAllStmt());
	else if (argEnt == "assign") pkbResult = Util::convertIntToString(pkbReadOnly.getAssign());
	else if (argEnt == "while") pkbResult = Util::convertIntToString(pkbReadOnly.getWhile());
	else if (argEnt == "if") pkbResult = Util::convertIntToString(pkbReadOnly.getIf());
	else if (argEnt == "call") pkbResult = Util::convertIntToString(pkbReadOnly.getCall());
	else if (argEnt == "constant") pkbResult = pkbReadOnly.getAllConstants();

	return pkbResult;
	
}

tuple<vector<string>,vector<string>> suchThatAnalyzer::validatePKBResultsInt(vector<int> v1, vector<int> v2) {
	
	vector<string> arg1EntityValues;
	vector<string> arg2EntityValues;

	if(arg1 != WILDCARD_SYMBOL)
		arg1EntityValues = getArgEntityResults(arg1Entity);
	if(arg2 != WILDCARD_SYMBOL)
		arg2EntityValues = getArgEntityResults(arg2Entity);

	unordered_set<string> arg1EntResults(arg1EntityValues.begin(), arg1EntityValues.end());
	unordered_set<string> arg2EntResults(arg2EntityValues.begin(),arg2EntityValues.end());
	vector<string> arg1ValidatedResults; 
	vector<string> arg2ValidatedResults;  

	for (int i=0; i <  v1.size(); i++) {
		if (arg2 == WILDCARD_SYMBOL && arg1EntResults.find(to_string(v1[i])) != arg1EntResults.end())
		arg1ValidatedResults.push_back(to_string(v1[i]));
		else if (arg1 == WILDCARD_SYMBOL && arg2EntResults.find(to_string(v2[i])) != arg2EntResults.end())
		arg2ValidatedResults.push_back(to_string(v2[i]));
		else if(arg1EntResults.find(to_string(v1[i])) != arg1EntResults.end() && 
			arg2EntResults.find(to_string(v2[i])) != arg2EntResults.end()) {
			arg1ValidatedResults.push_back(to_string(v1[i]));
			arg2ValidatedResults.push_back(to_string(v2[i]));
		} 
	}
	if (arg2 == WILDCARD_SYMBOL && !arg1ValidatedResults.empty())
		arg1ValidatedResults = removeDuplicates(arg1ValidatedResults);
	else if (arg1 == WILDCARD_SYMBOL && !arg2ValidatedResults.empty())
		arg2ValidatedResults = removeDuplicates(arg2ValidatedResults);
	return make_tuple(arg1ValidatedResults, arg2ValidatedResults);
}