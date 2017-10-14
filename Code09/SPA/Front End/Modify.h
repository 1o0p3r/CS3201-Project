#pragma once
#include <vector>
#include <set>

using namespace std;

class Modify {
public:
	Modify();

	/**
	Sets statementNum to modify varName by calling setModifies(int statementNum, int varName)

	@param statementNum statement number that is modifying
	@param varName variable being modified by statementNum
	@param parentStarOfStmt variable that aids with setting modifies for container statements, i.e. parent of statementNum modifies varName
	@returns void
	*/
	void setModifies(int statementNum, int varName, vector<int> parentStarOfStmt);

	/*
	Sets procName to modify varName. Impicitly sets procs calledBy procName, or isCalledBy procName, and sets modifies varName

	@param procName procedure variable is modified by
	@param varName variable being modified by procName
	@param procIsCalledBy other procedures that called procName. Impicitly sets other procedures to have modified varName.
	@param procIsCalling procedures that procName is calling. Impicitly sets other procedures to have modified varName.
	@param procCalledByStmt statements that called procName. Implicitly sets statement to have modified varName.
	@returns void
	*/
	void setProcModifies(int procName, int varName, vector<int> procIsCalledBy, vector<int> procIsCalling, vector<int> procCalledByStmt);
	void setProcModifies(int procName, int varName);

	/*
	Gets variables modified by statementNum

	@param statementNum
	@returns vector<int> of variables modified by statementNum
	*/
	vector<int> getModifies(int statementNum);

	/*
	Gets variables modified by statementNum

	@param varName
	@returns vector<int> of statements that modified varName
	*/
	vector<int> getModifiedBy(int varName);

	/*
	Gets variables modified by procName

	@param procName
	@returns vector<int> of variables modified by procName
	*/
	vector<int> getProcModifies(int procName);

	/*
	Gets procedures modified by statementNum

	@param varName
	@returns vector<int> of procedures that modified varName
	*/
	vector<int> getProcModifiedBy(int varName);

private:

	vector<set<int>> modifiesTable; //stmt modifies var or container stmt modifes vars
	vector<set<int>> modifiedByTable; //var modifiedBy stmts
	vector<set<int>> procModifiesTable; //proc modifes vars
	vector<set<int>> procModifiedByTable; //proc modified by vars

	void setModifies(int statementNum, int varName);
	void setModifiedBy(int varName, int statementNum);
	void setProcModifiedBy(int procName, int varName);

};
