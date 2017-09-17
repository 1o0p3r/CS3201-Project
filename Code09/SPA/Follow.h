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

	/**
	Gets the statment number that follows the input statement.

	@param statementNum the statement number of the input statement
	@returns A vector containing one int value, the statement number which follows the input statement
	*/
	vector<int> getFollows(int statementNum);
	/**
	Gets the statment number that is followed by the input statement.

	@param statementNum the statement number of the input statement
	@returns A vector containing one int value, the statement number which is followed by the input statement
	*/
	vector<int> getFollowedBy(int statementNum);
	/**
	Gets the statment numbers that follows* the input statement.

	@param statementNum the statement number of the input statement
	@returns A vector containing integers, the statement numbers which follows* the input statement
	*/
	vector<int> getFollowsStar(int statementNum);
	/**
	Gets the statment numbers that is followed* by the input statement.

	@param statementNum the statement number of the input statement
	@returns A vector containing integers, the statement numbers is followed* by the input statement
	*/
	vector<int> getFollowedByStar(int statementNum);

private:
	vector<int> followsTable;
	vector<int> followedByTable;
	vector<vector<int>> followsStarTable;
	vector<vector<int>> followedByStarTable;

	void setFollowedBy(int s1, int s2);
	void setFollowsStar(int s1, int s2);
	void setFollowedByStar(int s1, int s2);
};