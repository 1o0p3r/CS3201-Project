#include "Util.h"
#include <iostream>

string Util::insertBrackets(string results) {
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
		tokens.push_back(item);
	}
	return tokens;
}