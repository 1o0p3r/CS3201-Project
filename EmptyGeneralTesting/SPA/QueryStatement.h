#pragma once
#include <vector>
#include <SynonymEntityPair.h>
#include <QueryElement.h>

using namespace std;
class QueryStatement
{
private:
	QueryElement selectElement;
	vector<QueryElement> suchThatElement;
	vector<QueryElement> patternElement;
	vector<SynonymEntityPair> synonymEntityList;
	bool addedSynonymEntityList = false;
public:
	QueryStatement();
	QueryElement getSelectQueryElement();
	vector<QueryElement> getSuchThatQueryElement();
	vector<QueryElement> getPatternQueryElement();
	vector<SynonymEntityPair> getSynonymEntityList();
	void addSelectQuery(QueryElement);
	void addSuchThatQuery(QueryElement);
	void addPatternQuery(QueryElement);
	void addSynonymEntityList(vector<SynonymEntityPair>);
};
