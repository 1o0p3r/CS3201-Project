#pragma once

#include "Follow.h"
#include "Modify.h"
#include "Parent.h"
#include "Use.h"
#include "Calls.h"
#include "Next.h"

#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>

using namespace std;
typedef short PROC;
typedef tuple<int, vector<int>> vTuple;
typedef tuple<int, vector<set<int>>> vsTuple;

class PKB {
public:
	PKB();
	/**
	@param statementNum the first statment of the else container
	@returns void
	 */
	void insertElse(int statementNum);
	/**
	@param statementNum the first statement of the statement list
	@returns void
	 */
	void insertStatementList(int statementNum);
	/**
	@returns vector of statement numbers which are the first lines of all statement lists
	 */
	vector<int> getStatementList();

	/**
	Returns all variables in PKB

	@returns vector string of all variables in PKB
	*/
	vector<string> getAllVariables();

	/**
	Returns all constants in PKB

	@returns vector string of all constants in PKB
	*/
	vector<string> getAllConstants();

	/**
	Returns all procedures in PKB

	@returns vector string of all procedures in PKB
	*/
	vector<string> getAllProcedures();
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

	@parem statementNum statement number of call
	@param procName1 procedure that is calling another procedure
	@param procName2 procedure being called
	@returns void
	*/
	void setCalls(int statementNum, string procName1, string procName2);

	/**
	Gets the stmt# that calls a procedure 

	 @returns a vector containing stmt# where it calls another procedure
	 */
	vector<int> getAllLineCalls();
	
	/**
	Gets the stmt# that calls a specified procedure
	@param procName that is being called

	@returns a vector containing stmt# where the specified procedure is called
	*/
	vector<int> getLineCalls(string procName);

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
	@returns void
	*/
	void createCFG();
	/**
	Gets the statements that are the next of the input statement
	@param stmtNum input statementNum
	@returns a vector containing the statment numbers that are the next of the input statement
	*/
	vector<int> getNext(int stmtNum);
	/**
	Gets the statements that are the previous of the input statement
	@param stmtNum input statementNum
	@returns a vector containing the statment numbers that are the previous of the input statement
	*/
	vector<int> getPrevious(int stmtNum);
	/**
	Get all the statements who has a next statement
	@returns a vector containing all the statements that has a next
	*/
	vector<int> getAllNext();
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
	vector<string> getAllCalls();

	/**
	Gets all the statements
	@returns a vector containing ints of all while statements
	*/
	vector<int> getAllStmt();

	/**
	Set the last line of a procedure
	@param procName the name of the procedure
	@param lastline the last line of the procedure
	*/
	void setLastline(string procName, int lastline);

	/**
	Gets the last line of a procedure
	@param procName the name of the procedure
	@returns the statement number of the last line
	*/
	int getLastline(string procName);

	/*
	Set the statement to be in the procedure
	@returns void
	*/
	void setProcedure(int stmtNum, string procedure);

	/**
	Checks if Affects(statement 1, statement 2), statement 1 != statement 2
	@param statementNum1 statement number 1
	@param statementNum2 statement number 2
	@returns true if Affects(statement 1, statement 2) is true
	*/
	bool getAffectsTwoLiterals(int statementNum1, int statementNum2);

	/**
	Gets Affects(statement, synonym)
	@param statementNum statement number of the literal
	@returns a vector of statements that is affected by the literal
	*/
	vector<int> getAffectsFirstLiteral(int statementNum);

	/**
	Gets Affects(synonym, statement)
	@param statementNum statement number of the literal
	@returns a vector of statements that affects the literal
	*/
	vector<int> getAffectsSecondLiteral(int statementNum);
	/**
	Gets Affects(synonym, synonym)
	@returns a tuple of vectors of statements that correspond to the first and second synonyms
	*/
	tuple<vector<int>, vector<int>> getAffectsTwoSynonyms();

	/**
	Add a Constant to PKB
	@param c the constant name
	*/
	void addConstant(string c);

