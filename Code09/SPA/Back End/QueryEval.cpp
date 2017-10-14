#include "QueryEval.h"

enum selectValue 
{	undefinedSelect, stmt, assign, _while, variable, constant,
	prog_line		
};

enum suchThatValue
{
	undefinedSuchThat, modifies, uses, parent, parentStar, follows, 
	followsStar
};

enum patternValue
{
	undefinedPattern, wildcard, _variable, _string
};

enum clauseSelection
{
	no, yes
};

enum patternExpType
{
	undefinedExpression, _wildcard, substring, exact
};

enum patternSelectSynonymType
{
	 noCommon, entity, arg1, emptyPatternClause
};

static std::map<std::string, selectValue> mapSelectValues;
static std::map<std::string, suchThatValue> mapSuchThatValues;
static std::map<std::string, patternValue> mapPatternValues;
static std::map<std::string, patternExpType> mapPatternExpType;


/* call QueryEval qe = queryEval(PKB, QueryStatement) - step 1
   qe.runQueryEval() - step 2
   step 2 returns a vector<string> as the answer. 
   answer is in 1st entry, i.e entry 0.
 */
QueryEval::QueryEval()
{
	initSelectMap();
	initSuchThatMap();
	initPatternMap();
	initPatternExpTypeMap();
}

void QueryEval::initPatternExpTypeMap()
{
	mapPatternExpType["wildcard"] = _wildcard;
	mapPatternExpType["substring"] = substring;
	mapPatternExpType["exact"] = exact;
}

void QueryEval::setPKB(PKB pkb) {
	pkbReadOnly = pkb;
}

void QueryEval::initPatternMap()
{
	mapPatternValues["wildcard"] = wildcard;
	mapPatternValues["variable"] = _string;
	mapPatternValues["synonym"] = _variable;
}

void QueryEval::initSelectMap()
{ //to check with pql whether QS uses these strings in the entity as defined here.
	mapSelectValues["stmt"] = stmt;
	mapSelectValues["assign"] = assign;
	mapSelectValues["while"] = _while;
	mapSelectValues["variable"] = variable;
	mapSelectValues["constant"] = constant;
	mapSelectValues["prog_line"] = prog_line;
}

void QueryEval::initSuchThatMap()
{// to check with pl whether QS uses these strings (i.e inside the square bracket) in the entity as defined here.
	mapSuchThatValues["Modifies"] = modifies;
	mapSuchThatValues["Uses"] = uses;
	mapSuchThatValues["Parent"] = parent;
	mapSuchThatValues["Parent*"] = parentStar;
	mapSuchThatValues["Follows"] = follows;
	mapSuchThatValues["Follows*"] = followsStar;

}

//use this function to run queryEval
vector<string> QueryEval::runQueryEval()
{
//to convert vector<int> into vector<string>
//step 1:Extract from QS, the elements requiring evaluation
//step 2:determine the order of clauses to be evalulated
//step 3:evalulate each clause individually
//step 4:get the intersection of the answer if the select synonym is in the clauses
//step 5:return the answer as a vector<string>.
	vector<string> queryEvalAns;

	initForNewQs();
	findQueryElements(); //step 1
	evalQueryElements(); //step 3
	queryEvalAns = getQueryAnswer();
	

	return queryEvalAns;
}

//credit to 0x499602D2 from stackoverflow for code
string QueryEval::intVectToString(vector<int> vec)
{
	string str;
	for (int i = 0; i < vec.size(); ++i) {
		str += std::to_string(vec[i]);
		if (i + 1 != vec.size()) { // if the next iteration isn't the last
			str += ", "; // add a comma (optional)
		}
	}

	
	return str;
}

