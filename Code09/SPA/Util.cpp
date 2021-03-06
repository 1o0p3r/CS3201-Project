#include "Util.h"
#include <iostream>
#include <algorithm>
#include <tuple>
#include <set>
#include <regex>

string Util::insertBrackets(string input) {
	string results = input;
	int index = 0;
	while (index < results.size()) {
		if (results[index] == '*') {
			int front;
			int back;
			if (results[index - 1] != ')' && results[index + 1] != '(') {
				front = index + 2;
				back = index - 1;
			} else if (results[index + 1] != '(') {
				int counter = 1;
				back = index - 1;
				while (counter > 0) {
					back--;
					if (results[back] == '(') {
						counter--;
					} else if (results[back] == ')') {
						counter++;
					}
				}
				front = index + 2;
			} else if (results[index - 1] != ')') {
				int counter = 1;
				front = index + 1;
				while (counter > 0) {
					front++;
					if (results[front] == '(') {
						counter++;
					} else if (results[front] == ')') {
						counter--;
					}
				}
				back = index - 1;
			} else {
				int counter = 1;
				front = index + 1;
				back = index - 1;
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
			}
			while (front < results.size() && isalnum(results[front])) {
				front++;
			}
			while (back > 0 && isalnum(results[back - 1])) {
				back--;
			}
			results.insert(front, ")");
			results.insert(back, "(");
			index++;
		}
		index++;
	}
	index = 0;
	while (index < results.size()) {
		if (results[index] == '+' || results[index] == '-') {
			int front;
			int back;
			if (results[index - 1] != ')' && results[index + 1] != '(') {
				front = index + 2;
				back = index - 1;
			} else if (results[index + 1] != '(') {
				int counter = 1;
				back = index - 1;
				while (counter > 0) {
					back--;
					if (results[back] == '(') {
						counter--;
					} else if (results[back] == ')') {
						counter++;
					}
				}
				front = index + 2;
			} else if (results[index - 1] != ')') {
				int counter = 1;
				front = index + 1;
				while (counter > 0) {
					front++;
					if (results[front] == ')') {
						counter--;
					} else if (results[front] == '(') {
						counter++;
					}
				}
				back = index - 1;
			} else {
				int counter = 1;
				back = index - 1;
				front = index + 1;
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
			}
			while (front < results.size() && isalnum(results[front])) {
				front++;
			}
			while (back > 0 && isalnum(results[back - 1])) {
				back--;
			}
			results.insert(front, ")");
			results.insert(back, "(");
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

int Util::getOperandPrec(char c){
	if (c == '*') return 2;
	else if (c == '+' || c == '-') return 1;
	else return 0;
}

string Util::removeSpace(string line) {
	const auto endpos = remove(line.begin(), line.end(), ' ');
	line.erase(endpos, line.end());
	return line;
}

bool Util::isEndOfWord(const char& c) {
	if (c == '-') return true;
	if (c == '+') return true;
	if (c == '*') return true;
	if (c == ')') return true;
	return false;
}

vector<string> Util::convertIntToString(const vector<int> &vint) {
	vector<string> result;
	for (auto const element : vint) {
		result.push_back(to_string(element));
	}
	return result;
}

vector<int> Util::convertStringToInt(const vector<string>& vstring) {
	vector<int> result;
	for (auto const element : vstring) {
		result.push_back(stoi(element));
	}
	return result;
}


//regex to remove leading,trailing,extra spaces


//code adapted from csegeek
string Util::getPostFixExp(string line) {
	line = removeSpace(line);
	stack<char> s;
	string postfix;
	int size = line.length();
	int weight;
	int i = 0;
	char ch;
	bool isNewWord = true;
	// iterate over the line expression   
	while (i < size) {
		ch = line[i];
		if (ch == '(') {
			// simply push the opening parenthesis
//			postfix += ' '; 
			s.push(ch);
			i++;
			continue;
		}
		else if (ch == ')') {

			//to capture end of a word
			if (!isNewWord)
			{
				postfix += ')';
				isNewWord = true;
	//			postfix += ' ';
			}

			// if we see a closing parenthesis,
			// pop of all the elements and append it to
			// the postfix expression till we encounter
			// a opening parenthesis
			while (!s.empty() && s.top() != '(') {
				postfix += s.top();	
				s.pop();
			}
			// pop off the opening parenthesis also
			if (!s.empty()) {
		//		postfix += ' ';
				s.pop();
			}
			i++;
			continue;
		}
		weight = getOperandPrec(ch);
		if (weight == 0) {
			// we saw an operand
			// simply append it to postfix expression

			if(isNewWord)
			{
				isNewWord = false;
				postfix += '(';
			}
			postfix += ch;
		}
		else {
			// we saw an operator
			
			if (!isNewWord) {
				postfix += ')';
			//	postfix += ' ';
				isNewWord = true;
			}

			if (s.empty()) {
				// simply push the operator onto stack if
				// stack is empty
				s.push(ch);
			}
			else {
				// pop of all the operators from the stack and
				// append it to the postfix expression till we
				// see an operator with a lower precedence that
				// the current operator

				while (!s.empty() && s.top() != '(' &&
					weight <= getOperandPrec(s.top())) {		

					postfix += s.top();
				//	postfix += ' '; //spacing between operators
					s.pop();
					
				}
				//capture end of a word
				
				
					
				
			//	postfix += ' ';
				// push the current operator onto stack
				
				s.push(ch);
			}
		}
		i++;
	}
	// pop of the remaining operators present in the stack
	// and append it to postfix expression 
	if (!isNewWord) {
		postfix += ')';
		isNewWord = true;
	}
	while (!s.empty()) {
		
	//	postfix += ' ';
		postfix += s.top();
		
		s.pop();
	}
	return postfix;
	// return trimLead(postfix);
}

//code adapted from stackoverflow, remove extra spaces
string Util::trimLead(const string& str)
{
	string output;
	unique_copy(str.begin(), str.end(), back_insert_iterator<string>(output),
		[](char a, char b) { return isspace(a) && isspace(b); });
	return output;
}

bool Util::contains(vector<int> list, int i) {
	for each (int j in list) {
		if (i == j) {
			return true;
		}
	}
	return false;
}

vector<int> Util::getIntersection(vector<int> v1, vector<int> v2) {
	vector<int> result;
	sort(v1.begin(), v1.end());
	sort(v2.begin(), v2.end());
	set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(result));
	return result;
}