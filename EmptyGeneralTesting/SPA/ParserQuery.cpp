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
#include <regex>

const int ZERO = 0;
const int ONE = 1;
const int TWO = 2;
const char SYMBOL_WHITESPACE = ' ';
const char SYMBOL_COMMA = ',';
const char SYMBOL_LEFT_BRACKET = '(';
const char SYMBOL_RIGHT_BRACKET = ')';

const string SYMBOL_SEMI_COLON = ";";
const string SELECT_STRING = "Select";
const string COMMENT_STRING = "comment";
const string SUCH_THAT_STRING = "such that";
const string PATTERN_STRING = "pattern";
const string COMMA_WHITESPACE_STRING = ", ";
const string FOLLOWS_STRING = "Follows";
const string FOLLOWS_STAR_STRING = "Follows*";
const string PARENT_STRING = "Parent";
const string PARENT_STAR_STRING = "Parent";
const string MODIFIES_STRING = "Modifies";
const string USES_STRING = "Uses";
const string UNDER_SCORE_STRING = "_";
const string WILDCARD_STRING = "wildcard";
const string CONSTANT_STRING = "constant";
const string RELATIONSHIP_STRING = "relationship";
const string SYMBOL_LEFT_BRACKET_STRING = "(";
const string SYMBOL_RIGHT_BRACKET_STRING = ")";
const string NUMBER_STRING = "number";
const string PATTERN_REGEX = "pattern \w+[(][^\s]+,\s[^\s]+[)]";

