#pragma once

#include "Follow.h"
#include "Parent.h"
#include "Modify.h"
#include "Use.h"
#include "PKB.h"
#include <iterator>
#include <algorithm>
#include "QueryStatement.h"
#include "QueryElement.h"

#include <map>
#include <vector>
#include <string>
using namespace std;

class QueryEval
{
public:
	static const int ASSIGNMOD = 0;
	static const int ASSIGNUSE = 0;
	static const int VARMODBY = 1;
	static const int VARUSEBY = 1;
	static const int PROCMOD = 2;
	static const int PROCUSE = 2;
	static const int PROCMODBY = 3;
	static const int PROCUSEBY = 3;

	QueryEval(PKB pkb, QueryStatement qs);

	void initSelectMap();

	void initSuchThatMap();
	
	vector<string> runQueryEval();
	void evalQueryElements();
	void combineSelectSuchThat(int hasSynoSuchThat);
	void resultSelectSuchThatPattern(int hasPatternSuchThat);
	int evalQuerySelect();
	int evalQueryPattern();
	void findQueryElements();
	void setQueryStatement(QueryStatement qs);

	int evalQuerySuchThat();

	void isSuchThatFalse(bool clauseValue);

private:
	PKB pkbReadOnly;
	string comSynonym;
	QueryStatement qsReadOnly;
	bool hasSuchThatClause;
	bool hasPatternClause;
	bool isResultInt;

	QueryElement selectElement;
	vector<SynonymEntityPair> synonymElements;
	vector<QueryElement> suchThatElements;
	vector<QueryElement> patternElements;
	vector<string> queryAnswerString;
	vector<int> queryAnswerInt;

	vector<string> selectResultString;// for var and constant
	vector<int> selectResultInt; //for stmt etc
	vector<string> suchThatAnswerString;
	vector<int> suchThatAnswerInt;


	vector<int> followResult(int s1, int opt);
	vector<int> followStarResult(int s1, int opt);
	vector<int> parentResult(int s1, int opt);
	vector<int> parentStarResult(int s1, int opt);

};

/* 
Getters from querystatement
QueryStatement();
QueryElement getSelectQueryElement();
vector<QueryElement> getSuchThatQueryElement();
vector<QueryElement> getPatternQueryElement();
vector<SynonymEntityPair> getSynonymEntityList();

*/