#pragma once

#include <stdio.h>
#include <Util.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

#include "PKB.h"
#include "Parser.h"



bool Parse(string fileName) {

	ifstream file(fileName);
	string line;
	string procName;

	int lineCounter = 0;
	bool isSameLevel = false;

	vector<string> firstLine = Util::splitLine(line, ' ');

	if (firstLine[0] != "procedure" && firstLine[3] == "{") {
		return false;
	}

	else {
		procName = firstLine[1];
	}

	while (getline(file, line)) {
		lineCounter++;
		vector<string> nextLine = Util::splitLine(line, ' ');
		
		if (nextLine[0] == "}") {
			if (nextLine[1] == "else") {
				//more
			 }
			lineCounter--;
		}

		else {
			if (isSameLevel)

		}
	}

	return true;
}