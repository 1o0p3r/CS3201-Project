#include "Next.h"
#include <vector>
#include <tuple>

using namespace std;

Next::Next() { 

	vector<vector<int>> nextTable;
	vector<vector<int>> previousTable;
	vector<vector<int>> nextStarTable;
	vector<vector<int>> previousStarTable;

	vector<vector<int>> procLastLine;
}

void Next::createCFGTable(vector<int> stmtsAndType, vector<int> parentOfStmtVec, vector<tuple<int, int>> procFirstAndLastLines) {

	int firstLine;
	int lastLine;
	int numOfProc = 0;
	int procCompleteCounter = 0;

	procLastLine.resize(stmtsAndType.size());

	do {

		for (int i = 1; i < stmtsAndType.size(); i--) {
			if (stmtsAndType[i] == 4) {
				numOfProc++;
				for (int j = stmtsAndType[get<0>(procFirstAndLastLines[i])]; j < stmtsAndType[get<1>(procFirstAndLastLines[i]) + 1]; j++) {
					if (stmtsAndType[j] == 4)
						continue;
					else {
						createCFG(stmtsAndType, parentOfStmtVec, procFirstAndLastLines, get<0>(procFirstAndLastLines[i]), get<1>(procFirstAndLastLines[i]));
						procCompleteCounter++;
					}
				}
			}
		}

	} while (numOfProc != procCompleteCounter);

	nextTable.clear();
	previousTable.clear();
	nextStarTable.clear();
	previousStarTable.clear();

	createCFG(stmtsAndType, parentOfStmtVec, 1, stmtsAndType.size() - 1);

}

void Next::createCFG(vector<int> stmtsAndType, vector<int> parentOfStmtVec, vector<tuple<int, int>> procFirstAndLastLines, int firstLine, int lastLine) {

	vector<int> nestingLvlParent;
	vector<int> lastIfLine;

	nextTable.resize(stmtsAndType.size() + 1);
	previousTable.resize(stmtsAndType.size() + 1);
	nestingLvlParent.resize(stmtsAndType.size() + 1);
	lastIfLine.resize(stmtsAndType.size() + 1);

	nestingLvlParent[0] = 0;
	nestingLvlParent[stmtsAndType.size()] = 0;
	nextTable[stmtsAndType.size()].push_back(0);
	previousTable[0].push_back(0);
	int nestingLvl = 0;

	// 1 = while, 2 = assign, 3 = if, 4 = call, 5 = else
	for (int i = firstLine; i < lastLine + 1; i++) {

		if (parentOfStmtVec[i] != nestingLvlParent[nestingLvl]) { //check for if
			nextTable[lastIfLine[nestingLvl]].push_back(i);
			previousTable[i].push_back(lastIfLine[nestingLvl]);
			nestingLvl--;
		}

		if (stmtsAndType[i] == 1 || stmtsAndType[i] == 3) { //save parent of nesting level
			nestingLvl++;
			nestingLvlParent[nestingLvl] = i;
		}

		if (parentOfStmtVec[i + 1] != nestingLvlParent[nestingLvl] && stmtsAndType[nestingLvlParent[nestingLvl]] == 1) { //check for while
			nextTable[i].push_back(parentOfStmtVec[i]);
			previousTable[parentOfStmtVec[i]].push_back(i);

			if (stmtsAndType[i + 1] == 5) {
				nestingLvl--;
				lastIfLine[nestingLvl] = parentOfStmtVec[i];
				continue;
			}
			else {
				nextTable[parentOfStmtVec[i]].push_back(i + 1);
				previousTable[i + 1].push_back(parentOfStmtVec[i]);
				nestingLvl--;
				continue;
			}
		}

		if (stmtsAndType[i + 1] == 5) {
			lastIfLine[nestingLvl] = i;
			continue;
		}

		if (stmtsAndType[i] == 5) {
			nextTable[nestingLvlParent[nestingLvl]].push_back(i);
			previousTable[i].push_back(nestingLvlParent[nestingLvl]);
			continue;
		}

		nextTable[i].push_back(i + 1);
		previousTable[i + 1].push_back(i);
	}

	procLastLine[firstLine] = previousTable[lastLine + 1];
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