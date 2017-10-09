#include "Next.h"
#include <vector>
#include <tuple>
#include <set>

using namespace std;

Next::Next() { 

<<<<<<< HEAD
	vector<vector<int>> nextTable;
	vector<vector<int>> previousTable;
	vector<vector<int>> nextStarTable;
	vector<vector<int>> previousStarTable;

	vector<vector<int>> procLastLine;
	bool procChecksComplete = false;
=======
	vector<set<int>> nextTable;
	vector<set<int>> previousTable;
	vector<set<int>> nextStarTable;
	vector<set<int>> previousStarTable;
	set<int> allNextTable;
	
	vector<set<int>> procEndLine;
	bool procChecksAreComplete = false;
>>>>>>> PKB2
}

void Next::createCFGTable(vector<int> stmtsAndType, vector<int> parentOfStmtVec, vector<tuple<int, int>> procFirstAndLastLines) {

<<<<<<< HEAD
	int firstLine;
	int lastLine;
	int numOfProc = 0;
	int procCompleteCounter = 0;

	procLastLine.resize(stmtsAndType.size());

	do {

		for (int i = 1; i < stmtsAndType.size(); i++) {
			if (stmtsAndType[i] == 4) {
				numOfProc++;
				for (int j = get<0>(procFirstAndLastLines[i]); j < get<1>(procFirstAndLastLines[i]) + 1; j++) {
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

	procChecksComplete = true;
	createCFG(stmtsAndType, parentOfStmtVec, procFirstAndLastLines, 1, stmtsAndType.size() - 1);

}

void Next::createCFG(vector<int> stmtsAndType, vector<int> parentOfStmtVec, vector<tuple<int, int>> procFirstAndLastLines, int firstLine, int lastLine) {
=======
	int procFirstLine;
	int procLastLine;
	procEndLine.resize(stmtsAndType.size());

	for (int i = 1; i < stmtsAndType.size(); i++) {
		
		if (stmtsAndType[i] == 4) {

			procFirstLine = get<0>(procFirstAndLastLines[i]);
			procLastLine = get<1>(procFirstAndLastLines[i]);
	
			createCFGTable(stmtsAndType, parentOfStmtVec, procFirstLine, procLastLine);
			//if (stmtsAndType[procLastLine] == 4) {
			//	createCFGTable(stmtsAndType, parentOfStmtVec, get<0>(procFirstAndLastLines[procLastLine]), get<1>(procFirstAndLastLines[procLastLine]));
			//	procEndLine[i] = previousTable[0];
			//}

			//else 
				procEndLine[i] = previousTable[0];
		}
		nextTable.clear();
		previousTable.clear();
	}
	procChecksAreComplete = true;
	createCFGTable(stmtsAndType, parentOfStmtVec, 1, stmtsAndType.size() - 1);
}

void Next::createCFGTable(vector<int> stmtsAndType, vector<int> parentOfStmtVec, int firstLine, int lastLine) {
>>>>>>> PKB2

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
<<<<<<< HEAD
	for (int i = firstLine; i < lastLine + 1; i++) {

		if (parentOfStmtVec[i] != nestingLvlParent[nestingLvl]) { //check for if
			nextTable[lastIfLine[nestingLvl]].push_back(i);
			if (stmtsAndType[lastIfLine[nestingLvl]] != 4)
				previousTable[i].push_back(lastIfLine[nestingLvl]);
=======
	for (int i = firstLine; i < (lastLine + 1); i++) {

		if (parentOfStmtVec[i] != nestingLvlParent[nestingLvl]) { //check for if
			nextTable[lastIfLine[nestingLvl]].insert(i);
			allNextTable.insert(lastIfLine[nestingLvl]);
			previousTable[i].insert(lastIfLine[nestingLvl]);
>>>>>>> PKB2
			nestingLvl--;
		}

		if (stmtsAndType[i] == 1 || stmtsAndType[i] == 3) { //save parent of nesting level
			nestingLvl++;
			nestingLvlParent[nestingLvl] = i;
		}

		//if ((i + 1) != (lastLine + 1) && )

		if (parentOfStmtVec[i + 1] != nestingLvlParent[nestingLvl] && stmtsAndType[nestingLvlParent[nestingLvl]] == 1) { //check for while
<<<<<<< HEAD
			nextTable[i].push_back(parentOfStmtVec[i]);
			if (stmtsAndType[i] != 4)
				previousTable[parentOfStmtVec[i]].push_back(i);
=======
				nextTable[i].insert(parentOfStmtVec[i]);
				allNextTable.insert(i);
				previousTable[parentOfStmtVec[i]].insert(i);
>>>>>>> PKB2

			if (stmtsAndType[i + 1] == 5) {
				nestingLvl--;
				lastIfLine[nestingLvl] = parentOfStmtVec[i];
				continue;
			}
			else {
<<<<<<< HEAD
				nextTable[parentOfStmtVec[i]].push_back(i + 1);
				previousTable[i + 1].push_back(parentOfStmtVec[i]);
				nestingLvl--;
				continue;
=======
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
>>>>>>> PKB2
			}
		}

		if (stmtsAndType[i + 1] == 5) {
			lastIfLine[nestingLvl] = i;
			continue;
		}

		if (stmtsAndType[i] == 5) {
<<<<<<< HEAD
			nextTable[nestingLvlParent[nestingLvl]].push_back(i);
			if (stmtsAndType[nestingLvlParent[nestingLvl]] != 4)
				previousTable[i].push_back(nestingLvlParent[nestingLvl]);
			continue;
		}

		nextTable[i].push_back(i + 1);
		if (stmtsAndType[i] != 4)
			previousTable[i + 1].push_back(i);
	}

	procLastLine[firstLine] = previousTable[lastLine + 1];

	if (procChecksComplete) {

		for (int i = 1; i < stmtsAndType.size(); i++) {
			if (stmtsAndType[i] == 4) {
				vector<int> next = nextTable[i];
				nextTable[i].clear();
				nextTable[i].push_back(get<0>(procFirstAndLastLines[i]));
				for (int j = 0; j < next.size(); j++) {
					nextTable[get<1>(procFirstAndLastLines[i])].push_back(next[j]);
					previousTable[next[j]].push_back(get<1>(procFirstAndLastLines[i]));
				}
			}
=======
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
>>>>>>> PKB2
		}
	}
}

vector<int> Next::getNext(int stmtNum) {
	vector<int> nextTableV;
	nextTableV.insert(nextTableV.end(), nextTable[stmtNum].begin(), nextTable[stmtNum].end());
	return nextTableV;
}

vector<int> Next::getPrevious(int stmtNum) {
<<<<<<< HEAD
	return previousTable[stmtNum];
=======
	vector<int> previousTableV;
	previousTableV.insert(previousTableV.end(), previousTable[stmtNum].begin(), previousTable[stmtNum].end());
	return previousTableV;
>>>>>>> PKB2
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