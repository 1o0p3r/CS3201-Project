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
#define OB "{"
#define CB '}'
#define WHILE "while"
#define IF "if"
#define THEN "then"
#define THEN_B "then{"
#define ASSIGN "assign"
#define PROCEDURE "procedure"
#define CALL "call"
#define ELSE "else"
#define ELSE_B "else{"
#define SPACE ' '
#define SEMICOLON ";"
#define TAB '\t'

bool Parse(string fileName, PKB& pkb) {

	ifstream file(fileName);
	string line;
	string procName;

	int lineCounter = 0;
	bool isSameLevel = false;	//is same nesting level
	bool isNewContainer = true;
	int prevFollow;
	int lastLine;
	tuple<string, string> temp;
	vector<int> Parent;

	while (getline(file, line)) { //read line by line
		line = Util::trim(line);
		lineCounter++;
		vector<string> nextLine = Util::splitLine(line, ' ');
		if (nextLine.size() == 0) {
			lineCounter--;
		} else if (isElseStatement(nextLine)) {
			pkb.insertElse(lineCounter);
			lineCounter--;
			Parent.push_back(prevFollow);
			isSameLevel = false;
			isNewContainer = true;
		} else if (isProcedure(nextLine) && Parent.size() == 0) {
			pkb.insertStatementList(lineCounter);
			lineCounter--;
			if (lineCounter > 1) {
				pkb.setLastline(procName, lineCounter);
			}
			if (nextLine.size() == 3) {
				procName = nextLine[1];
			} else {
				procName = nextLine[1].substr(0, nextLine[1].size() - 1);
			}
			Parent.push_back(0);
			isNewContainer = true;
			isSameLevel = false;
		} else if (startsWithBrackets(nextLine) && !isNewContainer) {
			int i = 0;
			while (i < line.size()) {
				if (line[i] == CB) {
					isSameLevel = false;
					prevFollow = Parent.back();
					Parent.pop_back();
				} else if (line[i] != SPACE && line[i] != TAB) {
					break;
				}
				i++;
			}
			if (i < line.size()) {
				line = line.substr(i, line.size());
				if (!isElseStatement(Util::splitLine(line, ' '))) {
					return false;
				} else {
					Parent.push_back(prevFollow);
					isSameLevel = false;
					isNewContainer = true;
					pkb.insertElse(lineCounter);
				}
			}
			lineCounter--;
		} else {
			if (!isNewContainer) {
				if (isSameLevel) {
					pkb.setFollows(lineCounter - 1, lineCounter);
				} else {
					pkb.setFollows(prevFollow, lineCounter);
				}
			}
			pkb.setProcedure(lineCounter, procName);
			if (Parent.size() > 1) {
				pkb.setParent(Parent.back(), lineCounter);
			}
			if (isAssignStatement(line)) {
				int pos = line.find("=");
				string var = Util::trim(line.substr(0, pos));
				pkb.setStatementType(lineCounter, ASSIGN);
				pkb.setModifies(lineCounter, var);
				pkb.setProcModifies(procName, var);
				string expression = Util::getExpression(line.substr(pos + 1));
				vector<string> expressionList = Util::constructExpression(expression);
				for each (string item in expressionList) {
					if (Util::isNumber(item)) {
						pkb.addConstant(item);
					} else if (Util::isValidName(item)) {
						pkb.setUses(lineCounter, item);
						pkb.setProcUses(procName, item);
					} else {
						return false;
					}
				}
				pkb.addAssignPattern(lineCounter, var, expression);
				isNewContainer = false;
				isSameLevel = true;
			} else if (isIfStatement(nextLine)) {
				pkb.insertStatementList(lineCounter + 1);
				pkb.setStatementType(lineCounter, IF);
				string var = nextLine[1];
				pkb.addIfPattern(lineCounter, var);
				pkb.setUses(lineCounter, var);
				pkb.setProcUses(procName, var);
				Parent.push_back(lineCounter);
				isSameLevel = false;
				isNewContainer = true;
			} else if (isWhileStatement(nextLine)) {
				pkb.insertStatementList(lineCounter + 1);
				string var;
				if (nextLine.size() == 3) {
					var = nextLine[1];
				} else {
					var = nextLine[1].substr(0, nextLine[1].size() - 1);
				}
				pkb.addWhilePattern(lineCounter, var);
				pkb.setStatementType(lineCounter, WHILE);
				pkb.setUses(lineCounter, var);
				pkb.setProcUses(procName, var);
				Parent.push_back(lineCounter);
				isSameLevel = false;
				isNewContainer = true;
			} else if (isCallStatement(nextLine)) {
				string proc_name;
				if (nextLine[1].substr(nextLine[1].size() - 1) == SEMICOLON) {
					proc_name = nextLine[1].substr(0, nextLine[1].size() - 1);
				} else {
					proc_name = nextLine[1];
				}
				pkb.setStatementType(lineCounter, CALL);
				pkb.setCalls(lineCounter, procName, proc_name);
				isSameLevel = true;
				isNewContainer = false;
			} else {
				return false;
			}
			// after all statements, check for '}'s at the end
			for (int i = line.size() - 1; i >= 0; i--) {
				if (line[i] == CB) {
					if (isNewContainer) {
						return false;
					} else {
						isSameLevel = false;
						prevFollow = Parent.back();
						Parent.pop_back();
					}
				} else if (line[i] != SPACE && line[i] != TAB) {
					break;
				}
			}
		}
	}
	pkb.setLastline(procName, lineCounter);
	if (Parent.size() == 0) {
		pkb.createCFG();
		return true;
	} else {
		return false;
	}
}

