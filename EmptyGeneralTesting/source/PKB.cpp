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

void PKB::setVarIndex(string varName) {

	if (find(varIndexTable.begin(), varIndexTable.end(), varName) != varIndexTable.end())
		return;
	
	else 
		varIndexTable.push_back(varName);
}

int PKB::getVarIndex(string varName) {

	int pos = find(varIndexTable.begin(), varIndexTable.end(), varName) - varIndexTable.begin();

	if (pos < varIndexTable.size())
		return pos;

	else
		throw new invalid_argument("Variable not found in PKB.");
}

void PKB::setProcIndex(string varName) {

	if (find(procIndexTable.begin(), procIndexTable.end(), varName) != procIndexTable.end())
		return;

	else
		procIndexTable.push_back(varName);
}

int PKB::getProcIndex(string varName) {

	int pos = find(procIndexTable.begin(), procIndexTable.end(), varName) - procIndexTable.begin();

	if (pos < procIndexTable.size())
		return pos;

	else
		throw new invalid_argument("Variable not found in PKB.");
}