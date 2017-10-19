#pragma once

#include "PKB.h"
#include "Follow.h"
#include "Parent.h"
#include "Modify.h"
#include "Use.h"
#include "Calls.h"
#include "Next.h"
#include "Util.h"

#include <string>
#include <vector>
#include <set>
#include <map>
#include <tuple>

using namespace std;

enum typeValue {
	undefinedSelect, _while, assign, _if, _call
};

static std::map<std::string, typeValue> mapTypeValues;

void PKB::initTypeMap() { //to check with pql whether QS uses these strings in the entity as defined here.
	mapTypeValues["assign"] = assign;
	mapTypeValues["while"] = _while;
	mapTypeValues["if"] = _if;
	mapTypeValues["call"] = _call;
}

PKB::PKB() {
	
	Follow follow;
	Parent parent;
	Modify modify;
	Use use;
	Calls call;
	Next next;

	vector<string> varIndexTable;
	vector<string> procIndexTable;
	vector<int> whileTable;
	vector<int> assignTable;
	vector<int> ifTable;
	vector<int> callTable;
	vector<int> typeTable;
	vector<int> firstlineTable;
	vector<int> lastlineTable;
	vector<vector<tuple<int, string>>> patternTable;
	vector<vector<int>> whilePatternTable;
	vector<vector<int>> ifPatternTable;
	set<string> allVariables;
	set<string> allConstants;
	set<string> allProcedures;
	set<int> elseSet;

	initTypeMap();
}

void PKB::insertElse(int statementNum) {
	elseSet.insert(statementNum);
}

int PKB::getVarIndex(string varName) {

	allVariables.insert(varName);

	if (find(varIndexTable.begin(), varIndexTable.end(), varName) != varIndexTable.end())
		return find(varIndexTable.begin(), varIndexTable.end(), varName) - varIndexTable.begin();
	
	else {
		varIndexTable.push_back(varName);
		return find(varIndexTable.begin(), varIndexTable.end(), varName) - varIndexTable.begin();
	}
}

int PKB::getProcIndex(string procName) {

	allProcedures.insert(procName);

	if (find(procIndexTable.begin(), procIndexTable.end(), procName) != procIndexTable.end())
		return find(procIndexTable.begin(), procIndexTable.end(), procName) - procIndexTable.begin();

	else {
		procIndexTable.push_back(procName);
		return find(procIndexTable.begin(), procIndexTable.end(), procName) - procIndexTable.begin();
	}
}

vector<string> PKB::getAllVariables() {

	vector<string> result;
	result.insert(result.end(), allVariables. begin(), allVariables.end());
	return result;
}

void PKB::addConstant(string c) {
	allConstants.insert(c);
}

void PKB::addVariable(string v) {
	getVarIndex(v);
}

void PKB::addProcedure(string p) {
	getProcIndex(p);
}

void PKB::addAssignPattern(int statementNum, string leftVariable, string rightExpression) {
	int varIndex = getVarIndex(leftVariable);
	tuple<int, string> entry = { statementNum, Util::insertBrackets(rightExpression) };
	if (patternTable.size() <= varIndex) {
		patternTable.resize(varIndex + 1);
	}
	patternTable[varIndex].push_back(entry);
}

void PKB::addWhilePattern(int statementNum, string variable) {
	int varIndex = getVarIndex(variable);
	if (whilePatternTable.size() <= varIndex) {
		whilePatternTable.resize(varIndex + 1);
	}
	whilePatternTable[varIndex].push_back(statementNum);
}

vector<int> PKB::getPatternWhile(string variable) {
	int varIndex = getVarIndex(variable);
	if (varIndex >= whilePatternTable.size()) {
		return vector<int>{};
	} else {
		return whilePatternTable[varIndex];
	}
}

void PKB::addIfPattern(int statementNum, string variable) {
	int varIndex = getVarIndex(variable);
	if (ifPatternTable.size() <= varIndex) {
		ifPatternTable.resize(varIndex + 1);
	}
	ifPatternTable[varIndex].push_back(statementNum);
}

