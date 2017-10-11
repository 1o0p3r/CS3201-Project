#include "QueryAnalyzer.h"

enum selectValue
{
	undefinedSelect, stmtSelect, assignSelect, whileSelect, variableSelect, constantSelect,
	prog_lineSelect, ifSelect, procedureSelect
};

enum suchThatValue
{
	undefinedSuchThat, modifies, uses, parent, parentStar, follows,
	followsStar, calls, callsStar, next_, nextStar
};

enum parentArgCase
{
	intInt, intSyn, intWild, synInt, synSyn, synWild, 
	wildInt, wildSyn, wildWild
};

enum usesArgCase
{
	intString, intSyn_, intWild_, synString, synSyn_, synWild_,
	wildString, wildSyn_, wildWild_
};

enum parentSynType
{
	undefinedType, stmt, _while, _if, _assign, prog_line
};

enum mergeCase
{
	 oneResultArg1Found, oneResultArg1Missing, bothSynFound, arg1Found, arg2Found, noSynFound
};

enum patternValues
{
	assign_, while_, if_
};

enum patternExpType
{
	undefinedExpression, exact, substring, _wildcard_
};
static map<string, patternExpType> mapPatternExpType;
static map<string, selectValue> mapSelectValues;
static map<string, suchThatValue> mapSuchThatValues;
static map<string, parentSynType> mapParentSynTypeValues;
static map<string, patternValues> mapPatternValues;

void QueryAnalyzer::initMapPatternExpType() {
	mapPatternExpType["exact"] = exact;
	mapPatternExpType["substring"] = substring;
	mapPatternExpType["wildcard"] = _wildcard_;
}

void QueryAnalyzer::initSelectMap()
{ //to check with pql whether QS uses these strings in the entity as defined here.
	mapSelectValues["stmt"] = stmtSelect;
	mapSelectValues["assign"] = assignSelect;
	mapSelectValues["while"] =  whileSelect;
	mapSelectValues["variable"] = variableSelect;
	mapSelectValues["constant"] = constantSelect;
	mapSelectValues["prog_line"] = prog_lineSelect;
	mapSelectValues["if"] = ifSelect;
	mapSelectValues["procedure"] = procedureSelect;
}

void QueryAnalyzer::initSuchThatMap()
{// to check with pl whether QS uses these strings (i.e inside the square bracket) in the entity as defined here.
	mapSuchThatValues["Modifies"] = modifies;
	mapSuchThatValues["Uses"] = uses;
	mapSuchThatValues["Parent"] = parent;
	mapSuchThatValues["Parent*"] = parentStar;
	mapSuchThatValues["Follows"] = follows;
	mapSuchThatValues["Follows*"] = followsStar;
	mapSuchThatValues["Calls"] = calls;
	mapSuchThatValues["Calls*"] = callsStar;
	mapSuchThatValues["Next"] = next_;
	mapSuchThatValues["Next*"] = nextStar;

}

void QueryAnalyzer::initParentSynTypeMap() {
	mapParentSynTypeValues["stmt"] = stmt;
	mapParentSynTypeValues["while"] = _while;
	mapParentSynTypeValues["if"] = _if;
	mapParentSynTypeValues["assign"] = _assign;
	mapParentSynTypeValues["prog_line"] = prog_line;
}

void QueryAnalyzer::initPatternValueMap() {
	mapPatternValues["assign"] = assign_;
	mapPatternValues["while"] = while_;
	mapPatternValues["if"] = if_;
}

const string SYNONYM = "synonym";
const string WILDCARD = "wildcard";
const string WILDCARD_SYMBOL = "_";
const int ARGONE = 0;
const int ARGTWO = 1;
const int TABLEPOS = 0;
const int SYNPOS = 1;
const int VECINTERSECTION = 0;
const int TTMINDEX = 1;
const int STRINDEX = 2;
const int SAMETABLE = 0;
const int TWO_DISJOINT_TABLE = 1;
const int VECTRESULT = 1;
const int BOOLRESULT = 0;

QueryAnalyzer::QueryAnalyzer() {
	initSelectMap();
	initSuchThatMap();
	initParentSynTypeMap();
	initPatternValueMap();
	initMapPatternExpType();
	vector<string> result;
	synTableMap = unordered_map<string, tuple<int, int>>();
	hasSTClause = true;
	hasPatternClause = true;

}

void QueryAnalyzer::setPKB(PKB pkb) {
	pkbReadOnly = pkb;
}

void QueryAnalyzer::setQS(QueryStatement qs){
	qsReadOnly = qs;
}

vector<string> QueryAnalyzer::runQueryEval() {
	vector<string> result;
	synTableMap = unordered_map<string, tuple<int, int>>();
	mergedQueryTable = vector<vector<vector<string>>>();
	hasSTClause = true;
	hasPatternClause = true;
	findQueryElements();
	solveSTClause();
	solvePatternClause();
	result = analyzeClauseResults();
	return result;
}



void QueryAnalyzer::findQueryElements() {
	selectElement = qsReadOnly.getSelectQueryElement();
	stElements = qsReadOnly.getSuchThatQueryElement(); 
	patternElements = qsReadOnly.getPatternQueryElement();
}

vector<string> QueryAnalyzer::analyzeClauseResults() {
	vector<string> answer;
	if (!hasSTClause || !hasPatternClause)
		return{};

	string selectEntity = selectElement.getSelectEntity();
	string selectSyn = selectElement.getSelectSynonym();
	auto search = synTableMap.find(selectSyn);
	if (search != synTableMap.end()) {
		answer = mergedQueryTable.at(get<ARGONE>(search->second))
				.at(get<ARGTWO>(search->second));
		answer = removeVectDuplicates(answer);
		answer = analyzeSelect(answer, selectEntity);
	} else {
		answer = analyzeSelect(answer, selectEntity);
	}
	return answer;
}

