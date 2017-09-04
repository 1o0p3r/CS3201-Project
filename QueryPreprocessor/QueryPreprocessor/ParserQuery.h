#pragma once
#include <string>
#include <vector>
#include "stdafx.h"

using std::string;
using std::vector;

class ParserQuery
{
private:
	vector<string> validEntities;
	int numClauses;
	string declarationString;
	string queryString;
	void parseQueryStatement(string);
	void parseEntityAndSynonym(string);

	void parseLine(string, string);
	string parseEntityOrQuery(string, int);
	string appendAdditionalSpace(string, string);
public:
	ParserQuery();
	~ParserQuery();
	vector<string> getValidEntities();
	int getNumClauses();
	void startParsing();
};
