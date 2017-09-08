#pragma once
#include <vector>

using namespace std;

class Follow {
public:
	Follow();
	/**
	Sets s2 to follow s1. Meaning: s2 > s1.

	@param s1 the statment before
	@param s2 the statement after
	@returns void
	*/
	void setFollows(int s1, int s2);
	void setFollowsStar(int s1, int s2);

	vector<int> getFollows(int statementNum);
	vector<int> getFollowedBy(int statementNum);
	vector<int> getFollowsStar(int statementNum);
	vector<int> getFollowedByStar(int statementNum);

private:
	vector<int> followsTable;
	vector<int> followedByTable;
	vector<vector<int>> followsStarTable;
	vector<vector<int>> followedByStarTable;

	void setFollowedBy(int s1, int s2);
	void setFollowedByStar(int s1, int s2);
};