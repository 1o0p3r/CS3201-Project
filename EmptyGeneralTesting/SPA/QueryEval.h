#pragma once

#include "PKB.h"
#include "QueryStatement.h"
#include "QueryElement.h"

#include <iterator>
#include <algorithm>
#include <sstream>
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

	void initPatternExpTypeMap();
	void initPatternMap();
	void initSelectMap();
	void initSuchThatMap();
	
	vector<string> runQueryEval();
	string intVectToString(vector<int> vec);
	vector<string> getQueryAnswer();
	void initForNewQs();
	void evalQueryElements();
	void combineSelectSuchThat(int hasSynoSuchThat);
	void resultSelectSuchThatPattern(int hasPatternSuchThat);
	int evalQuerySelect();
	int evalQueryPattern();
	tuple<vector<int>, vector<string>> evalSinglePatternResult( vector<string> evalVarPatterns, 
																	QueryElement patternElements);
	void findQueryElements();
	void setQueryStatement(QueryStatement qs);

	int evalQuerySuchThat();

	void isSuchThatFalse(bool clauseValue);

	void isPatternFalse(bool clauseValue);

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
	vector<string> patternAnswerString;
	vector<int> patternAnswerInt;
	


	vector<int> followResult(int s1, int opt);
	vector<int> followStarResult(int s1, int opt);
	vector<int> parentResult(int s1, int opt);
	vector<int> parentStarResult(int s1, int opt);

	vector<string> intersectionStringVect(vector<string>& v1, vector<string>& v2);

	vector<int> intersectionIntVect(vector<int>& v1, vector<int>& v2);

};
