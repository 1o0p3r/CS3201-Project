#pragma once
#include <vector>

using namespace std;

class Parent {
public:
	Parent();
	/**
	Sets s1 to be the parent of s2. Meaning: s2 > s1.

	@param s1 the parent statment
	@param s2 the child statement
	@returns void
	*/
	void setParent(int s1, int s2);

	/**
	Gets the statment number that is the parent of the input statement.

	@param statementNum the statement number of the input statement
	@returns A vector containing one int value, the statement number which is the parent of the input statement
	*/
	vector<int> getParent(int statementNum);
	/**
	Gets the statment number that is the child of the input statement.

	@param statementNum the statement number of the input statement
	@returns A vector containing one int value, the statement number which is the child of the input statement
	*/
	vector<int> getChild(int statementNum);
	/**
	Gets the statment numbers that are the parent* of the input statement.

	@param statementNum the statement number of the input statement
	@returns A vector containing integers, the statement numbers which are the parent* of the input statement
	*/
	vector<int> getParentStar(int statementNum);
	/**
	Gets the statment numbers that are the child* of the input statement.

	@param statementNum the statement number of the input statement
	@returns A vector containing integers, the statement numbers which are the child* of the input statement
	*/
	vector<int> getChildStar(int statementNum);

private:
	vector<int> parentTable;
	vector<int> childTable;
	vector<vector<int>> parentStarTable;
	vector<vector<int>> childStarTable;

	void setChild(int s1, int s2);
	void setParentStar(int s1, int s2);
	void setChildStar(int s1, int s2);
};