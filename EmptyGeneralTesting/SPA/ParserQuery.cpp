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

	int numClauses = 0;
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
			synonymAndEntityList.clear();
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
		/**
		int initialIndex = 0;
		std::size_t found = str.find_first_of(SYMBOL_SEMI_COLON);
		while (found != std::string::npos) {
		//	currentString = parseEntityAndSynonym(currentString, str, found);
		//	found = str.find_first_of(SYMBOL_SEMI_COLON, found + 1);
			currentString = str.substr(initialIndex, found);
			parseEntityAndSynonym(currentString, str, found);
			found = str.find_first_of(SYMBOL_SEMI_COLON, found + 1);
		}**/
		size_t pos = 0;
		std::string token;
		string originalString = str;
		while ((pos = str.find(SYMBOL_SEMI_COLON) != std::string::npos)) {
			token = str.substr(0, pos);	//gets the string up to ;
			
			if(!parseEntityAndSynonym(token)) {
				cout << "Error occured while parsing Entity and Synonym, please key in a valid Entity";
			}
			//After parsing 1 token, erase the parsed token
			else {
				str.erase(0, pos + 1);
			}
		}
		//Since the last found will be the index of a whitespace, before a select statement is here
		//Make sure that it is indeed a whitespace first, before proceeding to parse the select clauses
		if (str.find(SELECT_STRING)) {
			parseSelectClauses(str);
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

//This function parses the Entity And Synonym and checks for the validity of Entity
bool ParserQuery::parseEntityAndSynonym(string currentString) {
	/**
	
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
	} **/
	if (currentString.find(SELECT_STRING)) {
		cout << "Error occured, please check syntax of the query";
		return false;
	}
	else {
		if (checkEntityAndSynonym(currentString)) {
			return true;
		}
		else {
			return false;
		}
	}
}
//Parse EntityAndSynonym checks for validity of the entitiy by comparing with the validEntitiesList, if its valid, it will proceed to adding the synonym
bool ParserQuery::checkEntityAndSynonym(string currentString) {
//	currentString += currentString + str;
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
	//checks if the entity exists, if so, do parsing on the declarations
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
	
	//Obtains the entity and erase it from vector
	string reqEntity = splitString.front();
	splitString.erase(splitString.begin());
	string synonymsStr;
	//Adds back the rest of the string into 1 string
	for (std::size_t i = 0; i < splitString.size(); i++) {
		synonymsStr += splitString.at(i);
	}
	//Split the synonymsStr by comma to obtain relevant synonyms create object SynonymEntityPair and places in the "EntTable"
	vector<string> synonymsAttached = split(synonymsStr, SYMBOL_COMMA);
	SynonymEntityPair tempPair = SynonymEntityPair(reqEntity, synonymsAttached);
	synonymAndEntityList.push_back(tempPair);
	return true;
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
	size_t pos = 0;
	std::string token;

	while ((pos = str.find(symbolSplitWith)) != std::string::npos) {
		token = str.substr(0, pos);
		result.push_back(token);
		str.erase(0, pos + 1);
	}
	return result;
}

vector <string> ParserQuery::split(string str, string symbolSplitWith) {

	vector<string> result = vector<string>();
	size_t pos = 0;
	std::string token;

	/**
	whe (std::getline(ss, token, symbolSplitWith)) {
	result.push_back(token);
	}
	return result;
	**/
	while ((pos = str.find(symbolSplitWith)) != std::string::npos) {
		token = str.substr(0, pos);
		result.push_back(token);
		str.erase(0, pos + symbolSplitWith.length);
	}
	return result;
}
void ParserQuery::setSynonymList(vector<SynonymEntityPair> sList) {
	this->synonymAndEntityList = sList;
}
int ParserQuery::getNumClauses() {
	return this->numClauses;
}