vector<string> QueryEval::getQueryAnswer()
{	
	string convertVectToString;
	vector<string> result;
	vector<string> answer = queryAnswerString;
	if (isResultInt) {
		convertVectToString = intVectToString(queryAnswerInt);
		answer = vector<string>();
		answer.push_back(convertVectToString);
	} 
	return answer;
}
void QueryEval::initForNewQs()
{
	//reset values for each new QS
	queryAnswerString = vector<string>();
	queryAnswerInt = vector<int>();
	selectResultString = vector<string>();
	selectResultInt = vector<int>();
	suchThatAnswerString = vector<string>();
	suchThatAnswerInt = vector<int>();
	patternAnswerString = vector<string>();
	patternAnswerInt = vector<int>();

}
void QueryEval::evalQueryElements()
{	
	int hasCommonSyno; // 0 = nil, 1 = 1st arg, 2 = 2nd arg
	int patternSynType;  // 0 = noCommon, 1 = assign, 2 = arg1, to be used with hasPatternClause 

	isResultInt = evalQuerySelect();
	hasCommonSyno = evalQuerySuchThat();
	patternSynType = evalQueryPattern();	
	combineSelectSuchThat(hasCommonSyno);
	resultSelectSuchThatPattern(patternSynType);

	//convert final result into a string here.
}

void QueryEval::combineSelectSuchThat(int hasCommonSyno) // similar code, can probaby refactor into simpler function in iter2
{
	if (isResultInt) {
		if (hasSuchThatClause) //by default it is always true, unless a suchThatClause fails
		{	
			int queryChoice = hasCommonSyno != 0 ? true : false;
			switch (queryChoice) //is there a common synonym in suchThatclause
			{
			case no:
				queryAnswerInt = selectResultInt;
				break;
			case yes:
				queryAnswerInt = suchThatAnswerInt;
				break;
			}
		} else {
			queryAnswerInt = vector<int>(); //to check if c++ requires initialisation to empty vector.
		}
	}
	else {
		if (hasSuchThatClause) //by default it is always true, unless a suchThatClause fails
		{
			int queryChoice = hasCommonSyno != 0 ? true : false;
			switch (queryChoice) //is there a common synonym in suchThatclause
			{
			case no:
				queryAnswerString = selectResultString;
				break;
			case yes:
				queryAnswerString = suchThatAnswerString;
				break;
			}
		}
	}
}

void QueryEval::resultSelectSuchThatPattern(int hasSynPattern)
{
	int commonSynoLocation;
	vector<int> selectSuchThatPatternInt;
	vector<string> selectSuchThatPatternString;
	if (hasPatternClause) //by default it is always true, unless a PatternClause fails
	{
		commonSynoLocation = hasSynPattern;
		switch (commonSynoLocation) //is there a common synonym in suchThatclause
		{
		case noCommon:
			break;
		case entity: //entity is the common Synonym
			selectSuchThatPatternInt = intersectionIntVect(queryAnswerInt,
				patternAnswerInt);
			queryAnswerInt = selectSuchThatPatternInt;
			break;
		case arg1:
			selectSuchThatPatternString = intersectionStringVect(queryAnswerString,
				patternAnswerString);
			queryAnswerString = selectSuchThatPatternString;
			break;
		case emptyPatternClause:
			break;
			}	
	} else {
		queryAnswerInt = vector<int>();
		queryAnswerString = vector<string>();
	}
}

int QueryEval::evalQuerySelect()
{	
	//ToDo: return 0 or 1 , 0 -> string 1 -> int
	//selectResultString = vector<string>(); To confirm c++ doesnt require initialisation.
	//selectResultInt = vector<int>();
	int choice = 1;

	comSynonym = selectElement.getSelectSynonym();
	switch (mapSelectValues[selectElement.getSelectEntity()]) 
	{
		case stmt: //stmt
			selectResultInt = pkbReadOnly.getAllStmt();
			break;		
		case assign: //assign
			selectResultInt = pkbReadOnly.getAssign();
			break;
		
		case _while: //while
			selectResultInt = pkbReadOnly.getWhile();
			break;

		case variable: //variable
			selectResultString = pkbReadOnly.getAllVariables();
			choice = 0;
			break;

		case constant: //constant
			selectResultString = pkbReadOnly.getAllConstants();
			choice = 0;
			break;

		case prog_line: //prog_line , not in use atm
			break;

	}
	return choice;
}

