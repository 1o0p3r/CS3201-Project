#pragma once
#include <vector>
#include <set>

using namespace std;

class Use {
public:
	Use();

	void setUses(int statementNum, int varName, vector<int> parentStarOfStmt);
	void setUses(int statementNum, int varName);
	void setProcUses(int procName, int varName, vector<int> procIsCalledBy, vector<int> procIsCalling, vector<int> procCalledByStmt);
	void setProcUses(int procName, int varName);

	vector<int> getUses(int statementNum);
	vector<int> getUsedBy(int varName);
	vector<int> getProcUses(int procName);
	vector<int> getProcUsedBy(int varName);

private:

	vector<set<int>> usesTable; //stmt modifies var or container stmt modifes vars
	vector<set<int>> usedByTable; //var modifiedBy stmts
	vector<set<int>> procUsesTable; //proc modifes vars
	vector<set<int>> procUsedByTable; //proc modified by vars
	void setUsedBy(int varName, int statementNum);
	void setProcUsedBy(int procName, int varName);
};