#include "Next.h"
#include <vector>
#include <tuple>
#include <set>

using namespace std;

Next::Next() { 

	vector<vector<int>> nextTable;
	vector<vector<int>> previousTable;
	vector<vector<int>> nextStarTable;
	vector<vector<int>> previousStarTable;
	set<int> allNextTable;
}

void Next::createCFGTable(vector<int> stmtsAndType, vector<int> parentOfStmtVec, vector<tuple<int, int>> procFirstAndLastLines) {

	vector<int> nestingLvlParent;
	vector<int> lastIfLine;

	nextTable.resize(stmtsAndType.size());
	previousTable.resize(stmtsAndType.size());
	nestingLvlParent.resize(stmtsAndType.size());
	lastIfLine.resize(stmtsAndType.size());

	nestingLvlParent[0] = 0;
	int nestingLvl = 0;

	// 1 = while, 2 = assign, 3 = if, 4 = call, 5 = else
	for (int i = 1; i < stmtsAndType.size(); i++) {

		if (parentOfStmtVec[i] != nestingLvlParent[nestingLvl]) { //check for if
			nextTable[lastIfLine[nestingLvl]].push_back(i);
			allNextTable.insert(lastIfLine[nestingLvl]);
			previousTable[i].push_back(lastIfLine[nestingLvl]);
			nestingLvl--;
		}

		if (stmtsAndType[i] == 1 || stmtsAndType[i] == 3) { //save parent of nesting level
			nestingLvl++;
			nestingLvlParent[nestingLvl] = i;
		}

		if (parentOfStmtVec[i + 1] != nestingLvlParent[nestingLvl] && stmtsAndType[nestingLvlParent[nestingLvl]] == 1) { //check for while
				nextTable[i].push_back(parentOfStmtVec[i]);
				allNextTable.insert(i);
				previousTable[parentOfStmtVec[i]].push_back(i);

			if (stmtsAndType[i + 1] == 5) {
				nestingLvl--;
				lastIfLine[nestingLvl] = parentOfStmtVec[i];
				continue;
			}
			else {
				if ((i + 1) != stmtsAndType.size()) {
					nextTable[parentOfStmtVec[i]].push_back(i + 1);
					allNextTable.insert(parentOfStmtVec[i]);
					previousTable[i + 1].push_back(parentOfStmtVec[i]);
					nestingLvl--;
					continue;
				}
				else {
					previousTable[0].push_back(parentOfStmtVec[i]);
					continue;
				}
			}
		}

		if (stmtsAndType[i + 1] == 5) {
			lastIfLine[nestingLvl] = i;
			continue;
		}

		if (stmtsAndType[i] == 5) {
			nextTable[nestingLvlParent[nestingLvl]].push_back(i);
			allNextTable.insert(nestingLvlParent[nestingLvl]);
			previousTable[i].push_back(nestingLvlParent[nestingLvl]);
			continue;
		}

		if ((i + 1) != stmtsAndType.size()) {
			nextTable[i].push_back(i + 1);
			allNextTable.insert(i);
			previousTable[i + 1].push_back(i);
		}
		else {
			previousTable[0].push_back(i);
		}
	}
}

vector<int> Next::getNext(int stmtNum) {
	return nextTable[stmtNum];
}

vector<int> Next::getPrevious(int stmtNum) {
	return previousTable[stmtNum];
}

vector<int> Next::getAllNext() {
	vector<int> allNextTableV;
	allNextTableV.insert(allNextTableV.end(), allNextTable.begin(), allNextTable.end());
	return allNextTableV;
}