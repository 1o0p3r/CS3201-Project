﻿#include "QueryValidator.h"
#include <iostream>
#include <string>
#include <vector>
#include <ctype.h>
#include <stdio.h>
#include <SynonymEntityPair.h>
#include <regex>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <Util.h>

const int ZERO = 0;
const int ONE = 1;
const int TWO = 2;
const int PATTERN_STRING_LENGTH = 7;
const char SYMBOL_WHITESPACE = ' ';
const char SYMBOL_COMMA = ',';
const char SYMBOL_LEFT_BRACKET = '(';
const char SYMBOL_RIGHT_BRACKET = ')';
const char INVERTED_COMMA = '\"';
const char WHITESPACE_CHAR = ' ';
const char DOUBLE_QUOTATION = '\"';

const string WHITESPACE_STRING = " ";
const string SYMBOL_SEMI_COLON_STRING = ";";
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
const string INVERTED_COMMA_STRING = "\"";
const string EXACT_STRING = "exact";
const string SUBSTRING_STRING = "substring";
const string VARIABLE_STRING = "variable";
const string SYNONYM_STRING = "synonym";
const string INTEGER_STRING = "integer";
const string INVALID_STRING = "invalid";
const string WRONG_SYNTAX_ERROR = "wrong syntax entered";
const string INVALID_ENTITY_ERROR = "invalid entity";
const string INVALID_QUERY = "Invalid query";
const string INVALID_SYNONYM_QUERIED_ERROR = "Invalid synonym queried";
const string INVALID_ARGUMENT_ERROR = "Invalid argument";
const string NUMBER_ZERO_TO_NINE = "0123456789";
const string OR = "|";
const string EMPTY_STRING = "empty";
const string ASSIGN_STRING = "assign";
const string WHILE_STRING = "while";
const string IF_STRING = "if";
const string AND_STRING = "and";
const string ASTERIK = "*";

const string NAME_STRING_REGEX = "([a-zA-Z])([a-zA-Z]|\\d)*";
const string INTEGER_STRING_REGEX = "\\d+";
const string CONSTANT_STRING_REGEX = "\\d+";
const string FACTOR_STRING_REGEX = "\\d+|([a-zA-Z])([a-zA-Z]|\\d)*";
const string IDENT_STRING_REGEX = "([a-zA-Z])([a-zA-Z]|\\d|\#)*";
const string SYNONYM_STRING_REGEX = "([a-zA-Z])([a-zA-Z]|\\d|\#)*";
const string STMTREF_STRING_REGEX = "(([a-zA-Z])([a-zA-Z]|\\d|\#)*)|(\_)|(\\d+)";
const string LINEREF_STRING_REGEX = STMTREF_STRING_REGEX;
const string ENTREF_STRING_REGEX = "(([a-zA-Z])([a-zA-Z]|\\d|\#)*)|(\_)|(\"([a-zA-Z])([a-zA-Z]|\\d|\#)*\")";
const string EXPSPEC_STRING_REGEX = "((\_\"(([a-zA-Z])([a-zA-Z]|\\d)*)\"\_)|(\_)|(\"(([a-zA-Z])([a-zA-Z]|\\d)*)\"))";
const string DESIGN_ENTITY_REGEX = "(stmt|assign|while|variable|constant|prog_line)";
const string DECLARATION_STRING_REGEX = "(stmt|assign|while|variable|constant|prog_line)\\s+(([a-zA-Z])([a-zA-Z]|\\d|\#)*)\\s*(\,\\s*([a-zA-Z])([a-zA-Z]|\\d|\#)*)*;";
const string DECLARATIONS_STRING_REGEX = "(((stmt|assign|while|variable|constant|prog_line)\\s+(([a-zA-Z])([a-zA-Z]|\d|\#)*)\\s*(\,\\s*([a-zA-Z])(([a-zA-Z]|\\d|\#)\\s*)*)*;)\\s*)+";
const string VARIABLE_STRING_REGEX = "\"([a-zA-Z])([a-zA-Z]|\\d|\#)*\"";