vector<int> PKB::getPatternIf(string variable) {
	int varIndex = getVarIndex(variable);
	if (varIndex >= ifPatternTable.size()) {
		return vector<int>{};
	} else {
		return ifPatternTable[varIndex];
	}
}

vector<tuple<int, string>> PKB::getPattern(string varName) {
	int varIndex = getVarIndex(varName);
	return patternTable[varIndex];
}

vector<string> PKB::getAllConstants() {
	vector<string> result;
	result.insert(result.end(), allConstants.begin(), allConstants.end());
	return result;
}

vector<string> PKB::getAllProcedures() {
	vector<string> result;
	result.insert(result.end(), allProcedures.begin(), allProcedures.end());
	return result;
}

void PKB::setFollows(int statementNum1, int statementNum2) {
	follow.setFollows(statementNum1, statementNum2);
}

vector<int> PKB::getFollows(int statementNum) {
	return follow.getFollows(statementNum);
}

vector<int> PKB::getFollowedBy(int statementNum) {
	return follow.getFollowedBy(statementNum);
}

vector<int> PKB::getFollowsStar(int statementNum) {
	return follow.getFollowsStar(statementNum);
}

vector<int> PKB::getFollowedByStar(int statementNum) {
	return follow.getFollowedByStar(statementNum);
}

void PKB::setParent(int statementNum1, int statementNum2) {
	parent.setParent(statementNum1, statementNum2);
}

vector<int> PKB::getParent(int statementNum) {
	return parent.getParent(statementNum);
}

vector<int> PKB::getChild(int statementNum) {
	return parent.getChild(statementNum);
}

vector<int> PKB::getParentStar(int statementNum) {
	return parent.getParentStar(statementNum);
}

vector<int> PKB::getChildStar(int statementNum) {
	return parent.getChildStar(statementNum);
}

vector<int> PKB::getAllParent() {
	return parent.getAllParent();
}

void PKB::setModifies(int statementNum, string varName) {
	int index = getVarIndex(varName);
	modify.setModifies(statementNum, index, parent.getParentStar(statementNum));
}

void PKB::setProcModifies(string procName, string varName) {
	int procIndex = getProcIndex(procName);
	int varIndex = getVarIndex(varName);

	modify.setProcModifies(procIndex, varIndex, call.getCalledBy(procIndex), call.getCalls(procIndex), call.getProcCalledByStmt(procIndex));
}

vector<string> PKB::convertToVarNames(vector<int> input) {
	vector<string> results;
	for each (int index in input) {
		results.push_back(varIndexTable[index]);
	}
	return results;
}

vector<string> PKB::convertToProcNames(vector<int> input) {
	vector<string> results;
	for each (int index in input) {
		results.push_back(procIndexTable[index]);
	}
	return results;
}

vector<string> PKB::getModifies(int statementNum) {
	vector<int> results = modify.getModifies(statementNum);
	return convertToVarNames(results);
}

vector<int> PKB::getModifiedBy(string varName) {
	int index = getVarIndex(varName);
	return modify.getModifiedBy(index);
}

vector<string> PKB::getProcModifies(string procName) {
	int index = getProcIndex(procName);
	vector<int> results = modify.getProcModifies(index);
	return convertToVarNames(results);
}

vector<string> PKB::getProcModifiedBy(string varName) {
	int index = getVarIndex(varName);
	vector<int> results = modify.getProcModifiedBy(index);
	return convertToProcNames(results);
}

void PKB::setUses(int statementNum, string varName) {
	int index = getVarIndex(varName);
	use.setUses(statementNum, index, parent.getParentStar(statementNum));
}

void PKB::setProcUses(string procName, string varName) {
	int procIndex = getProcIndex(procName);
	int varIndex = getVarIndex(varName);

	use.setProcUses(procIndex, varIndex, call.getCalledBy(procIndex), call.getCalls(procIndex), call.getProcCalledByStmt(procIndex));
}

