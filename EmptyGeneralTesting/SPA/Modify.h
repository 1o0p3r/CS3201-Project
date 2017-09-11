#pragma once
#include <vector>

using namespace std;

class Modify {
public:
	Modify();

	void setModifies(int s, int varName);
	void setProcModifies(int procName, int varName);

	vector<int> getModifies(int s);
	vector<int> getModifiedBy(int varName);
	vector<int> getProcModifies(int procName);
	vector<int> getProcModifiedBy(int varName);

private:

	vector<vector<int>> modifiesTable; //stmt modifies var or container stmt modifes vars
	vector<vector<int>> modifiedByTable; //var modifiedBy stmts
	vector<vector<int>> procModifiesTable; //proc modifes vars
	vector<vector<int>> procModifiedByTable; //proc modified by vars

	void setModifiedBy(int varName, int s);
	void setProcModifiedBy(int procName, int varName);
};
