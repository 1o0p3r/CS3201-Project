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
#include <QueryTree.h>

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

using namespace std;
ParserQuery::ParserQuery()
{
	//entityList, synonymList needed

	int numClauses = 0;
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
			removeTree(queryTree);
		}
		else {
			parseLine(str, currentString);
		}
	}
}
void ParserQuery::removeTree(QueryTree querytree) {
	QueryTree emptyTree;
	queryTree = emptyTree;
}
void ParserQuery::parseLine(string str, string currentString) {

	//If currentLine consist of a ; It likely implies that a declaration is being done
	//So firstly check for the position of
	if (str.find(SYMBOL_SEMI_COLON)) {
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
bool ParserQuery::parseSelectClauses(string selectString) {
	if (selectString.find(SELECT_STRING)) {
		isValidResultClauses(selectString);
		return true;
	}
	else {
		return false;
	}
}	

bool ParserQuery::isValidResultClauses(string selectString) {

	//First split the select string to different partition i.e. easier for query tree to process later on
	//Can do so by creating a vector of string initially with selectString inside
	vector<string> initialVector;
	initialVector.push_back(selectString);
	
	//This vector now contains of first element consisting of select sth, and subsequent elements are of different clauses
	initialVector = splitDiffClauses(initialVector);

	//Afters splitting, do validation on different parts of the vector
	if (isValidSelect(initialVector) && isValidClauses(initialVector)) {
		return true;
	}
	else {
		return false;
	}
}


bool ParserQuery::isValidPattern(string str, string syn) {
	//Idea: From given string, ignore Pattern, obtain (arg1, arg2)
	//Example of current string : pattern a(v,"procs*ifs")	or pattern a(_,_"procs*while"_)

	if (!str.find(SYMBOL_LEFT_BRACKET) || !str.find(SYMBOL_RIGHT_BRACKET)) {
		return false;
	}
	//Since index of  what we are interested in is from index 8 onwards, create a variable patternStr to hold the value we are interested in
	string tempPatternStr = str.substr(8, str.length()-1);

	//Now find the next occurence of "("
	int idxLeft = tempPatternStr.find(SYMBOL_LEFT_BRACKET);

	//Create new string to store PatternStr
	string PatternStr = tempPatternStr.substr(idxLeft, tempPatternStr.length()-1);

	//Clean up the unwated bracket
	PatternStr = removeSymbols(PatternStr, SYMBOL_LEFT_BRACKET_STRING);
	PatternStr = removeSymbols(PatternStr, SYMBOL_RIGHT_BRACKET_STRING);

	//Split the string further by comma and space
	vector<string> splitPattern = split(PatternStr, COMMA_WHITESPACE_STRING);
	
	if (splitPattern.size() == 1) {
		return false;
	}
	string arg1 = splitPattern.at(0);
	string arg2 = splitPattern.at(1);

	addPatternClauseTree(Clause(PATTERN_STRING, arg1, arg2));
	
	return true;
}
vector<string> ParserQuery::splitDiffClauses(vector<string> currentVector)
{
	//Split by such that
	currentVector = splitSelect(currentVector, SUCH_THAT_STRING);

	//Split by pattern
	currentVector = splitSelect(currentVector, PATTERN_STRING);

	return currentVector;
}

//This function function checks for the select portion of the select query and ensure that the synonym
bool ParserQuery::isValidSelect(vector<string> vectorClauses) {
	//First element of this vector gives the select clause
	string selectClause = vectorClauses.at(0);
	vector<string> selectClauseSplit = split(selectClause, SYMBOL_WHITESPACE);
	string syn = selectClauseSplit.at(1);
	vector<string> tempVec;
	tempVec.push_back(syn);
	if (isValidSynonym(syn)) {
		//Add to the tree Select clause
		addSelectClauseTree(Clause(SELECT_STRING, tempVec));
		return true;
	}
	else {
		cout << "Invalid synonym query";
		return false;
	}
}
bool ParserQuery::isValidClauses(vector<string> vectorClauses) {

	//First check if vectorClause is greater size than 1 to check for redundant statement like Select stmt s
	if (vectorClauses.size() < 2) {
		return true;
	}
	else {	//loop thru the rest of the clauses
		string selectClause = vectorClauses.at(0);
		vector<string> selectClausesSplit = split(selectClause, SYMBOL_WHITESPACE);
		string syn = selectClausesSplit.at(1);
		for (size_t i = 1; i < vectorClauses.size(); i++) {
			if (vectorClauses.at(i).find(SUCH_THAT_STRING)) {
				return isValidSuchThat(vectorClauses.at(i), syn);
			}
			else if (vectorClauses.at(i).find(PATTERN_STRING)) {
				return isValidPattern(vectorClauses.at(i), syn);
			}
			else {
				return false;
			}
		}
	}
}
bool ParserQuery::isValidSuchThat(string str, string syn) {
	//Firstly extract/split till a relation is found
	string tempString = str.substr(9, str.length()-1);

	vector<string> tempVec = split(tempString, SYMBOL_LEFT_BRACKET);
	string relation = tempVec.at(0);
	//string argsWithoutBracket = tempVec.at(1).substr(1, tempVec.at(1).end);
	string argsWithoutBracket = tempVec.at(1).substr(1, tempVec.at(1).length() - 2);
	vector<string> args = split(argsWithoutBracket, COMMA_WHITESPACE_STRING);

	string arg1 = args.at(0);
	string arg2 = args.at(1);

	string type1 = checkDeclaration(arg1);
	string type2 = checkDeclaration(arg2);
	bool arg1Valid = false;
	bool arg2Valid = false;
	bool arg1_NUM = false;
	bool arg2_NUM = false;
	bool arg1_UNDER = false;
	bool arg2_UNDER = false;

	//Implies that it is part of the declared synonym
	if (type1 != "nth") {
		arg1Valid = checkRelationshipTable(relation, type1, 1);
	}
	else {
		//Implies that it is not part of the declaration synonyms
		//So it could be either a digit or a wildcard
		if (is_number(type1)) {
			//If exists in table and part of declared relationship, create a clause and add it to query tree
			arg1Valid = checkRelationshipTable("constant", type1, 1);
			if (arg1Valid) {
				arg1_NUM = true;
			}
			else {
				arg1_NUM = false;
			}
		}
		else if (type1 == UNDER_SCORE_STRING) {
			arg1Valid = checkRelationshipTable(UNDER_SCORE_STRING, type1, 1);
			if (arg1Valid) {
				arg2_UNDER = true;
			}
			else {
				arg2_UNDER = false;
			}
		}
	}
	//Implies that it is part of the delcared synonym
	if (type2 != "nth") {
		arg2Valid = checkRelationshipTable(relation, type2, 2);
	}
	else {
		if (is_number(type2)) {
			arg2Valid = checkRelationshipTable(CONSTANT_STRING, type2, 2);
			if (arg2Valid) {
				arg2_NUM = true;
			}
			else {
				arg2_NUM = false;
			}
		}
		else if (type2 == UNDER_SCORE_STRING) {
			arg2Valid = checkRelationshipTable(UNDER_SCORE_STRING, type2, 2);
			if (arg2Valid) {
				arg2_UNDER = true;
			}
			else {
				arg2_UNDER = false;
			}
		}
	}
	//If both are valid and true, create the clause
	if (arg1Valid  && arg2Valid) {
		if (!createRelClause(arg1_NUM, arg1_UNDER, arg2_NUM, arg2_UNDER, relation, arg1, arg2)) {
			cout << "Error ending clause to tree";
			return false;
		}
	}
	else {
		return false;
	}
	
}
bool ParserQuery::createRelClause(bool arg1_NUM, bool arg1_UNDER, bool arg2_NUM, bool arg2_UNDER, string relType, string arg1, string arg2) {
	if (arg1_NUM == false && arg1_UNDER == false) {
		if (arg2_NUM == false && arg2_UNDER == false) {
			//This implies that the clause for arg1 is not a num/under, arg2 is not a num/under
			Clause toAddClause = Clause(RELATIONSHIP_STRING, relType, arg1, arg2);
		}
		else if (arg2_NUM == false && arg2_UNDER == true) {
			//Implies that the clause for arg1 is not a num/under, arg2 is not a num, arg2 is an UNDER
			Clause toAddClause = Clause(RELATIONSHIP_STRING, relType, arg1, WILDCARD_STRING);
		}
		else if (arg2_NUM == true && arg2_UNDER == false) {
			Clause toAddClause = Clause(RELATIONSHIP_STRING, relType, CONSTANT_STRING, arg2);
		}
	}
	else if (arg1_NUM == true && arg1_UNDER == false) {
		if (arg2_NUM == false && arg2_UNDER == false) {
			Clause toAddClause = Clause(RELATIONSHIP_STRING, relType, CONSTANT_STRING, arg2);
		}
		else if (arg2_NUM == false && arg2_UNDER == true) {
			Clause toAddClause = Clause(RELATIONSHIP_STRING, relType, CONSTANT_STRING, UNDER_SCORE_STRING);
		}
		else if (arg2_NUM == true && arg2_UNDER == false) {
			Clause toAddClause = Clause(RELATIONSHIP_STRING, relType, CONSTANT_STRING, CONSTANT_STRING);
		}
	}
	else if (arg1_NUM == false && arg1_UNDER == true) {
		if (arg2_NUM == false && arg2_UNDER == false) {
			Clause toAddClause = Clause(RELATIONSHIP_STRING, relType, UNDER_SCORE_STRING, arg2);
		}
		else if (arg2_NUM == false && arg2_UNDER == true) {
			Clause toAddClause = Clause(RELATIONSHIP_STRING, relType, UNDER_SCORE_STRING, UNDER_SCORE_STRING);
		}
		else if (arg2_NUM == true && arg2_UNDER == false) {
			Clause toAddClause = Clause(RELATIONSHIP_STRING, relType, UNDER_SCORE_STRING, CONSTANT_STRING);
		}
	}
}
bool ParserQuery::addSelectClauseTree(Clause clause) {
	queryTree.addSelectClause(clause);
	return true;
}
bool ParserQuery::addRelClauseTree(Clause clause) {
	queryTree.addRelClause(clause);
}
bool ParserQuery::addPatternClauseTree(Clause clause) {
	queryTree.addPatternClause(clause);
}
bool ParserQuery::checkRelationshipTable(string str, string type, int idx) {
	if (relationshipTable.isValidArg(str, type, idx)) {
		return true;
	}
	else {
		return false;
	}
}
string ParserQuery::checkDeclaration(string arg) {
	for (size_t i = 0; i < synonymAndEntityList.size(); i++) {
	//	vector<SynonymEntityPair> tempVec = synonymAndEntityList.at(i);
		SynonymEntityPair tempPair = synonymAndEntityList.at(i);
		vector<string> synonymList = tempPair.getSynonymList();
		size_t tempSize = synonymList.size();
		for (size_t j = 0; j <tempSize; j++) {
			if (arg == synonymList.at(j)) {
				//Since arg1 is part of the declaration, we find its corresponding Entity and return it
				return synonymAndEntityList.at(i).getEntity();
			}
		}
	}
	 return "nth";
}

bool ParserQuery::is_number(string s) {
	return s.find_first_not_of("0123456789") == string::npos;
}

//This function takes in a string to check if synonym asked exists in the SynonymEntityPair
bool ParserQuery::isValidSynonym(string syn) {
	for (size_t i = 0; i < synonymAndEntityList.size(); i++) {
		SynonymEntityPair tempPair = synonymAndEntityList.at(i);
		vector<string> tempVec = tempPair.getSynonymList();
		for (size_t j = 0; j < tempVec.size(); j++) {
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

	while ((pos = str.find(symbolSplitWith)) != std::string::npos) {
		token = str.substr(0, pos);
		result.push_back(token);
		size_t toAdd = symbolSplitWith.length();
		str.erase(0, pos+toAdd);
	}
	return result;
}

vector<string> ParserQuery::splitSelect(vector<string> vectorToSplit, string strToSplitWith) {

	vector<string> result = vector<string>();
	size_t pos = 0;
	std::string token;
	for (size_t i = 0; i < vectorToSplit.size(); i++) {
		while ((pos = vectorToSplit.at(i).find(strToSplitWith)) != std::string::npos) {
			token = vectorToSplit.at(i).substr(0, pos);
			result.push_back(token);
			vectorToSplit.at(i).erase(0, pos + strToSplitWith.length());
		}
	}
	return result;
}
string ParserQuery::removeSymbols(string str, string symbolToRemove) {
	string toReturn = "";
	size_t pos = 0;
	std::string token;

	while ((pos = str.find(symbolToRemove)) != std::string::npos) {
		token = str.substr(0, pos);
		toReturn += token;
		size_t toAdd = symbolToRemove.length();
		str.erase(0, pos + toAdd);
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