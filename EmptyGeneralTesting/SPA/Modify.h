#pragma once
#include <vector>

using namespace std;

class Modify
{
public:
	Modify();
	int varIndexer(string varName);
	int procIndexer(string procName);

	void setModifies(int s, string varName);
	void setModifiedBy(string varName, int s);
	void setProcModifies(string procName, string varName);
	void setProcModifiedBy(string procName, string varName);

	vector<string> getModifies(int s);
	vector<int> getModifiedBy(string varName);
	vector<string> getProcModifies(string procName);
	vector<string> getProcModifiedBy(string procName);

private:
	vector<string> varIndexTable;
	vector<string> procIndexTable;
	vector<vector<string>> modifiesTable; //stmt modifies var or container stmt modifes vars
	vector<vector<int>> modifiedByTable; //var modifiedBy stmts
	vector<vector<string>> procModifiesTable; //proc modifes vars
	vector<vector<string>> procModifiedByTable; //proc modified by vars
};
