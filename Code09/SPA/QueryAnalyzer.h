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
#include <unordered_map>
using namespace std;

class QueryAnalyzer
{
public:
	QueryAnalyzer();

	void setPKB(PKB pkb);
	void setQS(QueryStatement qs);
	vector<string> runQueryEval();
private:
	QueryStatement qsReadOnly;
	QueryElement selectElement;
	vector<QueryElement> stElements;
	vector<QueryElement> patternElements;
	vector<vector<vector<string>>> mergedQueryTable;
	unordered_map<string, tuple<int,int>> synTableMap;
	PKB pkbReadOnly;
	bool hasSTClause;
	bool hasPatternClause;

	void initSelectMap();
	void initSuchThatMap();
	void initParentSynTypeMap();

	void findQueryElements();
	vector<string> analyzeClauseResults();
	vector<string> analyzeSelect(string selectEntity);
	void solveSTClause();
	void insertSTResult(vector<vector<string>> stResult);
	
	void insertArg1Arg2CommonSynTable(vector<vector<string>> stResult);
	void merge2DVectorTwoSyno(vector<vector<string>> tableToMerge1, 
			vector<vector<string>> stResult, tuple<int,int> tableLocation1, tuple<int,int> tableLocation2);
	void merge2DVectorDisjointTable(vector<vector<string>> tableToMerge1, vector<vector<string>> tableToMerge2,
		vector<vector<string>> stResult, tuple<int, int> tableLocation1, tuple<int, int> tableLocation2);
	void insertArg1CommonSynTable(vector<vector<string>>stResult, bool hasArg2);
	void insertArg2CommonSynTable(vector<vector<string>>stResult, bool hasArg2);
	void merge2DVector(vector<vector<string>> tableToMerge, vector<vector<string>> stResult, 
		 tuple<int,int> tableLocation, int option, bool hasArg2);
	tuple<vector<string>, vector<int>, vector<int>> stringVectIntersect(vector<string> v1, vector<string> v2);
	void insertNoCommonSynToTable(vector<vector<string>> stResult, 
			int tableIndex, bool hasArg2);
	

	vector<vector<string>> solveParent(QueryElement suchThatClause);
	void isParent(string arg1, string arg2);
	vector<string> evalParentIntSyn(string arg1, string arg2);
	void hasChildOfArg1(string arg1);
	vector<string> hasParentOfArg2(string arg2);
	tuple<vector<string>, vector<string>> evalParentSynSyn(string arg1, string arg2, string arg1Type,
				string arg2Type);
	vector<tuple<string,string>> evalParStmt(vector<int> arg1Candidates, vector<int> arg2Candidates);
	vector<int> setCandidateValues(vector<int> allStmt,vector<int> allWhiles, 
		vector<int> allIfs, int arg1Entity);
	vector<tuple<string, string>> intersectionTupleInt(vector<tuple<string, string>> &arg1Candidates, 
			vector<tuple<string, string>>& arg2Candidates);
	vector<string> evalParentSynWild(string arg1, string arg1Type);
	vector<string> evalParArg1Stmt(vector<int> arg1Candidates, string arg1);
	vector<string> evalParentWildSyn(string arg2, string arg2Type);
	vector<string> evalParArg2Stmt(vector<int> arg2Candidates, string arg2);
	void hasParentForArg2(string arg2);
	void hasParentStmts();
};

