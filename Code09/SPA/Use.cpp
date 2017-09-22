#include "Use.h"
#include <string>
#include <set>

Use::Use() {

	vector<set<int>> usesTable; //vector of string, at stmt can be a container stmt
	vector<set<int>> usedByTable;
	vector<set<int>> procUsesTable;
	vector<set<int>> procUsedByTable;
}

void Use::setUses(int s, int varName, vector<int> parentStarOfStmt) {

	if (s <= 0) {

		throw new invalid_argument("Statement Number must be more than 0.");
	}

	setUses(s, varName);
	setUsedBy(varName, s);

	if (parentStarOfStmt.size() != 0)
		for (int i = 0; i < parentStarOfStmt.size(); i++) {
			setUses(parentStarOfStmt[i], varName);
		}
}

void Use::setUses(int s, int varName) {

	if (s <= 0) {

		throw new invalid_argument("Statement Number must be more than 0.");
	}

	if (s >= usesTable.size())
		usesTable.resize(s + 1);

	usesTable[s].insert(varName);
	setUsedBy(varName, s);
}

void Use::setUsedBy(int varName, int s) {

	if (varName >= usedByTable.size()) {

		usedByTable.resize(varName + 1);
		usedByTable[varName].insert(s);
	}

	else {

		usedByTable[varName].insert(s);
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

vector<int> Use::getUses(int s) {
	if (usesTable.size() > s) {
		vector<vector<int>> result(1);
		result[0].insert(result[0].end(), usesTable[s].begin(), usesTable[s].end());
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