using namespace std;
ParserQuery::ParserQuery()
{
	//entityList, synonymList needed

	int numClauses = ZERO;
	vector<string> validEntities = { "procedure", "stmtLst", "stmt", "assign", "call", "while", "if",
		"variable", "constant", "prog_line" };
}
void ParserQuery::startParsing() {
	cout << "Parsing input file" << endl;
	std::ifstream file("Example.txt");
	std::string str;
	string currentString;
	while (std::getline(file, str)) {
		if (currentString.find(COMMENT_STRING)) {
			//Everytime a query is done, the string needs to be changed, the queryTree need to be changed,
			//The synonymAndEntityList need to be changed.
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
		size_t pos = ZERO;
		std::string token;
		string originalString = str;
		while ((pos = str.find(SYMBOL_SEMI_COLON) != std::string::npos)) {
			token = str.substr(ZERO, pos);	//gets the string up to ;
			
			if(!parseEntityAndSynonym(token)) {
				cout << "Error occured while parsing Entity and Synonym, please key in a valid Entity";
			}
			//After parsing 1 token, erase the parsed token
			else {
				str.erase(ZERO, pos + ONE);
			}
		}
		//Since the last found will be the index of a whitespace, before a select statement is here
		//Make sure that it is indeed a whitespace first, before proceeding to parse the select clauses
		if (str.find(SELECT_STRING)) {
			//parseSelectClauses(str);
			parseQueryLine(str);
		}
		else {
			cout << "Syntax is wrong, please key in the proper syntax for query";
		}
	}
	else { 
		cout << "Syntax is wrong, please key in semi colon";
		exit(ZERO);
	}
}

//This function parses the Entity And Synonym and checks for the validity of Entity
bool ParserQuery::parseEntityAndSynonym(string currentString) {

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

void ParserQuery::parseQueryLine(string str) {
	if (!isValidQueryLine(str)) {
		cout << "Invalid queryLine" << endl;
	}
}
//Parse EntityAndSynonym checks for validity of the entitiy by comparing with the validEntitiesList, if its valid, it will proceed to adding the synonym
bool ParserQuery::checkEntityAndSynonym(string currentString) {

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
	
	for (std::size_t i=ZERO; i < entityList.size(); i++) {
		if (entity == entityList.at(i)) {
			return true;
		}
	}
	return false;
}
//Parses the synonym with the associated entity into the synonymTable
bool ParserQuery::parseDeclaration(vector<string> splitString) {
	
	//Obtains the entity and erase it from vector
	string reqEntity = splitString.front();
	splitString.erase(splitString.begin());
	string synonymsStr;
	//Adds back the rest of the string into 1 string
	for (std::size_t i = ZERO; i < splitString.size(); i++) {
		synonymsStr += splitString.at(i);
	}
	//Split the synonymsStr by comma to obtain relevant synonyms create object SynonymEntityPair and places in the "EntTable"
	vector<string> synonymsAttached = split(synonymsStr, SYMBOL_COMMA);
	SynonymEntityPair tempPair = SynonymEntityPair(reqEntity, synonymsAttached);
	synonymAndEntityList.push_back(tempPair);
	return true;
}

bool ParserQuery::isValidQueryLine(string selectString) {

	vector<string> initialVector;
	initialVector.push_back(selectString);

	initialVector = splitStatement(initialVector);

	//After splitting do validation different parts of the vector

	if(isValidSelect(initialVector) && isValidOthers(initialVector)) {
		return true;
	}
	else {
		return false;

	}
}
bool ParserQuery::isValidPattern(string str, string syn) {
	//Idea: From given string, ignore Pattern, obtain (arg1, arg2)
	//Example of current string : pattern a(v,"procs*ifs")	or pattern a(_,_"procs*while"_)
	std::regex patternRgx(PATTERN_REGEX);
	
	if (std::regex_match(str, patternRgx)) {
		//do my stuff as the syntax matches and i can obtain my string in a hardcode way

		//remove the the string pattern
		string tempPatternStr = str.substr(8, str.length() - ONE);

		//Obtain the index of left bracket
		int idxLeft = tempPatternStr.find(SYMBOL_LEFT_BRACKET);

		//Create new string to store PatternStr
		string patternStr = tempPatternStr.substr(idxLeft, tempPatternStr.length() - ONE);
		
		patternStr = removeSymbols(patternStr, SYMBOL_LEFT_BRACKET_STRING);
		patternStr = removeSymbols(patternStr, SYMBOL_RIGHT_BRACKET_STRING);

		//Split the string further by comma and space
		vector<string> splitPattern = split(patternStr, COMMA_WHITESPACE_STRING);

		if (splitPattern.size() == ONE) {
			return false;
		}
		string arg1 = splitPattern.at(ZERO);
		string arg2 = splitPattern.at(ONE);

		string ent = getCorrespondingEntity(syn);
		addPatternQueryElement(arg1, arg2, ent, syn);
	}
	else {
		return false;
	}
}

vector<string> ParserQuery::splitStatement(vector<string> currentVector) {
	//Split by such that
	currentVector = split(currentVector, SUCH_THAT_STRING);

	//Split by pattern
	currentVector = split(currentVector, PATTERN_STRING);

	return currentVector;
}
//This function function checks for the select portion of the select query and ensure that the synonym
bool ParserQuery::isValidSelect(vector<string> vectorClauses) {
	//First element of this vector gives the select clause
	string selectClause = vectorClauses.at(ZERO);
	vector<string> selectClauseSplit = split(selectClause, SYMBOL_WHITESPACE);
	string syn = selectClauseSplit.at(ONE);
	vector<string> tempVec;
	tempVec.push_back(syn);
	if (isValidSynonym(syn)) {
		//Add to the tree Select clause
		//I neeed entity and synonym, synonym is now valid and can be used, nw i need toget corresponding entity
		string ent = getCorrespondingEntity(syn);
		addSelectQueryElement(ent, syn);
		
		return true;
	}
	else {
		cout << "Invalid synonym query";
		return false;
	}
}
void ParserQuery::addSelectQueryElement(string ent, string syn) {
	queryStatement.addSelectQuery(QueryElement(ent, syn));
}
void ParserQuery::addPatternQueryElement(string arg1, string arg2, string ent, string syn) {
	queryStatement.addPatternQuery(QueryElement(arg1, arg2, ent, syn, ONE));
}
void ParserQuery::addSuchThatQueryElement(QueryElement qe) {
	queryStatement.addSuchThatQuery(qe);
}
bool ParserQuery::isValidOthers(vector<string> vec) {
	if (vec.size() < 2) {
		return true;
	}
	else {
		string select = vec.at(ZERO);
		vector<string> vecSplit = split(select, SYMBOL_WHITESPACE);
		
		string syn = vecSplit.at(ONE);
		for (size_t i = ONE; i < vecSplit.size(); i++) {
			if (vecSplit.at(i).find(SUCH_THAT_STRING)) {
				return isValidSuchThat(vecSplit.at(i), syn);
			}
			else if (vecSplit.at(i).find(PATTERN_STRING)) {
				return isValidPattern(vecSplit.at(i), syn);
			}
			else {
				return false;
			}
		}
	}
}
bool ParserQuery::isValidSuchThat(string str, string syn) {
	//Firstly extract/split till a relation is found
	string tempString = str.substr(9, str.length()-ONE);

	vector<string> tempVec = split(tempString, SYMBOL_LEFT_BRACKET);
	string relation = tempVec.at(ZERO);
	string argsWithoutBracket = tempVec.at(ONE).substr(ONE, tempVec.at(ONE).length() - 2);
	vector<string> args = split(argsWithoutBracket, COMMA_WHITESPACE_STRING);

	string arg1 = args.at(ZERO);
	string arg2 = args.at(ONE);

	string arg1Ent = getCorrespondingEntity(arg1);	//Attempts to get corresponding entity for each entity
	string arg2Ent = getCorrespondingEntity(arg2);	//E.g. Follows(s,4).	type1= stmt, type2 = number
	
	bool arg1Valid = false;
	bool arg2Valid = false;
	bool arg1_NUM = false;
	bool arg2_NUM = false;
	bool arg1_UNDER = false;
	bool arg2_UNDER = false;

	if (arg1Ent != "nth") {
		//Implies that a corresponding entity was obtained
		arg1Valid = checkRelationshipTable(relation, arg1Ent, 1);
	}
	else {
		//If no correspond entity was obtained, try to check if it is a number or wildcard
		if (is_number(arg1Ent)) {
			//If exists in table and part of declared relationship, create a clause and add it to query tree
			arg1Valid = checkRelationshipTable(relation, NUMBER_STRING, ONE);
			if (arg1Valid) {
				arg1_NUM = true;
			}
			else {
				arg1_NUM = false;
			}
		}
		else if (arg1Ent == UNDER_SCORE_STRING) {
			arg1Valid = checkRelationshipTable(relation, WILDCARD_STRING, ONE);
			if (arg1Valid) {
				arg1_UNDER = true;
			}
			else {
				arg1_UNDER = false;
			}
		}
		else {
			arg1Valid = false;
		}
	}
	//Implies that it is part of the delcared synonym
	if (arg2Ent != "nth") {
		arg2Valid = checkRelationshipTable(relation, arg2Ent, TWO);
	}
	else {
		if (is_number(arg2Ent)) {
			arg2Valid = checkRelationshipTable(CONSTANT_STRING, NUMBER_STRING, TWO);
			if (arg2Valid) {
				arg2_NUM = true;
			}
			else {
				arg2_NUM = false;
			}
		}
		else if (arg2Ent == UNDER_SCORE_STRING) {
			arg2Valid = checkRelationshipTable(UNDER_SCORE_STRING, WILDCARD_STRING, TWO);
			if (arg2Valid) {
				arg2_UNDER = true;
			}
			else {
				arg2_UNDER = false;
			}
		}
		else {
			arg2Valid = false;
		}
	}
	//If both are valid and true, create the clause
	if (arg1Valid  && arg2Valid) {
		if (!addSuchThatQueryElement(arg1_NUM, arg1_UNDER, arg2_NUM, arg2_UNDER, relation, arg1, arg2, arg1Ent, arg2Ent)) {
			cout << "Error ending clause to tree";
			return false;
		}
	}
	else {
		return false;
	}
	
}

bool ParserQuery::addSuchThatQueryElement(bool arg1_NUM, bool arg1_UNDER, bool arg2_NUM, bool arg2_UNDER, string relType, string arg1, string arg2, string type1, string type2) {
	if (arg1_NUM == false && arg1_UNDER == false) {
		//If a match is found create a query element with following parameters (arg1, arg1Type, arg2, arg2Type, rel)
		if (arg2_NUM == false && arg2_UNDER == false) {
			QueryElement queryElement = QueryElement(arg1, type1, arg2, type2, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		else if (arg2_NUM == false && arg2_UNDER == true) {
			//Implies that the clause for arg1 is not a num/under, arg2 is not a num, arg2 is an UNDER
			QueryElement queryElement = QueryElement(arg1, type1, UNDER_SCORE_STRING, WILDCARD_STRING, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		else if (arg2_NUM == true && arg2_UNDER == false) {
			QueryElement queryElement = QueryElement(arg1, type1, arg2, NUMBER_STRING, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		else {
			return false;
		}
	}
	else if (arg1_NUM == true && arg1_UNDER == false) {
		if (arg2_NUM == false && arg2_UNDER == false) {
			QueryElement queryElement = QueryElement(arg1, NUMBER_STRING, arg2, type2, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		else if (arg2_NUM == false && arg2_UNDER == true) {
			QueryElement queryElement = QueryElement(arg1, NUMBER_STRING, UNDER_SCORE_STRING, WILDCARD_STRING, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		else if (arg2_NUM == true && arg2_UNDER == false) {
			QueryElement queryElement = QueryElement(arg1, NUMBER_STRING, arg2, NUMBER_STRING, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		else {
			return false;
		}
	}
	else if (arg1_NUM == false && arg1_UNDER == true) {
		if (arg2_NUM == false && arg2_UNDER == false) {
			QueryElement queryElement = QueryElement(UNDER_SCORE_STRING, WILDCARD_STRING, arg2, type2, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		else if (arg2_NUM == false && arg2_UNDER == true) {
			QueryElement queryElement = QueryElement(UNDER_SCORE_STRING, WILDCARD_STRING, UNDER_SCORE_STRING, WILDCARD_STRING, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		else if (arg2_NUM == true && arg2_UNDER == false) {
			QueryElement queryElement = QueryElement(UNDER_SCORE_STRING, WILDCARD_STRING, arg2, NUMBER_STRING, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		else {
			return false;
		}
	}
}

//This function takes in e.g. str = Follows, type = stmt, idx = 1
bool ParserQuery::checkRelationshipTable(string rel, string type, int idx) {
	if (relationshipTable.isValidArg(rel, type, idx)) {
		return true;
	}
	else {
		return false;
	}
}
string ParserQuery::getCorrespondingEntity(string syn) {
	string reqEntity = "nth";
	for (size_t i = ZERO; i < synonymAndEntityList.size(); i++) {
		SynonymEntityPair tempPair = synonymAndEntityList.at(i);
		vector<string> tempVec = tempPair.getSynonymList();
		for (size_t j = ZERO; j < tempVec.size(); j++) {
			if (tempVec.at(j) == syn) {
				reqEntity = tempPair.getEntity();
				return reqEntity;
			}
		}
	}
	return reqEntity;
}
bool ParserQuery::is_number(string s) {
	return s.find_first_not_of("0123456789") == string::npos;
}

//This function takes in a string to check if synonym asked exists in the SynonymEntityPair
bool ParserQuery::isValidSynonym(string syn) {
	for (size_t i = ZERO; i < synonymAndEntityList.size(); i++) {
		SynonymEntityPair tempPair = synonymAndEntityList.at(i);
		vector<string> tempVec = tempPair.getSynonymList();
		for (size_t j = ZERO; j < tempVec.size(); j++) {
			if (changeLowerCase(tempVec.at(j)) ==  changeLowerCase(syn)){
				return true;
			}
		}
	}
	return false;
}
//Splits the string into vectors of string with the required symbol and returns a vector of string, mainly for usage of declaration
vector<string> ParserQuery::split(string str, char symbolSplitWith) {
	
	vector<string> result = vector<string>();
	size_t pos = ZERO;
	std::string token;

	while ((pos = str.find(symbolSplitWith)) != std::string::npos) {
		token = str.substr(ZERO, pos);
		result.push_back(token);
		str.erase(ZERO, pos + 1);
	}
	return result;
}

vector <string> ParserQuery::split(string str, string symbolSplitWith) {

	vector<string> result = vector<string>();
	size_t pos = ZERO;
	std::string token;

	while ((pos = str.find(symbolSplitWith)) != std::string::npos) {
		token = str.substr(ZERO, pos);
		result.push_back(token);
		size_t toAdd = symbolSplitWith.length();
		str.erase(ZERO, pos+toAdd);
	}
	return result;
}

vector<string> ParserQuery::split(vector<string> vectorToSplit, string strToSplitWith) {

	vector<string> result = vector<string>();
	size_t pos = ZERO;
	std::string token;
	for (size_t i = ZERO; i < vectorToSplit.size(); i++) {
		while ((pos = vectorToSplit.at(i).find(strToSplitWith)) != std::string::npos) {
			token = vectorToSplit.at(i).substr(ZERO, pos);
			result.push_back(token);
			vectorToSplit.at(i).erase(ZERO, pos + strToSplitWith.length());
		}
	}
	return result;
}
string ParserQuery::removeSymbols(string str, string symbolToRemove) {
	string toReturn = "";
	size_t pos = ZERO;
	std::string token;

	while ((pos = str.find(symbolToRemove)) != std::string::npos) {
		token = str.substr(ZERO, pos);
		toReturn += token;
		size_t toAdd = symbolToRemove.length();
		str.erase(ZERO, pos + toAdd);
	}
	return toReturn;
}
void ParserQuery::setSynonymList(vector<SynonymEntityPair> sList) {
	this->synonymAndEntityList = sList;
}
int ParserQuery::getNumClauses() {
	return this->numClauses;
}
string ParserQuery::changeLowerCase(string str) {
	 std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	 return str;
}