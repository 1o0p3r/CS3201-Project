#pragma once
#include <vector>

using namespace std;

class Use {
public:
	Use();

	void setUses(int s, int varName);
	void setProcUses(int procName, int varName);

	vector<int> getUses(int s);
	vector<int> getUsedBy(int varName);
	vector<int> getProcUses(int procName);
	vector<int> getProcUsedBy(int varName);

private:

	vector<vector<int>> usesTable; //stmt modifies var or container stmt modifes vars
	vector<vector<int>> usedByTable; //var modifiedBy stmts
	vector<vector<int>> procUsesTable; //proc modifes vars
	vector<vector<int>> procUsedByTable; //proc modified by vars
	void setUsedBy(int varName, int s);
	void setProcUsedBy(int procName, int varName);
};