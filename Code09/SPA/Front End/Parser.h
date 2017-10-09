#pragma once

#include "PKB.h"
#include <iostream>

using namespace std;
bool Parse(string filename, PKB& pkb);
bool isElseStatement(vector<string> line);
bool startsWithBrackets(vector<string> line);
bool endsWithBrackets(vector<string> line, int pos);
bool isProcedure(vector<string> line);
bool isCallStatement(vector<string> line);
bool isWhileStatement(vector<string> line);
bool isIfStatement(vector<string> line);
bool isAssignStatement(string line);