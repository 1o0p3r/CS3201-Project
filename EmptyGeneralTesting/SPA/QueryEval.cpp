#include "QueryEval.h"

static enum selectValue 
{	undefined, stmt, assign, _while, variable, constant,
	prog_line		
};

static enum suchThatValue
{
	undefined, modifies, uses, parent, parentStar, follows, 
	followsStar
};

static std::map<std::string, selectValue> mapSelectValues;
static std::map<std::string, suchThatValue> mapSuchThatValues;

QueryEval::QueryEval(PKB pkb, QueryStatement qs)
{
	pkbReadOnly = pkb;
	qsReadOnly = qs;
	
	initSelectMap();
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
}

void QueryEval::evalQueryElements()
{
	evalQuerySelect();
	evalQuerySuchThat();
	evalQueryPattern();	
}

void QueryEval::evalQuerySelect()
{	
	//ToDo: return 0 or 1 , 0 -> string 1 -> int
	selectResultString = vector<string>();
	selectResultInt = vector<int>();

	comSynonym = selectElement.getSelectSynonym();
	switch (mapSelectValues[selectElement.getSelectEntity()]) 
	{
		case stmt: //stmt
			//selectResultInt = ??;
			break;
		
		case assign: //assign
			selectResultInt = pkbReadOnly.getAssign();
			break;
		
		case _while: //while
			selectResultInt = pkbReadOnly.getWhile();
			break;

		case variable: //variable
			selectResultString = pkbReadOnly.getAllVariables();
			break;

		case constant: //constant
			selectResultString = pkbReadOnly.getAllConstants();
			break;

		case prog_line: //prog_line , not in use atm
			break;

	}
}

void QueryEval::evalQuerySuchThat()
{
	vector<vector<int>> suchThatResult;
	

	for (int i = 0; i < suchThatElements.size(); i++) //evaluate 1 suchThat clause at a time
	{ 
		int argEval; //option to determine which argument to evaluate
		//case 0: no common synonym, case 1: arg1 = common synonym, case 2: arg 2 = common synonym
		int comEval = comSynonym.compare(suchThatElements[i].getSuchThatArg1) ? 1 :
			comSynonym.compare(suchThatElements[i].getSuchThatArg2) ? 2 : 0;
		string argType1 = suchThatElements[i].getSuchThatArgType1();
		string argType2 = suchThatElements[i].getSuchThatArgType2();
		argEval = (argType1.compare("constant"))? 1 : 0;
		vector<string> intermediateResultString;
		vector<int> intermediateResultInt;
		suchThatClause = 0;

		switch (mapSuchThatValues[suchThatElements[i].getSuchThatType]) //check with pql, this is meant to be follow etc
		{
			case modifies:
				switch (comEval)
				{
					case 0:
						suchThatClause = true;
						break;
					case 1:
						intermediateResultInt = 
						pkbReadOnly.getModifiedBy(suchThatElements[i].getSuchThatArg2);
						suchThatClause = intermediateResultInt.empty ? false : true;
						break;
					case 2:
						intermediateResultString =
							pkbReadOnly.getModifies(suchThatElements[i].getSuchThatArg1);
						suchThatClause = intermediateResultString.empty ? false : true;
						break;
				}
				break;
			case uses:
				switch (comEval)
				{
					case 0:
						suchThatClause = true;
						break;
					case 1:
						intermediateResultInt =
							pkbReadOnly.getUsedBy(suchThatElements[i].getSuchThatArg2);
						suchThatClause = intermediateResultInt.empty ? false : true;
						break;
					case 2:
						intermediateResultString =
							pkbReadOnly.getUses(suchThatElements[i].getSuchThatArg1);
						suchThatClause = intermediateResultString.empty ? false : true;
						break;
				}
				break;
			case parent:
				suchThatResult.push_back(
					argEval ?
					parentResult(suchThatElements[i].getSuchThatArg1(), argEval) :
					parentResult(suchThatElements[i].getSuchThatArg2(), argEval));
				suchThatClause = suchThatResult.empty ? false : true;
				break;
			case parentStar:
				suchThatResult.push_back(
					argEval ?
					parentStarResult(suchThatElements[i].getSuchThatArg1(), argEval) :
					parentStarResult(suchThatElements[i].getSuchThatArg2(), argEval));
				suchThatClause = suchThatResult.empty ? false : true;
				break;
			case follows:
				suchThatResult.push_back( 
					argEval ? 
					followResult(suchThatElements[i].getSuchThatArg1(),argEval) :
					followResult(suchThatElements[i].getSuchThatArg2(),argEval)); 
				suchThatClause = suchThatResult.empty ? false : true;
				// i = arg1/2 , j = 1 => arg1 -> constant, 0 =>arg2 -> constant
				break;
			case followsStar:
				suchThatResult.push_back(
					argEval ?
					followStarResult(suchThatElements[i].getSuchThatArg1(), argEval) :
					followStarResult(suchThatElements[i].getSuchThatArg2(), argEval));
				suchThatClause = suchThatResult.empty ? false : true;
				break;
		}
	}
}

void QueryEval::findQueryElements() 
{
	selectElement = qsReadOnly.getSelectQueryElement();
	suchThatElements = qsReadOnly.getSuchThatQueryElement();
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

vector<int> QueryEval::modifiesResult(int s1, int opt) {
	vector<int> queryResult = vector<int>();

	switch (opt) {
		//Modifies("1",z)
		case ASSIGNMOD :
			queryResult = pkbReadOnly.getModifies(s1);
			break;
		
		//Modifies(s1,"z")
		case VARMODBY :
			queryResult = pkbReadOnly.getModifiedBy(s1);
			break;
		
		//Modifies("someProc", z)
		case PROCMOD :
			queryResult = pkbReadOnly.getProcModifies(s1);
			break;

		//Modifies(procedure,"z")
		case PROCMODBY :
			queryResult = pkbReadOnly.getProcModifiedBy(s1);

	}
	return queryResult;
}

vector<int> QueryEval::usesResult(string s1, int opt) {
	vector<int> queryResult = vector<int>();

	switch (opt) {
		//uses("1",z)
	case ASSIGNMOD:
		queryResult = pkbReadOnly.getUses(s1);
		break;

		//uses(s1,"z")
	case VARMODBY:
		queryResult = pkbReadOnly.getUsedBy(s1);
		break;

		//uses("someProc", z)
	case PROCMOD:
		queryResult = pkbReadOnly.getProcUses(s1);
		break;

		//uses(procedure,"z")
	case PROCMODBY:
		queryResult = pkbReadOnly.getProcUsedBy(s1);

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