vector<string> QueryAnalyzer::analyzeSelect(vector<string> queryResult, string selectEntity) {
	vector<int> selectResultInt;
	vector<string> answer;

	switch (mapSelectValues[selectEntity]) {
	case stmtSelect: //stmt
		selectResultInt = pkbReadOnly.getAllStmt();
		break;
	case assignSelect: //assign
		selectResultInt = pkbReadOnly.getAssign();
		break;

	case whileSelect: //while
		selectResultInt = pkbReadOnly.getWhile();
		break;

	case variableSelect: //variable
		answer = pkbReadOnly.getAllVariables();
		break;

	case constantSelect: //constant
		answer = pkbReadOnly.getAllConstants();
		break;

	case prog_lineSelect: //prog_line , not in use atm
		break;

	case ifSelect:
		selectResultInt = pkbReadOnly.getIf();
		break;

	case procedureSelect:
		answer = pkbReadOnly.getAllProcedures();
		break;
	}
	if (!selectResultInt.empty())
		for (int i : selectResultInt)
			answer.push_back(to_string(i));
	if (!queryResult.empty()) {
		answer = intersection(answer, queryResult);
	}
	return answer;
}

vector<string> QueryAnalyzer::intersection(vector<string> v1, vector<string> v2)
{

	vector<string> v3;

	sort(v1.begin(), v1.end());
	sort(v2.begin(), v2.end());

	set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v3));

	return v3;
}


vector<string> QueryAnalyzer::removeVectDuplicates(vector<string> selectClause) {
	unordered_set<string> shortlisted;
	vector<string> answer;

	selectClause.pop_back(); // remove synonym indicator
	for (string entry : selectClause) {
		shortlisted.insert(entry);
	}

	if (!shortlisted.empty()) { //removing duplicates and sort
		answer.assign(shortlisted.begin(), shortlisted.end());
		sort(answer.begin(), answer.end());
	}

	return answer;
}

vector<vector<vector<string>>> QueryAnalyzer::solveSTClause() {
	string stClauseType;
	vector<vector<string>> stResult;
	tuple<bool, vector<vector<string>>> clauseResult;
	int evaluateSTRelation;
	for (QueryElement stClause : stElements) {
		stClauseType = stClause.getSuchThatRel();
		evaluateSTRelation = mapSuchThatValues[stClauseType];
		switch (evaluateSTRelation) {
			case modifies:
				clauseResult = ModifiesAnalyzer(stClause, pkbReadOnly).solveClause();
				stResult = get<VECTRESULT>(clauseResult);
				hasSTClause = get<BOOLRESULT>(clauseResult);
				break;
			case uses:
				clauseResult = UsesAnalyzer(stClause, pkbReadOnly).solveClauseStmt();
				stResult = get<VECTRESULT>(clauseResult);
				hasSTClause = get<BOOLRESULT>(clauseResult);
				break;
			case parent: 
				clauseResult = ParentAnalyzer(stClause, pkbReadOnly).solveClauseStmt();
				stResult = get<VECTRESULT>(clauseResult);
				hasSTClause = get<BOOLRESULT>(clauseResult);
				break;
			case parentStar:
				clauseResult = ParentStarAnalyzer(stClause, pkbReadOnly).solveClauseStmt();
				stResult = get<VECTRESULT>(clauseResult);
				hasSTClause = get<BOOLRESULT>(clauseResult);
				break;
			case follows:
				clauseResult = FollowsAnalyzer(stClause, pkbReadOnly).solveClauseStmt();
				stResult = get<VECTRESULT>(clauseResult);
				hasSTClause = get<BOOLRESULT>(clauseResult);
				break;
			case followsStar:
				clauseResult = FollowsStarAnalyzer(stClause, pkbReadOnly).solveClauseStmt();
				stResult = get<VECTRESULT>(clauseResult);
				hasSTClause = get<BOOLRESULT>(clauseResult);
				break;
			case calls:
				clauseResult = CallsAnalyzer(stClause, pkbReadOnly).solveClauseStmt();
				stResult = get<VECTRESULT>(clauseResult);
				hasSTClause = get<BOOLRESULT>(clauseResult);
				break;
			case callsStar:
				break;
			case next_:
				break;
			case nextStar:
				break;
		}
		if (!stResult.empty())
			insertSTResult(stResult);
	}
	return mergedQueryTable;
}

void QueryAnalyzer::solvePatternClause() {
	string patternClauseType;
	vector<vector<string>> patternResult;
	int evaluatePatternRelation;
	for (QueryElement patternClause : patternElements) {
		patternClauseType = patternClause.getPatternEntity();
		evaluatePatternRelation = mapPatternValues[patternClauseType];
		switch (evaluatePatternRelation) {
			case assign_:
				patternResult = solveAssignPattern(patternClause);
				break;
			case while_:
				break;
			case if_:
				break;
		}
		if (!patternResult.empty())
			insertSTResult(patternResult);
		if (!hasPatternClause)
			break;
	}
		
}

