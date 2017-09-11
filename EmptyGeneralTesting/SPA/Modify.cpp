#include "Modify.h"
#include <string>

Modify::Modify() {

	vector<vector<int>> modifiesTable; //vector of string, at stmt can be a container stmt
	vector<vector<int>> modifiedByTable;
	vector<vector<int>> procModifiesTable;
	vector<vector<int>> procModifiedByTable;
}

void Modify::setModifies(int s, int varName) {

	if (s <= 0) { 

		throw new invalid_argument("Statement Number must be more than 0.");
	}

	modifiesTable.resize(s + 1);
	modifiesTable[s].push_back(varName);
	setModifiedBy(varName, s);
}

void Modify::setModifiedBy(int varName, int s) {

	if ((varName+1) > modifiedByTable.size()) {
		
		modifiedByTable.resize(varName + 1);
		modifiedByTable[varName].push_back(s);
	}

	else {

		modifiedByTable[varName].push_back(s);
	}
}

void Modify::setProcModifies(int procName, int varName) {

	if ((procName + 1) > procModifiesTable.size()) {
		
		procModifiesTable.resize(procName + 1);
		procModifiesTable[procName].push_back(varName);
	}
	else {

		procModifiesTable[procName].push_back(varName);
	}

	setProcModifiedBy(varName, procName);
}

void Modify::setProcModifiedBy(int varName, int procName) {

	if ((varName + 1) > procModifiedByTable.size()) {
		procModifiedByTable.resize(varName + 1);
		procModifiedByTable[varName].push_back(procName);
	}
	else {
		procModifiedByTable[varName].push_back(procName);
	}
}

vector<int> Modify::getModifies(int s) {

	vector<vector<int>> result(1);
	result[0] = modifiesTable[s];
	return result[0];
}

vector<int> Modify::getModifiedBy(int varName) {

	vector<vector<int>> result(1);
	result[0] = modifiedByTable[varName];
	return result[0];
}

vector<int> Modify::getProcModifies(int procName) {

	vector<vector<int>> result(1);
	result[0] = procModifiesTable[procName];
	return result[0];
}

vector<int> Modify::getProcModifiedBy(int varName) {

	vector<vector<int>> result(1);
	result[0] = procModifiedByTable[varName];
	return result[0];
}