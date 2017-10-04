#pragma once
#include "PKB.h"
#include "QueryStatement.h"
#include "QueryElement.h"
#include "suchThatAnalyzer.h"
#include "FollowsAnalyzer.h"
#include "ParentStarAnalyzer.h"
#include "FollowsStarAnalyzer.h"
#include "ModifiesAnalyzer.h"
#include "TupleHash.h"

#include <iterator>
#include <algorithm>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class QueryAnalyzer
{
public:
	QueryAnalyzer();

	void setPKB(PKB pkb);
	void setQS(QueryStatement qs);
	vector<string> runQueryEval();
//private:
	QueryStatement qsReadOnly;
	QueryElement selectElement;
	vector<QueryElement> stElements;
	vector<QueryElement> patternElements;
	vector<vector<vector<string>>> mergedQueryTable;
	unordered_map<string, tuple<int,int>> synTableMap;

	PKB pkbReadOnly;
	bool hasSTClause;
	bool hasPatternClause;

	void initMapPatternExpType();
	void initSelectMap();
	void initSuchThatMap();
	void initParentSynTypeMap();
	void initPatternValueMap();


	void findQueryElements();
	vector<string> analyzeClauseResults();
	vector<string> analyzeSelect(vector<string> answer, string selectEntity);
	vector<string> intersection(vector<string> v1, vector<string> v2);
	vector<string> removeVectDuplicates(vector<string> selectClause);
	vector<vector<vector<string>>> solveSTClause();
	void solvePatternClause();
	vector<vector<string>> solveAssignPattern(QueryElement patternClause);
	tuple<vector<string>, vector<string>> solvePatAssignSyn(string arg1, 
			string patExp, string patType, string patSyn);
	vector<string> validatedPatAssignSyn(string arg1, string patExp,
			string patType, string patSyn);
	vector<vector<vector<string>>> insertSTResult(vector<vector<string>> stResult);
	
	void insertArg1Arg2CommonSynTable(vector<vector<string>> stResult);
	void restrictTableValues(vector<vector<string>> tableToMerge1, int joinColArg1, int joinColArg2,
			int tableLoc, vector<vector<string>> stResult);
	void addSingleCommonSynTable(vector<vector<string>> stResult, string arg, int clauseJoinIndex);
	void insertNoCommonSynToTable(vector<vector<string>> stResult, 
			int tableIndex, bool hasArg2);
	
	vector<vector<string>> solveUses(QueryElement suchThatClause);
	void solveUsesProc(QueryElement suchThatClause);
	vector<vector<string>> solveUsesStmt(QueryElement suchThatClause);
	bool validateUses(string arg1, string arg2, int scenario);
	vector<vector<string>> toAddUsesSynVect(string arg1, string arg2, int scenario);

	vector<vector<string>> solveParent(QueryElement suchThatClause);
	bool isParent(string arg1, string arg2);
	vector<string> evalParentIntSyn(string arg1, string arg2);
	bool hasChildOfArg1(string arg1);
	vector<string> hasParentOfArg2(string arg1, string arg2);
	tuple<vector<string>, vector<string>> evalParentSynSyn(string arg1, string arg2, string arg1Type, 
			string arg2Type, string arg1EntityType, string arg2EntityType);
	vector<tuple<string,string>> evalParStmt(vector<int> arg1Candidates, vector<int> arg2Candidates);
	vector<int> setCandidateValues(vector<int> allStmt, vector<int> allWhile, vector<int> allIf,
			int arg1Entity, vector<int> allAssign);
	vector<tuple<string, string>> intersectionTupleInt(vector<tuple<string, string>> &arg1Candidates, 
			vector<tuple<string, string>>& arg2Candidates);
	vector<string> evalParentSynWild(string arg1, string arg1Type);
	vector<string> evalParArg1Stmt(vector<int> arg1Candidates, string arg1);
	vector<string> evalParentWildSyn(string arg2, string arg2Type);
	vector<string> evalParArg2Stmt(vector<int> arg2Candidates, string arg2);
	bool hasParentForArg2(string arg2);
	bool hasParentStmts();
	vector<vector<string>> hashJoin(vector<vector<string>> queryAnalyzerTable, int qaJoinIndex, 
			vector<vector<string>> clauseTable, int clausetableJoinIndex, int qaTableLoc1, int qaTableLoc2);
	
	vector<vector<vector<string>>> getMergedQueryTable();
};

