#include "Util.h"
#include <iostream>
#include <algorithm>
#include <tuple>
#include <set>

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
			if (results[index - 1] != ')' && results[index + 1] != '(') {
				results.insert(index + 2, ")");
				results.insert(index - 1, "(");
			} else if (results[index + 1] != '(') {
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
			} else if (results[index - 1] != ')') {
				int counter = 1;
				int front = index + 1;
				while (counter > 0) {
					front++;
					if (results[front] == '(') {
						counter++;
					} else if (results[front] == ')') {
						counter--;
					}
				}
				results.insert(front, ")");
				results.insert(index - 1, "(");
			} else {
				int counter = 1;
				int front = index + 1;
				int back = index - 1;
				while (counter > 0) {
					front++;
					if (results[front] == '(') {
						counter++;
					} else if (results[front] == ')') {
						counter--;
					}
				}
				counter = 1;
				while (counter > 0) {
					back--;
					if (results[back] == '(') {
						counter--;
					} else if (results[back] == ')') {
						counter++;
					}
				}
				results.insert(front, ")");
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
			} else if (results[index + 1] != '(') {
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
			} else if (results[index - 1] != ')') {
				int counter = 1;
				int front = index + 1;
				while (counter > 0) {
					front++;
					if (results[front] == ')') {
						counter--;
					} else if (results[front] == '(') {
						counter++;
					}
				}
				results.insert(front, ")");
				results.insert(index - 1, "(");
			} else {
				int counter = 1;
				int back = index - 1;
				int front = index + 1;
				while (counter > 0) {
					back--;
					if (results[back] == '(') {
						counter--;
					} else if (results[back] == ')') {
						counter++;
					}
				}
				counter = 1;
				while (counter > 0) {
					front++;
					if (results[front] == ')') {
						counter--;
					} else if (results[front] == '(') {
						counter++;
					}
				}
				results.insert(front, ")");
				results.insert(back, "(");
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

bool Util::isOperand(char c) {
	if (c == '+' || c == '-' || c == '*' || c == '(' || c == ')') {
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

string Util::getExpression(string line) {
	int back = line.size() - 1;
	while (line[back] != ';') {
		back--;
	}
	string result = "";
	for (int i = 0; i < back; i++) {
		if (line[i] != ' ' && line[i]  != '\t') {
			result.push_back(line[i]);
		}
	}
	return result;
}

tuple<string, string> Util::extractBackBrackets(string s) {
	int bracket = 0;
	while (s[bracket] != '{') {
		bracket++;
	}
	string name = s.substr(0, bracket);
	string open = s.substr(bracket, s.size());
	tuple<string, string> result = { name, open };
	return result;
}

vector<int> Util::removeDuplicates(vector<int> input) {
	set<int> temp;
	vector<int> result;
	for each (int item in input) {
		temp.insert(item);
	}
	for each (int item in temp) {
		result.push_back(item);
	}
	return result;
}

vector<string> Util::removeDuplicates(vector<string> input) {
	set<string> temp;
	vector<string> result;
	for each (string item in input) {
		temp.insert(item);
	}
	for each (string item in temp) {
		result.push_back(item);
	}
	return result;
}

vector<string> Util::constructExpression(string expression) {
	vector<string> result;
	string current = "";
	for (int i = 0; i < expression.size(); i++) {
		if (Util::isOperand(expression[i])) {
			if (!current.empty()) {
				result.push_back(current);
			}
			current = "";
		} else {
			current.push_back(expression[i]);
		}
	}
	if (!current.empty()) {
		result.push_back(current);
	}
	return result;
}

bool Util::isValidExpression(string expression) {
	bool expectOperandsNext = false;
	int bracketCounter = 0;
	bool expectBrackets = true;
	for (int i = 0; i < expression.size(); i++) {
		if (isdigit(expression[i]) && !expectOperandsNext) {
			while (i < expression.size() && isdigit(expression[i + 1])) {
				i++;
			}
			expectOperandsNext = true;
			expectBrackets = true;
		} else if (isalpha(expression[i]) && !expectOperandsNext) {
			while (i < expression.size() && isalnum(expression[i + 1])) {
				i++;
			}
			expectOperandsNext = true;
			expectBrackets = true;
		} else if (expression[i] == '(') {
			bracketCounter++;
			expectBrackets = false;
		} else if (expression[i] == ')' && bracketCounter > 0 && expectBrackets) {
			bracketCounter--;
			expectBrackets = true;
		} else if (expectOperandsNext && Util::isOperand(expression[i])) {
			expectOperandsNext = false;
			expectBrackets = true;
		} else if (expression[i] != ' ' && expression[i] != '\t') {
			return false;
		}
	}
	return bracketCounter == 0 && expectOperandsNext;
}