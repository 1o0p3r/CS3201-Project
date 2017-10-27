#include "QueryAnalyzer.h"
#include "PatternAnalyzer.h"

enum selectValue
{
	undefinedSelect, stmtSelect, assignSelect, whileSelect, variableSelect, constantSelect,
	prog_lineSelect, ifSelect, procedureSelect, callSelect, stmtLstSelect
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

enum resultType
{
	undefinedResultType, synonymResult, tupleResult, booleanResult
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
static map<string, resultType > mapResultTypeValues;

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
	mapSelectValues["call"] = callSelect;
	mapSelectValues["stmtLst"] = stmtLstSelect;

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

void QueryAnalyzer::initResultTypeMap()
{	
	mapResultTypeValues["synonym"] = synonymResult;
	mapResultTypeValues["BOOLEAN"] = booleanResult;
	mapResultTypeValues["tuple"] = tupleResult;
}

QueryAnalyzer::QueryAnalyzer() {
	initSelectMap();
	initSuchThatMap();
	initParentSynTypeMap();
	initPatternValueMap();
	initMapPatternExpType();
	initResultTypeMap();
	vector<string> result;
	synTableMap = unordered_map<string, tuple<int, int>>();
	selectSynMap = unordered_map<int, int>();
	hasSTClause = true;
	hasPatternClause = true;
	hasWithClause = true;

}

void QueryAnalyzer::setPKB(PKB &pkb) {
	pkbPtr = pkb;
}

void QueryAnalyzer::setQS(QueryStatement qs){
	qsReadOnly = qs;
}

void QueryAnalyzer::solveWithClause()
{
	vector<vector<string>> withResult;
	for (QueryElement withClause : withElements) {
		auto clauseResult = WithAnalyzer(withClause, pkbPtr).analyze();
		withResult = get<VECTRESULT>(clauseResult);
		hasWithClause = get<BOOLRESULT>(clauseResult);
		if (!hasWithClause)
			break;
		if (!withResult.empty())
			insertSTResult(withResult);

	}
	
}

vector<string> QueryAnalyzer::runQueryEval() {
	synTableMap = unordered_map<string, tuple<int, int>>();
	mergedQueryTable = vector<vector<vector<string>>>();
	hasSTClause = true;
	hasPatternClause = true;
	hasWithClause = true;
	findQueryElements();
	solveWithClause();
	solveSTClause();
	solvePatternClause();
	vector<string> result = analyzeClauseResults();
	return result;
}



void QueryAnalyzer::findQueryElements() {
	selectElement = qsReadOnly.getSelectQueryElement();
	stElements = qsReadOnly.getSuchThatQueryElement(); 
	patternElements = qsReadOnly.getPatternQueryElement();
	withElements = qsReadOnly.getWithQueryElement();
}

void QueryAnalyzer::selectSynonym(vector<string> &answer)
{
	string selectEntity = selectElement.getSelectEntity();
	string selectSyn = selectElement.getSelectSynonym();
	auto search = synTableMap.find(selectSyn);
	if (search != synTableMap.end()) {
		answer = mergedQueryTable.at(get<ARGONE>(search->second))
		                         .at(get<ARGTWO>(search->second));
		answer = removeVectDuplicates(answer);
		answer = validateResult(answer, selectEntity);
	} else {
		answer = validateResult(answer, selectEntity);
	}
}

void QueryAnalyzer::setClauseFalse()
{ 
	hasWithClause,hasPatternClause,hasSTClause = false; //future implementation, include with
}

vector<string> QueryAnalyzer::rearrange(vector<string> cartVec, 
		vector<string> selectOrder, const unordered_map<string, int>& cartMap) {
	vector<string> result;
	for(auto entry: cartVec) {
		vector<string> cartToken = Util::splitLine(entry, ',');
		string reorderedTuple;
		for(auto candidate: selectOrder) {
			reorderedTuple.append(cartToken[cartMap.find(candidate)->second]);
			reorderedTuple.append(",");
		}
		reorderedTuple.pop_back();
		result.push_back(reorderedTuple);
	}
	return result;

}

void QueryAnalyzer::selectTuple(vector<string> &answer)
{
	auto tupleSynonyms = selectElement.getSelectSynonym();
	auto tupleEntity = selectElement.getSelectEntity();
	vector<string> synonymTokens = Util::splitLine(tupleSynonyms, ',');
	auto synonymEntities = Util::splitLine(tupleEntity, ',');
	vector<vector<tuple<int,int,string>>> synLoc;
	vector<vector<string>> synTableConcatEntries;
	unordered_map<string, int> synCartMap;


	//init mapping of synonyms from same table
	for (int i = 0; i < synonymTokens.size(); i++) {
		auto searchInQueryTable = synTableMap.find(synonymTokens[i]);
		if (searchInQueryTable != synTableMap.end()) { //if synonym in immediate table
			vector<tuple<int, int, string>> selectSynTableAttr;
			auto searchInSelectSynMap = selectSynMap.find(get<TABLELOC>(searchInQueryTable->second));
			if (searchInSelectSynMap == selectSynMap.end()) { //find synonyms from same table, insert at next row if not found,
				selectSynMap.insert(make_pair(get<TABLELOC>(searchInQueryTable->second), synLoc.size()));
				selectSynTableAttr.push_back(make_tuple(
					get<TABLELOC>(searchInQueryTable->second),
					get<SYNVECLOC>(searchInQueryTable->second),
					synonymEntities[i]));
				synLoc.push_back(selectSynTableAttr);
			} else {
				//insert synonym results at common table row 
				int insertAtRow = searchInSelectSynMap->second;
				synLoc[insertAtRow].push_back(make_tuple(
					get<TABLELOC>(searchInQueryTable->second),
					get<SYNVECLOC>(searchInQueryTable->second),
					synonymEntities[i]));
			}
		} else {
			synLoc.push_back({ make_tuple(NOSYNENTRY,NOSYNENTRY,synonymEntities[i]) });
		}
	}

	int k = 0;
	for (auto commonTableSyn : synLoc) {

		//record arrangement of synonym for rearrangement after cartesian product
		for (auto entry : commonTableSyn) {
			string synonym = mergedQueryTable[get<TABLELOC>(entry)][get<SYNVECLOC>(entry)].back();
			synCartMap.insert(make_pair(synonym, k));
			k++;
		}

		vector<string> vecAppendedSynValues;
		//synonym exists in intermediate table
		if (get<TABLELOC>(commonTableSyn.front()) != NOSYNENTRY) {
			int numVecElements = mergedQueryTable[get<TABLELOC>(commonTableSyn.front())].front().size();
			vecAppendedSynValues = vector<string>();
			vector<vector<int>> columnToKeep;

			//find intersection of vector and design entity
		
			for (auto concatSyn : commonTableSyn) { //elements in vector
				vector<string> synEntityVec = validateResult({}, get<SYNENTITY>(concatSyn)); //empty vector instead of {}
				unordered_set<string> synEntitySet(make_move_iterator(synEntityVec.begin()),
					make_move_iterator(synEntityVec.end()));
				vector<int> elementNumsToKeep;
				for (int i = 0; i < numVecElements; i++) {
					string element = mergedQueryTable[get<TABLELOC>(concatSyn)][get<SYNVECLOC>(concatSyn)][i];
					if (synEntitySet.find(element) != synEntitySet.end())
							elementNumsToKeep.push_back(i);
				}
				columnToKeep.push_back(elementNumsToKeep);
			
			}
			vector<int> columnResult = columnToKeep.front(); //init for merging vecs
			for (int i=1; i<columnToKeep.size();i++) {
				columnResult = intersectionT(columnResult, columnToKeep[i]);
			}
			
			//terminating condition
			if (columnResult.empty()) {
				setClauseFalse();
				answer = {};
			}

			//TODO: fix bug from here.
			//append values to vector
			for (const auto& entry : columnResult) {
				string appendSynValue;
				for (auto concatSyn : commonTableSyn) {
					string element = mergedQueryTable[get<TABLELOC>(concatSyn)][get<SYNVECLOC>(concatSyn)][entry];
					appendSynValue.append(element);
					appendSynValue.append(DELIMITER);		
				}
				appendSynValue.pop_back(); //remove last delimiter ","
				vecAppendedSynValues.push_back(appendSynValue);
			}
		} else {
			vecAppendedSynValues = validateResult({}, get<SYNENTITY>(commonTableSyn.front()));
		}
		synTableConcatEntries.push_back(vecAppendedSynValues);

	}

	vector<string> vecToCartProd = synTableConcatEntries.front();
	//cartesian product synonyms.
	if (!vecToCartProd.empty()) {
		for (int i = 1; i < synTableConcatEntries.size(); i++) {
			auto result = cross(vecToCartProd, synTableConcatEntries[i]);
			vector<string> vecCartProdstring;
			for (auto& j : result) {
				string concatCartProdString;
				concatCartProdString.append(get<0>(j))
					.append(DELIMITER)
					.append(get<1>(j));
				vecCartProdstring.push_back(concatCartProdString);
			}
			vecToCartProd = vecCartProdstring;
		}
	}
	
	vecToCartProd = rearrange(vecToCartProd,synonymTokens,synCartMap);
	answer = vecToCartProd;


}

bool QueryAnalyzer::isQueryFalse()
{
	if (!hasSTClause || !hasPatternClause || !hasWithClause)
		return true;
	return false;
}

vector<string> QueryAnalyzer::analyzeClauseResults() {
	vector<string> answer;
	string resultEntity = selectElement.getSelectType();
	if (isQueryFalse())
		if (resultEntity == "BOOLEAN")
			return{ "FALSE" };
		else
			return {};

	switch(mapResultTypeValues[resultEntity])
	{
		case booleanResult:
			return{ "TRUE" };

		case synonymResult: 
			selectSynonym(answer);
			break;
		
		case tupleResult:
			selectTuple(answer);
			break;
	}
	return answer;
}

vector<string> QueryAnalyzer::validateResult(vector<string> queryResult, string selectEntity) {
	vector<int> selectResultInt;
	vector<string> answer;

	switch (mapSelectValues[selectEntity]) {
	case stmtSelect: //stmt
		selectResultInt = pkbPtr.getAllStmt();
		break;
	case assignSelect: //assign
		selectResultInt = pkbPtr.getAssign();
		break;

	case whileSelect: //while
		selectResultInt = pkbPtr.getWhile();
		break;

	case variableSelect: //variable
		answer = pkbPtr.getAllVariables();
		break;

	case constantSelect: //constant
		answer = pkbPtr.getAllConstants();
		break;

	case prog_lineSelect: //prog_line , not in use atm
		selectResultInt = pkbPtr.getAllStmt();
		break;

	case ifSelect:
		selectResultInt = pkbPtr.getIf();
		break;

	case procedureSelect:
		answer = pkbPtr.getAllProcedures();
		break;

	case callSelect:
		selectResultInt = pkbPtr.getCall();
		break;

	case stmtLstSelect: //unimplemented stmt# of the first Stmt in the list
		break;
	}
	if (!selectResultInt.empty())
		for (int i : selectResultInt)
			answer.push_back(to_string(i));
	if (!queryResult.empty()) {
		answer = intersectionT(answer, queryResult);
	}
	return answer;
}

//vector<string> QueryAnalyzer::intersection(vector<string> v1, vector<string> v2)
//{
//
//	vector<string> v3;
//
//	sort(v1.begin(), v1.end());
//	sort(v2.begin(), v2.end());
//
//	set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v3));
//
//	return v3;
//}


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
	if (isQueryFalse())
		return {{{}}};