const string MODIFIESS_STRING_REGEX = "Modifies\\s*\\((\\s*((([a-zA-Z])([a-zA-Z]|\\d|\#)*)|(\_)|(\\d+))\\s*,\\s*((([a-zA-Z])([a-zA-Z]|\\d|\#)*)|(\_)|(\"([a-zA-Z])([a-zA-Z]|\\d|\#)*\")))\\s*\\)";
const string USESS_STRING_REGEX = "Uses\\s*\\((\\s*((([a-zA-Z])([a-zA-Z]|\\d|\#)*)|(\_)|(\\d+))\\s*,\\s*((([a-zA-Z])([a-zA-Z]|\d|\#)*)|(\_)|(\"([a-zA-Z])([a-zA-Z]|\\d|\#)*\")))\\s*\\)";
const string MODIFIESP_STRING_REGEX = "Modifies\\s*\\((\\s*((([a-zA-Z])([a-zA-Z]|\\d|\#)*)|(\_)|(\"([a-zA-Z])([a-zA-Z]|\\d|\#)*\"))\\s*,\\s*((([a-zA-Z])([a-zA-Z]|\\d|\#)*)|(\_)|(\"([a-zA-Z])([a-zA-Z]|\\d|\#)*\")))\\s*\\)";
const string USESP_STRING_REGEX = "Uses\\s*\\((\\s*((([a-zA-Z])([a-zA-Z]|\\d|\#)*)|(\_)|(\"([a-zA-Z])([a-zA-Z]|\\d|\#)*\"))\\s*,\\s*((([a-zA-Z])([a-zA-Z]|\\d|\#)*)|(\_)|(\"([a-zA-Z])([a-zA-Z]|\\d|\#)*\")))\\s*\\)";
const string PARENT_STRING_REGEX = "Parent\\s*\\((\\s*((([a-zA-Z])([a-zA-Z]|\\d|\#)*)|(\_)|(\\d+))\\s*,\\s*((([a-zA-Z])([a-zA-Z]|\\d|\#)*)|(\_)|(\\d+))\\s*)\\)";
const string PARENTT_STRING_REGEX = "Parent\\*\\s*\\((\\s*((([a-zA-Z])([a-zA-Z]|\\d|\#)*)|(\_)|(\\d+))\\s*,\\s*((([a-zA-Z])([a-zA-Z]|\\d|\#)*)|(\_)|(\\d+))\\s*)\\)";
const string FOLLOWS_STRING_REGEX = "Follows\\s*\\((\\s*((([a-zA-Z])([a-zA-Z]|\\d|\#)*)|(\_)|(\\d+))\\s*,\\s*((([a-zA-Z])([a-zA-Z]|\\d|\#)*)|(\_)|(\\d+))\\s*)\\)";
const string FOLLOWST_STRING_REGEX = "Follows\\*\\s*\\((\\s*((([a-zA-Z])([a-zA-Z]|\\d|\#)*)|(\_)|(\\d+))\\s*,\\s*((([a-zA-Z])([a-zA-Z]|\\d|\#)*)|(\_)|(\\d+))\\s*)\\)";

const string CALLS_STRING_REGEX = "Calls\\s*\\((\\s*((([a-zA-Z])([a-zA-Z]|\\d|\#)*)|(\_)|(\"([a-zA-Z])([a-zA-Z]|\\d|\#)*\"))\\s*,\\s*((([a-zA-Z])([a-zA-Z]|\\d|\#)*)|(\_)|(\"([a-zA-Z])([a-zA-Z]|\\d|\#)*\")))\\s*\\)";
const string CALLST_STRING_REGEX = "Calls\\*\\s*\\((\\s*((([a-zA-Z])([a-zA-Z]|\\d|\#)*)|(\_)|(\"([a-zA-Z])([a-zA-Z]|\\d|\#)*\"))\\s*,\\s*((([a-zA-Z])([a-zA-Z]|\\d|\#)*)|(\_)|(\"([a-zA-Z])([a-zA-Z]|\\d|\#)*\")))\\s*\\)";
const string NEXT_STRING_REGEX = "Next\\s*\\((\\s*((([a-zA-Z])([a-zA-Z]|\\d|\#)*)|(\_)|(\\d+))\\s*,\\s*((([a-zA-Z])([a-zA-Z]|\\d|\#)*)|(\_)|(\\d+))\\s*)\\)";
const string NEXTT_STRING_REGEX = "Next\\*\\s*\\((\\s*((([a-zA-Z])([a-zA-Z]|\\d|\#)*)|(\_)|(\\d+))\\s*,\\s*((([a-zA-Z])([a-zA-Z]|\\d|\#)*)|(\_)|(\\d+))\\s*)\\)";