bool isElseStatement(vector<string> line) {
	return (line.size() == 2 && line[0] == ELSE && line[1] == OB) ||
		(line.size() == 1 && line[0] == ELSE_B);
}

bool startsWithBrackets(vector<string> line) {
	return line.size() > 0 && line[0][0] == CB;
}

bool endsWithBrackets(vector<string> line, int pos) {
	for (int i = pos; i < line.size(); i++) {
		for (int j = 0; j < line[i].size(); j++) {
			if (line[i][j] != CB) {
				return false;
			}
		}
	}
	return true;
}

bool isProcedure(vector<string> line) {
	return (line.size() == 3 && line[0] == PROCEDURE && Util::isValidName(line[1]) && line[2] == OB) ||
		(line.size() == 2 && line[0] == PROCEDURE && Util::isValidName(line[1].substr(0, line[1].size() - 1)) && line[1].substr(line[1].size() - 1) == OB);
}

bool isCallStatement(vector<string> line) {
	return (line.size() >= 2 && line[0] == CALL) &&
		((line[0] == CALL && Util::isValidName(line[1].substr(0, line[1].size() - 1)) && line[1].substr(line[1].size() - 1) == SEMICOLON && endsWithBrackets(line, 2)) ||
		(line.size() >= 3 && Util::isValidName(line[1]) && line[2] == SEMICOLON && endsWithBrackets(line, 3)));
}

bool isWhileStatement(vector<string> line) {
	return (line.size() == 3 && line[0] == WHILE && Util::isValidName(line[1]) && line[2] == OB) ||
		(line.size() == 2 && line[0] == WHILE && Util::isValidName(line[1].substr(0, line[1].size() - 1)) && line[1].substr(line[1].size() - 1) == OB);
}

bool isIfStatement(vector<string> line) {
	return (line.size() == 4 && line[0] == IF && Util::isValidName(line[1]) && line[2] == THEN && line[3] == OB) ||
		(line.size() == 3 && line[0] == IF && Util::isValidName(line[1]) && line[2] == THEN_B);
}

bool isAssignStatement(string line) {
	int equal = line.find("=");
	int semicolon = line.find(";");
	string expression;
	if (equal <= 0 || semicolon <= 1 || !Util::isValidName(Util::trim(line.substr(0, equal)))) {
		return false;
	} else {
		expression = line.substr(equal + 1, semicolon - (equal + 1));
		for (int i = semicolon + 1; i < line.size(); i++) {
			if (line[i] != CB && line[i] != SPACE && line[i] != TAB) {
				return false;
			}
		}
	}
	return Util::isValidExpression(expression);
}