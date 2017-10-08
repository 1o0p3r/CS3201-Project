#include "Next.h"
#include <vector>
#include <tuple>
#include <set>

using namespace std;

Next::Next() { 

	vector<set<int>> nextTable;
	vector<set<int>> previousTable;
	vector<set<int>> nextStarTable;
	vector<set<int>> previousStarTable;
	set<int> allNextTable;
	
	vector<set<int>> procEndLine;
	bool procChecksAreComplete = false;
}

void Next::createCFGTable(vector<int> stmtsAndType, vector<int> parentOfStmtVec, vector<tuple<int, int>> procFirstAndLastLines) {

	int procFirstLine;
	int procLastLine;
	procEndLine.resize(stmtsAndType.size());

	for (int i = 1; i < stmtsAndType.size(); i++) {
		
		if (stmtsAndType[i] == 4) {

			procFirstLine = get<0>(procFirstAndLastLines[i]);
			procLastLine = get<1>(procFirstAndLastLines[i]);
	
			createCFGTable(stmtsAndType, parentOfStmtVec, procFirstLine, procLastLine);
			//int checkForCalls;
			//checkForCalls = previousTable[0][0];
			//if (stmtsAndType[next(previousTable[0].begin()] == 4) { //if last line of proc is calls

			//}
			//else {
				procEndLine[i] = previousTable[0];
			//}
		}
		nextTable.clear();
		previousTable.clear();
	}
	procChecksAreComplete = true;
	createCFGTable(stmtsAndType, parentOfStmtVec, 1, stmtsAndType.size() - 1);
}

void Next::createCFGTable(vector<int> stmtsAndType, vector<int> parentOfStmtVec, int firstLine, int lastLine) {

	vector<int> nestingLvlParent;
	vector<int> lastIfLine;

	nextTable.resize(stmtsAndType.size());
	previousTable.resize(stmtsAndType.size());
	nestingLvlParent.resize(stmtsAndType.size());
	lastIfLine.resize(stmtsAndType.size());

	nestingLvlParent[0] = 0;
	int nestingLvl = 0;

	// 1 = while, 2 = assign, 3 = if, 4 = call, 5 = else
	for (int i = firstLine; i < (lastLine + 1); i++) {

		if (parentOfStmtVec[i] != nestingLvlParent[nestingLvl]) { //check for if
			nextTable[lastIfLine[nestingLvl]].insert(i);
			allNextTable.insert(lastIfLine[nestingLvl]);
			previousTable[i].insert(lastIfLine[nestingLvl]);
			nestingLvl--;
		}

		if (stmtsAndType[i] == 1 || stmtsAndType[i] == 3) { //save parent of nesting level
			nestingLvl++;
			nestingLvlParent[nestingLvl] = i;
		}

		if (parentOfStmtVec[i + 1] != nestingLvlParent[nestingLvl] && stmtsAndType[nestingLvlParent[nestingLvl]] == 1) { //check for while
				nextTable[i].insert(parentOfStmtVec[i]);
				allNextTable.insert(i);
				previousTable[parentOfStmtVec[i]].insert(i);

			if (stmtsAndType[i + 1] == 5) {
				nestingLvl--;
				lastIfLine[nestingLvl] = parentOfStmtVec[i];
				continue;
			}
			else {
				if ((i + 1) != (lastLine + 1)) {
					nextTable[parentOfStmtVec[i]].insert(i + 1);
					allNextTable.insert(parentOfStmtVec[i]);
					previousTable[i + 1].insert(parentOfStmtVec[i]);
					nestingLvl--;
					continue;
				}
				else {
					previousTable[0].insert(parentOfStmtVec[i]);
					continue;
				}
			}
		}

		if (stmtsAndType[i + 1] == 5) {
			lastIfLine[nestingLvl] = i;
			continue;
		}

		if (stmtsAndType[i] == 5) {
			nextTable[nestingLvlParent[nestingLvl]].insert(i);
			allNextTable.insert(nestingLvlParent[nestingLvl]);
			previousTable[i].insert(nestingLvlParent[nestingLvl]);
			continue;
		}

		if ((i + 1) != (lastLine + 1)) {
			nextTable[i].insert(i + 1);
			allNextTable.insert(i);
			previousTable[i + 1].insert(i);
		}
		else {
			previousTable[0].insert(i);
		}
	}
}

vector<int> Next::getNext(int stmtNum) {
	vector<int> nextTableV;
	nextTableV.insert(nextTableV.end(), nextTable[stmtNum].begin(), nextTable[stmtNum].end());
	return nextTableV;
}

vector<int> Next::getPrevious(int stmtNum) {
	vector<int> previousTableV;
	previousTableV.insert(previousTableV.end(), previousTable[stmtNum].begin(), previousTable[stmtNum].end());
	return previousTableV;
}

vector<int> Next::getAllNext() {
	vector<int> allNextTableV;
	allNextTableV.insert(allNextTableV.end(), allNextTable.begin(), allNextTable.end());
	return allNextTableV;
}

//for testing
vector<int> Next::getProcEndLine(int stmtNum) {
	vector<int> procEndLineV;
	procEndLineV.insert(procEndLineV.end(), procEndLine[stmtNum].begin(), procEndLine[stmtNum].end());
	return procEndLineV;
}