const string RELREF_STRING_REGEX = "(" + MODIFIESS_STRING_REGEX + OR + USESS_STRING_REGEX + OR
+ PARENTT_STRING_REGEX + OR + PARENT_STRING_REGEX + OR + FOLLOWS_STRING_REGEX + OR + FOLLOWST_STRING_REGEX 
+ OR + NEXT_STRING_REGEX + OR + NEXTT_STRING_REGEX + OR  + CALLS_STRING_REGEX + OR + CALLST_STRING_REGEX + OR
+ MODIFIESP_STRING_REGEX + OR + USESP_STRING_REGEX + ")";

const string RELCOND_STRING_REGEX = RELREF_STRING_REGEX + "\\s*" + SYMBOL_LEFT_BRACKET_STRING + AND_STRING + "\\s+" + RELREF_STRING_REGEX + "\\s*" + SYMBOL_RIGHT_BRACKET_STRING + ASTERIK;
const string ITR2_SUCH_THAT_CL_REGEX = SUCH_THAT_STRING + "\\s+"  + RELCOND_STRING_REGEX;

const string SUCH_THAT_CL_REGEX = SUCH_THAT_STRING + "\\s*" + RELREF_STRING_REGEX;
const string PATTERN_CL_REGEX = PATTERN_STRING + "\\s+" + 
"([a-zA-Z])([a-zA-Z]|\\d|\#)*\\(\\s*((([a-zA-Z])([a-zA-Z]|\\d|\\#)*)|(\\_)|(\"([a-zA-Z])([a-zA-Z]|\\d|\\#)*\"))\\s*,\\s*(\\_\"([a-zA-Z])(\\w)*((\\+|\\*|\\-)\\w+)*\"\\_|\\_|\"([a-zA-Z])(\\w)*((\\+|\\*|\\-)\\w+)*\")\\s*\\)";
const string SELECT_INITIAL_REGEX = SELECT_STRING + "\\s+" + "([a-zA-Z])([a-zA-Z]|\\d|\\#)*";

//Whaat methods  do i need to check, 1 for declaration, 1 for such-that, 1 for pattern
using namespace std;
QueryValidator::QueryValidator()
{
}
//This function takes in a string query
void QueryValidator::startParsing(string str) {
	//Upon taking in a string query, clear the following
	queryStatement = QueryStatement();
	synonymAndEntityList = vector<SynonymEntityPair>();
	parseInput(str);
}

//With a given str, loop thru to find ;
bool QueryValidator::parseInput(string str) {
	str = removeDuplicatesWhiteSpaces(str);

	if (str.find(SYMBOL_SEMI_COLON_STRING)) {
		bool valQuery = false;	
		size_t pos;
		std::string token;

		while ((pos = str.find(SYMBOL_SEMI_COLON_STRING) != std::string::npos)) {
			pos = str.find(SYMBOL_SEMI_COLON_STRING);
			token = str.substr(ZERO, pos);
			string toCheck = token + SYMBOL_SEMI_COLON_STRING;
			toCheck = trim(toCheck);
			if (!isValidDeclarationRegex(toCheck)) {
				return false;
			}
			if (!isEntityAndSynonym(token)) {
				return false;
			}
			str.erase(ZERO, pos + ONE);
		}
		//After the loop ends, comes the select query
		if (str.find(SELECT_STRING)) {
			return parseQueryLine(str);
		}
		else {
			return false;
		}
	}
	else {
		cout << WRONG_SYNTAX_ERROR;
		return false;
	}
}

//To be deleted 
bool QueryValidator::isValidModifiesP(string str) {
	regex modifiesPRegex(MODIFIESP_STRING_REGEX);
	return regex_match(str, modifiesPRegex);
}
//This function parses the Entity And Synonym and checks for the validity of Entity
bool QueryValidator::isEntityAndSynonym(string currentString) {
	if (isValidEntity(currentString)) {
		addSynonymEntityList();
		return true;
	}
	else {
		cout << INVALID_ENTITY_ERROR;
		return false;
	}
}
//This function adds the synonymEntityList to the QueryStatement to give the query evaluator to use
void QueryValidator::addSynonymEntityList() {
	queryStatement.addSynonymEntityList(synonymAndEntityList);
}
bool QueryValidator::parseQueryLine(string str) {
	if (!isValidQueryLine(str)) {
		cout << INVALID_QUERY << endl;
		return false;
	}
	else {
		return true;
	}
}

