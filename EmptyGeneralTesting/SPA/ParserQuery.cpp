#include "ParserQuery.h"
#include <iostream>
#include <string>
#include <vector>
#include <ctype.h>
#include <stdio.h>
#include <SynonymEntityPair.h>
#include <regex>
#include <fstream>
#include <sstream>

const char SYMBOL_WHITESPACE = ' ';
const char SYMBOL_COMMA = ',';
const char SYMBOL_LEFT_BRACKET = '(';
const char SYMBOL_RIGHT_BRACKET = ')';

const string SYMBOL_SEMI_COLON = ";";
const string SELECT_STRING = "Select";
const string COMMENT_STRING = "comment";
const string SUCH_THAT_STRING = "such that";
const string PATTERN_STRING = "pattern";

using namespace std;
ParserQuery::ParserQuery()
{
	//entityList, synonymList needed

	vector<SynonymEntityPair> synonymAndEntityList;
	int numClauses = 0;
	string declarationString;
	string queryString;
	vector<string> validEntities = { "procedure", "stmtLst", "stmt", "assign", "call", "while", "if",
		"variable", "constant", "prog_line" };
}
ParserQuery::~ParserQuery()
{

}
void ParserQuery::startParsing() {
	cout << "Parsing input file" << endl;
	std::ifstream file("Example.txt");
	std::string str;
	string currentString;
	while (std::getline(file, str)) {
		if (currentString.find(COMMENT_STRING)) {
			currentString.clear();
			str.clear();
		}
		else {
			parseLine(str, currentString);
		}
	}
}

void ParserQuery::parseLine(string str, string currentString) {

	//If currentLine consist of a ; It likely implies that a declaration is being done
	//So firstly check for the position of
	if (str.find(SYMBOL_SEMI_COLON)) {
		std::size_t found = str.find_first_of(SYMBOL_SEMI_COLON);
		while (found != std::string::npos) {
			currentString = parseEntityAndSynonym(currentString, str, found);
			found = str.find_first_of(SYMBOL_SEMI_COLON, found + 1);
		}
		//Since the last found will be the index of a whitespace, before a select statement is here
		//Make sure that it is indeed a whitespace first, before proceeding to parse the select clauses
		if (str.find(SELECT_STRING)) {

		}
		else {
			cout << "Syntax is wrong, please key in the proper syntax for query";
		}
	}
	else { 
		cout << "Syntax is wrong, please key in semi colon";
		exit(0);
	}
}

//This function
string ParserQuery::parseEntityAndSynonym(string currentString, string str, int found) {
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
bool ParserQuery::inEntityList(string entity) {
	vector<string> entityList = validEntities;
	
	for (std::size_t i=0; i < entityList.size(); i++) {
		if (entity == entityList.at(i)) {
			return true;
		}
	}
	
	/**for (std::vector<string>::iterator it = entityList.begin(); it != entityList.end(); it++) {

	}**/
	return false;
}
//Parses the synonym with the associated entity into the synonymTable
bool ParserQuery::parseDeclaration(vector<string> splitString) {

	string entity = splitString.front();
	splitString.erase(splitString.begin());
	string variableStr;
	for (std::size_t i = 0; i < splitString.size(); i++) {
		variableStr += splitString.at(i);
	}
	//Obtain relevant synonyms
	vector<string> synonymsAndEntity = split(variableStr, SYMBOL_COMMA);
	string reqEntity = synonymsAndEntity.at(0);
	if (isValidEntity(reqEntity)) {
		synonymsAndEntity.erase(synonymsAndEntity.begin());
		SynonymEntityPair tempPair = SynonymEntityPair(reqEntity, synonymsAndEntity);
		synonymAndEntityList.push_back(tempPair);
		return true;
	}
	else {
		//To be done
		
		return false;
	}
}

//This function assumes that the inputString has left out the semi_colon
bool parseSelectClauses(string selectString) {
	if (selectString.find(SELECT_STRING)) {

	}
	else {
		return false;
	}
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

void ParserQuery::parseQueryStatement(string currentString) {

}
void ParserQuery::setSynonymList(vector<SynonymEntityPair> sList) {
	this->synonymAndEntityList = sList;
}
int ParserQuery::getNumClauses() {
	return this->numClauses;
}