	for (QueryElement stClause : stElements) {
		stClauseType = stClause.getSuchThatRel();
		evaluateSTRelation = mapSuchThatValues[stClauseType];
		switch (evaluateSTRelation) {
			case modifies:
				clauseResult = ModifiesAnalyzer(stClause, pkbPtr).solveClause();
				stResult = get<VECTRESULT>(clauseResult);
				hasSTClause = get<BOOLRESULT>(clauseResult);
				break;
			case uses:
				clauseResult = UsesAnalyzer(stClause, pkbPtr).solveClauseStmt();
				stResult = get<VECTRESULT>(clauseResult);
				hasSTClause = get<BOOLRESULT>(clauseResult);
				break;
			case parent: 
				clauseResult = ParentAnalyzer(stClause, pkbPtr).solveClauseStmt();
				stResult = get<VECTRESULT>(clauseResult);
				hasSTClause = get<BOOLRESULT>(clauseResult);
				break;
			case parentStar:
				clauseResult = ParentStarAnalyzer(stClause, pkbPtr).solveClauseStmt();
				stResult = get<VECTRESULT>(clauseResult);
				hasSTClause = get<BOOLRESULT>(clauseResult);
				break;
			case follows:
				clauseResult = FollowsAnalyzer(stClause, pkbPtr).solveClauseStmt();
				stResult = get<VECTRESULT>(clauseResult);
				hasSTClause = get<BOOLRESULT>(clauseResult);
				break;
			case followsStar:
				clauseResult = FollowsStarAnalyzer(stClause, pkbPtr).solveClauseStmt();
				stResult = get<VECTRESULT>(clauseResult);
				hasSTClause = get<BOOLRESULT>(clauseResult);
				break;
			case calls:
				clauseResult = CallsAnalyzer(stClause, pkbPtr).solveClauseStmt();
				stResult = get<VECTRESULT>(clauseResult);
				hasSTClause = get<BOOLRESULT>(clauseResult);
				break;
			case callsStar:
				clauseResult = CallsStarAnalyzer(stClause, pkbPtr).solveClauseStmt();
				stResult = get<VECTRESULT>(clauseResult);
				hasSTClause = get<BOOLRESULT>(clauseResult);
			case next_:
				clauseResult = NextAnalyzer(stClause, pkbPtr).solveClauseStmt();
				stResult = get<VECTRESULT>(clauseResult);
				hasSTClause = get<BOOLRESULT>(clauseResult);
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

	if (!isQueryFalse()) {

		for (QueryElement patternClause : patternElements) {
			patternClauseType = patternClause.getPatternEntity();
			evaluatePatternRelation = mapPatternValues[patternClauseType];
			switch (evaluatePatternRelation) {
			case assign_:
				patternResult = get<VECTRESULT>(PatternAnalyzer(patternClause, pkbPtr).solvePatternAssign());
				hasPatternClause = get<BOOLRESULT>(PatternAnalyzer(patternClause, pkbPtr).solvePatternAssign());
				break;
			case while_: case if_:
				patternResult = get<VECTRESULT>(PatternAnalyzer(patternClause, pkbPtr).solvePatternIfWhile());
				hasPatternClause = get<BOOLRESULT>(PatternAnalyzer(patternClause, pkbPtr).solvePatternIfWhile());
				break;
			}
			if (!patternResult.empty())
				insertSTResult(patternResult);
			if (!hasPatternClause)
				break;
		}
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

vector<vector<string>> QueryAnalyzer::solveWhilePattern(QueryElement patternClause)
{
	return vector<vector<string>>();
}

tuple<vector<string>, vector<string>> QueryAnalyzer::solvePatAssignSyn(string arg1, 
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

vector<string> QueryAnalyzer::validatedPatAssignSyn(string arg1, string patExp, 
		string patType, string patSyn) {
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
	if (stResult.size() == 1){
		if (iteratorMapFindArg1 == synTableMap.end()) {
			scenario += 1;
		}
	} else if (stResult.size() > 1) {
		hasArg2 = true;
		if (iteratorMapFindArg1 == synTableMap.end())
			scenario += 4;
		else
			scenario += 2;
		if (synTableMap.find(stResult[ARGTWO].back()) == synTableMap.end()) {
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
				stResult, ARGONE, get<ARGONE>(tableLocation1), -1);
			clauseTableExists = get<ARGONE>(tableLocation2);
			result = hashJoin(result, mergedQueryTable[get<ARGONE>(tableLocation1)].size()-1, tableToMerge2,
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
		if (clauseTableLoc != -1) { //table is existing
			if (clauseTableLoc < qaTableLoc)
			{
				int temp = qaTableLoc;
				qaTableLoc = clauseTableLoc;
				clauseTableLoc = temp;
			}				
			mergedQueryTable.erase(mergedQueryTable.begin() + clauseTableLoc);
			for (auto entry : synTableMap) { //remap entire synTable for values greater than deleted index
				if (get<ARGONE>(entry.second) > clauseTableLoc)
					synTableMap[entry.first] = make_pair(get<ARGONE>(entry.second) - 1, get<ARGTWO>(entry.second));
			}
		}
	


		for (int i = 0; i < hashJoinTable.size(); i++) {
			hashJoinTable[i].push_back(tableKeys[i]);
			synTableMap[hashJoinTable.at(i).back()] = make_tuple(qaTableLoc, i);
		}
		
		mergedQueryTable.at(qaTableLoc) = hashJoinTable;
	}  
	return hashJoinTable;
}

vector<vector<vector<string>>> QueryAnalyzer::getMergedQueryTable()
{	// for debug
	return mergedQueryTable;
}

