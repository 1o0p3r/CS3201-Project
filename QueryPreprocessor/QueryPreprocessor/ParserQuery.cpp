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
#include <sstream>

const char SYMBOL_WHITESPACE = ' ';
const char SYMBOL_COMMA = ',';
const char SYMBOL_LEFT_BRACKET = '(';
const char SYMBOL_RIGHT_BRACKET = ')';

const string SYMBOL_SEMI_COLON = ";";
const string SELECT_STRING = "Select";
using namespace std;
ParserQuery::ParserQuery()
{
	//entityList, synonymList needed

	vector<string> entityList;
	vector<vector<string>> synonymList;
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

void ParserQuery::parseLine(string str, string currentString) {

	//If currentLine consist of a ; It likely implies that a declaration is being done
	//So firstly check for the position of
	if (str.find(SYMBOL_SEMI_COLON)) {
		std::size_t found = str.find_first_of(SYMBOL_SEMI_COLON);
		while (found != std::string::npos) {
			currentString = parseEntity(currentString, str, found);
			found = str.find_first_of(SYMBOL_SEMI_COLON, found + 1);
		}
	}
}

//This function
string ParserQuery::parseEntity(string currentString, string str, int found) {
	//If the currentString has greater size than 5 and first five words matches "Select", it is not a declaration string 
	if ((currentString.length() >= 6) && (currentString.substr(0, 5)) == (SELECT_STRING)) {
		//This implies that an error has occured as the Select clause is not supposed to have ;
		cout << "Wrong syntax of Select clause, please double check" << endl;
		exit(0);
	}
	//This implies that a colon has been encountered and a declaration has likely finished
	else {
		if (checkEntityAndSynonym(currentString, str)) {
			return "";
		}
		else {
			cout << "Parsing Error on Entities or Synonyms" << endl;
		}
	}

}
//Parse EntityAndSynonym checks for validity of the entitiy by comparing with the validEntitiesList, if its valid, it will proceed to adding the synonym
bool ParserQuery::checkEntityAndSynonym(string currentString, string str) {
	//first append the 2 string then split by whitespace/,
	currentString += currentString + str;
	if (isValidEntity(currentString)) {
		return true;
	}
	else {
		cout << "Invalid Entity entered" << endl;
		return false;
	}
}

//This function checks for the validity of the declaration by comparing with the the entityList given
//Returns true if there is a match, else returns false
bool ParserQuery::isValidEntity(string currentString) {
	std::vector<string> splitString = split(currentString, SYMBOL_WHITESPACE);
	if (inEntityList(splitString.front())) {
		
		if (parseDeclaration(splitString)) {
			return true;
		}
		else {
			return false;
		}
	} 
	else {
		return false;
	}
}

//Checks if a given entity is in the EntityTable
bool ParserQuery::InEntityList(string entity) {
	vector<string> entityList = this->validEntities;

	for (int i = 0; i < entityList.size; i++) {
		if (entity == entityList.at(i)) {
			return true;
		}
	}
	return false;
}
//Parses the synonym with the associated entity into the synonymTable
bool ParserQuery::parseDeclaration(vector<string> splitString) {
	vector<vector<string>> tempSynonymList;
	string entity = splitString.front();
	splitString.erase(splitString.begin());
	string variableStr;
	for (int i = 0; i < splitString.size(); i++) {
		variableStr += splitString.at(i);
	}
	vector<string> synonyoms = split(variableStr, SYMBOL_COMMA);
}

//Splits the string into vectors of string with the required symbol and returns a vector of string, mainly for usage of declaration
vector<string> ParserQuery::split(string str, char symbolSplitWith) {
	vector<string> result = vector<string>();
	std::istringstream ss(str);
	std::string token;

	while (std::getline(ss, token, symbolSplitWith)) {
		result.push_back(token);
	}
	return result;
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


ParserQuery::~ParserQuery()
{

}

void ParserQuery::parseQueryStatement(string currentString) {

}

void ParserQuery::setSynonymList(vector<vector<string>> sList) {
	this->synonymList = sList;
}
vector<string> ParserQuery::getValidEntities() {
	return this->validEntities;
}

int ParserQuery::getNumClauses() {
	return this->numClauses;
}

/**
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
**/

/**
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
//	currentString = appendAdditionalSpace(currentString, str);
}
else {
currentString = appendAdditionalSpace(currentString, str);
}
}
}
**/