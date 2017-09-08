#include "Follow.h"

Follow::Follow() {
	vector<int> followsTable;
	vector<int> followedByTable;
	vector<vector<int>> followsStarTable;
	vector<vector<int>> followedByStarTable;
}

void Follow::setFollows(int s1, int s2) {
	if (s1 <= 0 || s2 <= 0) {
		throw new invalid_argument("Statement Numbers must be more than 0.");
	}
	if (s2 <= s1) {
		throw new invalid_argument("s2 must be larger than s1.");

	}
	followsTable.resize(s1 + 1);
	followsTable[s1] = s2;
	setFollowedBy(s2, s1);
	setFollowsStar(s1, s2);
}

/*
	Sets s2 to be followed by s1. Meaning: s1 > s2
*/
void Follow::setFollowedBy(int s1, int s2) {
	followedByTable.resize(s1 + 1);
	followedByTable[s1] = s2;
}

void Follow::setFollowsStar(int s1, int s2) {
	if (s1 <= 0 || s2 <= 0) {
		throw new invalid_argument("Statement Number must be more than 0.");
	}
	followsStarTable[s1].push_back(s2);
	setFollowedByStar(s2, s1);
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