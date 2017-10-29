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
#include <iterator>
#include <algorithm>
#include <unordered_map>


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
	vector<tuple<vector<int>, vector<string>>> patternTable;
	unordered_map<string, tuple<vector<int>, vector<string>>> expressionTable;
	vector<int> statementList;
	vector<vector<int>> whilePatternTable;
	vector<vector<int>> ifPatternTable;
	set<string> allVariables;
	set<string> allConstants;
	set<string> allProcedures;
	set<int> elseSet;
	vector<int> statementProcedureTable;

	initTypeMap();
}

void PKB::insertElse(int statementNum) {
	this->insertStatementList(statementNum);
	elseSet.insert(statementNum);
}

void PKB::insertStatementList(int statementNum) {
	statementList.push_back(statementNum);
}

vector<int> PKB::getStatementList() {
	return statementList;
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
	result.insert(result.end(), allVariables.begin(), allVariables.end());
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
	string expression = Util::getPostFixExp(rightExpression);
	if (patternTable.size() <= varIndex) {
		patternTable.resize(varIndex + 1);
	}
	get<0>(patternTable[varIndex]).push_back(statementNum);
	get<1>(patternTable[varIndex]).push_back(expression);
	tuple<vector<int>, vector<string>> current = expressionTable[expression];
	get<0>(current).push_back(statementNum);
	get<1>(current).push_back(leftVariable);
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

tuple<vector<int>, vector<string>> PKB::getAllPatternWhile() {
	vector<int> statements;
	vector<string> variables;
	for (int i = 0; i < whilePatternTable.size(); i++) {
		for each (int line in whilePatternTable[i]) {
			statements.push_back(line);
			variables.push_back(varIndexTable[i]);
		}
	}
	return tuple<vector<int>, vector<string>> {statements, variables};
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

tuple<vector<int>, vector<string>> PKB::getAllPatternIf() {
	vector<int> statements;
	vector<string> variables;
	for (int i = 0; i < ifPatternTable.size(); i++) {
		for each (int line in ifPatternTable[i]) {
			statements.push_back(line);
			variables.push_back(varIndexTable[i]);
		}
	}
	return tuple<vector<int>, vector<string>> {statements, variables};
}

tuple<vector<int>, vector<string>> PKB::getPatternVariable(string varName) {
	int varIndex = getVarIndex(varName);

	if (varIndex >= patternTable.size()) {
		return tuple<vector<int>, vector<string>>{};
	} else {
		return patternTable[varIndex];
	}
}

tuple<vector<int>, vector<string>> PKB::getPatternExpression(string expression) {
	if (expressionTable.find(expression) == expressionTable.end()) {
		return tuple<vector<int>, vector<string>>{};
	} else {
		return expressionTable[expression];
	}
}

tuple<vector<int>, vector<string>> PKB::getPatternExpressionSubstring(string expression) {
	vector<int> statements;
	vector<string> variables;
	for (auto i = expressionTable.begin(); i != expressionTable.end(); i++) {
		if (i->first.find(expression) != string::npos) {
			statements.insert(statements.end(), get<0>(i->second).begin(), get<0>(i->second).end());
			variables.insert(variables.end(), get<1>(i->second).begin(), get<1>(i->second).end());
		}
	}
	return tuple<vector<int>, vector<string>>{ statements, variables };
}

vector<int> PKB::getPatternVariableExpressionSubstring(string variable, string expression) {
	vector<int> statements;
	for (auto i = expressionTable.begin(); i != expressionTable.end(); i++) {
		if (i->first.find(expression) != string::npos) {
			for (int j = 0; j < get<1>(i->second).size(); i++) {
				if (get<1>(i->second)[j] == variable) {
					statements.push_back(get<0>(i->second)[j]);
				}
			}
		}
	}
	return statements;
}

vector<int> PKB::getPatternVariableExpression(string variable, string expression) {
	vector<int> results;
	tuple<vector<int>, vector<string>> tup = getPatternExpression(expression);
	for (int i = 0; i < get<1>(tup).size(); i++) {
		if (get<1>(tup)[i] == variable) {
			results.push_back(get<0>(tup)[i]);
		}
	}
	return results;
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
	state.push_back(0);
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
		assignCallCFG(i);
		break;
	case _if:
		ifCFG(i);
		break;
	case _call:
		assignCallCFG(i);
	default:
		break;
	}
}

void PKB::whileCFG(int& i) {
	state.push_back(_while);
	int current = i;
	goBack.push_back(i);
	next.setNext(i, i + 1);
	i++;
	while (contains(parent.getParentStar(i), goBack.back())) {
		processNext(i);
	}
	if (i < typeTable.size()) {
		next.setNext(current, i);
	}
	state.pop_back();
}

void PKB::assignCallCFG(int& i) {
	if (state.back() == _while && (i >= typeTable.size() - 1 || parent.getParent(i + 1).empty() || (parent.getParent(i + 1)[0] != goBack.back()))) {
		next.setNext(i, goBack.back());
		goBack.pop_back();
	} else if (state.back() == _if && (i >= typeTable.size() - 1 || parent.getParent(i + 1).empty() || (elseSet.find(i + 1) != elseSet.end()) || (parent.getParent(i + 1)[0] != ifParent.back()))) {
		lastLineOfIf = i;
		ifParent.pop_back();
	} else if (i < typeTable.size() - 1 && (find(firstlineTable.begin(), firstlineTable.end(), i + 1) == firstlineTable.end())) {
		next.setNext(i, i + 1);
	}
	i++;
}

void PKB::ifCFG(int& i) {
	state.push_back(_if);
	ifParent.push_back(i);
	int current = i;
	next.setNext(i, i + 1);
	i++;
	while (contains(parent.getParentStar(i), current) && (elseSet.find(i) == elseSet.end())) {
		processNext(i);
	}
	ifParent.push_back(current);
	int store = lastLineOfIf;
	next.setNext(current, i);
	while (contains(parent.getParentStar(i), current)) {
		processNext(i);
	}
	state.pop_back();
	int nextLine;
	if (elseSet.find(i) != elseSet.end()) {
		ifHolder.push_back(store);
		ifHolder.push_back(lastLineOfIf);
	} else if (i < typeTable.size()) {
		if (state.back() == _while) {
			nextLine = goBack.back();
			goBack.pop_back();
		} else {
			nextLine = i;
		}
		if (find(firstlineTable.begin(), firstlineTable.end(), nextLine) == firstlineTable.end()) {
			next.setNext(store, nextLine);
			next.setNext(lastLineOfIf, nextLine);
			for each (int line in ifHolder) {
				next.setNext(line, nextLine);
			}
			ifHolder.clear();
		}
	}
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
	result.insert(result.end(), _call.begin(), _call.end());
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

void PKB::setProcedure(int stmtNum, string procedure) {
	if (statementProcedureTable.size() <= stmtNum) {
		statementProcedureTable.resize(stmtNum + 1);
	}
	statementProcedureTable[stmtNum] = getProcIndex(procedure);
}

bool PKB::getAffectsTwoLiterals(int statementNum1, int statementNum2) {
	if (typeTable[statementNum1] != assign || typeTable[statementNum2] != assign || statementProcedureTable[statementNum1] != statementProcedureTable[statementNum2]) {
		return false;
	}
	vector<int> variables = getIntersection(modify.getModifies(statementNum1), use.getUses(statementNum2));
	if (variables.size() != 1) {
		return false;
	}
	int var = variables[0];
	vector<int> frontier;
	frontier.push_back(statementNum1);
	set<int> explored;
	while (!frontier.empty()) {
		vector<int> nextFrontier;
		for each (int current in frontier) {
			for each (int statement in next.getNext(current)) {
				if (statement == statementNum2) {
					return true;
				} else if ((typeTable[statement] == _call || typeTable[statement] == assign) && !contains(modify.getModifies(statement), var) && explored.find(statement) == explored.end()) {
					nextFrontier.push_back(statement);
					explored.insert(statement);
				} else if ((typeTable[statement] == _while || typeTable[statement] == _if) && explored.find(statement) == explored.end()) {
					nextFrontier.push_back(statement);
					explored.insert(statement);
				}
			}
		}
		frontier = nextFrontier;
	}
	return false;
}

vector<int> PKB::getAffectsFirstLiteral(int statementNum) {
	if (typeTable[statementNum] != assign) {
		return vector<int>{};
	}
	int var = modify.getModifies(statementNum)[0];
	vector<int> results;
	vector<int> frontier;
	frontier.push_back(statementNum);
	set<int> explored;
	while (!frontier.empty()) {
		vector<int> nextFrontier;
		for each (int current in frontier) {
			for each (int statement in next.getNext(current)) {
				if ((typeTable[statement] == _call) && !contains(modify.getModifies(statement), var) && explored.find(statement) == explored.end()) {
					nextFrontier.push_back(statement);
					explored.insert(statement);
				} else if ((typeTable[statement] == _while || typeTable[statement] == _if) && explored.find(statement) == explored.end()) {
					nextFrontier.push_back(statement);
					explored.insert(statement);
				} else if (typeTable[statement] == assign && explored.find(statement) == explored.end()) {
					if (contains(use.getUses(statement), var)) {
						results.push_back(statement);
					}
					if (modify.getModifies(statement)[0] != var) {
						nextFrontier.push_back(statement);
					}
					explored.insert(statement);
				}
			}
		}
		frontier = nextFrontier;
	}
	return results;
}

vector<int> PKB::getAffectsSecondLiteral(int statementNum) {
	if (typeTable[statementNum] != assign) {
		return vector<int>{};
	}
	vector<int> variables = use.getUses(statementNum);
	vector<int> results;
	for each (int var in variables) {
		vector<int> frontier;
		frontier.push_back(statementNum);
		set<int> explored;
		while (!frontier.empty()) {
			vector<int> nextFrontier;
			for each (int current in frontier) {
				for each (int statement in next.getPrevious(current)) {
					if ((typeTable[statement] == _call) && !contains(modify.getModifies(statement), var) && explored.find(statement) == explored.end()) {
						nextFrontier.push_back(statement);
						explored.insert(statement);
					} else if ((typeTable[statement] == _while || typeTable[statement] == _if) && explored.find(statement) == explored.end()) {
						nextFrontier.push_back(statement);
						explored.insert(statement);
					} else if (typeTable[statement] == assign && explored.find(statement) == explored.end()) {
						if (modify.getModifies(statement)[0] != var) {
							nextFrontier.push_back(statement);
						} else {
							results.push_back(statement);
						}
						explored.insert(statement);
					}
				}
			}
			frontier = nextFrontier;
		}
	}
	return results;
}
vector<int> PKB::getIntersection(vector<int> v1, vector<int> v2) {
	vector<int> result;
	sort(v1.begin(), v1.end());
	sort(v2.begin(), v2.end());
	set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(result));
	return result;
}

int PKB::getFollowsCount() {
	return follow.getFollowsCount();
}

int PKB::getFollowStarCount() {
	return follow.getFollowStarCount();
}

int PKB::getParentCount() {
	return parent.getParentCount();
}

int PKB::getParentStarCount() {
	return parent.getParentStarCount();
}

int PKB::getModifyCount() {
	return modify.getModifyCount();
}

int PKB::getProcModifyCount() {
	return modify.getProcModifyCount();
}

int PKB::getUseCount() {
	return use.getUseCount();
}

int PKB::getProcUseCount() {
	return use.getProcUseCount();
}