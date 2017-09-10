#pragma once
#include <vector>

using namespace std;

class Modify {
public:
	Modify();

	void setModifies(int s, int varName);
	void setModifiedBy(int varName, int s);
	void setProcModifies(int procName, int varName);
	void setProcModifiedBy(int procName, int varName);

	vector<int> getModifies(int s);
	vector<int> getModifiedBy(int varName);
	vector<int> getProcModifies(int procName);
	vector<int> getProcModifiedBy(int procName);

private:

	vector<vector<int>> modifiesTable; //stmt modifies var or container stmt modifes vars
	vector<vector<int>> modifiedByTable; //var modifiedBy stmts
	vector<vector<int>> procModifiesTable; //proc modifes vars
	vector<vector<int>> procModifiedByTable; //proc modified by vars
};
