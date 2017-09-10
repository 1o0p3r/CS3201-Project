#include "Modify.h"
#include <string>

Modify::Modify() {

	vector<string> varIndexTable;
	vector<string> procIndexTable;

	vector<vector<string>> modifiesTable; //vector of string, at stmt can be a container stmt
	vector<vector<int>> modifiedByTable;
	vector<vector<string>> procModifiesTable;
	vector<vector<string>> procModifiedByTable;
}

int Modify::varIndexer(string varName) { //creates var index, or creates one if it doesn't exist

	vector<string>::iterator varIndex = find(varIndexTable.begin(), varIndexTable.end(), varName);

	if (varIndex != varIndexTable.end()) {

		return distance(varIndexTable.begin(), varIndex);
	}
	else {

		varIndexTable.resize(varIndexTable.size() + 1);
		varIndexTable[varIndexTable.size()] = varName;
		return varIndexTable.size() - 1;
	}
}

int Modify::procIndexer(string procName) { //creates proc index, or creates one if it doesn't exist

	vector<string>::iterator procIndex = find(procIndexTable.begin(), procIndexTable.end(), procName);

	if (procIndex != procIndexTable.end()) {

		return distance(procIndexTable.begin(), procIndex);
	}
	else {

		procIndexTable.resize(procIndexTable.size() + 1);
		procIndexTable[procIndexTable.size()] = procName;
		return procIndexTable.size() - 1;
	}
}

void Modify::setModifies(int s, string varName) {

	if (s < modifiesTable.size()) { //modifiesTable increases with statement, stmtNo larger than table means incorrect program reading

		throw new invalid_argument("Statement is already in PKB");
	}

	modifiesTable.resize(s + 1);
	modifiesTable[s].push_back(varName);
	setModifiedBy(varName, s);
}

void Modify::setModifiedBy(string varName, int s) {

	int indexedVarName = varIndexer(varName);

	if (find(modifiedByTable[indexedVarName].begin(), modifiedByTable[indexedVarName].end(), s) != modifiedByTable[indexedVarName].end()) {

		throw invalid_argument("Statement is already in PKB");
	}

	else {

		modifiedByTable[indexedVarName].push_back(s);
	}
}

void Modify::setProcModifies(string procName, string varName) {

	int indexedProcName = procIndexer(procName);

	if (find(procModifiesTable[indexedProcName].begin(), procModifiesTable[indexedProcName].end(), varName) != procModifiesTable[indexedProcName].end()) {
		return; //does nothing as variable is accounted for
	}
	else {
		procModifiesTable[indexedProcName].push_back(varName);
	}
}

void Modify::setProcModifiedBy(string procName, string varName) {

	int indexedVarName = varIndexer(varName);

	if (find(procModifiedByTable[indexedVarName].begin(), procModifiedByTable[indexedVarName].end(), procName) != procModifiedByTable[indexedVarName].end()) {
		return; //does nothing as var iable is accounted for
	}
	else {
		procModifiedByTable[indexedVarName].push_back(procName);
	}
}

vector<string> Modify::getModifies(int s) {

	vector<vector<string>> result(1);
	result[0] = modifiesTable[s];
	return result[0];
}

vector<int> Modify::getModifiedBy(string varName) {

	vector<vector<int>> result(1);
	result[0] = modifiedByTable[varIndexer(varName)];
	return result[0];
}

vector<string> Modify::getProcModifies(string procName) {

	vector<vector<string>> result(1);
	result[0] = procModifiesTable[procIndexer(procName)];
	return result[0];
}

vector<string> Modify::getProcModifiedBy(string procName) {

	vector<vector<string>> result(1);
	result[0] = procModifiedByTable[procIndexer(procName)];
	return result[0];
}