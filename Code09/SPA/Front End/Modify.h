#pragma once
#include <vector>
#include <set>

using namespace std;

class Modify {
public:
	Modify();

	void setModifies(int statementNum, int varName, vector<int> parentStarOfStmt);
	void setModifies(int statementNum, int varName);
	void setProcModifies(int procName, int varName, vector<int> procIsCalledBy, vector<int> procIsCalling);
	void setProcModifies(int procName, int varName);

	vector<int> getModifies(int statementNum);
	vector<int> getModifiedBy(int varName);
	vector<int> getProcModifies(int procName);
	vector<int> getProcModifiedBy(int varName);

private:

	vector<set<int>> modifiesTable; //stmt modifies var or container stmt modifes vars
	vector<set<int>> modifiedByTable; //var modifiedBy stmts
	vector<set<int>> procModifiesTable; //proc modifes vars
	vector<set<int>> procModifiedByTable; //proc modified by vars

	void setModifiedBy(int varName, int statementNum);
	void setProcModifiedBy(int procName, int varName);

};
