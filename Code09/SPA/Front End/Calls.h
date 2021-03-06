#pragma once
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

class Calls {
public:
	Calls();

	void setCalls(int statementNum, int procName1, int procName2);
	vector<int> getCalls(int procName);
	vector<int> getCalledBy(int procName);
	vector<int> getCallsStar(int procName);
	vector<int> getCalledByStar(int procName);
	vector<int> getProcCalledByStmt(int procName);
	int getStmtCallProc(int statementNum);
	vector<int> getAllCalls();

private:

	vector<vector<int>> callsTable;
	vector<vector<int>> calledByTable;
	vector<vector<int>> callsStarTable;
	vector<vector<int>> calledByStarTable;
	vector<vector<int>> procCalledByStmtTable;
	vector<int> stmtCallProcTable;
	set<int> allCallsTable;

	bool isRecursive;

	void setCallsStar(int procName1, int procName2);
	void setCalledBy(int procName1, int procName2);
	void setCalledByStar(int procName1, int procName2);
	void setProcCalledByStmt(int procName2, int statementNum);
	void setStmtCallProc(int statementNum, int procName2);

	void checkIfRecursive();
	
};
