#pragma once
#include <string>
#include <Clause.h>
#include <vector>

using namespace std;
class QueryTree
{
public:
	QueryTree();
	int getNumClauses();
	vector<Clause> getClauses();
	QueryTree getQueryTree();	
	void addSuchThatClause();
	void addPatternClause();
};