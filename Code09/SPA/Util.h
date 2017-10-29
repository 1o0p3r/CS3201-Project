#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <stack>
#include <algorithm>
#include <cctype>
#include <locale>

using namespace std;

class Util {
public:
	static string insertBrackets(string input);
	static vector<string> splitLine(string s, char delim);
	static bool isValidName(string s);
	static bool isOperand(char c);
	static bool isNumber(string s);
	static string trim(string s);
	static string getExpression(string line);
	static tuple<string, string> extractBackBrackets(string s);
	static vector<int> removeDuplicates(vector<int> input);
	static vector<string> removeDuplicates(vector<string> input);
	static vector<string> constructExpression(string expression);
	static bool isValidExpression(string expression);
	static string getPostFixExp(string line);
	static string trimLead(const string& str);
	static int getOperandPrec(char c);
	static string removeSpace(string line);

};