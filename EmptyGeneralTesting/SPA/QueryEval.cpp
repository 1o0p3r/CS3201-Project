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

static std::map<std::string, selectValue> mapSelectValues;
static std::map<std::string, suchThatValue> mapSuchThatValues;
static std::map<std::string, patternValue> mapPatternValues;

QueryEval::QueryEval(PKB pkb, QueryStatement qs)
{
	pkbReadOnly = pkb;
	qsReadOnly = qs;
	hasSuchThatClause = true;
	hasPatternClause = true;
	initSelectMap();
	initSuchThatMap();
	initPatternMap();
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
//	undefined, modifiesS, usesS, parent, parentT, Follows,
	//	FollowsT
	mapSuchThatValues["modifies"] = modifies;
	mapSuchThatValues["uses"] = uses;
	mapSuchThatValues["parent"] = parent;
	mapSuchThatValues["parentStar"] = parentStar;
	mapSuchThatValues["follows"] = follows;
	mapSuchThatValues["followStar"] = followsStar;

}

vector<string> QueryEval::runQueryEval()
{
//to convert vector<int> into vector<string>
//step 1:Extract from QS, the elements requiring evaluation
//step 2:determine the order of clauses to be evalulated
//step 3:evalulate each clause individually
//step 4:get the intersection of the answer if the select synonym is in the clauses
//step 5:return the answer as a vector<string>.
	
	findQueryElements(); //step 1
	evalQueryElements(); //step 3

	return vector<string>();
}

void QueryEval::evalQueryElements()
{	
	int hasSynoSuchThat; // 0 = nil, 1 = 1st arg, 2 = 2nd arg
	int hasSynPattern;  // 0 = nil, 1 = assign, 2 = arg1, 

	isResultInt = evalQuerySelect();
	hasSynoSuchThat = evalQuerySuchThat();
	hasSynPattern = evalQueryPattern();	
	combineSelectSuchThat(hasSynoSuchThat);
	resultSelectSuchThatPattern(hasSynPattern);

	//convert final result into a string here.
}

