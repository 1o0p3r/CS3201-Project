#pragma once
#include <vector>
#include <tuple>

using namespace std;

class Next {
public:
	Next();

	void createCFGTable(vector<int> stmtsAndType, vector<int> parentOfStmtVec, vector<tuple<int, int>> procFirstAndLastLines);
	vector<int> getNext(int stmtNum);
	vector<int> getPrevious(int stmtNum);
	vector<int> Next::getAllNext();

private:

	vector<vector<int>> nextTable;
	vector<vector<int>> previousTable;
	vector<vector<int>> nextStarTable;
	vector<vector<int>> previousStarTable;

	vector<vector<int>> procLastLine;

	void createCFG(vector<int> stmtsAndType, vector<int> parentOfStmtVec, int firstLine, int lastLine);
	};