//This function checks for the validity of the declaration by comparing with the the entityList given
//Returns true if there is a match, else returns false
bool QueryValidator::isValidEntity(string currentString) {
	currentString = trim(currentString);
	//Places the first vector with the unchecked entity, and subsequent vectors with synonyms
	std::vector<string> splitString = splitBySymbol(currentString, SYMBOL_WHITESPACE);
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

bool QueryValidator::isValidSynDesignEntity(string synPattern) {
	string entPattern = getCorrespondingEntity(synPattern);
	
	if (entPattern == ASSIGN_STRING) {
		return true;
	}
	else if ((entPattern == WHILE_STRING) || (entPattern == IF_STRING)) {
		return true;
	}
	else {
		return false;
	}
}
//For now this funcction assumes that pattern is of valid syntax

bool QueryValidator::isValidPattern(string str, string syn) {
	//Idea: From given string, ignore Pattern, obtain (arg1, arg2)
	//Example of current string : pattern a(v,"procs*ifs")	or pattern a(_,_"procs*while"_)
	str = trim(str);
	str = str.substr(8, str.length() - 1);
	int idxLeftBracket = str.find(SYMBOL_LEFT_BRACKET_STRING);
	string synDesignEnt = str.substr(0, idxLeftBracket);
	str = removeSymbols(str, WHITESPACE_STRING);
	str = PATTERN_STRING + WHITESPACE_STRING + str;

	if (!isValidPatternRegex(str) || !isValidSynDesignEntity(synDesignEnt)) {
		return false;
	}
	else {
		string argStr = trimPatternArgs(str);
		vector<string> splitPattern = splitBySymbol(argStr, SYMBOL_COMMA);

		string arg1 = splitPattern.at(ZERO);
		string arg2 = splitPattern.at(ONE);
		string ent = getCorrespondingEntity(synDesignEnt);

		bool arg1Variable = isVariableArg1(arg1);
		bool arg1Wildcard = isWildcard(arg1);
		bool arg1Synonym = isValidSynonym(arg1);

		//Now removed the inverted commas
		arg1 = removeSymbols(arg1, INVERTED_COMMA_STRING);
		//For arg2, first check if its just a wildcard
		bool arg2Wildcard = false;
		bool arg2Substring = false;
		bool arg2Exact = false;

		if (arg2 == UNDER_SCORE_STRING) {
			arg2Wildcard = true;
		}
		if (arg2Wildcard == false) {
			arg2Substring = isSubstringArg2(arg2);
		}
		if (isExactString(arg2)) {
			arg2Exact = true;
		}

		//Clean up for arg1
		if (arg1Variable) {
			arg1 = removeSymbols(arg1, WHITESPACE_STRING);
			arg1 = removeSymbols(arg1, INVERTED_COMMA_STRING);
		}
		else if (arg1Wildcard) {
			arg1 = removeSymbols(arg1, WHITESPACE_STRING);
		}
		else if (arg1Synonym) {
			arg1 = removeSymbols(arg1, WHITESPACE_STRING);
		}
		else {
			cout << INVALID_ARGUMENT_ERROR;
			return false;
		}
		//Clean up for arg2
		if (arg2Substring) {
			arg2 = removeSymbols(arg2, WHITESPACE_STRING);
			arg2 = removeSymbols(arg2, UNDER_SCORE_STRING);
			arg2 = removeSymbols(arg2, INVERTED_COMMA_STRING);
		}
		else if (arg2Exact) {
			arg2 = removeSymbols(arg2, WHITESPACE_STRING);
			arg2 = arg2.substr(ONE, arg2.length() - 2);
		}
		else if (arg2Wildcard) {
			arg2 = removeSymbols(arg2, WHITESPACE_STRING);
		}
		else {
			cout << INVALID_ARGUMENT_ERROR;
			return false;
		}
		//The following are needed, dont remove, uses limjie's method
		//Arg 1 can be either a synonym, wildcard or variable, i only insert brackets when it is a variable
		/**
		if (arg1Variable) {
			arg1 = Util::insertBrackets(arg1);
		}**/

		//Arg2 can be a substring, exactstring or a wildcard
		//Insert brackets only when arg2 is a substring or exact string
		if(arg2Substring|| arg2Exact) {
			arg2 = Util::insertBrackets(arg2);
		}

		addPatternQueryElement(arg1, arg2, ent, synDesignEnt, arg1Variable, arg1Wildcard, arg1Synonym, arg2Substring, arg2Exact, arg2Wildcard);
		return true;
	}
}

//This funtion checks if the given arg 1 of pattern is a variable type
bool QueryValidator::isVariableArg1(string arg1) {
	if (arg1.at(ZERO) == DOUBLE_QUOTATION   && arg1.at(arg1.length() - 1) == DOUBLE_QUOTATION) {
		return true;
	}
	else {
		return false;
	}
}
bool QueryValidator::isWildcard(string arg) {
	return (arg == UNDER_SCORE_STRING);
}
//This function checks if the given arg2 of pattern is of a substring type i.e. _"x+y"_
bool QueryValidator::isSubstringArg2(string arg2) {
	string substringPattern = "_\".+\"_";
	std::regex pattern(substringPattern);
	std::smatch sm;
	
	return std::regex_match(arg2, sm, pattern);
}
//This function takes in an argument(2) and checks if it is an exact string
bool QueryValidator::isExactString(string arg2) {
	if ((arg2.at(0) == DOUBLE_QUOTATION)  && (arg2.at(arg2.length() - 1) == DOUBLE_QUOTATION)) {
		return true;
	}
	else {
		return false;
	}
	return true;
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

	int toAdd = ZERO;
	selectClause = trim(selectClause);
	if (!isValidSelectInitialRegex(selectClause)) {
		return false;
	}

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
		cout << INVALID_SYNONYM_QUERIED_ERROR;
		return false;
	}
}
void QueryValidator::addSelectQueryElement(string ent, string syn) {
	queryStatement.addSelectQuery(QueryElement(ent, syn));
}