int QueryEval::evalQueryPattern()
{	//review code
	vector<vector<int>> intermediatePatternResultInt;
	vector<vector<string>> intermediatePatternResultString;
	vector<string> evalVarPatterns;
	vector<tuple<int, string>> pkbPattern;
	vector<int> patternVarStmtValues;
	vector<string> patternVarValues;
	tuple<vector<int>, vector<string>> singleResult;
	vector<string> tempVectString;
	int comEval = 3; // evaluate if common synonym exist
	//comSynonym


	for (int i = 0; i < patternElements.size(); i++){
		comEval = (comSynonym ==(patternElements[i].getPatternArg1())) ? 2 : //2 = arg1 pattern syno 
			(comSynonym == (patternElements[i].getPatternSynonym())) ? 1 : 0; //1 = entity syno , 0 = no common syno		
		switch (comEval){
			case 0: //no common synonym
				evalVarPatterns = pkbReadOnly.getAllVariables();
				singleResult = evalSinglePatternResult(evalVarPatterns, patternElements[i]);
				patternVarStmtValues = get<0>(singleResult);
				patternVarValues = get<1>(singleResult);
				if (patternVarStmtValues.empty() && patternVarValues.empty())
					isPatternFalse(false);
				break;
			case 1: //wants stmt numbers
			case 2: //wants var values
				switch (mapPatternValues[patternElements[i].getPatternArg1Type()]){ //inefficient, room for impv
					case wildcard:
					case _variable:
						evalVarPatterns = pkbReadOnly.getAllVariables();
						singleResult = evalSinglePatternResult(evalVarPatterns, patternElements[i]);
						patternVarStmtValues = get<0>(singleResult);
						patternVarValues = get<1>(singleResult);
						break;
					case _string:
						tempVectString.push_back(patternElements[i].getPatternArg1()); //to remove 0 when pql done editing
						singleResult = evalSinglePatternResult(tempVectString, patternElements[i]);
						patternVarStmtValues = get<0>(singleResult);
						patternVarValues = get<1>(singleResult);
						break;
				}
		}
		if(!patternVarValues.empty())
			intermediatePatternResultString.push_back(patternVarValues);
		if(!patternVarStmtValues.empty())
			intermediatePatternResultInt.push_back(patternVarStmtValues);
	}
	if (comEval != 3) {
		patternAnswerInt = intermediatePatternResultInt[0];
		patternAnswerString = intermediatePatternResultString[0]; //to be edited in iter 2, for now only 1 pattern.
	}
	return comEval;
};
tuple<vector<int>,vector<string>> QueryEval::evalSinglePatternResult(vector<string> evalVarPatterns, 
																		QueryElement patternElements)
{	
	vector<int> patternVarStmtValues;
	vector<string> patternVarValues;
	patternVarValues = vector<string>();
	bool varContainsPattern = false;
	vector<tuple<int, string>> pkbPattern;
	for (int j = 0; j < evalVarPatterns.size(); j++) {
		patternVarStmtValues = vector<int>();
		pkbPattern = pkbReadOnly.getPattern(evalVarPatterns[j]);
		for (tuple<int, string> evalPattExpression : pkbPattern) {
			// to check if it is sufficient for 1 variable to satisfy pattern constraint
			switch (mapPatternExpType[patternElements.getPatternArg2Type()]) {
			case _wildcard:
				varContainsPattern = true;
				patternVarStmtValues.push_back(get<0>(evalPattExpression));
				break;
			case substring: //to edit when pql done finishing edit
				if (get<1>(evalPattExpression).find(patternElements.getPatternArg2()) != string::npos) {
					patternVarStmtValues.push_back(get<0>(evalPattExpression));
					varContainsPattern = true;
				}
				break;
			case exact:
				if ((get<1>(evalPattExpression) == (patternElements.getPatternArg2())) == 0) {
					patternVarStmtValues.push_back(get<0>(evalPattExpression));
					varContainsPattern = true;
				}
				break;
			}
		}
		if (varContainsPattern)
			patternVarValues.push_back(evalVarPatterns[j]);
	}
	tuple<vector<int>, vector<string>> answer(patternVarStmtValues, patternVarValues);
	return answer;
}