vector<string> PKB::getUses(int statementNum) {
	vector<int> results = use.getUses(statementNum);
	return convertToVarNames(results);
}

vector<int> PKB::getUsedBy(string varName) {
	int index = getVarIndex(varName);
	vector<int> results = use.getUsedBy(index);
	return results;
}

vector<string> PKB::getProcUses(string procName) {
	int index = getProcIndex(procName);
	vector<int> results = use.getProcUses(index);
	return convertToVarNames(results);
}

vector<string> PKB::getProcUsedBy(string varName) {
	int index = getVarIndex(varName);
	vector<int> results = use.getProcUsedBy(index);
	return convertToProcNames(results);
}

void PKB::setCalls(int statementNum, string procName1, string procName2) {
	int index1 = getProcIndex(procName1);
	int index2 = getProcIndex(procName2);
	call.setCalls(statementNum, index1, index2);
	if (index2 < index1) {
		vector<int> modifies = modify.getProcModifies(index2);
		for each (int var in modifies) {
			modify.setProcModifies(index1, var);
			modify.setModifies(statementNum, var, parent.getParentStar(statementNum));
		}
		vector<int> uses = use.getProcUses(index2);
		for each (int var in uses) {
			use.setProcUses(index1, var);
			use.setUses(statementNum, var, parent.getParentStar(statementNum));
		}
	}
}

vector<string> PKB::getCalls(string procName) {
	int procNameIndex = getProcIndex(procName);
	vector<int> results = call.getCalls(procNameIndex);
	return convertToProcNames(results);
}

vector<string> PKB::getCalledBy(string procName) {
	int procNameIndex = getProcIndex(procName);
	vector<int> results = call.getCalledBy(procNameIndex);
	return convertToProcNames(results);
}

vector<string> PKB::getCallsStar(string procName) {
	int procNameIndex = getProcIndex(procName);
	vector<int> results = call.getCallsStar(procNameIndex);
	return convertToProcNames(Util::removeDuplicates(results));
}

vector<string> PKB::getCalledByStar(string procName) {
	int procNameIndex = getProcIndex(procName);
	vector<int> results = call.getCalledByStar(procNameIndex);
	return convertToProcNames(Util::removeDuplicates(results));
}

void PKB::createCFG() {
	int i = 1;
	while (i < typeTable.size()) {
		if (next.getNext(i).empty()) {
			processNext(i);
		} else {
			i++;
		}
	}
}

void PKB::processNext(int& i) {
	switch (typeTable[i]) {
	case _while:
		whileCFG(i);
		break;
	case assign:
		assignCFG(i);
		break;
	case _if:
		ifCFG(i);
		break;
	case _call:
		callCFG(i);
	default:
		break;
	}
}

void PKB::whileCFG(int& i) {
	int current = i;
	goBack.push_back(i);
	next.setNext(i, i + 1);
	i++;
	while (contains(parent.getParentStar(i), goBack.back())) {
		state = _while;
		processNext(i);
	}
	if (i < typeTable.size()) {
		next.setNext(current, i);
	}
	state = 0;
}

void PKB::assignCFG(int& i) {
	if (state == _while && ( i >= typeTable.size() - 1 || parent.getParent(i + 1).empty() || (parent.getParent(i + 1)[0] != goBack.back()))) {
		next.setNext(i, goBack.back());
		goBack.pop_back();
	} else if (state == _if && (i >= typeTable.size() - 1 || parent.getParent(i + 1).empty() || (elseSet.find(i) == elseSet.end()) || (parent.getParent(i + 1)[0] != goBack.back()))) {
		lastLineOfIf = i;
		goBack.pop_back();
	} else if (i < typeTable.size() - 1){
		next.setNext(i, i + 1);
	}
	i++;
}

void PKB::ifCFG(int& i) {
	goBack.push_back(i);
	int current = i;
	next.setNext(i, i + 1);
	i++;
	while (contains(parent.getParentStar(i), current) && (elseSet.find(i) == elseSet.end())) {
		state = _if;
		processNext(i);
	}
	int store = lastLineOfIf;
	while (contains(parent.getParentStar(i), current)) {
		state = _if;
		processNext(i);
	}
	if (i < typeTable.size()) {
		next.setNext(store, i);
		next.setNext(lastLineOfIf, i);
	}
	state = 0;
}

