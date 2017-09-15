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

	if (firstLine[0] != "procedure" && firstLine[2] == "{") {

		return false;
	}

	else {

		procName = firstLine[1]; //need to validate > does not start with digit
	}

	while (getline(file, line)) { //read line by line

		lineCounter++;
		vector<string> nextLine = Util::splitLine(line, ' ');

		if (nextLine[0] == "else" && nextLine[1] == "{") { // line starts with else, no statement number

			lineCounter--;
			continue;

//		closing: if (nextLine.back() == "}")

		}

		else {

			if (nextLine[1] == "=" && nextLine.back() == ";") { //assign statements
				pkb.setStatementType(lineCounter, 2);

				if (lineCounter != 1 && isSameLevel) {
					pkb.setFollows(lineCounter - 1, lineCounter);
				}

				pkb.setModifies(lineCounter, nextLine[0]);

				for (int i = 2; i < nextLine.size(); i++) {

					if (isdigit(nextLine[i])) { //if value is a digit, add to constants table
												//code to add to constants table ??
						return false;
					}

					else if (isalpha(nextLine[i]) != 0) {	// compile error: no suitable conversion function from std::string to int exists
						pkb.setUses(lineCounter, nextLine[i]);
						return true;
					}

					else {
						return false;
					}

				}
				isSameLevel = true;

			} //end of assign statement

			else {	//while, if statements

				if (nextLine[0] == "if" && nextLine[2] == "then") {
					nestLevel++;
					pkb.setStatementType(lineCounter, 3);
					isSameLevel = true;

				}

				else if (nextLine[0] == "while" && nextLine[2] == "{") {
					nestLevel++;
					pkb.setStatementType(lineCounter, 1);
					isSameLevel = true;


				}

			}

		}
	}

	return false;
}