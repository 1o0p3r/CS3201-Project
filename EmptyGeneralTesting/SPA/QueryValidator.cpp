#include "QueryValidator.h"
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
#include <algorithm>

const int ZERO = 0;
const int ONE = 1;
const int TWO = 2;
const char SYMBOL_WHITESPACE = ' ';
const char SYMBOL_COMMA = ',';
const char SYMBOL_LEFT_BRACKET = '(';
const char SYMBOL_RIGHT_BRACKET = ')';

const string WHITESPACE_STRING = " ";
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
QueryValidator::QueryValidator()
{
	//entityList, synonymList needed

	int numClauses = ZERO;

}

//This function takes in a string query
void QueryValidator::startParsing(string str) {
	/**
	
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
			queryStatement = QueryStatement();	//Make a new query statement after every query is done
			validDeclaration = true;
			validQuery = true;
		}
		else {
			parseLine(str, currentString);
		}
	}**/
	parseInput(str);
}

//With a given str, loop thru to find ;
bool QueryValidator::parseInput(string str) {
	if (str.find(SYMBOL_SEMI_COLON)) {
		size_t pos = ZERO;
		std::string token;
		string originalStr = str;
		bool valDel = false;
		bool valQuery = false;

		//While i can find a semicolon
		while ((pos = str.find(SYMBOL_SEMI_COLON) != std::string::npos)) {
			token = str.substr(ZERO, pos);
			if (isEntityAndSynonym(token)) {
				valDel = true;
			}
			else {
				str.erase(ZERO, pos + ONE);
			}
		}
		//After the loop ends, comes the select query
		if (str.find(SELECT_STRING)) {
			valQuery = parseQueryLine(str);
		}
	}
	else {
		cout << "Syntax is wrong, please key in semi colon";
		return false;
	}
}

