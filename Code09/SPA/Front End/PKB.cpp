#pragma once

#include "PKB.h"
#include "Follow.h"
#include "Parent.h"
#include "Modify.h"
#include "Use.h"
#include "Calls.h"
#include "Util.h"

#include <stdio.h>
#include <iostream>
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

	vector<string> varIndexTable;
	vector<string> procIndexTable;
	vector<int> whileTable;
	vector<int> assignTable;
	vector<int> ifTable;
	vector<int> callTable;
	vector<int> firstlineTable;
	vector<int> lastlineTable;
	vector<vector<tuple<int, string>>> patternTable;
	set<string> allVariables;
	set<string> allConstants;
	set<string> allProcedures;

	initTypeMap();
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

void PKB::addPattern(int statementNum, string leftVariable, string rightExpression) {
	int varIndex = getVarIndex(leftVariable);
	tuple<int, string> entry = { statementNum, Util::insertBrackets(rightExpression) };
	if (patternTable.size() <= varIndex) {
		patternTable.resize(varIndex + 1);
	}
	patternTable[varIndex].push_back(entry);
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

void PKB::setModifies(int statementNum, string varName) {
	addVariable(varName);
	int index = getVarIndex(varName);
	modify.setModifies(statementNum, index, parent.getParentStar(statementNum));
}

void PKB::setProcModifies(string procName, string varName) {
	addVariable(varName);
	addProcedure(procName);
	int procIndex = getProcIndex(procName);
	int varIndex = getVarIndex(varName);

	modify.setProcModifies(procIndex, varIndex, call.getCalledBy(procIndex), call.getCalls(procIndex));
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
	addVariable(varName);
	int index = getVarIndex(varName);
	use.setUses(statementNum, index, parent.getParentStar(statementNum));
}

void PKB::setProcUses(string procName, string varName) {
	addVariable(varName);
	addProcedure(procName);
	int procIndex = getProcIndex(procName);
	int varIndex = getVarIndex(varName);

	use.setProcUses(procIndex, varIndex, call.getCalledBy(procIndex), call.getCalls(procIndex));
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

void PKB::setCalls(string procName1, string procName2) {
	int index1 = getProcIndex(procName1);
	int index2 = getProcIndex(procName2);
	call.setCalls(index1, index2);
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

void PKB::setStatementType(int statementNum, string type) {
	// 1 = while, 2 = assign, 3 = if, 4 = call
	switch (mapTypeValues[type]) {
	case _while: 
		whileTable.push_back(statementNum);
		break;
	case assign: 
		assignTable.push_back(statementNum);
		break;
	case _if: 
		ifTable.push_back(statementNum);
		break;
	case _call:
		callTable.push_back(statementNum);
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
	set<int> setOfAllCalls = call.getAllCalls();
	vector<int> results;
	results.insert(results.end(), setOfAllCalls.begin(), setOfAllCalls.end());
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