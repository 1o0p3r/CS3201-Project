#pragma once
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

class Calls {
public:
	Calls();

	void setCalls(int procName1, int procName2);
	vector<int> getCalls(int procName);
	vector<int> getCalledBy(int procName);
	vector<int> getCallsStar(int procName);
	vector<int> getCalledByStar(int procName);
	set<int> getAllCalls();

private:

	vector<vector<int>> callsTable;
	vector<vector<int>> calledByTable;
	vector<vector<int>> callsStarTable;
	vector<vector<int>> calledByStarTable;
	set<int> allCallsTable;

	bool isRecursive;

	void setCallsStar(int procName1, int procName2);
	void setCalledBy(int procName1, int procName2);
	void setCalledByStar(int procName1, int procName2);

	void checkIfRecursive();
	
};
