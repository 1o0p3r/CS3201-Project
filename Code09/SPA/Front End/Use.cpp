#include "Use.h"

Use::Use() {

	vector<set<int>> usesTable;
	vector<set<int>> usedByTable;
	vector<set<int>> procUsesTable;
	vector<set<int>> procUsedByTable;
	int useCount = 0;
	int procUseCount = 0;
}

void Use::setUses(int statementNum, int varName, vector<int> parentStarOfStmt) {

	if (statementNum <= 0) {

		throw new invalid_argument("Statement Number must be more than 0.");
	}

	setUses(statementNum, varName);
	setUsedBy(varName, statementNum);

	if (parentStarOfStmt.size() != 0)
		for (int i = 0; i < parentStarOfStmt.size(); i++) {
			setUses(parentStarOfStmt[i], varName);
		}
}

void Use::setUses(int statementNum, int varName) {

	if (statementNum <= 0) {

		throw new invalid_argument("Statement Number must be more than 0.");
	}

	if (statementNum >= usesTable.size())
		usesTable.resize(statementNum + 1);
	useCount++;
	usesTable[statementNum].insert(varName);
	setUsedBy(varName, statementNum);
}

void Use::setUsedBy(int varName, int statementNum) {

	if (varName >= usedByTable.size()) {

		usedByTable.resize(varName + 1);
		usedByTable[varName].insert(statementNum);
	}

	else {

		usedByTable[varName].insert(statementNum);
	}
}

void Use::setProcUses(int procName, int varName) {

	if ((procName) >= procUsesTable.size()) {

		procUsesTable.resize(procName + 1);
		procUsesTable[procName].insert(varName);
	}
	else {

		procUsesTable[procName].insert(varName);
	}
	procUseCount++;
	setProcUsedBy(varName, procName);
}

void Use::setProcUsedBy(int varName, int procName) {

	if (varName >= procUsedByTable.size()) {
		procUsedByTable.resize(varName + 1);
		procUsedByTable[varName].insert(procName);
	}
	else {
		procUsedByTable[varName].insert(procName);
	}
}

vector<int> Use::getUses(int statementNum) {
	if (usesTable.size() > statementNum) {
		vector<vector<int>> result(1);
		result[0].insert(result[0].end(), usesTable[statementNum].begin(), usesTable[statementNum].end());
		return result[0];
	} else {
		return vector<int>();
	}
}

vector<int> Use::getUsedBy(int varName) {
	if (usedByTable.size() > varName) {
		vector<vector<int>> result(1);
		result[0].insert(result[0].end(), usedByTable[varName].begin(), usedByTable[varName].end());
		return result[0];
	} else {
		return vector<int>();
	}
}

vector<int> Use::getProcUses(int procName) {
	if (procUsesTable.size() > procName) {
		vector<vector<int>> result(1);
		result[0].insert(result[0].end(), procUsesTable[procName].begin(), procUsesTable[procName].end());
		return result[0];
	} else {
		return vector<int>();
	}
}

vector<int> Use::getProcUsedBy(int varName) {
	if (procUsedByTable.size() > varName) {
		vector<vector<int>> result(1);
		result[0].insert(result[0].end(), procUsedByTable[varName].begin(), procUsedByTable[varName].end());
		return result[0];
	} else {
		return vector<int>();
	}
}

int Use::getUseCount() {
	return useCount;
}

int Use::getProcUseCount() {
	return procUseCount;
}