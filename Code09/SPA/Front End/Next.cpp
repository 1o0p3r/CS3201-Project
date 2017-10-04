#include "Next.h"
#include "PKB.h"
#include <vector>

using namespace std;

Next::Next() {

	vector<vector<int>> nextTable;
	vector<vector<int>> previousTable;
	vector<vector<int>> nextStarTable;
	vector<vector<int>> previousStarTable;
}

void Next::createCFGTable(vector<int> stmtsAndType) {

	PKB pkb;
	nextTable.resize(stmtsAndType.size());

	vector<int> saveWhile;
	vector<int> saveIf;
	vector<int> saveElse;
	int nestingLevel = 0;

	// 1 = while, 2 = assign, 3 = if, 4 = call, 5 = else
	for (int i = 1; i < stmtsAndType.size(); i++) {

		int parentOfStmt = pkb.getParent(stmtsAndType[i])[0];
		if (parentOfStmt != (saveWhile[nestingLevel])) {
			nextTable[i - 1].clear();
			nextTable[i - 1].push_back(saveWhile[nestingLevel]);
			nextTable[saveWhile[nestingLevel]].push_back(i);
			nestingLevel--;
		}

		switch (stmtsAndType[i]) {

		case 1:
			saveWhile.resize(nestingLevel + 1);
			saveWhile[nestingLevel] = i;
			nestingLevel++;
			if (!(i + 1) >= stmtsAndType.size()) //check if progLine exists
				nextTable[i].push_back(i + 1);
			break;

		case 2:
			if (pkb.getParent(i).size() == 0)
				if (!(i + 1) >= stmtsAndType.size()) //check if progLine exists
					nextTable[i].push_back(i + 1);
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