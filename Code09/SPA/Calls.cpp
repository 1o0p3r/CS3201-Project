#include "Calls.h"
#include <set>
#include <algorithm>

using namespace std;

Calls::Calls() {

	vector<vector<int>> callsTable; 
	vector<vector<int>> calledByTable;
	vector<vector<int>> callsStarTable;
	vector<vector<int>> calledByStarTable;
	set<int> allCallsTable;

	bool isRecursive = false;
}

void Calls::setCalls(int procName1, int procName2) {

	if (procName1 == procName2) {

		throw new invalid_argument("Procedure calls itself. Program is recursive");
	}

	if (callsTable.size() <= procName1) {
		callsTable.resize(procName1 + 1);
	}
	
	callsTable[procName1].push_back(procName2);
	setCalledBy(procName1, procName2);
	setCallsStar(procName1, procName2);
	setCalledByStar(procName1, procName2);
	allCallsTable.insert(procName2);

	checkIfRecursive();
}

void Calls::setCalledBy(int procName1, int procName2) {

	if (calledByTable.size() <= procName2) {
		calledByTable.resize(procName2 + 1);
	}
	calledByTable[procName2].push_back(procName1);
}

void Calls::setCallsStar(int procName1, int procName2) {

	if (callsStarTable.size() <= procName1) {
		callsStarTable.resize(procName1 + 1);
	}

	callsStarTable[procName1].push_back(procName2);
	vector<int> procCalledBy = getCalledBy(procName1);

	for (int i = 0; i < procCalledBy.size(); i++) {
		callsStarTable[procCalledBy[i]].push_back(procName2);
	}
}

void Calls::setCalledByStar(int procName1, int procName2) {

	if (calledByStarTable.size() <= procName2) {
		calledByStarTable.resize(procName2 + 1);
	}

	calledByStarTable[procName2].push_back(procName1);
	vector<int> procCalledBy = getCalledBy(procName1);

	for (int i = 0; i < procCalledBy.size(); i++) {
		calledByStarTable[procName2].push_back(procCalledBy[i]);
	}
}

vector<int> Calls::getCalls(int procName) {

	if (callsTable.size() > procName) {
		vector<int> result = callsTable[procName];
		return result;
	}
	else {
		return vector<int>();
	}
}

vector<int> Calls::getCalledBy(int procName) {

	if (calledByTable.size() > procName) {
		vector<int> result = calledByTable[procName];
		return result;
	}
	else {
		return vector<int>();
	}
}

vector<int> Calls::getCallsStar(int procName) {

	if (callsStarTable.size() > procName) {
		vector<int> result = callsStarTable[procName];
		return result;
	}
	else {
		return vector<int>();
	}
}

vector<int> Calls::getCalledByStar(int procName) {

	if (calledByStarTable.size() > procName) {
		vector<int> result = calledByStarTable[procName];
		return result;
	}
	else {
		return vector<int>();
	}
}

set<int> Calls::getAllCalls() {

	return allCallsTable;
}

void Calls::checkIfRecursive() {

	for (int i = 0; i < callsStarTable.size(); i++) {
		if (find(callsStarTable[i].begin(), callsStarTable[i].end(), i) != callsStarTable[i].end()) {

			isRecursive = true;
			throw new invalid_argument("Procedure calls itself. Program is recursive");
		}

		else {
			
			isRecursive = false;
		}
	}
}