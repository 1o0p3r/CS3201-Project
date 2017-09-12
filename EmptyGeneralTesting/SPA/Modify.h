#pragma once
#include <vector>
#include <set>

using namespace std;

class Modify {
public:
	Modify();

	void setModifies(int s, int varName, vector<int> parentsOfstmt);
	void Modify::setModifies(int s, int varName);
	void setProcModifies(int procName, int varName);

	vector<int> getModifies(int s);
	vector<int> getModifiedBy(int varName);
	vector<int> getProcModifies(int procName);
	vector<int> getProcModifiedBy(int varName);

private:

	vector<set<int>> modifiesTable; //stmt modifies var or container stmt modifes vars
	vector<set<int>> modifiedByTable; //var modifiedBy stmts
	vector<set<int>> procModifiesTable; //proc modifes vars
	vector<set<int>> procModifiedByTable; //proc modified by vars

	void setModifiedBy(int varName, int s);
	void setProcModifiedBy(int procName, int varName);
};
