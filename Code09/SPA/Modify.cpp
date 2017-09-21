#include "Modify.h"
#include <set>
#include <string>

Modify::Modify() {

	vector<set<int>> modifiesTable; //vector of string, at stmt can be a container stmt
	vector<set<int>> modifiedByTable;
	vector<set<int>> procModifiesTable;
	vector<set<int>> procModifiedByTable;
} 

void Modify::setModifies(int s, int varName, vector<int> parentStarOfStmt) {

	if (s <= 0) { 

		throw new invalid_argument("Statement Number must be more than 0.");
	}

	setModifies(s, varName);
	setModifiedBy(varName, s);

	if (parentStarOfStmt.size() != 0)
		for (int i = 0; i < parentStarOfStmt.size(); i++) {
			setModifies(parentStarOfStmt[i], varName);
	}
}

void Modify::setModifies(int s, int varName) {
	if (s <= 0) {
		throw new invalid_argument("Statement Number must be more than 0.");
	}
	if (modifiesTable.size() <= s) {
		modifiesTable.resize(s + 1);
	}
	modifiesTable[s].insert(varName);
	setModifiedBy(varName, s);
}

void Modify::setModifiedBy(int varName, int s) {
	if (modifiedByTable.size() <= s) {
		modifiedByTable.resize(varName + 1);
	}
	modifiedByTable[varName].insert(s);
}

void Modify::setProcModifies(int procName, int varName) {
	if (procModifiesTable.size() <= procName) {
		procModifiesTable.resize(procName + 1);
	}
	procModifiesTable[procName].insert(varName);
	setProcModifiedBy(varName, procName);
}

void Modify::setProcModifiedBy(int varName, int procName) {
	if (procModifiedByTable.size() <= varName) {
		procModifiedByTable.resize(varName + 1);
	}
	procModifiedByTable[varName].insert(procName);
}

vector<int> Modify::getModifies(int s) {
	if (modifiesTable.size() > s) {
		vector<int> result;
		result.insert(result.end(), modifiesTable[s].begin(), modifiesTable[s].end());
		return result;
	} else {
		return vector<int>();
	}
}

vector<int> Modify::getModifiedBy(int varName) {
	if (modifiedByTable.size() > varName) {
		vector<int> result;
		result.insert(result.end(), modifiedByTable[varName].begin(), modifiedByTable[varName].end());
		return result;
	} else {
		return vector<int>();
	}
}

vector<int> Modify::getProcModifies(int procName) {
	if (procModifiesTable.size() > procName) {
		vector<int> result;
		result.insert(result.end(), procModifiesTable[procName].begin(), procModifiesTable[procName].end());
		return result;
	} else {
		return vector<int>();
	}
}

vector<int> Modify::getProcModifiedBy(int varName) {
	if (procModifiedByTable.size() > varName) {
		vector<int> result;
		result.insert(result.end(), procModifiedByTable[varName].begin(), procModifiedByTable[varName].end());
		return result;
	} else {
		return vector<int>();
	}
}