#include "stdafx.h"
#include "ParserQuery.h"
#include <iostream>
#include <string>
#include <vector>
#include <ctype.h>
#include <stdio.h>
#include <regex>
#include <fstream>
#include "stdafx.h"

using namespace std;
ParserQuery::ParserQuery()
{
	//entityList, synonymList needed

	vector<string> entityList = vector<string>();
	vector<string> synonymList = vector<string>();
	int numClauses = 0;
	string declarationString;
	string queryString;
	vector<string> validEntities = { "procedure", "stmtLst", "stmt", "assign", "call", "while", "if",
		"variable", "constant", "prog_line" };
}

void ParserQuery::startParsing() {
	cout << "Parsing input file" << endl;
	std::ifstream file("Example.txt");
	std::string str;
	string currentString;
	while (std::getline(file, str)) {
		parseLine(str, currentString);
	}


}

//This function takes in 1 line of string and does either declaration check
void ParserQuery::parseLine(string str, string currentString) {


	//Attempt without regex
	//cout << "Test" + str << endl;
	string colonSymbol = ";";

	//If the current String consists of ';'
	if (str.find(colonSymbol)) {
	std:size_t found = str.find_first_of(";");
		while (found != std::string::npos) {
			currentString = parseEntityOrQuery(currentString, found);
			found = str.find_first_of(";", found + 1);
		}
	}
	//Line does not consist of ";", so we just add on whatever that is available from input
	//Or current String may consist of Select Statement which does not consist of ";"
	else {
		if ((currentString.length() >= 6) && (currentString.substr(0, 5) == "Select")) {
			currentString = appendAdditionalSpace(currentString, str);
		}
		else {
			currentString = appendAdditionalSpace(currentString, str);
		}
	}
}

string ParserQuery::appendAdditionalSpace(string currentString, string str) {
	if (isspace(currentString.at(currentString.length()))) {
		currentString += str;
		return currentString;
	}
	else {
		currentString += (" " + str);
		return currentString;
	}
}
/*
//	while()
//Attempt with regex
//If its a declaration line or part of the declarations

regex regexDeclare("(([[:w:]]+)[\s|\.|\;]?)");

sregex_token_iterator pos(str.cbegin(), str.cend(), regexDeclare);
sregex_token_iterator end;

for (; pos != end; pos++) {
cout << "Matched: " << pos->str() << endl;
}*/

ParserQuery::~ParserQuery()
{

}
string ParserQuery::parseEntityOrQuery(string currentString, int found) {
	string selectStrQuery = "Select";

	//If the currentString has greater size than 5 and first five words matches "Select", it is not a declaration string 
	if ((currentString.length() >= 6) && (currentString.substr(0, 5)) == (selectStrQuery)) {
		parseQueryStatement(currentString);

		return "";
	}
	else {
		parseEntityAndSynonym(currentString);
		return "";
	}
}

void ParserQuery::parseQueryStatement(string currentString) {

}

//Parse EntityAndSynonym checks for validity of the entitiy by comparing with the validEntitiesList, if its valid, it will proceed to adding the synonym
void ParserQuery::parseEntityAndSynonym(string currentString) {
	//splitStringByWhiteSpace()
}
vector<string> ParserQuery::getValidEntities() {
	return this->validEntities;
}

int ParserQuery::getNumClauses() {
	return this->numClauses;
}
