#pragma once
#include <string>
#include <vector>
#include <SynonymEntityPair.h>
#include <RelationshipTable.h>
#include <Relationship.h>

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
	vector <string> declarationString;
	vector <string> queryString;

	void parseLine(string, string);
	void setSynonymList(vector<SynonymEntityPair> synonymList);

	bool parseEntityAndSynonym(string);
	bool checkEntityAndSynonym(string);
	bool isValidEntity(string);
	bool inEntityList(string entity);
	bool parseDeclaration(vector<string> splitString);
	
public:
	ParserQuery();
	int getNumClauses();
	void startParsing();
};
