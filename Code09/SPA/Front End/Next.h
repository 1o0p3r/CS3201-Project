#pragma once
#include <vector>
#include <tuple>
#include <set>

using namespace std;

class Next {
public:
	Next();

	void createCFGTable(vector<int> stmtsAndType, vector<int> parentOfStmtVec, vector<tuple<int, int>> procFirstAndLastLines);
	vector<int> getNext(int stmtNum);
	vector<int> getPrevious(int stmtNum);
	vector<int> Next::getAllNext();

	//for testing
	vector<int> getProcEndLine(int stmtNum);

private:

	vector<set<int>> nextTable;
	vector<set<int>> previousTable;
	vector<set<int>> nextStarTable;
	vector<set<int>> previousStarTable;
	set<int> allNextTable;

	vector<set<int>> procEndLine;
	bool procChecksAreComplete;

	void createCFGTable(vector<int> stmtsAndType, vector<int> parentOfStmtVec, int firstLine, int lastLine);
};
