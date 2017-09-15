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
	bool isSameLevel = false;

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
		
		if (nextLine[0] == "else") { // line starts with else, no statement number
			
			lineCounter--;
			continue;
		}

		else {
			
			if (nextLine[1] == "=") { //assign statements
				
				pkb.setStatementType(lineCounter, 2);
				isSameLevel = true;
				
				if (lineCounter != 1) {			
					pkb.setFollows(lineCounter - 1, lineCounter);
				}

				for (int i = 2; i < nextLine.size(); i++) {

					if (isdigit(nextLine[i])) //if value is a digit, add to constants table
						return false;
				}
			} //end of assign statemnt
			return false;
		}
	}

	return false;
}