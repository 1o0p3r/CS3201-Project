#pragma once
#include <string>
#include <Clause.h>
#include <vector>

using namespace std;
class QueryTree
{
private:
	vector<vector<Clause>> queryClause;
	void removeTree(vector<vector<Clause>>);
public:
	QueryTree();
	int getNumClauses();
	vector<Clause> getClauses(string clauseType);
	vector<vector<Clause>> getQueryTree();	
	void addRelClause(Clause);
	void addPatternClause(Clause);
	void addSelectClause(Clause);
};