#pragma once
#include <vector>
#include <unordered_map>
using namespace std;

class Follow
{
public:
	Follow();
	void setFollows(int s1, int s2);
	void setFollowedBy(int s1, int s2);
	void setFollowsStar(int s1, int s2);
	void setFollowedByStar(int s1, int s2);

	vector<int> getFollows(int statementNum);
	vector<int> getFollowedBy(int statementNum);
	vector<int> getFollowsStar(int statementNum);
	vector<int> getFollowedByStar(int statementNum);

private:
	vector<int> followsTable;
	vector<int> followedByTable;
	vector<vector<int>> followsStarTable;
	vector<vector<int>> followedByStarTable;
};