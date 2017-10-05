#pragma once
#include <vector>
#include <tuple>

using namespace std;

class Next {
public:
	Next();

	void createCFGTable(vector<int> stmtsAndType, vector<int> parentOfStmtVec, vector<tuple<int, int>> procFirstAndLastLines);
	vector<int> getNext(int stmtNum);

private:

	vector<vector<int>> nextTable;
	vector<vector<int>> previousTable;
	vector<vector<int>> nextStarTable;
	vector<vector<int>> previousStarTable;
};
