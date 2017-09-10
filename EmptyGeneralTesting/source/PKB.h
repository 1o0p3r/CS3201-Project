#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
typedef short PROC;

class VarTable;  // no need to #include "VarTable.h" as all I need is pointer

class PKB {

public:

	PKB();

	void PKB::setVarIndex(string varName);
	int PKB::getVarIndex(string varName);
	void PKB::setProcIndex(string varName);
	int PKB::getProcIndex(string varName);

private:

	vector<string> varIndexTable;
	vector<string> procIndexTable;
};