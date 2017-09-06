#include "Modify.h"
#include <string>

using namespace std;

Modify::Modify() {
  vector<string> modifyTableVar;
	vector<vector<string>> modifiesTable; //stmt modifies var or container stmt modifes vars
	vector<vector<int*>> modifiedByTable; //var modifiedBy stmts
  vector<vector<string>> procModifiesTable; //proc modifes vars
  vector<vector<string>> procModifiedByTable; //proc modified by vars
}

int Modify::ModifyTable(string varName) { //creates var index, or creates one if it doesn't exist
  if modifyTableVar.contains(varName) {
    return vectorPositionWithVarName //edit
  }
  else {
    modifyVarTable.push_back(varName);
    return vectorPositionWithVarName //edit
  } 
}

void Modify::setModifies(int s, string varName) {
	if (ModifiesTable.size() < s) {
		ModifiesTable.reserve(s + 1);
		ModifiesTable[s] = varName;
	} else {
		throw invalid_argument("Statement is already in PKB");
	}
}

void Modify::setModifiedBy(String varName, int s) {
  int indexedVarName = modifyTable(varName);
	if (modifiedByByTable[indexedVarName].contains(s)) {
		followedByTable.reserve(s1 + 1);
		followedByTable[s1] = s2;
	} else {
		throw invalid_argument("Statement is already in PKB");
	}
}

void Follow::setFollowsStar(int s1, int s2) {
	followsStarTable[s1].push_back(s2);
}

void Follow::setFollowedByStar(int s1, int s2) {
	followedByStarTable[s1].push_back(s2);
}

vector<int> Follow::getFollows(int statementNum) {
	vector<int> result(1);
	result[0] = followsTable[statementNum];
	return result;
}

vector<int> Follow::getFollowedBy(int statementNum) {
	vector<int> result(1);
	result[0] = followedByTable[statementNum];
	return result;
}

vector<int> Follow::getFollowsStar(int statementNum) {
	return followsStarTable[statementNum];
}

vector<int> Follow::getFollowedByStar(int statementNum) {
	return followedByStarTable[statementNum];
}