int QueryEval::evalQuerySuchThat()
{
	vector<vector<int>> suchThatResult;
	vector<vector<string>> suchThatResultString;
	int argEval = 3; //option to determine which argument to evaluate (s1,s2) 
				// 1 => arg1 = evaluate arg1 since it is integer, 0=> evalulate arg 2
				// 3 => no such that clause
	

	for (int i = 0; i < suchThatElements.size(); i++) //evaluate 1 suchThat clause at a time
	{ 
		//option 0 : both args = syno, 1 = arg1 syno, 2 = arg2 syno for finding common synonym
		int comEval = (comSynonym == (suchThatElements[i].getSuchThatArg1())) ? 1 :
			(comSynonym == (suchThatElements[i].getSuchThatArg2())) ? 2 : 0;
		string argType1 = suchThatElements[i].getSuchThatArg1Type();
		string argType2 = suchThatElements[i].getSuchThatArg2Type();
		argEval = (argType1 == "number")? 1 : 0; //confirm with pql it is number
		vector<string> intermediateResultString;
		vector<int> intermediateResultInt;
		vector<int> tempVect;
		vector<int> tempVect2;
		vector<string> tempStringVect;
		vector<string> tempStringVect2;

		switch (mapSuchThatValues[suchThatElements[i].getSuchThatRel()]) //check with pql, this is meant to be follow etc
		{
			case modifies:
				switch (comEval)
				{
					case 0: //both synonyms, 
						isSuchThatFalse(true);
						break;
					case 1: //arg1 synonym
						if (suchThatElements[i].getSuchThatArg2Type() != "number") { //arg1 = wildcard or some syno
							tempVect = pkbReadOnly.getAssign();
							for (int a = 0; a < tempVect.size(); a++) {
								tempStringVect = vector<string>();
								tempStringVect = pkbReadOnly.getModifies(tempVect[a]);
								if (!tempStringVect.empty()) {
									tempVect2.push_back(tempVect[a]);
								}
							}
							intermediateResultInt = tempVect2;
						}
						else {
							intermediateResultInt =
								pkbReadOnly.getModifiedBy(suchThatElements[i].getSuchThatArg2());
						}
						intermediateResultInt.empty() ? isSuchThatFalse(false) : 
							suchThatResult.push_back(intermediateResultInt);
						break;
					case 2: //arg2 synonym
						if (suchThatElements[i].getSuchThatArg1Type() != "number") { //arg1 = wildcard or some syno
							tempStringVect = pkbReadOnly.getAllVariables();
							for (int a = 0; a < tempStringVect.size(); a++) {
								tempVect = vector<int>();
								tempVect=pkbReadOnly.getModifiedBy(tempStringVect[a]);
								if (!tempVect.empty()) {
									tempStringVect2.push_back(tempStringVect[a]);
								}
							}
							intermediateResultString = tempStringVect2;
						}
						else {
							tempStringVect = (pkbReadOnly.getModifies(3)); // pkb error , try 2,3,4, to be remove
							intermediateResultString =
								pkbReadOnly.getModifies(stoi(suchThatElements[i].getSuchThatArg1()));
						}
						intermediateResultString.empty() ? isSuchThatFalse(false) :
								suchThatResultString.push_back(intermediateResultString);
						
						break;
				}
				break;
			case uses:
				switch (comEval)
				{
					case 0:
						isSuchThatFalse(true);
						break;
					case 1:
						intermediateResultInt =
							pkbReadOnly.getUsedBy(suchThatElements[i].getSuchThatArg2());
						intermediateResultInt.empty() ? isSuchThatFalse(false) : 
							suchThatResult.push_back(intermediateResultInt);
						break;
					case 2:
						intermediateResultString =
							pkbReadOnly.getUses(stoi(suchThatElements[i].getSuchThatArg1()));
						intermediateResultString.empty() ? isSuchThatFalse(false) : 
							suchThatResultString.push_back(intermediateResultString);
						break;
				}
				break;
			case parent: // if = check for both arguments number
				if ((argType1 == ("number")) && (argType2==("number"))) {
					parentResult(stoi(suchThatElements[i].getSuchThatArg1()), argEval).front() ==
						stoi(suchThatElements[i].getSuchThatArg2()) ? 
						isSuchThatFalse(true) : isSuchThatFalse(false);
				} else {
					suchThatResult.push_back(
						argEval ?
						parentResult(stoi(suchThatElements[i].getSuchThatArg1()), argEval) :
						parentResult(stoi(suchThatElements[i].getSuchThatArg2()), argEval));
					suchThatResult[i].empty() ? isSuchThatFalse(false) : isSuchThatFalse(true); //to be fine tune in iter2, need to consider intermediate results
				}
				break;
			case parentStar:
				if ((argType1 == ("number")) && (argType2==("number"))) {
					tempVect = parentStarResult(stoi(suchThatElements[i].getSuchThatArg1()), argEval);
					find(tempVect.begin(), tempVect.end(), stoi(suchThatElements[i].getSuchThatArg2())) !=
						tempVect.end() ? isSuchThatFalse(true) : isSuchThatFalse(false);
				}
				else {
					suchThatResult.push_back(
						argEval ?
						parentStarResult(stoi(suchThatElements[i].getSuchThatArg1()), argEval) :
						parentStarResult(stoi(suchThatElements[i].getSuchThatArg2()), argEval));
					suchThatResult[i].empty() ? isSuchThatFalse(false) : isSuchThatFalse(true);
				}
				break;
			case follows:
				if ((argType1==("number")) && (argType2==("number"))) {
					followResult(stoi(suchThatElements[i].getSuchThatArg1()), argEval).front() ==
						stoi(suchThatElements[i].getSuchThatArg2()) ?
						isSuchThatFalse(true) : isSuchThatFalse(false);
				}
				else {
					suchThatResult.push_back(
						argEval ?
						followResult(stoi(suchThatElements[i].getSuchThatArg1()), argEval) :
						followResult(stoi(suchThatElements[i].getSuchThatArg2()), argEval));
					suchThatResult[i].empty() ? isSuchThatFalse(false) : isSuchThatFalse(true);
					// i = arg1/2 , j = 1 => arg1 -> constant, 0 =>arg2 -> constant
				}
				break;
			case followsStar:
				if ((argType1==("number")) && (argType2==("number"))) {
					tempVect = followStarResult(stoi(suchThatElements[i].getSuchThatArg1()), argEval);
					find(tempVect.begin(), tempVect.end(), stoi(suchThatElements[i].getSuchThatArg2())) !=
						tempVect.end() ? isSuchThatFalse(true) : isSuchThatFalse(false);
				}
				else {
					suchThatResult.push_back(
						argEval ?
						followStarResult(stoi(suchThatElements[i].getSuchThatArg1()), argEval) :
						followStarResult(stoi(suchThatElements[i].getSuchThatArg2()), argEval));
					suchThatResult[i].empty() ? isSuchThatFalse(false) : isSuchThatFalse(true);
				}
				break;
		}
		if(!suchThatResult.empty())
			suchThatAnswerInt = suchThatResult[0]; // to be changed in iter2, since only at most 1 suchThat in iter1
		if(!suchThatResultString.empty())
			suchThatAnswerString = suchThatResultString[0];
		if (argEval != 3)
			argEval = comEval;
	}

	return argEval;
}

