#pragma once
#include <string>
#include <vector>
#include <SynonymEntityPair.h>
#include <RelationshipTable.h>
#include <Relationship.h>
#include <Clause.h>

using std::string;
using std::vector;

class ParserQuery
{
private:
	int numClauses;
	
	RelationshipTable relationshipTable;

	vector<SynonymEntityPair> synonymAndEntityList;
	vector<string> validEntities;
	vector<string> split(string, char);
	vector<string> split(string str, string symbolSplitWith);
	vector<string> declarationString;
	vector<string> queryString;
	vector<string> splitDiffClauses(vector<string>);
	vector<string> splitSelect(vector<string> vectorToSplit, string strToSplitWith);

	void parseLine(string, string);
	void setSynonymList(vector<SynonymEntityPair> synonymList);
	bool isValidSelect(vector<string> vectorClauses);
	bool isValidClauses(vector<string> vectorClauses);
	bool isValidSuchThat(string str, string syn);
	string checkDeclaration(string arg);
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
public:
	ParserQuery();
	int getNumClauses();
	string changeLowerCase(string str);
	void startParsing();

};
