#pragma once

#include "PKB.h"
#include "Follow.h"
#include "Parent.h"
#include "Modify.h"
#include "Use.h"

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

PKB::PKB() {
	
	Follow follow;
	Parent parent;
	Modify modify;
	Use use;

	vector<string> varIndexTable;
	vector<string> procIndexTable;
}

int PKB::getVarIndex(string varName) {

	if (find(varIndexTable.begin(), varIndexTable.end(), varName) != varIndexTable.end())
		return find(varIndexTable.begin(), varIndexTable.end(), varName) - varIndexTable.begin();
	
	else {
		varIndexTable.push_back(varName);
		return find(varIndexTable.begin(), varIndexTable.end(), varName) - varIndexTable.begin();
	}
}

int PKB::getProcIndex(string procName) {

	if (find(procIndexTable.begin(), procIndexTable.end(), procName) != procIndexTable.end())
		return find(procIndexTable.begin(), procIndexTable.end(), procName) - procIndexTable.begin();

	else {
		procIndexTable.push_back(procName);
		return find(procIndexTable.begin(), procIndexTable.end(), procName) - procIndexTable.begin();
	}
}

void PKB::setFollows(int s1, int s2) {
	follow.setFollows(s1, s2);
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

void PKB::setParent(int s1, int s2) {
	parent.setParent(s1, s2);
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

void PKB::setModifies(int s, string varName, vector<int> parentsOfstmt) {
	int index = getVarIndex(varName);
	modify.setModifies(s, index, parent.getParentStar(s));
}

void PKB::setProcModifies(string procName, string varName) {
	int procIndex = getProcIndex(procName);
	int varIndex = getVarIndex(varName);
	modify.setProcModifies(procIndex, varIndex);
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

vector<string> PKB::getModifies(int s) {
	vector<int> results = modify.getModifies(s);
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

void PKB::setUses(int s, string varName, vector<int> parentsOfstmt) {
	int index = getVarIndex(varName);
	use.setUses(s, index, parent.getParentStar(s));
}

void PKB::setProcUses(string procName, string varName) {
	int varIndex = getVarIndex(varName);
	int procIndex = getProcIndex(procName);
	use.setProcUses(procIndex, varIndex);
}

vector<string> PKB::getUses(int s) {
	vector<int> results = use.getUses(s);
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
