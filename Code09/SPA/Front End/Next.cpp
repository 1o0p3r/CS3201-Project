#include "Next.h"
#include "PKB.h"
#include <vector>

using namespace std;

Next::Next() {

	vector<vector<int>> nextTable;
	vector<vector<int>> previousTable;
	vector<vector<int>> nextStarTable;
	vector<vector<int>> previousStarTable;
	vector<int> nestingLvl; //delete
}

void Next::createCFGTable(vector<int> stmtsAndType, PKB pkb) {

	nextTable.resize(stmtsAndType.size());

	vector<int> saveWhile;
	vector<int> saveIf;
	vector<int> saveElse;
	int nestingLevel = 0;

	// 1 = while, 2 = assign, 3 = if, 4 = call, 5 = else
	for (int i = 1; i < stmtsAndType.size(); i++) {

		vector<int> parentOfStmtVec = pkb.getParent(i);
		nestingLvl.push_back(parentOfStmtVec.size()); //to test for parent, delete
		if (parentOfStmtVec.size() != 0) {
			int parentOfStmt = parentOfStmtVec[0];
			if (parentOfStmt != (saveWhile[nestingLevel])) {
				nextTable[i - 1].clear();
				nextTable[i - 1].push_back(saveWhile[nestingLevel]);
				nextTable[saveWhile[nestingLevel]].push_back(i);
				nestingLevel--;
			}
		}

		switch (stmtsAndType[i]) {

		case 1:
			nestingLevel++;
			saveWhile.resize(nestingLevel + 1);
			saveWhile[nestingLevel] = i;
			nextTable[i].push_back(i + 1); //need to check if progLine exists
			break;

		case 2:
			nextTable[i].push_back(i + 1); //need to check if progLine exists
			break;

		case 3:

			break;

		case 4:

			break;

		case 5:

			break;

		default:
			break;
		}
	}
}

vector<int> Next::getNext(int stmtNum) {
	return nextTable[stmtNum];
}