#include "Next.h"
#include <vector>
#include <tuple>
#include <set>

using namespace std;

Next::Next() {

	vector<vector<int>> nextTable;
	vector<vector<int>> previousTable;
	vector<int> allNextTable;
	int count;
}

void Next::setNext(int stmtNum1, int stmtNum2) {
	if (nextTable.size() <= stmtNum1) {
		nextTable.resize(stmtNum1 + 1);
	}
	count++;
	nextTable[stmtNum1].push_back(stmtNum2);
	setPrevious(stmtNum2, stmtNum1);
	allNextTable.push_back(stmtNum1);
}

void Next::setPrevious(int stmtNum1, int stmtNum2) {
	if (previousTable.size() <= stmtNum1) {
		previousTable.resize(stmtNum1 + 1);
	}
	previousTable[stmtNum1].push_back(stmtNum2);
}

vector<int> Next::getNext(int stmtNum) {
	if (stmtNum >= nextTable.size()) {
		return {};
	} else {
		return nextTable[stmtNum];
	}
}

vector<int> Next::getPrevious(int stmtNum) {
	if (stmtNum >= previousTable.size()) {
		return {};
	} else {
		return previousTable[stmtNum];
	}
}

vector<int> Next::getAllNext() {
	return allNextTable;
}

int Next::getNextCount() {
	return count;
}