//This class is a query object to be passed to query evaluator and contains all the necessary information about the query.
#pragma once
#include <vector>
#include <SynonymEntityPair.h>
#include <QueryElement.h>
#include <map>
#include <utility>

using namespace std;
class QueryStatement
{
private:
	QueryElement selectElement;
	vector<QueryElement> withElement;
	vector<SynonymEntityPair> synonymEntityList;
	vector<QueryElement> normalQueryElements;
	vector<QueryElement> hardQueryElements;
	multimap<string, pair<int, int>> normalMultiMap;
	multimap<string, pair<int, int>> hardMultiMap;
	bool invalidQuery;
	
public:
	QueryStatement();
	QueryElement getSelectQueryElement();
	vector<QueryElement> getWithQueryElement();
	vector<SynonymEntityPair> getSynonymEntityList();

	vector<QueryElement> getNormalQueryElements();
	vector<QueryElement> getHardQueryElements();
	multimap<string, pair<int, int>> getNormalMultiMap();
	multimap<string, pair<int, int>> getHardMultiMap();
	void setInvalidQueryBoolean();
	void addNormalQueryElement(QueryElement qe);
	void addHardQueryElement(QueryElement qe);
	//E.g. Select BOOLEAN pattern a(v, "x") such that Follows(a, a2),
	//inside normalMultiMap:
	//syn = a		argumentNum = 0		idxAtNormalQueryElements = 0
	//syn = v		argumentNum = 1		idxAtNormalQueryElements = 0;
	//syn = a		argumentNum = 1		idxAtNormalQueryElements = 1;
	//syn = a2		argumentNum = 2		idxAtNormalQueryElements = 1;
	void addNormalMultiMap(string syn, int argumentNum, int idxAtNormalQueryElements);
	void addHardMultiMap(string syn, int argumentNum, int idxAtHardQueryElements);
	int getNormalQueryElementsSize();
	int getHardQueryElementsSize();
	bool getInvalidQueryBoolean();

	void addSelectQuery(QueryElement);
	void addWithQuery(QueryElement);
	void addSynonymEntityList(vector<SynonymEntityPair>);
};