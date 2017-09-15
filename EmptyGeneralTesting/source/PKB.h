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
	Returns all variables in PKB

	@returns vector string of all variables in PKB
	*/
	vector<string> getAllVariables();

	/**
	Returns all constants in PKB

	@returns vector string of all constants in PKB
	*/
	vector<string> PKB::getAllConstants();

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

	/**
	Sets s to modifies varName. Implicitly set varName to be modifiedBy s and sets modifies for container statements.

	@param s statement number of the input statement
	@param s2 variable the statement modifies
	@param parentsOfstmt parent statements of input statements
	@returns void
	*/
	void setModifies(int s, string varName, vector<int> parentStarOfStmt);

	/**
	Sets proc to modifies varName. Implicitly set varName to be modifiedBy proc.

	@param procName procedure the statement line is nested in
	@param varName variable the procedure modifies
	@returns void
	*/
	void setProcModifies(string procName, string varName);

	/**
	Gets the variables statement s modifies

	@param s statement number of the input statement 
	@returns a vector containing strings the names of the variables the input statement modifies
	*/
	vector<string> getModifies(int s);

	/**
	Gets the statements modified by variable varName

	@param varName variable being modified by input statements
	@returns a vector containing integers of statement numbers that modified the variable varName
	*/
	vector<int> getModifiedBy(string varName);

	/**
	Gets the variables the procedure modifies

	@param procName procedure name of the input procedure 
	@returns a vector containing strings the names of the variables the input statement modifies
	*/
	vector<string> getProcModifies(string procName);

	/**
	Gets the procedures modified by variable varName

	@param varName variable being modified by input procedures
	@returns a vector containing strings of procedure names that modified the variable varName
	*/
	vector<string> getProcModifiedBy(string varName);


	/**
	Sets s to uses varName. Implicitly set varName to be UsedBy s and sets uses for container statements.

	@param s statement number of the input statement
	@param s2 variable the statement uses
	@param parentsOfstmt parent statements of input statements
	@returns void
	*/
	void setUses(int s, string varName, vector<int> parentStarOfStmt);

	/**
	Sets proc to uses varName. Implicitly set varName to be UsedBy proc.

	@param procName procedure the variable is nested in
	@param varName variable the procedure uses
	@returns void
	*/
	void setProcUses(string procName, string varName);


	/**
	Gets the variables statement s uses

	@param s statement number of the input statement
	@returns a vector containing strings the names of the variables the input statement uses
	*/
	vector<string> getUses(int s);

	/**
	Gets the statements used by variable varName

	@param varName variable being used by input statements
	@returns a vector containing integers of statement numbers that used the variable varName
	*/
	vector<int> getUsedBy(string varName);

	/**
	Gets the variables the procedure uses

	@param procName procedure name of the input procedure
	@returns a vector containing strings the names of the variables the input statement uses
	*/
	vector<string> getProcUses(string procName);

	/**
	Gets the procedures used by variable varName

	@param varName variable being used by input procedures
	@returns a vector containing strings of procedure names that used the variable varName
	*/
	vector<string> getProcUsedBy(string varName);

	/**
	Sets the statement type of each line of the program

	@param statementNum statement number of input statement
	@param type indexed name of statement type
	@returns void
	*/
	void setStatementType(int statementNum, int type);

	/**
	Gets all statement numbers of While statements

	@returns void
	*/
	vector<int> getWhile();

	/**
	Gets all Assign numbers of While statements

	@returns void
	*/
	vector<int> getAssign();

	/**
	Gets all statement numbers of Is statements

	@returns void
	*/
	vector<int> getIf();

	/**
	Gets all statement numbers in program

	@returns void
	*/
	vector<int> getAllStmt();

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

	set<string> allVariables;
	set<string> allConstants;

	int getVarIndex(string varName);
	int getProcIndex(string varName);
	void addConstant(string c);
	vector<string> convertToVarNames(vector<int> input);
	vector<string> convertToProcNames(vector<int> input);
};