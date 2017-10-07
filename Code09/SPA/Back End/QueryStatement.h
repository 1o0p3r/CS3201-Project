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
	vector<QueryElement> withElement;
	vector<SynonymEntityPair> synonymEntityList;
public:
	QueryStatement();
	QueryElement getSelectQueryElement();
	vector<QueryElement> getSuchThatQueryElement();
	vector<QueryElement> getPatternQueryElement();
	vector<QueryElement> getWithQueryElement();
	vector<SynonymEntityPair> getSynonymEntityList();
	void addSelectQuery(QueryElement);
	void addSuchThatQuery(QueryElement);
	void addPatternQuery(QueryElement);
	void addWithQuery(QueryElement);
	void addSynonymEntityList(vector<SynonymEntityPair>);
};

