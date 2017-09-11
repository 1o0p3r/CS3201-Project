#pragma once
#include <string>
#include <vector>
#include <SynonymEntityPair.h>
#include <RelationshipTable.h>
#include <Relationship.h>
#include <Clause.h>
#include <QueryTree.h>

using std::string;
using std::vector;

class ParserQuery
{
private:
	int numClauses;
	
	RelationshipTable relationshipTable;
	QueryTree queryTree; //Has to be refresh after each query

	vector<SynonymEntityPair> synonymAndEntityList; //Has to be refresh after each query
	string checkDeclaration(string arg);
	
	vector<string> validEntities;
	vector<string> split(string, char);
	vector<string> split(string str, string symbolSplitWith);
	vector<string> declarationString;
	vector<string> queryString;
	vector<string> splitDiffClauses(vector<string>);
	vector<string> splitSelect(vector<string> vectorToSplit, string strToSplitWith);

	string removeSymbols(string str, string symbolToRemove);

	void parseLine(string, string);
	void setSynonymList(vector<SynonymEntityPair> synonymList);
	
	bool isValidSelect(vector<string> vectorClauses);
	bool isValidClauses(vector<string> vectorClauses);
	bool isValidSuchThat(string str, string syn);
	bool addSelectClauseTree(Clause clause);
	bool addRelClauseTree(Clause clause);
	bool addPatternClauseTree(Clause clause);
	bool is_number(string);
	bool isValidPattern(string str, string syn);
	bool parseEntityAndSynonym(string);
	bool checkEntityAndSynonym(string);
	bool isValidEntity(string);
	bool isValidSynonym(string);
	bool inEntityList(string entity);
	bool parseDeclaration(vector<string> splitString);
	bool parseSelectClauses(string selectString);
	bool isValidResultClauses(string);
	bool createRelClause(bool arg1_NUM, bool arg1_UNDER, bool arg2_NUM, bool arg2_UNDER, string relType, string arg1, string arg2);
	bool checkRelationshipTable(string, string, int);
public:
	ParserQuery();
	int getNumClauses();
	string changeLowerCase(string str);
	void startParsing();
	void removeTree(QueryTree querytree);
};
