#include "QueryTree.h"

const string SELECT_STRING = "select";
const string RELATIONSHIP_STRING = "relationship";
const string PATTERN_STRING = "pattern";
using namespace std;

void QueryTree::removeTree(vector<vector<Clause>> toRemove)
{
	vector<vector<Clause>> emptyVec;
	toRemove = emptyVec;
}

QueryTree::QueryTree()
{
	
}

int QueryTree::getNumClauses() {

}

vector<Clause> QueryTree:: getClauses(string clauseType) {
	if (clauseType == SELECT_STRING) {
		return queryClause.at(0);
	}
	else if (clauseType == RELATIONSHIP_STRING) {
		return queryClause.at(1);
	}
	else if (clauseType == PATTERN_STRING) {
		return queryClause.at(2);
	}
}

void QueryTree::addSelectClause(Clause clause){
	queryClause.at(0).push_back(clause);
}
void QueryTree::addRelClause(Clause clause) {
	queryClause.at(1).push_back(clause);
}
void QueryTree::addPatternClause(Clause clause) {
	queryClause.at(2).push_back(clause);
}
vector<vector<Clause>> QueryTree::getQueryTree() {
	return queryClause;
}