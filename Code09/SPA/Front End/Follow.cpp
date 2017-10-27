#include "Follow.h"

Follow::Follow() {
	vector<int> followsTable;
	vector<int> followedByTable;
	vector<vector<int>> followsStarTable;
	vector<vector<int>> followedByStarTable;
	int followsCount = 0;
	int followStarCount = 0;
}

void Follow::setFollows(int s1, int s2) {
	if (s1 <= 0 || s2 <= 0) {
		throw new invalid_argument("Statement Numbers must be more than 0.");
	}
	if (s2 <= s1) {
		throw new invalid_argument("s2 must be larger than s1.");

	}
	if (followsTable.size() <= s1) {
		followsTable.resize(s1 + 1);
	}
	followsTable[s1] = s2;
	followsCount++;
	setFollowedBy(s2, s1);
	setFollowsStar(s1, s2);
	vector<int> holder = getFollowedBy(s1);
	while (!holder.empty()) {
		int follower = holder[0];
		setFollowsStar(follower, s2);
		holder = getFollowedBy(follower);
	}
}

void Follow::setFollowedBy(int s1, int s2) {
	if (followedByTable.size() <= s1) {
		followedByTable.resize(s1 + 1);
	}
	followedByTable[s1] = s2;
}

void Follow::setFollowsStar(int s1, int s2) {
	if (s1 <= 0 || s2 <= 0) {
		throw new invalid_argument("Statement Number must be more than 0.");
	}
	if (followsStarTable.size() <= s1) {
		followsStarTable.resize(s1 + 1);
	}
	followStarCount++;
	followsStarTable[s1].push_back(s2);
	setFollowedByStar(s2, s1);
}

void Follow::setFollowedByStar(int s1, int s2) {
	if (followedByStarTable.size() <= s1) {
		followedByStarTable.resize(s1 + 1);
	}
	followedByStarTable[s1].push_back(s2);
}

vector<int> Follow::getFollows(int statementNum) {
	if (followsTable.size() > statementNum) {
		vector<int> result(1);
		result[0] = followsTable[statementNum];
		if (result[0] == 0) {
			return vector<int>();
		} else {
			return result;
		}
	} else {
		return vector<int>();
	}
}

vector<int> Follow::getFollowedBy(int statementNum) {
	if (followedByTable.size() > statementNum) {
		vector<int> result(1);
		result[0] = followedByTable[statementNum];
		if (result[0] == 0) {
			return vector<int>();
		} else {
			return result;
		}
	} else {
		return vector<int>();
	}
}

vector<int> Follow::getFollowsStar(int statementNum) {
	if (followsStarTable.size() > statementNum) {
		return followsStarTable[statementNum];
	} else {
		return vector<int>();
	}
}

vector<int> Follow::getFollowedByStar(int statementNum) {
	if (followedByStarTable.size() > statementNum) {
		return followedByStarTable[statementNum];
	} else {
		return vector<int>();
	}
}

int Follow::getFollowsCount() {
	return followsCount;
}

int Follow::getFollowStarCount() {
	return followStarCount;
}