void QueryEval::isSuchThatFalse(bool clauseValue)
{	//ensure that suchThatClauses is false if there exist 1
	//clause which is false.
	if (hasSuchThatClause)
		hasSuchThatClause = clauseValue;
}

void QueryEval::isPatternFalse(bool clauseValue)
{	//ensure that PatternClauses is false if there exist 1
	//clause which is false.
	if (hasPatternClause)
		hasPatternClause = clauseValue;
}

void QueryEval::findQueryElements() 
{
	hasSuchThatClause = true;
	hasPatternClause = true;
	selectElement = qsReadOnly.getSelectQueryElement();
	suchThatElements = qsReadOnly.getSuchThatQueryElement(); //all String
	patternElements = qsReadOnly.getPatternQueryElement();
	synonymElements = qsReadOnly.getSynonymEntityList();
}

void QueryEval::setQueryStatement(QueryStatement qs) 
{
	qsReadOnly = qs;
}

/**
s1 = some statement number, opt = 0 or 1
0-> use getFollows
1-> use getFollowedby
*/
vector<int> QueryEval::followResult(int s1, int opt)
{	
	vector<int> queryResult = vector<int>();
	
	//case: Follow("5",s)
	if (opt) {
		queryResult = pkbReadOnly.getFollows(s1);  
	}
	//case: Follow(s,"5")
	else {
		queryResult = pkbReadOnly.getFollowedBy(s1);
	}

	return queryResult;
}

