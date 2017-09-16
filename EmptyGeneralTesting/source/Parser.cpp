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

bool Parse(string fileName) {

	PKB pkb;

	ifstream file(fileName);
	string line;
	string procName;

	int lineCounter = 0;
	bool isSameLevel = false;	//is same nesting level
	int nestLevel = 0;

	vector<string> firstLine = Util::splitLine(line, ' ');

	//	tells you who is the immediate parent so you can call setParent	
	std::vector<int> Parent;

	//detect procedure
	if (firstLine[0] != "procedure" && firstLine[2] == "{") {

		return false;
	}

	else {
		procName = firstLine[1]; //need to validate > does not start with digit
	}
	//end procedure

	while (getline(file, line)) { //read line by line

		lineCounter++;
		vector<string> nextLine = Util::splitLine(line, ' ');

		if (nextLine[0] == "else" && nextLine[1] == "{") { // line starts with else, no statement number

			lineCounter--;
			continue;
		}

		else {
			//assign statement
			if (nextLine[1] == "=" && nextLine.back() == ";") {
				pkb.setStatementType(lineCounter, 2);

				if (lineCounter != 1 && isSameLevel) {
					pkb.setFollows(lineCounter - 1, lineCounter);
				}

				if (nestLevel > 1) {
					pkb.setParent(Parent.back().c, lineCounter);
				}

				pkb.setModifies(lineCounter, nextLine[0]);

				for (int i = 2; i < nextLine.size(); i++) {

					if (isdigit(nextLine[i])) { //if value is a digit, add to constants table
						pkb.addConstant(nextLine[i]);
					}

					else if (isalpha(nextLine[i])) {	// compile error: no suitable conversion function from std::string to int exists
						pkb.addVariable(nextLine[i]);
						pkb.setUses(lineCounter, nextLine[i]);
						return true;
					}

					else {
						continue;
					}

				}

				isSameLevel = true;
			}
			//end of assign statement

			//while, if, closing statements
			else {
				//if
				if (nextLine[0] == "if" && nextLine[2] == "then") {

					pkb.setStatementType(lineCounter, 3);

					if (isSameLevel) {
						pkb.setFollows(lineCounter - 1, lineCounter);
					}

					if (nestLevel > 1) {
						pkb.setParent(Parent.back().c, lineCounter);
					}

					nestLevel++;
					isSameLevel = false;
					//deal with else statements

				}
				//while
				else if (nextLine[0] == "while" && nextLine[2] == "{") {

					pkb.setStatementType(lineCounter, 1);

					if (isSameLevel) {
						pkb.setFollows(lineCounter - 1, lineCounter);
					}

					if (nestLevel > 1) {
						pkb.setParent(Parent.back().c, lineCounter);
					}

					Parent.push_back(lineCounter);

					nestLevel++;
					isSameLevel = false;
				}

				else {
					return false;
				}
			}
		}
		// after all statements, check for '}'s at the end
		for (int i = 0; i < nextLine.size(); i++) {
			if (nextLine[i] == "}") {
				if (isSameLevel) {
					pkb.setFollows(lineCounter - 1, lineCounter);
				}
				if (nestLevel > 1) {
					pkb.setParent(Parent.back().c, lineCounter);
				}

				isSameLevel = false;
				nestLevel--;
			}

			else {
				continue;
			}
		}
	}

	if (nestLevel == 0) {
		return true;
	}

	else {
		return false;
	}
}