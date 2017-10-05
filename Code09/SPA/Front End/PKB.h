#pragma once

#include "Follow.h"
#include "Modify.h"
#include "Parent.h"
#include "Use.h"
#include "Calls.h"
#include "Next.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <tuple>

using namespace std;
typedef short PROC;

class VarTable;  // no need to #include "VarTable.h" as all I need is pointer

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
	Returns all procedures in PKB

	@returns vector string of all procedures in PKB
	*/
	vector<string> PKB::getAllProcedures();
	/**
	Sets statementNum2 to follow statementNum1. Meaning: statementNum2 > statementNum1.

	@param statementNum1 the statment before
	@param statementNum2 the statement after
	@returns void
	*/
	void setFollows(int statementNum1, int statementNum2);

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
	Sets statementNum1 to be the parent of statementNum2. Meaning: statementNum2 > statementNum1.

	@param statementNum1 the parent statment
	@param statementNum2 the child statement
	@returns void
	*/
	void setParent(int statementNum1, int statementNum2);

	/**
	Gets the statment number that is the parent of the input statement.

	@param statementNum the statement number of the input statement
	@returns A vector containing one int value, the statement number which is the parent of the input statement
	*/
	vector<int> getParent(int statementNum);
	
	/**
	Gets the statment number that is the immediate child of the input statement.

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
	void setModifies(int statementNum, string varName);

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
	vector<string> getModifies(int statementNum);

	/**
	Gets the statements that modifies variable varName

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
	void setUses(int statementNum, string varName);

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
	vector<string> getUses(int statementNum);

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
	Sets proc1 calls proc2. Implicitly sets calledBy, callsStar and calledByStar using parentStar.

	@param procName1 procedure that is calling another procedure
	@param procName2 procedure being called
	@returns void
	*/
	void setCalls(string procName1, string procName2);

	/**
	Gets the procedures called directly by input procedure

	@param procName name of procedure calling other procedures directly nested in it
	@returns a vector containing strings the names of the procedures it directly calls
	*/

	vector<string> getCalls(string procName); 
	/**
	Gets the procedures that the input procedure is called by

	@param procName name of procedure calling other procedures directly nested in it
	@returns a vector containing strings the names of the procedures the input procedure is called by
	*/
	
	vector<string> getCalledBy(string procName); // @niev remove when done
	/**
	Gets the procedures called by input procedure directly and indirectly

	@param procName name of procedure calling other procedures 
	@returns a vector containing strings the names of the procedures it calls
	*/
	vector<string> getCallsStar(string procName);

	/**
	Gets the procedures that the input procedure is called by directly and indirectly

	@param procName name of procedure calling other procedures directly nested in it
	@returns a vector containing strings the names of the procedures the input procedure is called by
	*/
	vector<string> getCalledByStar(string procName);

	/**
	Creates the CFG of the program after one parsing

	@param stmtsAndType type of stmt at stmtNum, such as assign, while, etc.
	@param parentOfStmtVec parent of stmt, where stmt is the location in the vector
	@returns void
	*/
	void createCFG(vector<int> stmtsAndType, vector<int> parentOfStmtVec);

	/**
	Sets the stmt type of each stmt

	@param statementNum number of statement
	@param type type of the statement
	@returns void
	*/
	void setStatementType(int statementNum, string type);

	/**
	Gets all the while statements
	@returns a vector containing ints of all while statements
	*/
	vector<int> getWhile();

	/**
	Gets all the assign statements
	@returns a vector containing ints of all assign statements
	*/
	vector<int> getAssign();

	/**
	Gets all the if statements
	@returns a vector containing ints of all if statements
	*/
	vector<int> getIf();

	/**
	Gets all the call statements
	@returns a vector containing ints of all call statements
	*/
	vector<int> getCall();

	/**
	Gets all procedures that was called by another procedure
	@returns a vector containing procedure names of all called procedures
	*/
	vector<string> PKB::getAllCalls();

	/**
	Gets all the statements
	@returns a vector containing ints of all while statements
	*/
	vector<int> getAllStmt();

	/**
	Add a Constant to PKB
	@param c the constant name
	*/
	void addConstant(string c);

	/**
	Adds a pattern to PKB
	@param StatementNum Statement Number of the expression
	@param leftVariable the modified variable
	@param rightExpression the expression on the right side of "="
	*/
	void addPattern(int statementNum, string leftVariable, string rightExpression);
	/**
	Gets all the statement number and expressions that appears for a variable
	@param varNamr name of variable
	@returns a vector of tuple containing all the (statement number, expression) pairs for the input variable
	*/
	vector<tuple<int, string>> getPattern(string varName);

private:
	Follow follow;
	Parent parent;
	Modify modify;
	Use use;
	Calls call;
	Next next;

	vector<string> varIndexTable;
	vector<string> procIndexTable;
	vector<int> whileTable;
	vector<int> assignTable;
	vector<int> ifTable;
	vector<int> callTable;
	vector<vector<tuple<int, string>>> patternTable;

	set<string> allVariables;
	set<string> allConstants;
	set<string> allProcedures;

	int getVarIndex(string varName);
	int getProcIndex(string varName);
	vector<string> convertToVarNames(vector<int> input);
	vector<string> convertToProcNames(vector<int> input);
	void addVariable(string v);
	void addProcedure(string p);

	void initTypeMap();
};