/**
s1 = some statement number, opt = 0 or 1
0-> use getFollowsStar
1-> use getFollowedByStar
*/
vector<int> QueryEval::followStarResult(int s1, int opt) {
	vector<int> queryResult = vector<int>();
	//case: Follow*("7",s)
	if (opt) {
		queryResult = pkbReadOnly.getFollowsStar(s1);
	}
	//case: Follow*(s,"7")
	else {
		queryResult = pkbReadOnly.getFollowedByStar(s1);
	}
	return queryResult;
}

/**
s1 = some statement number, opt = 0 or 1
0-> use getParent
1-> use getChild
*/
vector<int> QueryEval::parentResult(int s1, int opt) {
	vector<int> queryResult = vector<int>();
	//case: Parent("7",s)
	if (opt) {
		queryResult = pkbReadOnly.getChild(s1);
	}
	//case: Parent(s,"7")
	else {
		queryResult = pkbReadOnly.getParent(s1);
	}
	return queryResult;
}

/**
s1 = some statement number, opt = 0 or 1
0-> use getParentStar
1-> use getChildStar
*/
vector<int> QueryEval::parentStarResult(int s1, int opt) {
	vector<int> queryResult = vector<int>();
	//case: Parent*("7",s)
	if (opt) {
		queryResult = pkbReadOnly.getChildStar(s1);
	}
	//case: Parent*(s,"7")
	else {
		queryResult = pkbReadOnly.getParentStar(s1);
	}
	return queryResult;
}

//Credit to stackoverflow deepmax
vector<string> QueryEval::intersectionStringVect(vector<string> &v1, vector<string> &v2)
{
	vector<string> v3;

	sort(v1.begin(), v1.end());
	sort(v2.begin(), v2.end());

	set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v3));

	return v3;
}

vector<int> QueryEval::intersectionIntVect(vector<int> &v1, vector<int> &v2)
{
	vector<int> v3;

	sort(v1.begin(), v1.end());
	sort(v2.begin(), v2.end());

	set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v3));

	return v3;
}


/* Old Pattern Code Snippet for after evalPatternValues case wildcard,variable
							for (int j = 0; j < evalVarPatterns.size(); j++) {
							patternVarStmtValues = vector<int>();
							pkbPattern = pkbPtr.getPattern(evalVarPatterns[j]);
							for (tuple<int,string> evalPattExpression : pkbPattern ) {
								// to check if it is sufficient for 1 variable to satisfy pattern constraint
								switch (mapPatternExpType[patternElements[i].getPatternArg2Type()]) {
									case _wildcard:
										varContainsPattern = true;
										patternVarStmtValues.push_back(get<0>(evalPattExpression));
									break;
									case substring:
										if (get<1>(evalPattExpression).find(patternElements[i].getPatternArg2) != string::npos) {
											patternVarStmtValues.push_back(get<0>(evalPattExpression));
											varContainsPattern = true;
										}
										break;
									case exact:
										if (get<1>(evalPattExpression).compare(patternElements[i].getPatternArg2) == 0) {
											patternVarStmtValues.push_back(get<0>(evalPattExpression));
											varContainsPattern = true;
										}											 										
										break;
								}
							}
							if(varContainsPattern)
								patternVarValues.push_back(evalVarPatterns[j]);
						} 
*/