void PKB::callCFG(int& i) {
	int proc = call.getStmtCallProc(i);
	int firstline = firstlineTable[proc];
	int lastline = lastlineTable[proc];
	next.setNext(i, firstline);
	if (state == _while && (i >= typeTable.size() - 1 || parent.getParent(i + 1).empty() || (parent.getParent(i + 1)[0] != goBack.back()))) {
		next.setNext(lastline, goBack.back());
		goBack.pop_back();
	} else if (state == _if && (i >= typeTable.size() - 1 || parent.getParent(i + 1).empty() || (elseSet.find(i) == elseSet.end()) || (parent.getParent(i + 1)[0] != goBack.back()))) {
		lastLineOfIf = lastline;
		goBack.pop_back();
	} else if (i < typeTable.size() - 1) {
		next.setNext(lastline, i + 1);
	}
	i++;
}

bool PKB::contains(vector<int> list, int i) {
	for each (int j in list) {
		if (i == j) {
			return true;
		}
	}
	return false;
}

vector<int> PKB::getNext(int stmtNum) {
	return next.getNext(stmtNum);
}

vector<int> PKB::getPrevious(int stmtNum) {
	return next.getPrevious(stmtNum);
}

vector<int> PKB::getAllNext() {
	return next.getAllNext();
}

void PKB::setStatementType(int statementNum, string type) {
	// 1 = while, 2 = assign, 3 = if, 4 = call
	if (statementNum >= typeTable.size()) {
		typeTable.resize(statementNum + 1);
	}
	switch (mapTypeValues[type]) {
	case _while: 
		whileTable.push_back(statementNum);
		typeTable[statementNum] = _while;
		break;
	case assign: 
		assignTable.push_back(statementNum);
		typeTable[statementNum] = assign;
		break;
	case _if: 
		ifTable.push_back(statementNum);
		typeTable[statementNum] = _if;
		break;
	case _call:
		callTable.push_back(statementNum);
		typeTable[statementNum] = _call;
		break;
	default:
		break;
	}
}

vector<int> PKB::getWhile() {
	return whileTable;
}

vector<int> PKB::getAssign() {
	return assignTable;
}

vector<int> PKB::getIf() {
	return ifTable;
}

vector<int> PKB::getCall() {
	return callTable;
}

vector<string> PKB::getAllCalls() {
	vector<int> results = call.getAllCalls();
	return convertToProcNames(results);
}

vector<int> PKB::getAllStmt() {
	vector<int> _while = getWhile();
	vector<int> assign = getAssign();
	vector<int> _if = getIf();
	vector<int> _call = getCall();
	vector<int> result;
	result.reserve(_while.size() + assign.size() + _if.size() + _call.size());
	result.insert(result.end(), _while.begin(), _while.end());
	result.insert(result.end(), assign.begin(), assign.end());
	result.insert(result.end(), _if.begin(), _if.end());
	result.insert(result.end(), _call.begin(), _call .end());
	return result;
}

void PKB::setFirstline(string procName, int firstline) {
	int procIndex = getProcIndex(procName);
	if (firstlineTable.size() <= procIndex) {
		firstlineTable.resize(procIndex + 1);
	}
	firstlineTable[procIndex] = firstline;
}

int PKB::getFirstline(string procName) {
	int procIndex = getProcIndex(procName);
	return firstlineTable[procIndex];
}

void PKB::setLastline(string procName, int lastline) {
	int procIndex = getProcIndex(procName);
	if (lastlineTable.size() <= procIndex) {
		lastlineTable.resize(procIndex + 1);
	}
	lastlineTable[procIndex] = lastline;
}

int PKB::getLastline(string procName) {
	int procIndex = getProcIndex(procName);
	return lastlineTable[procIndex];
}