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
	vector<int> Parent;

	while (getline(file, line)) { //read line by line
		line = Util::trim(line); //remove leading and trailing whitespaces
		lineCounter++;
		vector<string> nextLine = Util::splitLine(line, ' '); //split the line by spaces
		if (nextLine.size() == 0) {//empty line
			lineCounter--;
		} else if (isElseStatement(nextLine)) {
			pkb.insertElse(lineCounter);
			lineCounter--;
			Parent.push_back(prevFollow);
			isSameLevel = false; //start of a new container
			isNewContainer = true; //start of a new container
		} else if (isProcedure(nextLine) && Parent.size() == 0) {
			pkb.insertStatementList(lineCounter);
			lineCounter--;
			if (lineCounter > 1) {//first procedure do not have a previous procedure
				pkb.setLastline(procName, lineCounter); //set last line for the previous procedure
			}
			if (nextLine.size() == 3) {
				procName = nextLine[1];// spaces in between the procedure name and opening bracket
			} else {
				procName = nextLine[1].substr(0, nextLine[1].size() - 1);// no spaces in between the procedure name and opening bracket
			}
			Parent.push_back(0);// include a dummy parent to indicate the start of a procedure
			isNewContainer = true;
			isSameLevel = false;
		} else if (startsWithBrackets(nextLine) && !isNewContainer) {//make sure that there are statements in the container
			int i = 0;
			while (i < line.size()) {
				if (line[i] == CB) {
					isSameLevel = false;
					prevFollow = Parent.back();//closing brackets remove the immediate parent
					Parent.pop_back();
				} else if (line[i] != SPACE && line[i] != TAB) {
					break;
				}
				i++;
			}
			if (i < line.size()) {
				line = line.substr(i, line.size());
				if (!isElseStatement(Util::splitLine(line, ' '))) {// check if there is an else at the end of the line
					return false;// can only be else, so something is wrong
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
				if (isSameLevel) {//is not the start of a container and has the same nesting level as the previous statement
					pkb.setFollows(lineCounter - 1, lineCounter);
				} else {// is not the start of a container but is on a different nesting level as the previous statement
					pkb.setFollows(prevFollow, lineCounter);
				}
			}
			pkb.setProcedure(lineCounter, procName);
			if (Parent.size() > 1) {// if it is in a contrainer of a while or if
				pkb.setParent(Parent.back(), lineCounter);
			}
			if (isAssignStatement(line)) {
				int pos = line.find("=");
				string var = Util::trim(line.substr(0, pos));//obtain the right variable
				pkb.setStatementType(lineCounter, ASSIGN);
				pkb.setModifies(lineCounter, var);
				pkb.setProcModifies(procName, var);
				string expression = Util::getExpression(line.substr(pos + 1));//obtain the left expression
				vector<string> expressionList = Util::constructExpression(expression);//split the expression based on operators
				for each (string item in expressionList) {
					if (Util::isNumber(item)) {//not a variable
						pkb.addConstant(item);
					} else if (Util::isValidName(item)) {//variable
						pkb.setUses(lineCounter, item);
						pkb.setProcUses(procName, item);
					} else {
						return false;//something else, which is wrong
					}
				}
				pkb.addAssignPattern(lineCounter, var, expression);
				isNewContainer = false;
				isSameLevel = true;
			} else if (isIfStatement(nextLine)) {
				pkb.setStatementType(lineCounter, IF);
				string var = nextLine[1];// control variable
				pkb.addIfPattern(lineCounter, var);
				pkb.setUses(lineCounter, var);
				pkb.setProcUses(procName, var);
				Parent.push_back(lineCounter);
				isSameLevel = false;
				isNewContainer = true;
			} else if (isWhileStatement(nextLine)) {
				string var;
				if (nextLine.size() == 3) {
					var = nextLine[1];//spaces between control variable and opening bracket
				} else {
					var = nextLine[1].substr(0, nextLine[1].size() - 1);//no spaces between control variable and opening bracket
				}
				pkb.addWhilePattern(lineCounter, var);
				pkb.setStatementType(lineCounter, WHILE);
				pkb.setUses(lineCounter, var);
				pkb.setProcUses(procName, var);
				Parent.push_back(lineCounter);
				isSameLevel = false;
				isNewContainer = true;
			} else if (isCallStatement(nextLine)) {
				string proc_name = nextLine[1].substr(0, nextLine[1].find(SEMICOLON));//procedure being called
				pkb.setStatementType(lineCounter, CALL);
				pkb.setCalls(lineCounter, procName, proc_name);
				isSameLevel = true;
				isNewContainer = false;
			} else {
				return false;//nothing matched so unknown statement type
			}
			// after all statements, check for '}'s at the end
			for (int i = line.size() - 1; i >= 0; i--) {
				if (line[i] == CB) {
					if (isNewContainer) {
						return false;//not supposed to have empty containers
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
	pkb.setLastline(procName, lineCounter);//to set the last line of the last procedure
	if (Parent.size() == 0) {//all opening brackets should be closed
		pkb.createCFG();//create next information
		return true;
	} else {
		return false;
	}
}

bool isElseStatement(vector<string> line) {
	return (line.size() == 2 && line[0] == ELSE && line[1] == OB) ||//"else {" - space
		(line.size() == 1 && line[0] == ELSE_B);//"else{" - no space
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
	return (line.size() == 3 && line[0] == PROCEDURE && Util::isValidName(line[1]) && line[2] == OB) ||//"procedure name {" space
		(line.size() == 2 && line[0] == PROCEDURE && Util::isValidName(line[1].substr(0, line[1].size() - 1)) && line[1].substr(line[1].size() - 1) == OB);//"procedure name{" no space
}

bool isCallStatement(vector<string> line) {
	if (line.size() < 2) {
		return false;
	} else if (line[0] != CALL) {//first word must be call
		return false;
	} else if (line.size() == 2) {
		return Util::isValidName(Util::splitLine(line[1], ';')[0]) && line[1].find(SEMICOLON) != string::npos;//"call procedure;" no space
	} else {
		return Util::isValidName(Util::splitLine(line[1], ';')[0]) && (line[1].find(SEMICOLON) != string::npos || line[2].find(SEMICOLON) != string::npos);//"call procedure ;" space
	}
}

bool isWhileStatement(vector<string> line) {
	return (line.size() == 3 && line[0] == WHILE && Util::isValidName(line[1]) && line[2] == OB) ||//"while var {" space
		(line.size() == 2 && line[0] == WHILE && Util::isValidName(line[1].substr(0, line[1].size() - 1)) && line[1].substr(line[1].size() - 1) == OB);//"while var{" no space
}

bool isIfStatement(vector<string> line) {
	return (line.size() == 4 && line[0] == IF && Util::isValidName(line[1]) && line[2] == THEN && line[3] == OB) ||//"if var then {" space
		(line.size() == 3 && line[0] == IF && Util::isValidName(line[1]) && line[2] == THEN_B);//"if var then{" no space
}

bool isAssignStatement(string line) {
	int equal = line.find("=");
	int semicolon = line.find(";");
	string expression;
	if (equal <= 0 || semicolon <= 1 || !Util::isValidName(Util::trim(line.substr(0, equal)))) {//must have equal and semicolon and var name must be valid
		return false;
	} else {
		expression = line.substr(equal + 1, semicolon - (equal + 1));//left expression
		for (int i = semicolon + 1; i < line.size(); i++) {//check for after semicolon, can only be whitespaces or closing brackets
			if (line[i] != CB && line[i] != SPACE && line[i] != TAB) {
				return false;
			}
		}
	}
	return Util::isValidExpression(expression);//check if the expression is valid
}