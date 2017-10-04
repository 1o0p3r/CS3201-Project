#pragma once
#include <vector>
#include "PKB.h"

using namespace std;

class Next {
public:
	Next();

	void createCFGTable(vector<int> stmtsAndType, PKB pkb);
	vector<int> getNext(int stmtNum);

private:

	vector<vector<int>> nextTable;
	vector<vector<int>> previousTable;
	vector<vector<int>> nextStarTable;
	vector<vector<int>> previousStarTable;
	vector<int> nestingLvl;

};
