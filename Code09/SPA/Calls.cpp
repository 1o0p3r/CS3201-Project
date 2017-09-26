#include "Calls.h"
#include <set>
#include <string>
#include <algorithm>

using namespace std;

Calls::Calls() {

	vector<vector<int>> callsTable; 
	vector<vector<int>> calledByTable;
	vector<vector<int>> callsStarTable;
	vector<vector<int>> calledByStarTable;

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
	setCalledBy(procName2, procName1);
	setCallsStar(procName1);
	setCalledByStar(procName1);

	checkIfRecursive;
}

void Calls::setCalledBy(int procName2, int procName1) {

	if (calledByTable.size() <= procName2) {
		calledByTable.resize(procName2 + 1);
	}
	calledByTable[procName2].push_back(procName1);
}

void Calls::setCallsStar(int procName) {

	vector<int> procCalledBy = getCalledBy(procName);
	callsTable[procName].insert(callsTable[procName].begin(), procCalledBy.begin(), procCalledBy.end());
}

void Calls::setCalledByStar(int procName) {

	vector<int> procCalls = getCalls(procName);
	calledByTable[procName].insert(calledByTable[procName].begin(), procCalls.begin(), procCalls.end());
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

void Calls::checkIfRecursive() {

	set<int> setTable;

	for (int i = 0; i < callsTable.size(); i++) {
		setTable.insert(callsTable[i].begin(), callsTable[i].end());
		
		if (callsTable.size() == setTable.size()) {
			isRecursive = false;
		}
		else {
			isRecursive = true;
			throw new invalid_argument("Procedure calls itself. Program is recursive");
		}
		setTable.clear();
	}

	for (int i = 0; i < calledByTable.size(); i++) {
		setTable.insert(calledByTable[i].begin(), calledByTable[i].end());

		if (calledByTable.size() == setTable.size()) {
			isRecursive = false;
		}
		else {
			isRecursive = true;
			throw new invalid_argument("Procedure calls itself. Program is recursive");
		}
		setTable.clear();
	}

	for (int i = 0; i < callsStarTable.size(); i++) {
		setTable.insert(callsStarTable[i].begin(), callsStarTable[i].end());

		if (callsStarTable.size() == setTable.size()) {
			isRecursive = false;
		}
		else {
			isRecursive = true;
			throw new invalid_argument("Procedure calls itself. Program is recursive");
		}
		setTable.clear();
	}

	for (int i = 0; i < calledByStarTable.size(); i++) {
		setTable.insert(calledByStarTable[i].begin(), calledByStarTable[i].end());

		if (calledByStarTable.size() == setTable.size()) {
			isRecursive = false;
		}
		else {
			isRecursive = true;
			throw new invalid_argument("Procedure calls itself. Program is recursive");
		}
		setTable.clear();
	}
}