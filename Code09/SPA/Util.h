#pragma once
#include <string>
#include <vector>
#include <sstream>
using namespace std;

class Util {
public:
	static string insertBrackets(string input);
	static vector<string> splitLine(string s, char delim);
	static bool isValidName(string s);
	static bool isOperand(string c);
	static bool isNumber(string s);
	static string trim(string s);
	static string getExpression(vector<string> sList);
};