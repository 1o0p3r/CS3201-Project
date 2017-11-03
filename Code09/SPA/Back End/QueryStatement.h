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
	vector<QueryElement> suchThatElement;
	vector<QueryElement> patternElement;
	vector<QueryElement> withElement;
	vector<SynonymEntityPair> synonymEntityList;
	vector<QueryElement> withQueryElementsNoSyn;
	vector<QueryElement> withQueryElementsOneSyn;
	vector<QueryElement> withQueryElementsTwoSyn;
	vector<QueryElement> normalQueryElements;
	vector<QueryElement> hardQueryElements;
	multimap<string, pair<int, int>> normalMultiMap;
	multimap<string, pair<int, int>> hardMultiMap;
	
public:
	QueryStatement();
	QueryElement getSelectQueryElement();
	vector<QueryElement> getSuchThatQueryElement();
	vector<QueryElement> getPatternQueryElement();
	vector<QueryElement> getWithQueryElement();
	vector<SynonymEntityPair> getSynonymEntityList();

	vector<QueryElement> getWithQueryElementsNoSyn();
	vector<QueryElement> getWithQueryElementsOneSyn();
	vector<QueryElement> getWithQueryElementsTwoSyn();
	vector<QueryElement> getNormalQueryElements();
	vector<QueryElement> getHardQueryElements();
	multimap<string, pair<int, int>> getNormalMultiMap();
	multimap<string, pair<int, int>> getHardMultiMap();

	void addWithQueryElementNoSyn(QueryElement qe);
	void addWithQueryElementOneSyn(QueryElement qe);
	void addWithQueryElementTwoSyn(QueryElement qe);
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

	void addSelectQuery(QueryElement);
	void addSuchThatQuery(QueryElement);
	void addPatternQuery(QueryElement);
	void addWithQuery(QueryElement);
	void addSynonymEntityList(vector<SynonymEntityPair>);
};