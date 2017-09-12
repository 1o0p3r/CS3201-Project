#pragma once
#include "Follow.h"
#include "Modify.h"
#include "Parent.h"
#include "Use.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class PKB {
public:
	PKB();

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

	void setModifies(int s, string varName);
	void setProcModifies(string procName, string varName);

	vector<string> getModifies(int s);
	vector<int> getModifiedBy(string varName);
	vector<string> getProcModifies(string procName);
	vector<string> getProcModifiedBy(string varName);

	void setUses(int s, string varName);
	void setProcUses(string procName, string varName);

	vector<string> getUses(int s);
	vector<int> getUsedBy(string varName);
	vector<string> getProcUses(string procName);
	vector<string> getProcUsedBy(string varName);

	void setStatementType(int statementNum, int type);
	vector<int> getWhile();
	vector<int> getAssign();
	vector<int> getIf();

private:
	Follow follow;
	Parent parent;
	Modify modify;
	Use use;

	vector<string> varIndexTable;
	vector<string> procIndexTable;
	vector<int> whileTable;
	vector<int> assignTable;
	vector<int> ifTable;

	void PKB::setVarIndex(string varName);
	int PKB::getVarIndex(string varName);
	void PKB::setProcIndex(string varName);
	int PKB::getProcIndex(string varName);
	vector<string> convertToVarNames(vector<int> input);
	vector<string> convertToProcNames(vector<int> input);
};