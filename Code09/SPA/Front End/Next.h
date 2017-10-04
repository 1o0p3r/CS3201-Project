#pragma once
#include <vector>

using namespace std;

class Next {
public:
	Next();

	void createCFGTable(vector<int> stmtsAndType);
	vector<int> getNext(int stmtNum);

private:

	vector<vector<int>> nextTable;
	vector<vector<int>> previousTable;
	vector<vector<int>> nextStarTable;
	vector<vector<int>> previousStarTable;

};
