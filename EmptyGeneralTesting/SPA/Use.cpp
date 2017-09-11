#include "Use.h"
#include <string>

Use::Use() {

	vector<vector<int>> usesTable; //vector of string, at stmt can be a container stmt
	vector<vector<int>> usedByTable;
	vector<vector<int>> procUsesTable;
	vector<vector<int>> procUsedByTable;
}

void Use::setUses(int s, int varName) {

	if (s <= 0) {

		throw new invalid_argument("Statement Number must be more than 0.");
	}

	usesTable.resize(s + 1);
	usesTable[s].push_back(varName);
	setUsedBy(varName, s);
}

void Use::setUsedBy(int varName, int s) {

	if ((varName + 1) > usedByTable.size()) {

		usedByTable.resize(varName + 1);
		usedByTable[varName].push_back(s);
	}

	else {

		usedByTable[varName].push_back(s);
	}
}

void Use::setProcUses(int procName, int varName) {

	if ((procName + 1) > procUsesTable.size()) {

		procUsesTable.resize(procName + 1);
		procUsesTable[procName].push_back(varName);
	}
	else {

		procUsesTable[procName].push_back(varName);
	}

	setProcUsedBy(varName, procName);
}

void Use::setProcUsedBy(int varName, int procName) {

	if ((varName + 1) > procUsedByTable.size()) {
		procUsedByTable.resize(varName + 1);
		procUsedByTable[varName].push_back(procName);
	}
	else {
		procUsedByTable[varName].push_back(procName);
	}
}

vector<int> Use::getUses(int s) {

	vector<vector<int>> result(1);
	result[0] = usesTable[s];
	return result[0];
}

vector<int> Use::getUsedBy(int varName) {

	vector<vector<int>> result(1);
	result[0] = usedByTable[varName];
	return result[0];
}

vector<int> Use::getProcUses(int procName) {

	vector<vector<int>> result(1);
	result[0] = procUsesTable[procName];
	return result[0];
}

vector<int> Use::getProcUsedBy(int varName) {

	vector<vector<int>> result(1);
	result[0] = procUsedByTable[varName];
	return result[0];
}