#include "Modify.h"


Modify::Modify() {

	vector<set<int>> modifiesTable; //vector of string, at stmt can be a container stmt
	vector<set<int>> modifiedByTable;
	vector<set<int>> procModifiesTable;
	vector<set<int>> procModifiedByTable;
	int modifyCount = 0;
	int procModifyCount = 0;
} 

void Modify::setModifies(int statementNum, int varName, vector<int> parentStarOfStmt) {

	if (statementNum <= 0) {

		throw new invalid_argument("Statement Number must be more than 0.");
	}

	setModifies(statementNum, varName);
	setModifiedBy(varName, statementNum);

	if (parentStarOfStmt.size() != 0)
		for (int i = 0; i < parentStarOfStmt.size(); i++) {
			setModifies(parentStarOfStmt[i], varName);
	}
}

void Modify::setModifies(int statementNum, int varName) {
	if (statementNum <= 0) {
		throw new invalid_argument("Statement Number must be more than 0.");
	}
	if (modifiesTable.size() <= statementNum) {
		modifiesTable.resize(statementNum + 1);
	}
	modifyCount++;
	modifiesTable[statementNum].insert(varName);
	setModifiedBy(varName, statementNum);
}

void Modify::setModifiedBy(int varName, int statementNum) {
	if (modifiedByTable.size() <= varName) {
		modifiedByTable.resize(varName + 1);
	}
	modifiedByTable[varName].insert(statementNum);
}

void Modify::setProcModifies(int procName, int varName) {
	if (procModifiesTable.size() <= procName) {
		procModifiesTable.resize(procName + 1);
	}
	procModifyCount++;
	procModifiesTable[procName].insert(varName);
	setProcModifiedBy(varName, procName);
}

void Modify::setProcModifiedBy(int varName, int procName) {
	if (procModifiedByTable.size() <= varName) {
		procModifiedByTable.resize(varName + 1);
	}
	procModifiedByTable[varName].insert(procName);
}

vector<int> Modify::getModifies(int statementNum) {
	if (modifiesTable.size() > statementNum) {
		vector<int> result;
		result.insert(result.end(), modifiesTable[statementNum].begin(), modifiesTable[statementNum].end());
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

int Modify::getModifyCount() {
	return modifyCount;
}

int Modify::getProcModifyCount() {
	return procModifyCount;
}