//This function parses the Entity And Synonym and checks for the validity of Entity
bool QueryValidator::isEntityAndSynonym(string currentString) {

	if (currentString.find(SELECT_STRING) == true) {
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

bool QueryValidator::parseQueryLine(string str) {
	if (!isValidQueryLine(str)) {
		validQuery = false;
		cout << "Invalid queryLine" << endl;
	}
	else {
		return true;
	}
}

bool QueryValidator::isValidDeclarationAndQuery() {
	return (validQuery && validDeclaration);
}
//Parse EntityAndSynonym checks for validity of the entitiy by comparing with the validEntitiesList, if its valid, it will proceed to adding the synonym
bool QueryValidator::checkEntityAndSynonym(string currentString) {

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
bool QueryValidator::isValidEntity(string currentString) {
	
	//Places the first vector with the unchecked entity, and subsequent vectors with synonyms
	std::vector<string> splitString = splitBySymbol(currentString, SYMBOL_WHITESPACE);
	//checks if the entity exists, if so, do parsing on the declarations
	if (inEntityList(splitString.front()) == true) {
		if (parseDeclaration(splitString) == true) {
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
bool QueryValidator::inEntityList(string entity) {
	
	for (size_t i =ZERO; i < validEntities.size(); i++) {
		if (entity == validEntities.at(i)) {
			return true;
		}
	}
	return false;
}
//Parses the synonym with the associated entity into the synonymTable
bool QueryValidator::parseDeclaration(vector<string> splitString) {
	
	//Obtains the entity and erase it from vector
	string reqEntity = splitString.front();
	splitString.erase(splitString.begin());
	string synonymsStr;
	//Adds back the rest of the string into 1 string
	for (std::size_t i = ZERO; i < splitString.size(); i++) {
		synonymsStr += splitString.at(i);
	}
	synonymsStr = removeSymbols(synonymsStr, WHITESPACE_STRING);
	//Split the synonymsStr by comma to obtain relevant synonyms create object SynonymEntityPair and places in the "EntTable"
	vector<string> synonymsAttached = splitBySymbol(synonymsStr, SYMBOL_COMMA);
	synonymAndEntityList.push_back(SynonymEntityPair(reqEntity, synonymsAttached));
	return true;
}

bool QueryValidator::isValidQueryLine(string selectString) {

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
bool QueryValidator::isValidPattern(string str, string syn) {
	//Idea: From given string, ignore Pattern, obtain (arg1, arg2)
	//Example of current string : pattern a(v,"procs*ifs")	or pattern a(_,_"procs*while"_)
	std::regex patternRgx(PATTERN_REGEX);
	
	if (std::regex_match(str, patternRgx)) {
		//remove the the string 'pattern'
		string tempPatternStr = str.substr(8, str.length() - ONE);

		//Obtain the index of first occurence of left bracket
		int idxLeft = tempPatternStr.find(SYMBOL_LEFT_BRACKET);

		//Create new string to store PatternStr
		string tempArgStr = tempPatternStr.substr(idxLeft, tempPatternStr.length() - ONE);
		
		//Remove all the unncessary whitespace
		string argStr = removeSymbols(tempPatternStr, WHITESPACE_STRING);

		vector<string> splitPattern = splitBySymbol(argStr, SYMBOL_COMMA);

		if (splitPattern.size() == ONE) {
			return false;
		}
		string arg1 = splitPattern.at(ZERO);
		string arg2 = splitPattern.at(ONE);

		string ent = getCorrespondingEntity(syn);
		addPatternQueryElement(arg1, arg2, ent, syn);
		
		return true;
	}
	else {
		return false;
	}
}

vector<string> QueryValidator::splitStatement(vector<string> currentVector) {
	//Split by such that
	currentVector = split(currentVector, SUCH_THAT_STRING);

	//Split by pattern
	currentVector = split(currentVector, PATTERN_STRING);

	return currentVector;
}
//This function function checks for the select portion of the select query and ensure that the synonym
bool QueryValidator::isValidSelect(vector<string> vectorClauses) {
	//First element of this vector gives the select clause
	string selectClause = vectorClauses.at(ZERO);

	vector<string> selectClauseSplit = splitBySymbol(selectClause, SYMBOL_WHITESPACE);
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
void QueryValidator::addSelectQueryElement(string ent, string syn) {
	queryStatement.addSelectQuery(QueryElement(ent, syn));
}
void QueryValidator::addPatternQueryElement(string arg1, string arg2, string ent, string syn) {
	queryStatement.addPatternQuery(QueryElement(arg1, arg2, ent, syn, ONE));
}
void QueryValidator::addSuchThatQueryElement(QueryElement qe) {
	queryStatement.addSuchThatQuery(qe);
}
bool QueryValidator::isValidOthers(vector<string> vec) {
	//This implies its a short query i.e. stmt s
	if (vec.size() < 2) {
		return true;
	}
	else {
		string select = vec.at(ZERO);
		vector<string> vecSplit = splitBySymbol(select, SYMBOL_WHITESPACE);
		
		//Obtain the synonym
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
bool QueryValidator::isValidSuchThat(string str, string syn) {
	//Firstly extract/split till a relation is found
	string tempString = str.substr(10, str.length()-ONE);

	//So sth like Follows(s,4) is tempString, after calling the next statement, [0]= Follows	[1] = s,4)
	vector<string> tempVec = splitBySymbol(tempString, SYMBOL_LEFT_BRACKET);
	
	//Obtain the relation Follows
	string relation = tempVec.at(ZERO);

	//Obtain the string argsWithoutBracket i.e.  s,4 is obtained
	string argsWithoutBracket = tempVec.at(ONE).substr(ZERO, tempVec.at(ONE).length()-2);

	//Remove all the whitespace first
	argsWithoutBracket = removeSymbols(argsWithoutBracket, WHITESPACE_STRING);

	//Split them by Comma
	vector<string> args = splitBySymbol(argsWithoutBracket, SYMBOL_COMMA);
	
	/**
	for (size_t i = 0; i < args.size(); i++) {
		args[i] = removeSymbols(args.at(i), WHITESPACE_STRING);
	}**/

	string arg1 = args.at(ZERO);
	string arg2 = args.at(ONE);

	//At this pt,
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
	if (arg1Valid && arg2Valid) {
		if (!addSuchThatQueryElement(arg1_NUM, arg1_UNDER, arg2_NUM, arg2_UNDER, relation, arg1, arg2, arg1Ent, arg2Ent)) {
			cout << "Error ending clause to tree";
			return false;
		}
		else {
			return true;
		}
	}
	else {
		return false;
	}
}

bool QueryValidator::addSuchThatQueryElement(bool arg1_NUM, bool arg1_UNDER, bool arg2_NUM, bool arg2_UNDER, string relType, string arg1, string arg2, string type1, string type2) {
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
bool QueryValidator::checkRelationshipTable(string rel, string type, int idx) {
	if (relationshipTable.isValidArg(rel, type, idx)) {
		return true;
	}
	else {
		return false;
	}
}
string QueryValidator::getCorrespondingEntity(string syn) {
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
bool QueryValidator::is_number(string s) {
	return s.find_first_not_of("0123456789") == string::npos;
}

//This function takes in a string to check if synonym asked exists in the SynonymEntityPair
bool QueryValidator::isValidSynonym(string syn) {
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
//Splits the string into vectors of string with the required symbol and returns a vector of string
vector<string> QueryValidator::splitBySymbol(string str, char symbolSplitWith) {
	
	vector<string> result = vector<string>();
	std::string token;

	istringstream f(str);
	while (getline(f, token, symbolSplitWith)) {
		result.push_back(token);
	}
	return result;
}

vector<string> QueryValidator::split(vector<string> vectorToSplit, string strToSplitWith) {
	vector<string> result;

	for (size_t i = 0; i < vectorToSplit.size(); i++) {
		string curr = vectorToSplit.at(i);
		size_t pos = curr.find(strToSplitWith);
		int count = 0;
		while (pos != std::string::npos) {
			if (count == 0) {
				string before = curr.substr(0, pos);
				result.push_back(before);
				curr = curr.substr(pos + strToSplitWith.length()+1, curr.length());
				pos = curr.find(strToSplitWith);
				count++;
			}
			else {
				string before = curr.substr(0, pos);
				result.push_back(strToSplitWith + before);
				curr = curr.substr(pos + strToSplitWith.length()+1, curr.length());
				pos = curr.find(strToSplitWith);
				count--;
			}
		}
		if (count == 1) {
			result.push_back(strToSplitWith + " " + curr);
			count--;
		}
		else {
			result.push_back(curr);
		}
	}
	//What i need to do: with a given vector
	return result;
}
string QueryValidator::removeSymbols(string str, string symbolToRemove) {
	std::regex pattern(symbolToRemove);
	std::string toReturn = std::regex_replace(str, pattern , "");
	return toReturn;
}
void QueryValidator::setSynonymList(vector<SynonymEntityPair> sList) {
	this->synonymAndEntityList = sList;
}
int QueryValidator::getNumClauses() {
	return this->numClauses;
}
string QueryValidator::changeLowerCase(string str) {
	 std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	 return str;
}