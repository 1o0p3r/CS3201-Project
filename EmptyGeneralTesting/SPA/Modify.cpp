#include "Modify.h"
#include <set>
#include <string>

Modify::Modify() {

	vector<set<int>> modifiesTable; //vector of string, at stmt can be a container stmt
	vector<set<int>> modifiedByTable;
	vector<set<int>> procModifiesTable;
	vector<set<int>> procModifiedByTable;
} 

void Modify::setModifies(int s, int varName, vector<int> parentsOfstmt) {

	if (s <= 0) { 

		throw new invalid_argument("Statement Number must be more than 0.");
	}

	setModifies(s, varName);
	setModifiedBy(varName, s);

	if (parentsOfstmt.size() != 0)
		for (int i = 0; i < parentsOfstmt.size(); i++){
			setModifies(parentsOfstmt[i], varName);

	}
}

void Modify::setModifies(int s, int varName) {

	if (s <= 0) {

		throw new invalid_argument("Statement Number must be more than 0.");
	}

	if (s > modifiesTable.size())
		modifiesTable.resize(s + 1);

	modifiesTable[s].insert(varName);
	setModifiedBy(varName, s);
}

void Modify::setModifiedBy(int varName, int s) {

	if ((varName+1) > modifiedByTable.size()) {
		
		modifiedByTable.resize(varName + 1);
		modifiedByTable[varName].insert(s);
	}

	else {

		modifiedByTable[varName].insert(s);
	}
}

void Modify::setProcModifies(int procName, int varName) {

	if ((procName + 1) > procModifiesTable.size()) {
		
		procModifiesTable.resize(procName + 1);
		procModifiesTable[procName].insert(varName);
	}
	else {

		procModifiesTable[procName].insert(varName);
	}

	setProcModifiedBy(varName, procName);
}

void Modify::setProcModifiedBy(int varName, int procName) {

	if ((varName + 1) > procModifiedByTable.size()) {
		procModifiedByTable.resize(varName + 1);
		procModifiedByTable[varName].insert(procName);
	}
	else {
		procModifiedByTable[varName].insert(procName);
	}
}

vector<int> Modify::getModifies(int s) {

	vector<vector<int>> result(1);
	result[0].insert(result[0].end(), modifiesTable[s].begin(), modifiesTable[s].end());
	return result[0];
}

vector<int> Modify::getModifiedBy(int varName) {

	vector<vector<int>> result(1);
	result[0].insert(result[0].end(), modifiesTable[varName].begin(), modifiesTable[varName].end());
	return result[0];
}

vector<int> Modify::getProcModifies(int procName) {

	vector<vector<int>> result(1);
	result[0].insert(result[0].end(), procModifiesTable[procName].begin(), procModifiesTable[procName].end());
	return result[0];
}

vector<int> Modify::getProcModifiedBy(int varName) {

	vector<vector<int>> result(1);
	result[0].insert(result[0].end(), procModifiedByTable[varName].begin(), procModifiedByTable[varName].end());
	return result[0];
}