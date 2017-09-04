#pragma once
#include <string>
#include <vector>
#include "stdafx.h"

using std::string;
using std::vector;

class ParserQuery
{
private:
	int numClauses;
	vector<vector<string>> synonymList;
	vector<string> validEntities;
	string declarationString;
	string queryString;

	void parseQueryStatement(string);
	void parseLine(string, string);
	bool checkEntityAndSynonym(string, string);
	bool isValidEntity(string);
	bool InEntityList(string entity);
	bool inEntityList(string);
	string parseEntityOrQuery(string, int);
	string parseEntity(string , string, int);
	bool parseDeclaration(vector<string> splitString);
	string appendAdditionalSpace(string, string);
	vector<string> split(string, char);
	void setSynonymList(vector<vector<string>> synonymList);
public:
	ParserQuery();
	~ParserQuery();
	vector<string> getValidEntities();
	int getNumClauses();
	void startParsing();
};
