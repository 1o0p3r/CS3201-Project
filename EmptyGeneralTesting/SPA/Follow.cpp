#include "Follow.h"

Follow::Follow() {
	vector<int> followsTable;
	vector<int> followedByTable;
	vector<vector<int>> followsStarTable;
	vector<vector<int>> followedByStarTable;
}

void Follow::setFollows(int s1, int s2) {
	if (followsTable.size() < s1) {
		followsTable.reserve(s1 + 1);
		followsTable[s1] = s2;
	} else {
		throw invalid_argument("Statement is already in PKB");
	}
}

void Follow::setFollowedBy(int s1, int s2) {
	if (followedByTable.size() < s1) {
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