vector<vector<string>> QueryAnalyzer::solveAssignPattern(QueryElement patternClause) {
	string patSyn = patternClause.getPatternSynonym();
	string arg1 = patternClause.getPatternArg1();
	string arg1Type = patternClause.getPatternArg1Type();
	string patExp = patternClause.getPatternArg2();
	string patType = patternClause.getPatternArg2Type();
	tuple<vector<string>, vector<string>> evaluatedPatResult;
	vector<string> validatedPatResult;
	vector<vector<string>> patternAssignResult;
	if (arg1Type == SYNONYM) {
		evaluatedPatResult = solvePatAssignSyn(arg1, patExp, patType, patSyn);
		if (!get<ARGONE>(evaluatedPatResult).empty()) {
			patternAssignResult.push_back(get<ARGONE>(evaluatedPatResult));
			patternAssignResult.push_back(get<ARGTWO>(evaluatedPatResult));
		}
	} else {
		validatedPatResult = validatedPatAssignSyn(arg1, patExp, patType, patSyn);
		if (!validatedPatResult.empty())
			patternAssignResult.push_back(validatedPatResult);
	}
	return patternAssignResult;
}

tuple<vector<string>, vector<string>> QueryAnalyzer::solvePatAssignSyn(string arg1, 
		string patExp, string patType, string patSyn) {
	vector<tuple<int, string>> pkbPatResult;
	bool containsPattern = false;
	vector<string> entityVector;
	vector<string> variableVector;
	for (string candidates : pkbReadOnly.getAllVariables()) {
		pkbPatResult = pkbReadOnly.getPattern(candidates);
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

vector<string> QueryAnalyzer::validatedPatAssignSyn(string arg1, string patExp, 
		string patType, string patSyn) {
	vector<tuple<int, string>> pkbPatResult;
	bool containsPattern = false;
	vector<string> entityVector;
	unordered_set<string> shortlisted; // for removing duplicates
	vector<string> listOfCandidates;
	if (arg1 == WILDCARD_SYMBOL) 
		listOfCandidates = pkbReadOnly.getAllVariables();
	else
		listOfCandidates.push_back(arg1);
	for (string candidates : listOfCandidates) {
		pkbPatResult = pkbReadOnly.getPattern(candidates);
		for (tuple<int, string> evalPattExpression : pkbPatResult) {
			switch (mapPatternExpType[patType]) {
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


vector<vector<vector<string>>> QueryAnalyzer::insertSTResult(vector<vector<string>> stResult) {
	/*		  arg1, arg2
	 *case 0: hasSyn
	 *case 1: noSyn
	 *case 2: hasSyn, hasSyn
	 *case 3: hasSyn, noSyn -> arg2 present 
	 *case 4: noSyn, hasSyn
	 *case 5: noSyn, noSyn
	 */
	string arg1;
	string arg2;
	int tableIndex;
	auto iteratorMapFindArg1 = synTableMap.find(stResult[ARGONE].back());
	int scenario = 0;
	bool hasArg2 = false;
	
	if (iteratorMapFindArg1 == synTableMap.end()) { //element not found
		if (stResult.size() > 1) {
			scenario += 4;
			hasArg2 = true;
			if (synTableMap.find(stResult[ARGTWO].back()) == synTableMap.end()) {
				scenario += 1;
			}
		}
		else {
			scenario += 1;
		}
	}
	//oneResultArg1Found, oneResultArg1Missing, bothSynFound, arg1Found, arg2Found, noSynFound
	tableIndex = mergedQueryTable.size();
	switch (scenario) {
		case oneResultArg1Found:
			addSingleCommonSynTable(stResult, stResult[ARGONE].back(), ARGONE);
			break;
		case oneResultArg1Missing:
			addSingleCommonSynTable(stResult, stResult[ARGONE].back(), ARGONE); 
			break;
		case bothSynFound:
			insertArg1Arg2CommonSynTable(stResult);
			break;
		case arg1Found:
			addSingleCommonSynTable(stResult,stResult[ARGONE].back(),ARGONE);
			break;
		case arg2Found:
			addSingleCommonSynTable(stResult, stResult[ARGTWO].back(), ARGTWO); 
			break;
		case noSynFound:
			insertNoCommonSynToTable(stResult, tableIndex, hasArg2);
			break;
	}	 
	return mergedQueryTable;
}

void QueryAnalyzer::insertArg1Arg2CommonSynTable(vector<vector<string>> stResult) {
	int scenario;
	string arg1;
	string arg2;
	arg1 = stResult[ARGONE].back();
	arg2 = stResult[ARGTWO].back();
	auto searchSynLocation1 = synTableMap.find(arg1);
	auto searchSynLocation2 = synTableMap.find(arg2);
	tuple<int, int> tableLocation1 = searchSynLocation1->second;
	tuple<int, int> tableLocation2 = searchSynLocation2->second;
	vector<vector<string>> tableToMerge1 = mergedQueryTable.at(get<ARGONE>(tableLocation1));
	vector<vector<string>> tableToMerge2 = mergedQueryTable.at(get<ARGONE>(tableLocation2));
	vector<vector<string>> result;
	
	//case arg1arg2 from same table, arg1arg2 not from same table
	scenario = (get<ARGONE>(tableLocation1) == get<ARGONE>(tableLocation2)) ? 
			SAMETABLE : TWO_DISJOINT_TABLE;
	switch (scenario) {
		case SAMETABLE:
			restrictTableValues(tableToMerge1, get<ARGTWO>(tableLocation1), get<ARGTWO>(tableLocation2),
					get<ARGONE>(tableLocation1), stResult);
			break;
		case TWO_DISJOINT_TABLE:
			int clauseTableExists = -1;
			result = hashJoin(tableToMerge1, get<ARGTWO>(tableLocation1),
				stResult, ARGONE, get<ARGONE>(tableLocation1), clauseTableExists);
			clauseTableExists = get<ARGONE>(tableLocation2);
			result = hashJoin(result, get<ARGTWO>(tableLocation1), tableToMerge2,
				get<ARGTWO>(tableLocation2), get<ARGONE>(tableLocation1), clauseTableExists);
			break;
	}
}

void QueryAnalyzer::restrictTableValues(vector<vector<string>> queryAnalyzerTable, int joinColArg1,
		int joinColArg2, int tableLoc, vector<vector<string>> stResult)
{
	vector<int> keepRowIndex;
	vector<string> vectResult;
	vector<vector<string>> mergedResult;
	unordered_set<tuple<string, string>> commonValuesSet;
	for (int clauseRow = 0; clauseRow < stResult[ARGONE].size(); clauseRow++) {
		commonValuesSet.insert(make_tuple(stResult[ARGONE][clauseRow],
			stResult[ARGTWO][clauseRow]));
	}

	for (int dataTableRow = 0; dataTableRow < queryAnalyzerTable[ARGONE].size(); dataTableRow++) {
		if (commonValuesSet.find(make_tuple(queryAnalyzerTable[joinColArg1][dataTableRow],
			queryAnalyzerTable[joinColArg2][dataTableRow])) != commonValuesSet.end())
			keepRowIndex.push_back(dataTableRow);
	}
	if (keepRowIndex.empty()) {
		//terminate queryEval
		hasSTClause = false;
		hasPatternClause = false;
	}
	else {
		for (int synonymIndex = 0; synonymIndex < queryAnalyzerTable.size(); synonymIndex++) {
			vectResult = vector<string>();
			for (int row : keepRowIndex) {
				vectResult.push_back(queryAnalyzerTable[synonymIndex][row]);
			}
			mergedResult.push_back(vectResult);
		}
		mergedQueryTable[tableLoc] = mergedResult;
	}
}

void QueryAnalyzer::addSingleCommonSynTable(vector<vector<string>> stResult, string arg, int clauseJoinIndex) {
	//arg1 common, arg2 not common implies intersect on arg1

	int clauseTableExists = -1;
	auto searchSynLocation = synTableMap.find(arg);
	if (searchSynLocation == synTableMap.end()) { //cant find arg
		synTableMap.insert(make_pair(arg, make_tuple(mergedQueryTable.size(), ARGONE)));	//re-map synonym table
		mergedQueryTable.push_back(stResult);
	}
	else {
		tuple<int, int> tableLocation = searchSynLocation->second;
		vector<vector<string>> tableToMerge = mergedQueryTable.at(get<ARGONE>(tableLocation));
		vector<vector<string>> result = hashJoin(tableToMerge, get<ARGTWO>(tableLocation),
			stResult, clauseJoinIndex, get<ARGONE>(tableLocation), clauseTableExists);
		if (result.empty()) {
			hasSTClause = false;
			hasPatternClause = false;
		}
			
	}
}

void QueryAnalyzer::insertNoCommonSynToTable(vector<vector<string>> stResult, 
		int tableIndex, bool hasArg2) {
	string arg1;
	string arg2;
	arg1 = stResult[ARGONE].back();
	synTableMap.insert({ arg1, make_tuple(tableIndex, ARGONE) });
	if (hasArg2) {
		arg2 = stResult[ARGTWO].back();
		synTableMap.insert({ arg2, make_tuple(tableIndex, ARGTWO) });
	}
	mergedQueryTable.push_back(stResult);
}
//vector<vector<string>> QueryAnalyzer::solveUses(QueryElement suchThatClause) {
//	/* arg1, arg2  = case
//	 * stmtVersion = syn, wildcard, integer 9 cases, syn , literalstring, wilcard
//	 * procVersion = syn, wildcard, literalString, integer 12 cases	
//	 */
//	vector<vector<string>> usesResult;
//	string arg1Type = suchThatClause.getSuchThatArg1Type();
//	if (arg1Type == "procedure" || arg1Type == "call")
//		solveUsesProc(suchThatClause);
//	else
//		usesResult = solveUsesStmt(suchThatClause);
//	return usesResult;
//}
//void QueryAnalyzer::solveUsesProc(QueryElement suchThatClause) {
//	//TODO: implement in iteration ?
//} 
//vector<vector<string>> QueryAnalyzer::solveUsesStmt(QueryElement suchThatClause) {
//	/*  arg1 , arg2  = case
//	integer, literalstring = 0
//	integer, synonym = 1
//	integer, wildcard = 2
//	synonym, literalstring = 3
//	synonym, synonym = 4
//	synonym, wildcard = 5
//	wildcard, literalstring = 6
//	wildcard, synonym = 7
//	wildcard, wildcard = 8
//	*/
//	string arg1 = suchThatClause.getSuchThatArg1();
//	string arg2 = suchThatClause.getSuchThatArg2();
//	string arg1Type = suchThatClause.getSuchThatArg1Type();
//	string arg2Type = suchThatClause.getSuchThatArg2Type();
//	vector<vector<string>> usesResultStmt;
//
//	int scenario = 0;
//	if (arg1Type == "synonym") { scenario += 3; }
//	else if (arg1Type == WILDCARD) { scenario += 6; }
//	if (arg2Type == "synonym") { scenario += 1; }
//	else if (arg2Type == WILDCARD) { scenario += 2; }
//	
//	switch (scenario) {
//		case intString:
//			validateUses(arg1, arg2,scenario);
//			break;
//		case intSyn:
//			usesResultStmt = toAddUsesSynVect(arg1, arg2, scenario);
//			break;
//		case intWild:
//			validateUses(arg1, arg2, scenario);
//		case synString:
//			usesResultStmt = toAddUsesSynVect(arg1, arg2, scenario);
//			break;
//		case synSyn_:
//			usesResultStmt =  toAddUsesSynVect(arg1, arg2, scenario);
//			break;
//		case synWild_:
//			usesResultStmt = toAddUsesSynVect(arg1, arg2, scenario);
//			break;
//		case wildString:
//			validateUses(arg1, arg2, scenario);
//			break;
//		case wildSyn_:
//			usesResultStmt = toAddUsesSynVect(arg1, arg2, scenario);
//			break;
//		case wildWild_:
//			validateUses(arg1, arg2, scenario);
//			break;
//	}
//	return usesResultStmt;
//}
//bool QueryAnalyzer::validateUses(string arg1, string arg2, int scenario) {
//	vector<string> resultArg1Param;
//
//	switch (scenario) {
//		case intString:
//			resultArg1Param = pkbReadOnly.getUses(stoi(arg1));
//			if (find(resultArg1Param.begin(), resultArg1Param.end(), arg2) 
//				== resultArg1Param.end())
//				hasSTClause = false;
//			break;
//		case intWild_:
//			if (pkbReadOnly.getUses(stoi(arg1)).empty())
//				hasSTClause = false;
//			break;
//		case wildString:
//			if (pkbReadOnly.getUsedBy(arg2).empty())
//				hasSTClause = false;
//			break;
//		case wildWild_:
//			if (pkbReadOnly.getAssign().empty() && pkbReadOnly.getIf().empty() 
//				&& pkbReadOnly.getWhile().empty())
//				hasSTClause = false;
//			break;
//	}
//	return hasSTClause; // for debugging
//}
//vector<vector<string>> QueryAnalyzer::toAddUsesSynVect(string arg1, string arg2, int scenario) {
//	vector<string> pkbUsesResultString;
//	vector<int> pkbUsesResultInt;
//	vector<string> vectorToAdd;
//	vector<string> vectorToAdd2;
//	vector<vector<string>> usesResult;
//	vector<string> candidates;
//	vector<int> candidatesInt;
//	vector<int> allIf = pkbReadOnly.getIf();
//	vector<int> allWhile = pkbReadOnly.getWhile();
//	vector<int> allAssign = pkbReadOnly.getAssign();
//	unordered_set<string> shortlisted;
//
//
//	switch (scenario) {
//		case intSyn:
//			pkbUsesResultString = pkbReadOnly.getUses(stoi(arg1));
//			if (pkbUsesResultString.empty()) {
//				hasSTClause = false;
//			}
//			else {
//				pkbUsesResultString.push_back(arg2);
//				usesResult.push_back(pkbUsesResultString);
//			}
//			break;
//		case synString:
//			pkbUsesResultInt = pkbReadOnly.getUsedBy(arg2);
//			if (pkbUsesResultInt.empty()) {
//				hasSTClause = false;
//			} else {
//				for (int candidate : pkbUsesResultInt)
//					vectorToAdd.push_back(to_string(candidate));
//				vectorToAdd.push_back(arg1);
//				usesResult.push_back(vectorToAdd);
//			}
//			break;
//		case synSyn_: //how to solve pairing (1,"a"), (1,"x")
//			candidates = pkbReadOnly.getAllVariables();
//			for (string interviewee : candidates) {
//				pkbUsesResultInt = pkbReadOnly.getUsedBy(interviewee);
//				for (int shortlistedInterviewee : pkbUsesResultInt) {
//					vectorToAdd.push_back(to_string(shortlistedInterviewee));
//					vectorToAdd2.push_back(interviewee);
//				}
//			}
//			if(vectorToAdd.empty())
//				hasSTClause = false;
//			else {
//				vectorToAdd.push_back(arg1);
//				vectorToAdd2.push_back(arg2);
//				usesResult.push_back(vectorToAdd);
//				usesResult.push_back(vectorToAdd2);
//			}
//			break;
//		case synWild_:
//			candidates = pkbReadOnly.getAllVariables();
//			for (string interviewee : candidates) {
//				pkbUsesResultInt = pkbReadOnly.getUsedBy(interviewee);
//				for (int shortlistedInterviewee : pkbUsesResultInt) {
//					shortlisted.insert(to_string(shortlistedInterviewee));
//				}
//			}
//			if (!shortlisted.empty()) { //removing duplicates
//				vectorToAdd.assign(shortlisted.begin(), shortlisted.end());
//				vectorToAdd.push_back(arg1);
//				usesResult.push_back(vectorToAdd);
//			} else {
//				hasSTClause = false;
//			}
//			break;
//		case wildSyn_:
//			candidatesInt.reserve(allIf.size() + allWhile.size() + allAssign.size());
//			candidatesInt = pkbReadOnly.getAssign();
//			candidatesInt.insert(candidatesInt.end(), allIf.begin(), allIf.end());
//			candidatesInt.insert(candidatesInt.end(), allWhile.begin(), allWhile.end());
//			for (int interviewee : candidatesInt) {
//				pkbUsesResultString = pkbReadOnly.getUses(interviewee);
//				for (string shortlistedInterviewee : pkbUsesResultString) {
//					shortlisted.insert(shortlistedInterviewee);
//				}
//			}
//			if (!shortlisted.empty()) {
//				vectorToAdd.assign(shortlisted.begin(), shortlisted.end());
//				vectorToAdd.push_back(arg2);
//				usesResult.push_back(vectorToAdd);
//			} else {
//				hasSTClause = false;
//			}
//			break;
//	}
//	return usesResult;
//}
//vector<vector<string>> QueryAnalyzer::solveParent(QueryElement typeParent) {
//	int scenario;
//	bool hasParent;
//	string arg1Entity = typeParent.getSuchThatArg1Entity();
//	string arg1 = typeParent.getSuchThatArg1();
//	string arg1Type = typeParent.getSuchThatArg1Type();
//	string arg2Entity = typeParent.getSuchThatArg2Entity();
//	string arg2 = typeParent.getSuchThatArg2();
//	string arg2Type = typeParent.getSuchThatArg2Type();	
//	vector<string> parentSubResult;
//	tuple<vector<string>,vector<string>> parentSubResultTuple;
//	vector<vector<string>> parentResult;
//	//need to consider entity type(i.e if statement etc)
//	/*  arg1 , arg2  = case
//		integer, integer = 0
//		integer, synonym = 1
//		integer, wildcard = 2
//		synonym, integer = 3
//		synonym, synonym = 4
//		synonym, wildcard = 5
//		wildcard, integer = 6
//		wildcard, synonym = 7
//		wildcard, wildcard = 8
//	*/
//	scenario = 0; //base case
//	if (arg1Type == "synonym") { scenario += 3; }
//	else if (arg1Type == WILDCARD) { scenario += 6; }
//	if (arg2Type == "synonym") { scenario += 1; }
//	else if (arg2Type == WILDCARD) { scenario += 2; }
//
//	switch (scenario) {
//	case intInt:
//		isParent(arg1, arg2);
//		break;
//	case intSyn:
//		parentSubResult = evalParentIntSyn(arg1, arg2);
//		if(!parentSubResult.empty())
//			parentResult.push_back(parentSubResult);
//		break;
//	case intWild:
//		hasChildOfArg1(arg1);
//		break;
//	case synInt:
//		parentSubResult = hasParentOfArg2(arg1,arg2); 
//		if (!parentSubResult.empty())
//			parentResult.push_back(parentSubResult);
//		break;
//	case synSyn:
//		parentSubResultTuple = evalParentSynSyn(arg1, arg2, arg1Type, arg2Type, arg1Entity, arg2Entity); 
//		if (!((get<ARGONE>(parentSubResultTuple)).empty()) && !((get<ARGTWO>(parentSubResultTuple)).empty())) {
//			parentResult.push_back(get<ARGONE>(parentSubResultTuple));
//			parentResult.push_back(get<ARGTWO>(parentSubResultTuple));
//		}
//		break;
//	case synWild:
//		parentSubResult = evalParentSynWild(arg1, arg1Entity);
//		if (!parentSubResult.empty())
//			parentResult.push_back(parentSubResult);
//		break;
//	case wildInt:
//		hasParentForArg2(arg2);
//		break;
//	case wildSyn:
//		parentSubResult = evalParentWildSyn(arg2, arg2Entity);
//		if (!parentSubResult.empty())
//			parentResult.push_back(parentSubResult);
//		break;
//	case wildWild:
//		hasParentStmts();
//		break;
//	}
//	return parentResult;
//}
//
//bool QueryAnalyzer::isParent(string arg1, string arg2) {
//	vector<int> childOfArg1 = pkbReadOnly.getChild(stoi(arg1));
//	//if arg1 is not a parent of arg2
//	if (!(find(childOfArg1.begin(), childOfArg1.end(), stoi(arg2)) != childOfArg1.end()))
//		hasSTClause = false;
//	return hasSTClause;
//}
//
//vector<string> QueryAnalyzer::evalParentIntSyn(string arg1, string arg2) {
//	vector<string> parentIntSyn;
//	vector<int> childOfArg1 = pkbReadOnly.getChild(stoi(arg1));
//	if (!childOfArg1.empty()) {
//		for (int child : childOfArg1)
//			parentIntSyn.push_back(to_string(child));
//		parentIntSyn.push_back(arg2); //store the synonym at last index of the vector for identification
//	}
//	else {
//		hasSTClause = false;
//	}
//	return parentIntSyn;
//}
//
//bool QueryAnalyzer::hasChildOfArg1(string arg1) {
//	vector<int> childOfArg1 = pkbReadOnly.getChild(stoi(arg1));
//	if (childOfArg1.empty())
//		hasSTClause = false;
//	return hasSTClause;
//}
//
//vector<string> QueryAnalyzer::hasParentOfArg2(string arg1,string arg2) {
//	vector<string> parentSynInt;
//	vector<int> parentOfArg2 = pkbReadOnly.getParent(stoi(arg2));
//	if (!parentOfArg2.empty()) {
//		for (int parent : parentOfArg2)
//			parentSynInt.push_back(to_string(parent));
//		parentSynInt.push_back(arg1); //store the synonym at last index of the vector for identification
//	}
//	else {
//		hasSTClause = false;
//	}
//	return parentSynInt;
//}
//
//tuple<vector<string>, vector<string>> QueryAnalyzer::evalParentSynSyn(string arg1, string arg2,
//		string arg1Type, string arg2Type, string arg1EntityType, string arg2EntityType) {
//	//possible type of synonym = stmt, while, if , prog_line
//	int arg1Entity = mapParentSynTypeValues[arg1EntityType];
//	int arg2Entity = mapParentSynTypeValues[arg2EntityType];
//	vector<int> allWhiles = pkbReadOnly.getWhile();
//	vector<int> allIfs = pkbReadOnly.getIf();
//	vector<int> allStmt;
//	vector<int> allStmtChild;
//	vector<int> allAssign = pkbReadOnly.getAssign();
//	vector<tuple<string,string>> result;
//	vector<int> arg1Candidates;
//	vector<int> arg2Candidates;
//	vector<string> arg1Result;
//	vector<string> arg2Result;
//	
//	//setting values for allstmt
//	allStmt.reserve(allWhiles.size() + allIfs.size());
//	allStmt.insert(allStmt.end(), allWhiles.begin(), allWhiles.end());
//	allStmt.insert(allStmt.end(), allIfs.begin(), allIfs.end());
//	
//	allStmtChild.reserve(allStmt.size() + allAssign.size());
//	allStmtChild.insert(allStmtChild.end(), allStmt.begin(), allStmt.end());
//	allStmtChild.insert(allStmtChild.end(), allAssign.begin(), allAssign.end());
//
//	arg1Candidates = setCandidateValues(allStmt, allWhiles, allIfs, arg1Entity, allAssign);
//	arg2Candidates = setCandidateValues(allStmt, allWhiles, allIfs, arg2Entity, allAssign);
//	result = evalParStmt(arg1Candidates, arg2Candidates); //vector<string,string>
//
//	if (!result.empty()) {
//		for (tuple<string, string> candidates : result) {
//			arg1Result.push_back(get<ARGONE>(candidates));
//			arg2Result.push_back(get<ARGTWO>(candidates));
//		}
//		arg1Result.push_back(arg1); //put synonym identifier at the back of vector
//		arg2Result.push_back(arg2);
//		return make_tuple(arg1Result, arg2Result);
//	}
//	else{
//		return tuple<vector<string>, vector<string>>();
//	}
//}
//
//vector<tuple<string, string>> QueryAnalyzer::evalParStmt(vector<int> arg1Candidates, 
//		vector<int> arg2Candidates) {
//	vector<int> childCandidates;
//	vector<int> parentCandidates;
//	vector<tuple<string, string>> evalArg1Results;
//	vector<tuple<string, string>> evalArg2Results;
//	vector<tuple<string, string>> evalResults;
//	for (int candidate : arg1Candidates) {
//		childCandidates = pkbReadOnly.getChild(candidate);
//		if (!childCandidates.empty()) {
//			for (int children : childCandidates)
//				evalArg1Results.push_back(make_tuple(to_string(candidate), to_string(children)));
//		}
//	}
//	
//	for (int candidate : arg2Candidates) {
//		parentCandidates = pkbReadOnly.getParent(candidate);
//		if (!childCandidates.empty()) {
//			for (int parents : parentCandidates)
//				evalArg2Results.push_back(make_tuple(to_string(parents), to_string(candidate)));
//		}
//	}
//	evalResults = intersectionTupleInt(evalArg1Results, evalArg2Results);
//	if (evalResults.empty())
//		hasSTClause = false;
//	return evalResults;
//}
//
//vector<int> QueryAnalyzer::setCandidateValues(vector<int> allStmt, vector<int> allWhile, 
//		vector<int> allIf, int arg1Entity, vector<int> allAssign) {
//	vector<int> result;
//	switch (arg1Entity){
//		case stmt:
//			result = allStmt;
//			break;
//		case _while:
//			result = allWhile;
//			break;
//		case _if:
//			result = allIf;
//			break;
//		case _assign:
//			result = allAssign;
//			break;
//	}
//	return result;
//}
//
////careful , to review pointer &
//vector<tuple<string, string>> QueryAnalyzer::intersectionTupleInt(vector<tuple<string,string>> &arg1Candidates,
//	vector<tuple<string,string>> &arg2Candidates) {
//	vector<tuple<string,string>> chosenCandidates;
//	sort(arg1Candidates.begin(), arg1Candidates.end());
//	sort(arg2Candidates.begin(), arg2Candidates.end());
//	set_intersection(arg1Candidates.begin(), arg1Candidates.end(), arg2Candidates.begin(), 
//			arg2Candidates.end(), back_inserter(chosenCandidates));
//	
//	return chosenCandidates;
//}
//
//vector<string> QueryAnalyzer::evalParentSynWild(string arg1, string arg1Type)
//{
//	int arg1Entity = mapParentSynTypeValues[arg1Type];
//	vector<int> allWhiles = pkbReadOnly.getWhile();
//	vector<int> allIfs = pkbReadOnly.getIf();
//	vector<int> allStmt;
//	vector<int> arg1Candidates;
//	vector<int> allAssign = pkbReadOnly.getAssign();
//	vector<string> result;
//
//	//setting values for allstmt
//	allStmt.reserve(allWhiles.size() + allIfs.size());
//	allStmt.insert(allStmt.end(), allWhiles.begin(), allWhiles.end());
//	allStmt.insert(allStmt.end(), allIfs.begin(), allIfs.end());
//	
//	arg1Candidates = setCandidateValues(allStmt, allWhiles, allIfs, arg1Entity, allAssign);
//	result = evalParArg1Stmt(arg1Candidates, arg1); 
//	return result;
//}
//
//vector<string> QueryAnalyzer::evalParArg1Stmt(vector<int> arg1Candidates, string arg1)
//{	vector<int> childCandidates;
//	vector<string> evalArg1Results;
//	for (int candidate : arg1Candidates) {
//		childCandidates = pkbReadOnly.getChild(candidate);
//		if (!childCandidates.empty()) {
//			evalArg1Results.push_back(to_string(candidate));
//		}
//	}
//	if (evalArg1Results.empty())
//		hasSTClause = false;
//	else
//		evalArg1Results.push_back(arg1); //add synonym as identifier at the back of vector
//	return evalArg1Results;
//}
//
//vector<string> QueryAnalyzer::evalParentWildSyn(string arg2, string arg2Type) {
//	int arg2Entity = mapParentSynTypeValues[arg2Type];
//	vector<int> allWhiles = pkbReadOnly.getWhile();
//	vector<int> allIfs = pkbReadOnly.getIf();
//	vector<int> allStmt;
//	vector<int> arg2Candidates;
//	vector<string> result;
//	vector<int> allStmtChild;
//	vector<int> allAssign = pkbReadOnly.getAssign();
//
//	//setting values for allstmt
//	allStmt.reserve(allWhiles.size() + allIfs.size());
//	allStmt.insert(allStmt.end(), allWhiles.begin(), allWhiles.end());
//	allStmt.insert(allStmt.end(), allIfs.begin(), allIfs.end());
//
//	allStmtChild.reserve(allStmt.size() + allAssign.size());
//	allStmtChild.insert(allStmtChild.end(), allStmt.begin(), allStmt.end());
//	allStmtChild.insert(allStmtChild.end(), allAssign.begin(), allAssign.end());
//
//	arg2Candidates = setCandidateValues(allStmt, allWhiles, allIfs, arg2Entity, allAssign);
//	result = evalParArg2Stmt(arg2Candidates, arg2);
//	return result;
//}
//
//vector<string> QueryAnalyzer::evalParArg2Stmt(vector<int> arg2Candidates, string arg2) {
//	vector<int> parentCandidates;
//	vector<string> evalArg2Results;
//	for (int candidate : arg2Candidates) {
//		parentCandidates = pkbReadOnly.getParent(candidate);
//		if (!parentCandidates.empty()) {
//			evalArg2Results.push_back(to_string(candidate));
//		}
//	}
//	if (evalArg2Results.empty())
//		hasSTClause = false;
//	else
//		evalArg2Results.push_back(arg2); //add synonym identifier at back of vector
//	return evalArg2Results;
//}
//
//bool QueryAnalyzer::hasParentForArg2(string arg2) {
//	vector<int> parentOfArg2 = pkbReadOnly.getParent(stoi(arg2));
//	if (parentOfArg2.empty())
//		hasSTClause = false;
//	return hasSTClause;
//}
//
//bool QueryAnalyzer::hasParentStmts() {
//	vector<int> allWhiles = pkbReadOnly.getWhile();
//	vector<int> allIfs = pkbReadOnly.getIf();
//	if (allWhiles.empty() && allIfs.empty())
//		hasSTClause = false;
//	return hasSTClause;
//}

vector<vector<string>> QueryAnalyzer::hashJoin(vector<vector<string>> queryAnalyzerTable, 
	int qaJoinIndex, vector<vector<string>> clauseTable, int clausetableJoinIndex, int qaTableLoc, int clauseTableLoc) {
	multimap<string, int> valueToClauseRowIndexMap;
	typedef multimap<string, int>::iterator MMAPIterator;
	pair<MMAPIterator, MMAPIterator> commonValueIterator;
	vector<vector<string>> hashJoinTable;
	vector<string> addToTable;
	vector<string> hashKeys;
	vector<string> tableKeys;
	
	// A | B       A | C
	// 1   2       3   5
	// 1   4       1   6
	// 2   3       4   7
	// 3   1       
	
	// M = num of variables in QueryAnalyser, K = num of var in Clause Table 
	// N = number of elements in QueryAnalyser, V = number of common values found
	//complexity O((M+K-1)*NV )  
	//Poor implementation of dataStructure

	//poor implementation of mapping, required for bug fix when synonym exists as variable name in code
	for (int col = 0; col < queryAnalyzerTable.size(); col++) {
		tableKeys.push_back(queryAnalyzerTable[col].back());
		queryAnalyzerTable[col].pop_back();
	}

	for (int col = 0; col < clauseTable.size(); col++) {
		if(col != clausetableJoinIndex)
			tableKeys.push_back(clauseTable[col].back());
		clauseTable[col].pop_back();
	}

	//hash   value of clause (key) to  row indices in multimap
	for (int row = 0; row < clauseTable[0].size(); row++) {
		valueToClauseRowIndexMap.insert(make_pair(clauseTable[clausetableJoinIndex][row],row));
	}
	for (int col = 0; col < queryAnalyzerTable.size(); col++) {
		addToTable = vector<string>();
		for (int row = 0; row < queryAnalyzerTable[ARGONE].size(); row++) { //all vectors have equal length,hence any valid vector size works
			commonValueIterator = valueToClauseRowIndexMap.equal_range(queryAnalyzerTable[qaJoinIndex][row]);
			for (MMAPIterator it = commonValueIterator.first; it != commonValueIterator.second; it++) {
				addToTable.push_back(queryAnalyzerTable[col][row]);
			}
		}	
		if (!addToTable.empty()) {
			hashJoinTable.push_back(addToTable);
		
		}
	}
	for (int col = 0; col < clauseTable.size(); col++) {
		if (col == clausetableJoinIndex)
			continue;
		addToTable = vector<string>();
		for (int row = 0; row < queryAnalyzerTable[ARGONE].size(); row++) { //all vectors have equal length,hence any valid vector size works
			commonValueIterator = valueToClauseRowIndexMap.equal_range(queryAnalyzerTable[qaJoinIndex][row]);
			for (MMAPIterator it = commonValueIterator.first; it != commonValueIterator.second; it++) {
				addToTable.push_back(clauseTable[col][it->second]);
			}
		}
		if(!addToTable.empty())
			hashJoinTable.push_back(addToTable);
	}

	if (!hashJoinTable.empty()) {
		//re-map synonym table
		for (int i = 0; i < hashJoinTable.size(); i++) {
			hashJoinTable[i].push_back(tableKeys[i]);
			synTableMap[hashJoinTable.at(i).back()] = make_tuple(qaTableLoc, i);
		}
		if (clauseTableLoc != -1) { //table is existing
			mergedQueryTable.erase(mergedQueryTable.begin() + clauseTableLoc);
		}
		mergedQueryTable.at(qaTableLoc) = hashJoinTable;
	}  
	return hashJoinTable;
}

vector<vector<vector<string>>> QueryAnalyzer::getMergedQueryTable()
{	// for debug
	return mergedQueryTable;
}

