#include "WithAnalyzer.h"
#include "Abstract_QA_API.h"
#include "TupleHash.h"

enum
{
	undefined, stmtNum, value, varName, procName, integer, stringLiteral
};

void WithAnalyzer::initArgSynTypeMap()
{
	argSynTypeMap["stmt#"] = stmtNum;
	argSynTypeMap["value"] = value;
	argSynTypeMap["number"] = integer;
	argSynTypeMap["procName"] = procName;
	argSynTypeMap["varName"] = varName;
	argSynTypeMap["stringLiteral"] = stringLiteral;
}

WithAnalyzer::WithAnalyzer(QueryElement withClause, PKB &pkb)
{
	pkbPtr = pkb;
	arg1 = withClause.getWithArg1Syn();
	arg1Type = withClause.getWithArg1Type();
	arg1Ent = withClause.getWithArg1Ent();
	arg2 = withClause.getWithArg2Syn();
	arg2Type = withClause.getWithArg2Type();
	arg2Ent = withClause.getWithArg2Ent();
	initArgSynTypeMap();
}

tuple<bool, vector<vector<string>>> WithAnalyzer::analyze()
{	
	bool containsWith = false;
	vector<vector<string>> clauseResult;
	vector<string> candidateList{ arg1Type,arg2Type };
	vector<string> candidateListEnt{ arg1Ent,arg2Ent };
	vector<string> candidateListSyn{ arg1,arg2 };
	for (int i = 0; i < candidateList.size(); i++) {
		vector<string> refResults;
		switch (argSynTypeMap[candidateList[i]]) {
			case stmtNum:
				refResults = getStmtResults(candidateListEnt[i]);
				break;
			case value:
				refResults = pkbPtr.getAllConstants();
				break;
			case varName:
				refResults = pkbPtr.getAllVariables();
				break;
			case procName:
				refResults = getAllProcName(candidateListEnt[i]);
				break;
			case integer: case stringLiteral:
				refResults.push_back(candidateListSyn[i]);
				break; 
		}
		clauseResult.push_back(refResults);
		
	}
	clauseResult[0] = intersectionT(clauseResult[0], clauseResult[1]);
	clauseResult[1] = clauseResult[0];
	if (clauseResult[0].size() == 0)
		return make_tuple(containsWith, vector<vector<string>>());
	else
		addSynToVec(clauseResult,candidateList, candidateListSyn,containsWith);
	return make_tuple(containsWith, clauseResult);
}

vector<string> WithAnalyzer::getStmtResults(string ent)
{
	vector<int> pkbResult;
	vector<string> pkbStringResult;
	if (ent == "stmt#" || ent == "prog_line" || ent == "stmt")  pkbResult = pkbPtr.getAllStmt();
	else if (ent == "assign") pkbResult = pkbPtr.getAssign();
	else if (ent == "while") pkbResult = pkbPtr.getWhile();
	else if (ent == "if") pkbResult = pkbPtr.getIf();
	else if (ent == "call") pkbResult = pkbPtr.getCall();
	else if (ent == "constant") pkbStringResult = pkbPtr.getAllConstants();
	if(!pkbResult.empty())
	for (auto const &entry : pkbResult)
		pkbStringResult.push_back(to_string(entry));

	return pkbStringResult;
}

vector<string> WithAnalyzer::getAllProcName(const string ent)
{
	if (ent == "procedure") return pkbPtr.getAllProcedures();
	return pkbPtr.getAllCalls();
}

void WithAnalyzer::addSynToVec(vector<vector<string>> &clauseResult, const vector<string> &candidateList,
	const vector<string> &candidateListSyn, bool &containsWith)
{
	
	vector<int> toKeep;
	string ref1, ref2;
	vector<vector<string>> result;
	for(int i=0; i<clauseResult.size();i++)
	{
		if (!clauseResult[i].empty()) {
			if (argSynTypeMap[candidateList[i]] != stringLiteral && argSynTypeMap[candidateList[i]] != integer) {
				clauseResult[i].push_back(candidateListSyn[i]);
				toKeep.push_back(i);
			}
			else {
				if (i == 0)
					ref1 = candidateListSyn[i];
				else
					ref2 = candidateListSyn[i];
			}
			containsWith = true;
		}
	}
	
	if (!ref1.empty() && !ref2.empty()) {
		if (ref1 == ref2)
			containsWith = true;
	}
	
	for (const auto &keepIndex : toKeep)
		result.push_back(clauseResult[keepIndex]);
	clauseResult = result;
}

