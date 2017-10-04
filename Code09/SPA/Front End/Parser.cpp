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
#define PROCEDURE "procedure"
#define CALL "call"

bool Parse(string fileName, PKB& pkb) {

	ifstream file(fileName);
	string line;
	string procName;

	int lineCounter = 0;
	bool isSameLevel = false;	//is same nesting level
	bool isNewContainer = true;
	int nestLevel = 0;
	int prevFollow;
	tuple<string, string> temp;

	getline(file, line);
	vector<string> firstLine = Util::splitLine(line, ' ');

	//	tells you who is the immediate parent so you can call setParent	
	vector<int> Parent(1);

	//detect procedure
	if (firstLine[0] != PROCEDURE) {
		return false;
	} else {
		if (firstLine.size() == 3 && firstLine[2] == "{" && Util::isValidName(firstLine[1])) {
			procName = firstLine[1];
		} else if (firstLine.size() == 2) {
			temp = Util::extractBrackets(firstLine[1]);
			if (Util::isValidName(get<0>(temp)) && get<1>(temp) == "{") {
				procName = get<0>(temp);
			} else {
				return false;
			}
		} else {
			return false;
		}
		nestLevel++;
		pkb.setFirstline(procName, 1);
	}
	while (getline(file, line)) { //read line by line
		lineCounter++;
		vector<string> nextLine = Util::splitLine(Util::trim(line), ' ');
		if (nextLine.size() == 0) {
			lineCounter--;
		} else if (nextLine[0].substr(0, 4) == "else") {
			if (nextLine.size() == 2 && nextLine[1] != "{") {
				temp = Util::extractBrackets(nextLine[0]);
				if (get<0>(temp) != "else" || get<1>(temp) != "{") {
					return false;
				}
			}
			lineCounter--;
			nestLevel++;
			Parent.push_back(prevFollow);
			isNewContainer = true;
		} else if (nextLine[0] == PROCEDURE && nestLevel == 0 && Parent.size() != 0) {
			string proc_name;
			if (nextLine.size() == 3 && Util::isValidName(nextLine[1]) && nextLine[2] != "{") {
				proc_name = nextLine[1];
			} else if (nextLine.size() == 2) {
				temp = Util::extractBrackets(firstLine[1]);
				if (Util::isValidName(get<0>(temp)) && get<1>(temp) == "{") {
					proc_name = get<0>(temp);
				} else {
					return false;
				}
			} else {
				return false;
			}
			pkb.setLastline(procName, lineCounter - 1);
			procName = proc_name;
			Parent.push_back(0);
			lineCounter--;
			isNewContainer = true;
			isSameLevel = false;
			nestLevel++;
			pkb.setFirstline(procName, lineCounter + 1);
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
			} else if (nextLine[0] == IF && Util::isValidName(nextLine[1])) {
				if (!(nextLine.size() == 4 && nextLine[2] == "then" && nextLine[3] == "{")) {
					temp = Util::extractBrackets(nextLine[2]);
					if (nextLine.size() != 3 || get<0>(temp) != "then" || get<1>(temp) != "{") {
						return false;
					}
				}
				pkb.setStatementType(lineCounter, IF);
				string var = nextLine[1];
				pkb.setUses(lineCounter, var);
				pkb.setProcUses(procName, var);
				Parent.push_back(lineCounter);
				nestLevel++;
				isSameLevel = false;
				isNewContainer = true;
			} else if (nextLine[0] == WHILE) {
				string var;
				if (nextLine.size() == 3 && nextLine[2] == "{" && Util::isValidName(nextLine[1])) {
					var = nextLine[1];
				} else if (nextLine.size() == 2) {
					temp = Util::extractBrackets(nextLine[1]);
					if (Util::isValidName(get<0>(temp)) && get<1>(temp) == "{") {
						var = get<0>(temp);
					} else {
						return false;
					}
				} else {
					return false;
				}
				pkb.setStatementType(lineCounter, WHILE);
				pkb.setUses(lineCounter, var);
				pkb.setProcUses(procName, var);
				Parent.push_back(lineCounter);
				nestLevel++;
				isSameLevel = false;
				isNewContainer = true;
			} else if (nextLine[0] == CALL) {
				string proc_name;
				if (nextLine.size() == 2) {
					if (nextLine[1].back() != ';') {
						return false;
					} else {
						proc_name = nextLine[1].substr(0, nextLine[1].size() - 1);
					}
				} else if (nextLine.size() == 3) {
					if (nextLine[2] == ";") {
						proc_name = nextLine[1];
					} else {
						return false;
					}
				}
				if (Util::isValidName(proc_name)) {
					pkb.setCalls(procName, proc_name);
					isSameLevel = true;
					isNewContainer = false;
				} else {
					return false;
				}
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