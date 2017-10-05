#include "Next.h"
#include <vector>
#include <tuple>

using namespace std;

Next::Next() {

	vector<vector<int>> nextTable;
	vector<vector<int>> previousTable;
	vector<vector<int>> nextStarTable;
	vector<vector<int>> previousStarTable;
}

void Next::createCFGTable(vector<int> stmtsAndType, vector<int> parentOfStmtVec, vector<tuple<int, int>> procFirstAndLastLines) {

	nextTable.resize(stmtsAndType.size());

	vector<int> nestingLvlParent;
	vector<int> lastIf;
	int nestingLvl = 0;

	nestingLvlParent.resize(stmtsAndType.size());
	lastIf.resize(stmtsAndType.size());

	// 1 = while, 2 = assign, 3 = if, 4 = call, 5 = else
	for (int i = 1; i < stmtsAndType.size(); i++) {

		int parentOfNextStmt = parentOfStmtVec[i + 1];
		if (parentOfNextStmt != nestingLvlParent[nestingLvl]) { //while loop
			if (stmtsAndType[parentOfNextStmt] == 1) {
				nextTable[i].push_back(parentOfNextStmt);
				nextTable[parentOfNextStmt].push_back(i + 1); 
			}

			else { //if clause
				nextTable[i].push_back(i + 1);
				nextTable[lastIf[nestingLvl]].push_back(i + 1); 
				lastIf[nestingLvl] = i + 1;
			}
			nestingLvl--;
		}

		if (stmtsAndType[i] == 1 || stmtsAndType[i] == 3) {
			nestingLvl++;
			nestingLvlParent[nestingLvl] = i;
		}

		if (stmtsAndType[i + 1] == 4) {

		}

		else if (stmtsAndType[i + 1] == 5) {
			nextTable[parentOfStmtVec[i]].push_back(i + 1);
		}

		else {
			nextTable[i].push_back(i + 1);
		}
	}
}

vector<int> Next::getNext(int stmtNum) {
	return nextTable[stmtNum];
}

vector<int> Next::getPrevious(int stmtNum) {
	return vector<int>{};
}

vector<int> Next::getAllNext() {
	return vector<int>{};
}