//The following function
void QueryValidator::addPatternQueryElement(string arg1, string arg2, string ent, string syn, bool arg1Variable, bool arg1Wildcard, bool arg1Synonym, bool arg2Substring, bool arg2FullString, bool arg2Wilcard) {
	
	//Implies that arg1 is wildcard i.e. _
	if (arg1Wildcard) {
		if (arg2Substring) {
			queryStatement.addPatternQuery(QueryElement(arg1, arg2, ent, syn, WILDCARD_STRING, SUBSTRING_STRING));
		}
		else if (!arg2Substring && arg2FullString) {
			queryStatement.addPatternQuery(QueryElement(arg1, arg2, ent, syn, WILDCARD_STRING, EXACT_STRING));
		}
		else if (!arg2Substring && !arg2FullString && arg2Wilcard) {
			queryStatement.addPatternQuery(QueryElement(arg1, arg2, ent, syn, WILDCARD_STRING, WILDCARD_STRING));
		}
		else {
			//cout << "Error occured";
			exit(0);
		}
	}
	else if (arg1Variable) {
		if (arg2Substring) {
			queryStatement.addPatternQuery(QueryElement(arg1, arg2, ent, syn, VARIABLE_STRING, SUBSTRING_STRING));
		}
		else if (!arg2Substring && arg2FullString) {
			queryStatement.addPatternQuery(QueryElement(arg1, arg2, ent, syn, VARIABLE_STRING, EXACT_STRING));
		}
		else if(!arg2Substring && !arg2FullString && arg2Wilcard) {
			queryStatement.addPatternQuery(QueryElement(arg1, arg2, ent, syn, VARIABLE_STRING, WILDCARD_STRING));
		}
		else {
			//cout << "Error occured";
			exit(0);
		}
	}
	else if(arg1Synonym){
		if (arg2Substring) {
			queryStatement.addPatternQuery(QueryElement(arg1, arg2, ent, syn, SYNONYM_STRING, SUBSTRING_STRING));
		}
		else if (!arg2Substring && arg2FullString) {
			queryStatement.addPatternQuery(QueryElement(arg1, arg2, ent, syn, SYNONYM_STRING, EXACT_STRING));
		}
		else if (!arg2Substring && !arg2FullString && arg2Wilcard) {
			queryStatement.addPatternQuery(QueryElement(arg1, arg2, ent, syn, SYNONYM_STRING, WILDCARD_STRING));
		}
		else {
			//cout << "Error occured";
			exit(0);
		}
	}
	else {
		exit(0);
	}
}
void QueryValidator::addSuchThatQueryElement(QueryElement qe) {
	queryStatement.addSuchThatQuery(qe);
}

