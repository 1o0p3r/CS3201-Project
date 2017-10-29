#include "Next.h"
#include <vector>
#include <tuple>
#include <set>

using namespace std;

Next::Next() {

	vector<set<int>> nextTable;
	vector<set<int>> previousTable;
	set<int> allNextTable;
}

/*
void Next::createCFGTable(vector<int> stmtsAndType, vector<int> parentOfStmtVec, int firstLine, int lastLine) {

	vector<int> nestingLvlParent;
	vector<int> lastIfLine;

	nextTable.resize(stmtsAndType.size());
	previousTable.resize(stmtsAndType.size());

	nestingLvlParent.push_back(0);
	int nestingLvl = 0;

	// 1 = while, 2 = assign, 3 = if, 4 = call, 5 = else
	for (int i = firstLine; i < (lastLine + 1); i++) {

		if (i == (lastLine)) {

			if (nestingLvl == 0) {
				
			}

			else {

				int lastLineIf = 0;

				if (stmtsAndType[nestingLvlParent[nestingLvl]] == 3 && parentOfStmtVec[i] == nestingLvlParent[nestingLvl]) {
					lastLineIf = lastIfLine[nestingLvl];
					nestingLvl--;
				}

				while (nestingLvl != 0) {

					if (stmtsAndType[nestingLvlParent[nestingLvl]] == 1) {
						if (lastLineIf != 0) {
							nextTable[lastLineIf].insert(nestingLvlParent[nestingLvl]);
						}
						nextTable[i].insert(nestingLvlParent[nestingLvl]);
						nestingLvl--;
					}

					else if (stmtsAndType[nestingLvlParent[nestingLvl]] == 3 && parentOfStmtVec[i] != nestingLvlParent[nestingLvl]) {
						nextTable[lastIfLine[nestingLvl]].insert(i);
						allNextTable.insert(lastIfLine[nestingLvl]);
						nestingLvl--;
					}
				}
			}
			continue;
		}

		if (parentOfStmtVec[i] != nestingLvlParent[nestingLvl]) { //check for if
			nextTable[lastIfLine[nestingLvl]].insert(i);
			allNextTable.insert(lastIfLine[nestingLvl]);
			previousTable[i].insert(lastIfLine[nestingLvl]);
			nestingLvl--;
		}

		if (stmtsAndType[i] == 1 || stmtsAndType[i] == 3) { //save parent of nesting level

			if (nestingLvlParent.size() <= i)
				nestingLvlParent.resize(i + 1);

			nestingLvl++;
			nestingLvlParent[nestingLvl] = i;
		}

		if (parentOfStmtVec[i + 1] != nestingLvlParent[nestingLvl] && stmtsAndType[nestingLvlParent[nestingLvl]] == 1) { //check for while
			nextTable[i].insert(parentOfStmtVec[i]);
			allNextTable.insert(i);
			previousTable[parentOfStmtVec[i]].insert(i);

			if (stmtsAndType[i + 1] == 5) {

				if (lastIfLine.size() <= nestingLvl)
					lastIfLine.resize(nestingLvl + 1);

				nestingLvl--;
				lastIfLine[nestingLvl] = parentOfStmtVec[i];
				continue;
			}
			else {
				nextTable[parentOfStmtVec[i]].insert(i + 1);
				allNextTable.insert(parentOfStmtVec[i]);
				previousTable[i + 1].insert(parentOfStmtVec[i]);
				nestingLvl--;
				continue;
			}
		}

		if (stmtsAndType[i + 1] == 5) {

			if (lastIfLine.size() <= i)
				lastIfLine.resize(i + 1);

			lastIfLine[nestingLvl] = i;
			continue;
		}

		if (stmtsAndType[i] == 5) {
			nextTable[nestingLvlParent[nestingLvl]].insert(i);
			allNextTable.insert(nestingLvlParent[nestingLvl]);
			previousTable[i].insert(nestingLvlParent[nestingLvl]);
			continue;
		}

		nextTable[i].insert(i + 1);
		allNextTable.insert(i);
		previousTable[i + 1].insert(i);
	}
}
*/

void Next::setNext(int stmtNum1, int stmtNum2) {
	if (nextTable.size() <= stmtNum1) {
		nextTable.resize(stmtNum1 + 1);
	}
	nextTable[stmtNum1].insert(stmtNum2);
	setPrevious(stmtNum2, stmtNum1);
}

void Next::setPrevious(int stmtNum1, int stmtNum2) {
	if (previousTable.size() <= stmtNum1) {
		previousTable.resize(stmtNum1 + 1);
	}
	previousTable[stmtNum1].insert(stmtNum2);
}

vector<int> Next::getNext(int stmtNum) {
	vector<int> nextTableV;
	if (stmtNum >= nextTable.size()) {
		nextTableV = {};
	} else {
		nextTableV.insert(nextTableV.end(), nextTable[stmtNum].begin(), nextTable[stmtNum].end());
	}
	return nextTableV;
}

vector<int> Next::getPrevious(int stmtNum) {
	vector<int> previousTableV;
	if (stmtNum >= previousTable.size()) {
		previousTableV = {};
	} else {
		previousTableV.insert(previousTableV.end(), previousTable[stmtNum].begin(), previousTable[stmtNum].end());
	}
	return previousTableV;
}

vector<int> Next::getAllNext() {
	vector<int> allNextTableV;
	allNextTableV.insert(allNextTableV.end(), allNextTable.begin(), allNextTable.end());
	return allNextTableV;
}