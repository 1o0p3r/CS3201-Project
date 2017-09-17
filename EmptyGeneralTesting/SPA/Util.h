#pragma once
#include <string>
#include <vector>
using namespace std;

class Util {
public:
	static string insertBrackets(string input);
	static vector<string> splitLine(string s, char delim);
};