#include "QueryStatement.h"

using namespace std;
QueryStatement::QueryStatement() {
	QueryElement selectElement;
	invalidQueryBoolean = false;
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
vector<QueryElement> QueryStatement::getWithQueryElement() {
	return withElement;
}
vector<QueryElement> QueryStatement::getWithQueryElementsNoSyn() {
	return withQueryElementsNoSyn;
}
vector<QueryElement> QueryStatement::getWithQueryElementsOneSyn() {
	return withQueryElementsOneSyn;
}
vector<QueryElement> QueryStatement::getWithQueryElementsTwoSyn()
{
	return withQueryElementsTwoSyn;
}
vector<QueryElement> QueryStatement::getNormalQueryElements() {
	return normalQueryElements;
}
vector<QueryElement> QueryStatement::getHardQueryElements() {
	return hardQueryElements;
}
multimap<string, pair<int, int>> QueryStatement::getNormalMultiMap()
{
	return normalMultiMap;
}
multimap<string, pair<int, int>> QueryStatement::getHardMultiMap()
{
	return hardMultiMap;
}
void QueryStatement::addWithQueryElementNoSyn(QueryElement toAdd) {
	withQueryElementsNoSyn.push_back(toAdd);
}
void QueryStatement::addWithQueryElementOneSyn(QueryElement toAdd) {
	withQueryElementsOneSyn.push_back(toAdd);
}
void QueryStatement::addWithQueryElementTwoSyn(QueryElement toAdd){
	withQueryElementsTwoSyn.push_back(toAdd);
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
int QueryStatement::getNormalQueryElementsSize()
{
	return normalQueryElements.size();
}
int QueryStatement::getHardQueryElementsSize()
{
	return hardQueryElements.size();
}
bool QueryStatement::getInvalidQueryBoolean()
{
	return invalidQueryBoolean;
}
vector<SynonymEntityPair> QueryStatement::getSynonymEntityList() {
	return synonymEntityList;
}
void QueryStatement::setInvalidQueryBoolean() {
	invalidQueryBoolean = true;
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
void QueryStatement::addWithQuery(QueryElement toAdd) {
	withElement.push_back(toAdd);
}
void QueryStatement::addSynonymEntityList(vector<SynonymEntityPair> synEntityList) {
	synonymEntityList = synEntityList;
}