void QueryEval::combineSelectSuchThat(int hasSynoSuchThat) // similar code, can probaby refactor into simpler function in iter2
{
	if (isResultInt) {
		if (hasSuchThatClause) //by default it is always true, unless a suchThatClause fails
		{	
			int queryChoice = hasSynoSuchThat != 0 ? true : false;
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
			int queryChoice = hasSynoSuchThat != 0 ? true : false;
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
{	
	vector<vector<int>> intermediatePatternResultInt;
	vector<vector<string>> intermediatePatternResultString;
	int comEval;
	//comSynonym
	for (int i = 0; i < patternElements.size(); i++)
	{
		comEval = comSynonym.compare(patternElements[i].getPatternArg1(i)) ? 2 : //2 = arg1 pattern syno 
			comSynonym.compare(patternElements[i].getPatternSynonym(i)) ? 1 : 0; //1 = entity syno , 0 = no common syno		
		switch (comEval)
		{
			case 0: //no common synonym
				switch (mapPatternValues[patternElements[i].getPatternArg1Type()])
				{
					case wildcard:
					case _variable:

						break;
					case _string:
						break;
				}
				break;
		}
	}

	return 0;
}

int QueryEval::evalQuerySuchThat()
{
	vector<vector<int>> suchThatResult;
	vector<vector<string>> suchThatResultString;
	int argEval; //option to determine which argument to evaluate (s1,s2) 
				 //case 0: both synos, case 1: arg1 = integer, case 2: arg 2 = integer, case 3: both integer
	

	for (int i = 0; i < suchThatElements.size(); i++) //evaluate 1 suchThat clause at a time
	{ 
		//option 0 : both args = syno, 1 = arg1 syno, 2 = arg2 syno for finding common synonym
		int comEval = comSynonym.compare(suchThatElements[i].getSuchThatArg1(i)) ? 1 :
			comSynonym.compare(suchThatElements[i].getSuchThatArg2(i)) ? 2 : 0;
		string argType1 = suchThatElements[i].getSuchThatArg1Type(i);
		string argType2 = suchThatElements[i].getSuchThatArg2Type(i);
		argEval = (argType1.compare("number"))? 1 : 0; //confirm with pql it is number
		vector<string> intermediateResultString;
		vector<int> intermediateResultInt;
		

		switch (mapSuchThatValues[suchThatElements[i].getSuchThatRel()]) //check with pql, this is meant to be follow etc
		{
			case modifies:
				switch (comEval)
				{
					case 0: //both synonyms
						isSuchThatFalse(true);
						break;
					case 1: //arg1 synonym
						intermediateResultInt = 
						pkbReadOnly.getModifiedBy(suchThatElements[i].getSuchThatArg2(i));
						intermediateResultInt.empty() ? isSuchThatFalse(false) : 
							suchThatResult.push_back(intermediateResultInt);
						break;
					case 2: //arg2 synonym
						intermediateResultString =
							pkbReadOnly.getModifies(stoi(suchThatElements[i].getSuchThatArg1(i)));
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
							pkbReadOnly.getUsedBy(suchThatElements[i].getSuchThatArg2(i));
						intermediateResultInt.empty() ? isSuchThatFalse(false) : 
							suchThatResult.push_back(intermediateResultInt);
						break;
					case 2:
						intermediateResultString =
							pkbReadOnly.getUses(stoi(suchThatElements[i].getSuchThatArg1(i)));
						intermediateResultString.empty() ? isSuchThatFalse(false) : 
							suchThatResultString.push_back(intermediateResultString);
						break;
				}
				break;
			case parent:
				suchThatResult.push_back(
					argEval ?
					parentResult(stoi(suchThatElements[i].getSuchThatArg1(i)), argEval) :
					parentResult(stoi(suchThatElements[i].getSuchThatArg2(i)), argEval));
				suchThatResult.empty() ? isSuchThatFalse(false) : isSuchThatFalse(true); //to be fine tune in iter2, need to consider intermediate results
				break;
			case parentStar:
				suchThatResult.push_back(
					argEval ?
					parentStarResult(stoi(suchThatElements[i].getSuchThatArg1(i)), argEval) :
					parentStarResult(stoi(suchThatElements[i].getSuchThatArg2(i)), argEval));
				suchThatResult.empty() ? isSuchThatFalse(false) : isSuchThatFalse(true);
				break;
			case follows:
				suchThatResult.push_back( 
					argEval ? 
					followResult(stoi(suchThatElements[i].getSuchThatArg1(i)),argEval) :
					followResult(stoi(suchThatElements[i].getSuchThatArg2(i)),argEval)); 
				suchThatResult.empty() ? isSuchThatFalse(false) : isSuchThatFalse(true);
				// i = arg1/2 , j = 1 => arg1 -> constant, 0 =>arg2 -> constant
				break;
			case followsStar:
				suchThatResult.push_back(
					argEval ?
					followStarResult(stoi(suchThatElements[i].getSuchThatArg1(i)), argEval) :
					followStarResult(stoi(suchThatElements[i].getSuchThatArg2(i)), argEval));
				suchThatResult.empty() ? isSuchThatFalse(false) : isSuchThatFalse(true);
				break;
		}
		suchThatAnswerInt = suchThatResult[0]; // to be changed in iter2, since only at most 1 suchThat in iter1
		suchThatAnswerString = suchThatResultString[0];
	}
	return argEval;
}

void QueryEval::isSuchThatFalse(bool clauseValue)
{	//ensure that suchThatClauses is false if there exist 1
	//clause which is false.
	if (hasSuchThatClause)
		hasSuchThatClause = clauseValue;
}

void QueryEval::findQueryElements() 
{
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
vector<string> instersection(vector<string> &v1, vector<string> &v2)
{
	vector<string> v3;

	sort(v1.begin(), v1.end());
	sort(v2.begin(), v2.end());

	set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v3));

	return v3;
}