//isValid others currently accepts such that and pattern clause
bool QueryValidator::isValidOthers(vector<string> vec) {
	//This implies its a short query i.e. stmt s
	if (vec.size() < 2) {
		return true;
	}
	else {
		string selectStr = vec.at(ZERO);
		selectStr = trim(selectStr);
		vector<string> vecSplit = splitBySymbol(selectStr, SYMBOL_WHITESPACE);
		//Obtain the synonym
		string syn = vecSplit.at(ONE);
	
		//loop through every string to check if the first index and last index is a whitespace
		//If so, remove them
		for (size_t k = ONE; k < vec.size(); k++) {
			trim(vec.at(k));
		}
		vector<string> temp = vec;
		for (size_t i = ONE; i < vec.size(); i++) {
			if (vec.at(i).find(SUCH_THAT_STRING) != std::string::npos) {
				if (!isValidSuchThat(vec.at(i), syn)) {
					return false;
				}
			}
			else if (vec.at(i).find(PATTERN_STRING) != std::string::npos) {
				if (!isValidPattern(vec.at(i), syn)) {
					return false;
				}
			}
		}
		return true;
	}
}
vector<string> QueryValidator::extractSuchThatClauses(string str) {
	regex suchThatRelRegex(RELREF_STRING_REGEX);
	smatch m;
	//sregex_iterator iter(str.begin(), str.end(), suchThatRelRegex);
	//sregex_iterator end;
	
	
	
	vector<string> toReturnVec;
	/**
	while (regex_search(str, m, suchThatRelRegex)) {
		for (auto x : m) {
			toReturnVec.push_back(x);
			str = m.suffix().str();
		}
	}**/
	/**
	while (iter != end)
	{
		for (unsigned i = 0; i < iter->size(); ++i)
		{
			//std::cout << "the " << i + 1 << "th match" << ": " << (*iter)[i] << std::endl;
			toReturnVec.push_back((*iter)[i]);
		}
		++iter;
	}**/


	return toReturnVec;
}
bool QueryValidator::isValidSuchThat(string str, string syn) {
	int toAdd = ZERO;
	
	//Remove the additional leading and trailing whitespace
	str = trim(str);

	if (!isValidSuchThatRegex(str)) {
		return false;
	}
	//Firstly extract/split till a relation is found
	string tempString = str.substr(10, str.length()-ONE);

	//So sth like Follows(s,4) is tempString, after calling the next statement, [0]= Follows	[1] = s,4)
	vector<string> tempVec = splitBySymbol(tempString, SYMBOL_LEFT_BRACKET);
	
	//Obtain the relation e.g. Follows
	string relation = tempVec.at(ZERO);

	//Obtain the string argsWithoutBracket i.e.  s,4 is obtained
	string argsWithoutBracket = tempVec.at(ONE).substr(ZERO, tempVec.at(ONE).length()-1);

	//Remove all the whitespace first
	argsWithoutBracket = removeSymbols(argsWithoutBracket, WHITESPACE_STRING);

	//Split them by Comma
	vector<string> args = splitBySymbol(argsWithoutBracket, SYMBOL_COMMA);

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
	bool arg2_VARIABLE = false;

	if (arg1Ent != INVALID_STRING) {
		//Implies that a corresponding entity was obtained
		arg1Valid = checkRelationshipTable(relation, arg1Ent, 1);
	}
	else {
		//If no correspond entity was obtained, try to check if it is a number or wildcard
		if (is_number(arg1)) {
			//If exists in table and part of declared relationship, create a clause and add it to query tree
			arg1Valid = checkRelationshipTable(relation, NUMBER_STRING, ONE);
			if (arg1Valid) {
				arg1_NUM = true;
			}
			else {
				arg1_NUM = false;
			}
		}
		else if (arg1 == UNDER_SCORE_STRING) {
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
	if (arg2Ent != INVALID_STRING) {
		arg2Valid = checkRelationshipTable(relation, arg2Ent, TWO);
	}
	else {
		if (is_number(arg2)) {
			arg2Valid = checkRelationshipTable(relation, NUMBER_STRING, TWO);
			if (arg2Valid) {
				arg2_NUM = true;
			}
			else {
				arg2_NUM = false;
			}
		}
		else if (arg2 == UNDER_SCORE_STRING) {
			arg2Valid = checkRelationshipTable(relation, WILDCARD_STRING, TWO);
			if (arg2Valid) {
				arg2_UNDER = true;
			}
			else {
				arg2_UNDER = false;
			}
		}
		//Implies arg 2 is sth like "x"
		else if (isVariable(arg2)) {
			arg2Valid = checkRelationshipTable(relation, VARIABLE_STRING, TWO);
			if (arg2Valid) {
				arg2_VARIABLE = true;
			}
			else {
				arg2_VARIABLE = false;
			}
		}
		else {
			arg2Valid = false;
		}
	}
	//If both are valid and true, create the clause
	if (arg1Valid && arg2Valid) {
		if (!addSuchThatQueryElement(arg1_NUM, arg1_UNDER, arg2_NUM, arg2_UNDER, arg2_VARIABLE, relation, arg1, arg2, arg1Ent, arg2Ent)) {
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

bool QueryValidator::addSuchThatQueryElement(bool arg1_NUM, bool arg1_UNDER, bool arg2_NUM, bool arg2_UNDER, bool arg2_VARIABLE, string relType, string arg1, string arg2, string type1, string type2) {
	
	//Implies that arg1 is a synonym
	if (arg1_NUM == false && arg1_UNDER == false) {
		//Implies that arg2 is also a synonym
		if (arg2_NUM == false && arg2_UNDER == false && arg2_VARIABLE == false) {
			QueryElement queryElement = QueryElement(arg1, SYNONYM_STRING, type1, arg2, SYNONYM_STRING, type2, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		//Implies that arg1 is synonym, arg2 is a wildcard
		else if (arg2_NUM == false && arg2_UNDER == true && arg2_VARIABLE == false) {
			//Implies that the clause for arg1 is not a num/under, arg2 is not a num, arg2 is an UNDER
			QueryElement queryElement = QueryElement(arg1, SYNONYM_STRING, type1, UNDER_SCORE_STRING, WILDCARD_STRING, EMPTY_STRING, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		//Implies that arg1 is synonym, arg 2 is an integer i.e. number
		else if (arg2_NUM == true && arg2_UNDER == false && arg2_VARIABLE == false) {
			QueryElement queryElement = QueryElement(arg1, SYNONYM_STRING, type1, arg2, NUMBER_STRING, EMPTY_STRING, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		//Implies that arg1 is synonym, arg2 is a variable
		else if (arg2_NUM == false && arg2_UNDER == false && arg2_VARIABLE == true) {
			arg2 = removeSymbols(arg2, INVERTED_COMMA_STRING);
			QueryElement queryElement = QueryElement(arg1, SYNONYM_STRING, type1, arg2, VARIABLE_STRING, EMPTY_STRING, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		else {
			return false;
		}
	}
	//Implies that arg1 is a num
	else if (arg1_NUM == true && arg1_UNDER == false) {
		//Implies that arg1 is a number and arg2 is a synonym
		if (arg2_NUM == false && arg2_UNDER == false && arg2_VARIABLE == false) {
			QueryElement queryElement = QueryElement(arg1, NUMBER_STRING, EMPTY_STRING, arg2, SYNONYM_STRING, type2, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		//Implies that arg1 is a number and arg2 is a wildcard i.e underscore
		else if (arg2_NUM == false && arg2_UNDER == true && arg2_VARIABLE == false) {
			QueryElement queryElement = QueryElement(arg1, NUMBER_STRING, EMPTY_STRING, UNDER_SCORE_STRING, WILDCARD_STRING, EMPTY_STRING, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		//Implies that arg1 is a number and arg2 is a number
		else if (arg2_NUM == true && arg2_UNDER == false && arg2_VARIABLE == false) {
			QueryElement queryElement = QueryElement(arg1, NUMBER_STRING, EMPTY_STRING, arg2, NUMBER_STRING, EMPTY_STRING, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		//Implies taht arg1 is a number and arg2 is a variable type i.e. "IDENT"
		else if (arg2_NUM == false && arg2_UNDER == false && arg2_VARIABLE == true) {
			arg2 = removeSymbols(arg2, INVERTED_COMMA_STRING);
			QueryElement queryElement = QueryElement(arg1, NUMBER_STRING, EMPTY_STRING, arg2, VARIABLE_STRING, EMPTY_STRING, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		else {
			return false;
		}
	}
	//Implies that arg1 is a wildcard i.e. underscore
	else if (arg1_NUM == false && arg1_UNDER == true) {
		//Implies taht arg1 is a wildcard and arg 2 is a synonym
		if (arg2_NUM == false && arg2_UNDER == false && arg2_VARIABLE == false) {
			QueryElement queryElement = QueryElement(UNDER_SCORE_STRING, WILDCARD_STRING, EMPTY_STRING , arg2, SYNONYM_STRING, type2, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		//Implies that arg1 is a wild card and arg 2 is a wildcard
		else if (arg2_NUM == false && arg2_UNDER == true && arg2_VARIABLE == false) {
			QueryElement queryElement = QueryElement(UNDER_SCORE_STRING, WILDCARD_STRING, EMPTY_STRING, UNDER_SCORE_STRING, WILDCARD_STRING, EMPTY_STRING, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		//Implies that arg1 is a wildcard and arg2 is a number i.e. integer
		else if (arg2_NUM == true && arg2_UNDER == false && arg2_VARIABLE == false) {
			QueryElement queryElement = QueryElement(UNDER_SCORE_STRING, WILDCARD_STRING, EMPTY_STRING, arg2, NUMBER_STRING, EMPTY_STRING, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		//Implies that arg1 is a wildcard and arg2 is a variable i.e. "IDENT"
		else if (arg2_NUM == false && arg2_UNDER == false && arg2_VARIABLE == true) {
			arg2 = removeSymbols(arg2, INVERTED_COMMA_STRING);
			QueryElement queryElement = QueryElement(UNDER_SCORE_STRING, WILDCARD_STRING, EMPTY_STRING, arg2, VARIABLE_STRING, EMPTY_STRING, relType);
			addSuchThatQueryElement(queryElement);
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
	string reqEntity = INVALID_STRING;
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
	return s.find_first_not_of(NUMBER_ZERO_TO_NINE) == string::npos;
}
QueryStatement QueryValidator::getQueryStatement()
{
	return queryStatement;
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
	return result;
}
string QueryValidator::removeSymbols(string str, string symbolToRemove) {
	std::regex pattern(symbolToRemove);
	std::string toReturn = std::regex_replace(str, pattern , "");
	return toReturn;
}

string QueryValidator::removeDuplicatesWhiteSpaces(string str) {
	size_t pos;
	while ((pos = str.find("  ")) != std::string::npos)
		str = str.replace(pos, 2, " ");
	return str;
}

string QueryValidator::trim(string str) {
	return regex_replace(str, regex("^ +| +$|( ) +"), "$1");
}
/**
int QueryValidator::getNumClauses() {
	return this->numClauses;
}**/
string QueryValidator::changeLowerCase(string str) {
	 std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	 return str;
}
//Takes in a string and check if it matches the declaration regex
//Returns true if matches, else false
bool QueryValidator::isValidDeclarationRegex(string str) {
	regex declareRegex("((stmt|assign|while|variable|constant|prog_line)\\s+(([a-zA-Z])([a-zA-Z]|\\d|\#)*)\\s*(\\,\\s*([a-zA-Z])([a-zA-Z]|\\d|\#)*)*)\\;");
	return regex_match(str, declareRegex);
}
//Takes in a string and checks if it matches the such that regex
//Retunrs true if matches, else false
bool QueryValidator::isValidSuchThatRegex(string str) {
	regex suchThatRegex(SUCH_THAT_CL_REGEX);
	return regex_match(str, suchThatRegex);
}
//Takes in a string and checks if it matches the pattern regex
//Returns true if matches, else false
bool QueryValidator::isValidPatternRegex(string str) {
	regex patternRegex(PATTERN_CL_REGEX);
	return regex_match(str, patternRegex);
}
bool QueryValidator::isValidSelectInitialRegex(string str) {
	regex patternRegex(SELECT_INITIAL_REGEX);
	return regex_match(str, patternRegex);
}
bool QueryValidator::isVariable(string str) {
	regex patternRegex(VARIABLE_STRING_REGEX);
	return regex_match(str, patternRegex);
}
string QueryValidator::trimPatternArgs(string str) {
	int idxLeft = str.find(SYMBOL_LEFT_BRACKET_STRING);
	str = str.substr(idxLeft + ONE, str.length() - idxLeft - TWO);
	str = removeSymbols(str, WHITESPACE_STRING);
	return str;
}