	/**
	Adds a assign pattern to PKB
	@param StatementNum Statement Number of the expression
	@param leftVariable the modified variable
	@param rightExpression the expression on the right side of "="
	*/
	void addAssignPattern(int statementNum, string leftVariable, string rightExpression);
	/**
	Adds a while pattern to PKB
	@param StatementNum Statement Number of the while
	@param variable the modified variable
	*/
	void addWhilePattern(int statementNum, string variable);
	/**
	Gets all the while statement number that contains a variable
	@param variable name of variable
	@returns a vector containing all the statement number for the input variable
	*/
	vector<int> getPatternWhile(string variable);
	/**
	Gets all the while statement numbers with their control variable
	@returns a tuple of vectors containing all the statement number and the their control variable
	*/
	tuple<vector<int>, vector<string>> getAllPatternWhile();
	/**
	Adds a if pattern to PKB
	@param StatementNum Statement Number of the if
	@param variable the modified variable
	*/
	void addIfPattern(int statementNum, string variable);
	/**
	Gets all the if statement number that contains a variable
	@param variable name of variable
	@returns a vector containing all the statement number for the input variable
	*/
	vector<int> getPatternIf(string variable);
	/**
	Gets all the if statement numbers with their control variable
	@returns a tuple of vectors containing all the statement number and the their control variable
	*/
	tuple<vector<int>, vector<string>> getAllPatternIf();
	/**
	Gets all the statement number and expressions that appears for a variable
	@param varNamr name of variable
	@returns a vector of tuple containing all the (statement number, expression) pairs for the input variable
	*/
	tuple<vector<int>, vector<string>> getPatternVariable(string varName);
	/**
	Gets all the statement number and variables that matches the whole expression
	@param expression expression
	@returns a tuple of vectors containing all the (statement number, variables) pairs for the input expression
	*/
	tuple<vector<int>, vector<string>> getPatternExpression(string expression);
	/**
	Gets all the statement number and variables that matches part of a expression
	@param expression expression to match
	@returns a tuple of vectors containing all the (statement number, variables) pairs for the input expression
	*/
	tuple<vector<int>, vector<string>> getPatternExpressionSubstring(string expression);
	/**
	Gets all the statement numbers that matches a part of the expression and has the variable
	@param variable name of variable
	@param expression expression to match
	@returns a vector of statements that matches a part of the expression and modifies the variable
	*/
	vector<int> getPatternVariableExpressionSubstring(string variable, string expression);
	/**
	Gets all the statement numbers that matches the expression and has the variable
	@param variable name of variable 
	@param expression expression to match
	@returns a vector of statements that matches the expression and modifies the variable
	*/
	vector<int> getPatternVariableExpression(string variable, string expression);
	/**
	Get the number of follows relationship
	@returns the number of follows relationship
	*/
	int getFollowsCount();
	/**
	Get the number of follow* relationship
	@returns the number of follow* relationship
	*/
	int getFollowStarCount();
	/**
	Get the number of parent relationship
	@returns the number of parent relationship
	*/
	int getParentCount();
	/**
	Get the number of parent star relationship
	@returns the number of parent star relationship
	*/
	int getParentStarCount();
	/**
	Get the number of modify relationship
	@returns the number of modify relationship
	*/
	int getModifyCount();
	/**
	Get the number of procedure modify relationship
	@returns the number of procedure modify relationship
	*/
	int getProcModifyCount();
	/**
	Get the number of use relationship
	@returns the number of use relationship
	*/
	int getUseCount();
	/**
	Get the number of procedure use relationship
	@returns the number of procedure use relationship
	*/
	int getProcUseCount();
	/**
	Get the number of next relationship
	@returns the number of next relationship
	*/
	int getNextCount();
	/**
	Checks if next*(statement 1, statement 2)
	@param statementNum1 statement number 1
	@param statementNum2 statement number 2
	@returns true if next*(statement 1, statement 2) is true
	*/
	bool getNextStarTwoLiterals(int s1, int s2);
	/**
	Gets next*(synonym, statement)
	@param statementNum statement number of the literal
	@returns a vector of statements that is the next* of the literal
	*/
	vector<int> getNextStarFirstLiteral(int s);
	/**
	Gets next*(statement, synonym)
	@param statementNum statement number of the literal
	@returns a vector of statements that the literal is the next* of
	*/
	vector<int> getNextStarSecondLiteral(int s);
	/**
	Gets next*(synonym, synonym)
	@returns a tuple of vectors of statements that correspond to the first and second synonyms
	*/
	tuple<vector<int>, vector<int>> getNextStarTwoSynonyms();
	/*
	Gets all variables that have been modified
	@returns a vector of all variables that have been modified
	*/
	vector<string> getAllModifiedVariables();
	/*
	Gets all variables that have been used
	@returns a vector of all variables that have been used
	*/
	vector<string> getAllUsedVariables();
	/*
	Get the procedure that is called by the call statement
	@param statement the call statement
	@returns the procedure that is called by the statement
	*/
	string getProcCalledByStatement(int statement);
	/**
	Checks if affect*(statement 1, statement 2)
	@param statementNum1 statement number 1
	@param statementNum2 statement number 2
	@returns true if affect*(statement 1, statement 2) is true
	*/
	bool getAffectStarTwoLiterals(int s1, int s2);
	/**
	Gets affect*(synonym, statement)
	@param statementNum statement number of the literal
	@returns a vector of statements that is the affect* of the literal
	*/
	vector<int> getAffectStarFirstLiteral(int s1);
	/**
	Gets affect*(statement, synonym)
	@param statementNum statement number of the literal
	@returns a vector of statements that the literal is the affect* of
	*/
	vector<int> getAffectStarSecondLiteral(int s2);
	/**
	Gets affect*(synonym, synonym)
	@returns a tuple of vectors of statements that correspond to the first and second synonyms
	*/
	tuple<vector<int>, vector<int>> getAffectStarTwoSynonyms();

	int maxLevel;

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
	vector<int> typeTable;
	vector<int> lastlineTable;
	vector<tuple<vector<int>, vector<string>>> patternTable;
	unordered_map<string, tuple<vector<int>, vector<string>>> expressionTable;
	vector<int> statementList;
	vector<vector<int>> whilePatternTable;
	vector<vector<int>> ifPatternTable;

	set<string> allVariables;
	set<string> allConstants;
	set<string> allProcedures;
	set<int> elseSet;

	int getVarIndex(string varName);
	int getProcIndex(string varName);
	vector<string> convertToVarNames(vector<int> input);
	vector<string> convertToProcNames(vector<int> input);
	void addVariable(string v);
	void addProcedure(string p);

	void initTypeMap();
	void whileCFG(int & i);
	void assignCallCFG(int & i);
	void ifCFG(int & i);
	void processNext(int & i);
	bool toPop;
	vector<int> goBack;
	vector<int> state;
	int lastLineOfIf;
	vector<vector<int>> ifHolders;
	vector<int> ifParent;
	
	vector<int> statementProcedureTable;
	vector<string> modifiedVariables;
	vector<string> usedVariables;
	void affectsRecurse(vector<int>& s1, vector<int>& s2, vTuple current, int & max, vector<vector<int>> explored, vector<bool>& exploredOnce, set<pair<int, int>>& included);
	void affectStarRecurse(vector<int>& s1, vector<int>& s2, vsTuple current, int& max, vector<vector<set<int>>> explored, vector<int>& exploredOnce, set<pair<int, int>>& included);
};