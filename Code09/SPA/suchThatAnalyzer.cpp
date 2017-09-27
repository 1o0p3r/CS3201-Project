#include "SuchThatAnalyzer.h"

const string WILDCARD = "wildcard";

enum clauseCases 
{
	varVar, varSyn, varWild, synVar, synSyn, synWild,
	wildVar, wildSyn, wildWild
};

suchThatAnalyzer::suchThatAnalyzer(QueryElement stClause, PKB pkb)
{ 
	pkbReadOnly = pkb;
	stEntity = stClause.getSuchThatRel();
	arg1 = stClause.getSuchThatArg1();
	arg1Entity = stClause.getSuchThatArg1Entity();
	arg1Type = stClause.getSuchThatArg1Type();
	
	arg2 = stClause.getSuchThatArg2();
	arg2Entity = stClause.getSuchThatArg2Entity();
	arg2Type = stClause.getSuchThatArg2Type();

	hasSuchThatClause = true;
}

tuple<bool,vector<vector<string>>> suchThatAnalyzer::solveClause() 
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
	case wildSyn:
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
		clauseResult = addArgOneResult(arg1);
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
