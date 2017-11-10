//This class is a query object to be passed to query evaluator and contains all the necessary information about the query.
#include "QueryStatement.h"

using namespace std;
QueryStatement::QueryStatement() {
	QueryElement selectElement;
	invalidQuery = false;
}
QueryElement QueryStatement::getSelectQueryElement() {
	return selectElement;
}
vector<QueryElement> QueryStatement::getWithQueryElement() {
	return withElement;
}
vector<QueryElement> QueryStatement::getNormalQueryElements() {
	return normalQueryElements;
}
vector<QueryElement> QueryStatement::getHardQueryElements() {
	return hardQueryElements;
}
multimap<string, pair<int, int>> QueryStatement::getNormalMultiMap() {
	return normalMultiMap;
}
multimap<string, pair<int, int>> QueryStatement::getHardMultiMap() {
	return hardMultiMap;
}
void QueryStatement::addNormalQueryElement(QueryElement toAdd) {
	normalQueryElements.push_back(toAdd);
}
void QueryStatement::addHardQueryElement(QueryElement toAdd) {
	hardQueryElements.push_back(toAdd);
}
void QueryStatement::addNormalMultiMap(string syn, int argumentNum, int idx) {
	pair<int, int> normalPair = make_pair(argumentNum, idx);
	normalMultiMap.insert(pair<string, pair<int,int>> (syn, normalPair));
}
void QueryStatement::addHardMultiMap(string syn, int argumentNum, int idx) {
	pair<int, int> normalPair = make_pair(argumentNum, idx);
	hardMultiMap.insert(pair<string, pair<int, int>>(syn, normalPair));
}
int QueryStatement::getNormalQueryElementsSize() {
	return normalQueryElements.size();
}
int QueryStatement::getHardQueryElementsSize() {
	return hardQueryElements.size();
}
bool QueryStatement::getInvalidQueryBoolean() {
	return invalidQuery;
}
vector<SynonymEntityPair> QueryStatement::getSynonymEntityList() {
	return synonymEntityList;
}
void QueryStatement::setInvalidQueryBoolean() {
	invalidQuery= true;
}
void QueryStatement::addSelectQuery(QueryElement toAdd) {
	selectElement = toAdd;
}
void QueryStatement::addWithQuery(QueryElement toAdd) {
	withElement.push_back(toAdd);
}
void QueryStatement::addSynonymEntityList(vector<SynonymEntityPair> synEntityList) {
	synonymEntityList = synEntityList;
}