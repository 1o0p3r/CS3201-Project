#pragma once
#include <string>
#include <vector>
#include <SynonymEntityPair.h>

using std::string;
using std::vector;

class ParserQuery
{
private:
	int numClauses;
	vector<SynonymEntityPair> synonymAndEntityList;
	vector<string> validEntities;
	vector<string> split(string, char);

	string declarationString;
	string queryString;
	string parseEntityAndSynonym(string, string, int);
	

	void parseQueryStatement(string);
	void parseLine(string, string);
	void setSynonymList(vector<SynonymEntityPair> synonymList);

	bool checkEntityAndSynonym(string, string);
	bool isValidEntity(string);
	bool inEntityList(string entity);
	bool parseDeclaration(vector<string> splitString);
	
public:
	ParserQuery();
	~ParserQuery();
	vector<string> getValidEntities();
	int getNumClauses();
	void startParsing();
};
