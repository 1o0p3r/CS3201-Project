#include "Modify.h"
#include <set>
#include <string>

Modify::Modify() {

	vector<set<int>> modifiesTable; //vector of string, at stmt can be a container stmt
	vector<set<int>> modifiedByTable;
	vector<set<int>> procModifiesTable;
	vector<set<int>> procModifiedByTable;
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
	modifiesTable[statementNum].insert(varName);
	setModifiedBy(varName, statementNum);
}

void Modify::setModifiedBy(int varName, int statementNum) {
	if (modifiedByTable.size() <= varName) {
		modifiedByTable.resize(varName + 1);
	}
	modifiedByTable[varName].insert(statementNum);
}

void Modify::setProcModifies(int procName, int varName, vector<int> procIsCalledBy, vector<int> procIsCalling, vector<int> procCalledByStmt) {

	for (int i = 0; i < procIsCalledBy.size(); i++) {
		setProcModifies(procIsCalledBy[i], varName);
	}

	vector<int> variablesModified;
	for (int i = 0; i < procIsCalling.size(); i++) {
		variablesModified = getProcModifies(procIsCalling[i]);
		for (int j = 0; j < variablesModified.size(); j++) {
			setProcModifies(procName, variablesModified[j]);
		}
	}

	for (int i = 0; i < procCalledByStmt.size(); i++) {
		setModifies(procCalledByStmt[i], varName);
	}

	setProcModifies(procName, varName);
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