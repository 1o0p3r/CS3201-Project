#include "QueryStatement.h"

using namespace std;
QueryStatement::QueryStatement()
{
	QueryElement selectElement;
}
QueryElement QueryStatement::getSelectQueryElement() {
	return selectElement;
}
vector<QueryElement> QueryStatement::getSuchThatQueryElement() {
	return suchThatElement;
}
vector<QueryElement> QueryStatement::getPatternQueryElement() {
	return patternElement;
}
vector<SynonymEntityPair> QueryStatement::getSynonymEntityList() {
	return synonymEntityList;
}
void QueryStatement::addSelectQuery(QueryElement toAdd) {
	selectElement = toAdd;
}
void QueryStatement::addSuchThatQuery(QueryElement toAdd) {
	suchThatElement.push_back(toAdd);
}
void QueryStatement::addPatternQuery(QueryElement toAdd) {
	patternElement.push_back(toAdd);
}

void QueryStatement::addSynonymEntityList(vector<SynonymEntityPair> synEntityList)
{
	if (!addedSynonymEntityList) {
		synonymEntityList = synEntityList;
	}
}