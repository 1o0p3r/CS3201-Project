#pragma once
#include <vector>
#include <set>

using namespace std;

class Use {
public:
	Use();

	/**
	Sets statementNum to use varName by calling setUses(int statementNum, int varName)

	@param statementNum statement number that is using
	@param varName variable being used by statementNum
	@param parentStarOfStmt variable that aids with setting uses for container statements, i.e. parent of statementNum uses varName
	@returns void
	*/
	void setUses(int statementNum, int varName, vector<int> parentStarOfStmt);

	/*
	Sets procName to use varName. Impicitly sets procs calledBy procName, or isCalledBy procName, and sets uses varName

	@param procName procedure variable is used by
	@param varName variable being used by procName
	@param procIsCalledBy other procedures that called procName. Impicitly sets other procedures to have used varName.
	@param procIsCalling procedures that procName is calling. Impicitly sets other procedures to have used varName.
	@param procCalledByStmt statements that called procName. Implicitly sets statement to have used varName.
	@returns void
	*/
	void setProcUses(int procName, int varName, vector<int> procIsCalledBy, vector<int> procIsCalling, vector<int> procCalledByStmt);
	void setProcUses(int procName, int varName);
	/*
	Gets variables used by statementNum

	@param statementNum
	@returns vector<int> of variables used by statementNum
	*/
	vector<int> getUses(int statementNum);

	/*
	Gets variables used by statementNum

	@param varName
	@returns vector<int> of statements that used varName
	*/
	vector<int> getUsedBy(int varName);

	/*
	Gets variables used by procName

	@param procName
	@returns vector<int> of variables used by procName
	*/
	vector<int> getProcUses(int procName);

	/*
	Gets procedures used by statementNum

	@param varName
	@returns vector<int> of procedures that used varName
	*/
	vector<int> getProcUsedBy(int varName);

private:

	vector<set<int>> usesTable; //stmt modifies var or container stmt modifes vars
	vector<set<int>> usedByTable; //var modifiedBy stmts
	vector<set<int>> procUsesTable; //proc modifes vars
	vector<set<int>> procUsedByTable; //proc modified by vars

	void setUses(int statementNum, int varName);
	void setUsedBy(int varName, int statementNum);
	void setProcUsedBy(int procName, int varName);
};