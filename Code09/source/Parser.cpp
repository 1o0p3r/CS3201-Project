#pragma once

#include <stdio.h>
#include <Util.h>
#include "PKB.h"
#include "Parser.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;
#define WHILE "while"
#define IF "if"
#define ASSIGN "assign"

bool Parse(string fileName, PKB& pkb) {

	ifstream file(fileName);
	string line;
	string procName;

	int lineCounter = 0;
	bool isSameLevel = false;	//is same nesting level
	bool isNewContainer = true;
	int nestLevel = 1;
	int prevFollow;

	getline(file, line);
	vector<string> firstLine = Util::splitLine(line, ' ');

	//	tells you who is the immediate parent so you can call setParent	
	vector<int> Parent(1);

	//detect procedure
	if (firstLine[0] != "procedure" && firstLine[2] != "{" && !Util::isValidName(firstLine[1])) {
		return false;
	} else {
		procName = firstLine[1];
	}
	while (getline(file, line)) { //read line by line
		lineCounter++;
		vector<string> nextLine = Util::splitLine(Util::trim(line), ' ');
		if (nextLine[0] == "else" && nextLine[1] == "{") { // line starts with else, no statement number
			lineCounter--;
			isNewContainer = true;
		} else {
			if (!isNewContainer) {
				if (isSameLevel) {
					pkb.setFollows(lineCounter - 1, lineCounter);
				} else {
					pkb.setFollows(prevFollow, lineCounter);
				}
			}
			if (nestLevel > 1) {
				pkb.setParent(Parent.back(), lineCounter);
			}
			if (nextLine[1] == "=" && Util::isValidName(nextLine[0])) {
				bool hasSemicolon = false;
				pkb.setStatementType(lineCounter, ASSIGN);
				string var = nextLine[0];
				pkb.setModifies(lineCounter, var);
				pkb.setProcModifies(procName, var);
				for (int i = 2; i < nextLine.size(); i++) {
					string current = nextLine[i];
					if (current.back() == ';') {
						current = current.substr(0, current.size() - 1);
						hasSemicolon = true;
					}
					if (Util::isNumber(current)) {//number
						pkb.addConstant(current);
					} else if (Util::isValidName(current)) {//var
						pkb.setUses(lineCounter, current);
						pkb.setProcUses(procName, current);
					} else if (!Util::isOperand(current) && current != "}") {// only operands left
						return false;
					}
				}
				pkb.addPattern(lineCounter, var, Util::getExpression(nextLine));
				if (!hasSemicolon) {
					return false;
				}
				isNewContainer = false;
				isSameLevel = true;
			} else if (nextLine[0] == IF && nextLine[2] == "then" && Util::isValidName(nextLine[1]) && nextLine[3] == "{") {
				pkb.setStatementType(lineCounter, IF);
				string var = nextLine[1];
				pkb.setUses(lineCounter, var);
				pkb.setProcUses(procName, var);
				Parent.push_back(lineCounter);
				nestLevel++;
				isSameLevel = false;
				isNewContainer = true;
			} else if (nextLine[0] == WHILE && nextLine[2] == "{" && Util::isValidName(nextLine[1])) {
				pkb.setStatementType(lineCounter, WHILE);
				string var = nextLine[1];
				pkb.setUses(lineCounter, var);
				pkb.setProcUses(procName, var);
				Parent.push_back(lineCounter);
				nestLevel++;
				isSameLevel = false;
				isNewContainer = true;
			} else {
				return false;
			}
			// after all statements, check for '}'s at the end
			for (int i = nextLine.size() - 1; i >= 0; i--) {
				if (nextLine[i] == "}") {
					isSameLevel = false;
					nestLevel--;
					prevFollow = Parent.back();
					Parent.pop_back();
				} else {
					break;
				}
			}
		}
	}
	if (nestLevel == 0) {
		return true;
	} else {
		return false;
	}
}