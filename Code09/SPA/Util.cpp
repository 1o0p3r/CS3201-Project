#include "Util.h"
#include <iostream>
#include <algorithm>
#include <tuple>

string Util::insertBrackets(string input) {
	string results;
	for each (char c in input) {
		if (!isspace(c)) {
			results.push_back(c);
		}
	}
	//string results = input;
	int index = 0;
	while (index < results.size()) {
		if (results[index] == '*') {
			if (results[index - 1] != ')') {
				results.insert(index + 2, ")");
				results.insert(index - 1, "(");
			} else {
				int counter = 1;
				int back = index - 1;
				while (counter > 0) {
					back--;
					if (results[back] == '(') {
						counter--;
					} else if (results[back] == ')') {
						counter++;
					}
				}
				results.insert(index + 2, ")");
				results.insert(back, "(");
			}
			index++;
		}
		index++;
	}
	index = 0;
	while (index < results.size()) {
		if (results[index] == '+' || results[index] == '-') {
			if (results[index - 1] != ')' && results[index + 1] != '(') {
				results.insert(index + 2, ")");
				results.insert(index - 1, "(");
			} else if (results[index - 1] == ')' && results[index + 1] != '(') {
				int counter = 1;
				int back = index - 2;
				while (counter > 0) {
					if (results[back] == '(') {
						counter--;
					} else if (results[back] == ')') {
						counter++;
					}
					back--;
				}
				results.insert(index + 2, ")");
				results.insert(back + 1, "(");
			} else if (results[index + 1] == '(' && results[index - 1] != ')') {
				int counter = 1;
				int front = index + 2;
				while (counter > 0) {
					if (results[front] == ')') {
						counter--;
					} else if (results[front] == '(') {
						counter++;
					}
					front++;
				}
				results.insert(front - 1, ")");
				results.insert(index - 1, "(");
			} else {
				int counter = 1;
				int back = index - 2;
				int front = index + 2;
				while (counter > 0) {
					if (results[back] == '(') {
						counter--;
					} else if (results[back] == ')') {
						counter++;
					}
					back--;
				}
				counter = 1;
				while (counter > 0) {
					if (results[front] == ')') {
						counter--;
					} else if (results[front] == '(') {
						counter++;
					}
					front++;
				}
				results.insert(front - 1, ")");
				results.insert(back + 1, "(");
			}
			index++;
		}
		index++;
	}
	return results;
}

vector<string> Util::splitLine(string s, char delim) {
	stringstream ss(s);
	string item;
	vector<string> tokens;
	while (getline(ss, item, delim)) {
		if (item.size() != 0) {
			tokens.push_back(item);
		}
	}
	return tokens;
}

static inline bool isNotAlNum(char c) {
	return !isalnum(c);
}

bool Util::isValidName(string s) {
	if (isalpha(s[0])) {
		return find_if(s.begin(), s.end(), isNotAlNum) == s.end();
	} else {
		return false;
	}
}

bool Util::isOperand(string s) {
	if (s == "+" || s == "-" || s == "*") {
		return true;
	} else {
		return false;
	}
}

static inline bool isNotDigit(char c) {
	return !isdigit(c);
}

bool Util::isNumber(string s) {
	return find_if(s.begin(), s.end(), isNotDigit) == s.end();
}

string Util::trim(string s) {
	size_t p = s.find_first_not_of(" \t");
	s.erase(0, p);
	p = s.find_last_not_of(" \t");
	if (string::npos != p) {
		s.erase(p + 1);
	}
	return s;
}

string Util::getExpression(vector<string> sList) {
	int front = 0;
	int back = sList.size() - 1;
	while (sList[front] != "=") {
		front++;
	}
	while (sList[back].back() != ';') {
		back--;
	}
	front++;
	if (sList[back].size() == 1) {
		back--;
	} else {
		sList[back] = sList[back].substr(0, sList[back].size() - 1);
	}
	string s = "";
	while (front <= back) {
		s.append(sList[front]);
		front++;
	}
	return s;
}

tuple<string, string> Util::extractBrackets(string s) {
	int bracket = 0;
	while (s[bracket] != '{') {
		bracket++;
	}
	string name = s.substr(0, bracket);
	string open = s.substr(bracket, s.size());
	tuple<string, string> result = { name, open };
	return result;
}