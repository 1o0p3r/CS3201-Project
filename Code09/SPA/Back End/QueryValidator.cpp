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
#include <algorithm>
#include <Util.h>

const int ZERO = 0;
const int ONE = 1;
const int TWO = 2;
const int THREE = 3;
const int PATTERN_STRING_LENGTH = 7;
const char SYMBOL_WHITESPACE = ' ';
const char SYMBOL_COMMA = ',';
const char SYMBOL_LEFT_BRACKET = '(';
const char SYMBOL_RIGHT_BRACKET = ')';
const char INVERTED_COMMA = '\"';
const char WHITESPACE_CHAR = ' ';
const char DOUBLE_QUOTATION = '\"';
const char EQUAL_CHAR = '=';
const char DOT_CHAR = '.';

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
const string CALLS_STRING = "Calls";
const string NEXT_STRING = "Next";
const string AFFECTS_STRING = "Affects";
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
const string PROCEDURE_STRING = "procedure";
const string PROG_LINE_STRING = "prog_line";
const string STR_STRING = "string";
const string BOOLEAN_STRING = "BOOLEAN";
const string CALL_STRING = "call";
const string TAB_STRING = "\\t";
const string COMMA_STRING = ",";
const string TUPLE_STRING = "tuple";
const string ATTRREF_STRING = "attrRef";

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
const string EQUAL_STRING = "=";


const string WITH_STRING = "with";
const string PROCNAME = "procName";
const string VARNAME = "varName";
const string VALUE = "value";
const string STMTNUM = "stmt#";
const string STMT_STRING = "stmt";
const string STRING_LITERAL = "stringLiteral";

const string STRTYPE = "strType";
const string INTTYPE = "intType";

const string NAME_STRING_REGEX = "([a-zA-Z])([a-zA-Z]|\\d)*";
const string INTEGER_STRING_REGEX = "\\d+";
const string CONSTANT_STRING_REGEX = "\\d+";
const string FACTOR_STRING_REGEX = "\\d+|([a-zA-Z])([a-zA-Z]|\\d)*";
const string IDENT_STRING_REGEX = "(\\s*(([a-zA-Z])([a-zA-Z]|\\d|\#)*)\\s*)";
const string QUOTATION_IDENT_STRING_REGEX = SYMBOL_LEFT_BRACKET_STRING + "\"" + IDENT_STRING_REGEX + "\"" + SYMBOL_RIGHT_BRACKET_STRING;
const string SYNONYM_STRING_REGEX = "([a-zA-Z])([a-zA-Z]|\\d|\#)*";
const string STMTREF_STRING_REGEX = "((([a-zA-Z])([a-zA-Z]|\\d|\#)*)|(\_)|(\\d+))";
const string LINEREF_STRING_REGEX = STMTREF_STRING_REGEX;
const string ENTREF_STRING_REGEX = "((([a-zA-Z])([a-zA-Z]|\\d|\#)*)|(\_)|(\\d+)|(\"\\s*([a-zA-Z])([a-zA-Z]|\\d|\#)*\\s*\"))"; //Nth wrg here either
const string VARREF_STRING_REGEX = SYMBOL_LEFT_BRACKET_STRING + UNDER_SCORE_STRING + OR + SYNONYM_STRING_REGEX + OR + QUOTATION_IDENT_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING;
const string EXPSPEC_STRING_REGEX = "((\_\"(([a-zA-Z])([a-zA-Z]|\\d)*)\"\_)|(\_)|(\"(([a-zA-Z])([a-zA-Z]|\\d)*)\"))";
const string DESIGN_ENTITY_REGEX = "(stmt|assign|while|variable|constant|prog_line|procedure|stmtLst|if|call)";
const string DECLARATION_STRING_REGEX = "(stmt|assign|while|variable|constant|prog_line|procedure|stmtLst|if|call)\\s+(([a-zA-Z])([a-zA-Z]|\\d|\#)*)\\s*(\,\\s*([a-zA-Z])([a-zA-Z]|\\d|\#)*)*;";
const string DECLARATIONS_STRING_REGEX = "(((stmt|assign|while|variable|constant|prog_line|procedure|stmtLst|if|call)\\s+(([a-zA-Z])([a-zA-Z]|\d|\#)*)\\s*(\,\\s*([a-zA-Z])(([a-zA-Z]|\\d|\#)\\s*)*)*;)\\s*)+";
const string VARIABLE_STRING_REGEX = "\"([a-zA-Z])([a-zA-Z]|\\d|\#)*\"";
const string ATTRNAME_STRING_REGEX = SYMBOL_LEFT_BRACKET_STRING + "procName|varName|value|stmt#" + SYMBOL_RIGHT_BRACKET_STRING;
const string ATTRREF_STRING_REGEX = SYMBOL_LEFT_BRACKET_STRING + SYNONYM_STRING_REGEX + "\." + ATTRNAME_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING;
const string REF_STRING_REGEX = SYMBOL_LEFT_BRACKET_STRING + ATTRREF_STRING_REGEX + OR + SYNONYM_STRING_REGEX + OR + QUOTATION_IDENT_STRING_REGEX
+ OR + INTEGER_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING;
const string ATTRCOMPARE_STRING_REGEX = SYMBOL_LEFT_BRACKET_STRING + REF_STRING_REGEX + "\\s*" + EQUAL_STRING + "\\s*" + REF_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING;
const string ATTRCOND_STRING_REGEX = SYMBOL_LEFT_BRACKET_STRING + ATTRCOMPARE_STRING_REGEX + SYMBOL_LEFT_BRACKET_STRING + "\\s+" + AND_STRING + "\\s+" + ATTRCOMPARE_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING + ASTERIK + SYMBOL_RIGHT_BRACKET_STRING;
const string WITH_CL_REGEX = WITH_STRING + "\\s+" + ATTRCOND_STRING_REGEX;
const string TEMPORARY_WITH = SYMBOL_LEFT_BRACKET_STRING + WITH_CL_REGEX + SYMBOL_RIGHT_BRACKET_STRING;
const string WITH_CL_EXTENDED_REGEX = TEMPORARY_WITH + SYMBOL_LEFT_BRACKET_STRING + "\\s*" + TEMPORARY_WITH + "\\s*" + SYMBOL_RIGHT_BRACKET_STRING + ASTERIK;
const string LEAD_TRAIL = "and|\\s+";
const string STRING_REGEX = "[a-zA-Z]";
const string WHITESPACE_TAB_REGEX = "[\\s\\t]";
const string SUBSTRING_REGEX = "\\_\\s*\"([^_\"])+\"\\s*\\_";

//Regexs for such that relationships
const string MODIFIESP_STRING_REGEX = MODIFIES_STRING + "\\s*" + SYMBOL_LEFT_BRACKET_STRING + "\\(" + "\\s*" + ENTREF_STRING_REGEX + "\\s*"
+ "," + "\\s*" + ENTREF_STRING_REGEX + "\\s*" + "\\)" + SYMBOL_RIGHT_BRACKET_STRING;
const string MODIFIESS_STRING_REGEX = MODIFIES_STRING + "\\s*" + SYMBOL_LEFT_BRACKET_STRING + "\\(" + "\\s*" + STMTREF_STRING_REGEX + "\\s*"
+ "," + "\\s*" + ENTREF_STRING_REGEX + "\\s*" + "\\)" + SYMBOL_RIGHT_BRACKET_STRING;
const string USESP_STRING_REGEX = USES_STRING + "\\s*" + SYMBOL_LEFT_BRACKET_STRING + "\\(" + "\\s*" + ENTREF_STRING_REGEX + "\\s*"
+ "," + "\\s*" + ENTREF_STRING_REGEX + "\\s*" + "\\)" + SYMBOL_RIGHT_BRACKET_STRING;
const string USESS_STRING_REGEX = USES_STRING + "\\s*" + SYMBOL_LEFT_BRACKET_STRING + "\\(" + "\\s*" + STMTREF_STRING_REGEX + "\\s*"
+ "," + "\\s*" + ENTREF_STRING_REGEX + "\\s*" + "\\)" + SYMBOL_RIGHT_BRACKET_STRING;
const string CALLS_STRING_REGEX = CALLS_STRING + "\\s*" + SYMBOL_LEFT_BRACKET_STRING + "\\(" + "\\s*" + ENTREF_STRING_REGEX + "\\s*"
+ "," + "\\s*" + ENTREF_STRING_REGEX + "\\s*" + "\\)" + SYMBOL_RIGHT_BRACKET_STRING;
const string CALLST_STRING_REGEX = CALLS_STRING + "\\*" + "\\s*" + SYMBOL_LEFT_BRACKET_STRING + "\\(" + "\\s*" + ENTREF_STRING_REGEX + "\\s*"
+ "," + "\\s*" + ENTREF_STRING_REGEX + "\\s*" + "\\)" + SYMBOL_RIGHT_BRACKET_STRING;
const string PARENT_STRING_REGEX = PARENT_STRING + "\\s*" + SYMBOL_LEFT_BRACKET_STRING + "\\(" + "\\s*" + STMTREF_STRING_REGEX + "\\s*"
+ "," + "\\s*" + STMTREF_STRING_REGEX + "\\s*" + "\\)" + SYMBOL_RIGHT_BRACKET_STRING;
const string PARENTT_STRING_REGEX = PARENT_STRING + "\\*" + "\\s*" + SYMBOL_LEFT_BRACKET_STRING + "\\(" + "\\s*" + STMTREF_STRING_REGEX + "\\s*"
+ "," + "\\s*" + STMTREF_STRING_REGEX + "\\s*" + "\\)" + SYMBOL_RIGHT_BRACKET_STRING;
const string FOLLOWS_STRING_REGEX = FOLLOWS_STRING + "\\s*" + SYMBOL_LEFT_BRACKET_STRING + "\\(" + "\\s*" + STMTREF_STRING_REGEX + "\\s*"
+ "," + "\\s*" + STMTREF_STRING_REGEX + "\\s*" + "\\)" + SYMBOL_RIGHT_BRACKET_STRING;
const string FOLLOWST_STRING_REGEX = FOLLOWS_STRING + "\\*" + "\\s*" + SYMBOL_LEFT_BRACKET_STRING + "\\(" + "\\s*" + STMTREF_STRING_REGEX + "\\s*"
+ "," + "\\s*" + STMTREF_STRING_REGEX + "\\s*" + "\\)" + SYMBOL_RIGHT_BRACKET_STRING;
const string NEXT_STRING_REGEX = NEXT_STRING + "\\s*" + SYMBOL_LEFT_BRACKET_STRING + "\\(" + "\\s*" + LINEREF_STRING_REGEX + "\\s*"
+ "," + "\\s*" + STMTREF_STRING_REGEX + "\\s*" + "\\)" + SYMBOL_RIGHT_BRACKET_STRING;
const string NEXTT_STRING_REGEX = NEXT_STRING + "\\*" + "\\s*" + SYMBOL_LEFT_BRACKET_STRING + "\\(" + "\\s*" + LINEREF_STRING_REGEX + "\\s*"
+ "," + "\\s*" + STMTREF_STRING_REGEX + "\\s*" + "\\)" + SYMBOL_RIGHT_BRACKET_STRING;
const string AFFECTS_STRING_REGEX = AFFECTS_STRING + "\\s*" + SYMBOL_LEFT_BRACKET_STRING + "\\(" + "\\s*" + STMTREF_STRING_REGEX + "\\s*"
+ "," + "\\s*" + STMTREF_STRING_REGEX + "\\s*" + "\\)" + SYMBOL_RIGHT_BRACKET_STRING;
const string AFFECTST_STRING_REGEX = AFFECTS_STRING + +" \\*" + "\\s*" + SYMBOL_LEFT_BRACKET_STRING + "\\(" + "\\s*" + STMTREF_STRING_REGEX + "\\s*"
+ "," + "\\s*" + STMTREF_STRING_REGEX + "\\s*" + "\\)" + SYMBOL_RIGHT_BRACKET_STRING;

const string RELREF_STRING_REGEX = SYMBOL_LEFT_BRACKET_STRING + SYMBOL_LEFT_BRACKET_STRING + MODIFIESS_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING
+ OR + SYMBOL_LEFT_BRACKET_STRING + USESS_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING
+ OR + SYMBOL_LEFT_BRACKET_STRING + PARENTT_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING
+ OR + SYMBOL_LEFT_BRACKET_STRING + PARENT_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING
+ OR + SYMBOL_LEFT_BRACKET_STRING + FOLLOWS_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING
+ OR + SYMBOL_LEFT_BRACKET_STRING + FOLLOWST_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING
+ OR + SYMBOL_LEFT_BRACKET_STRING + NEXT_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING
+ OR + SYMBOL_LEFT_BRACKET_STRING + NEXTT_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING
+ OR + SYMBOL_LEFT_BRACKET_STRING + CALLS_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING
+ OR + SYMBOL_LEFT_BRACKET_STRING + CALLST_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING
+ OR + SYMBOL_LEFT_BRACKET_STRING + MODIFIESP_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING
+ OR + SYMBOL_LEFT_BRACKET_STRING + USESP_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING
+ OR + SYMBOL_LEFT_BRACKET_STRING + AFFECTS_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING
+ OR + SYMBOL_LEFT_BRACKET_STRING + AFFECTST_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING
+ SYMBOL_RIGHT_BRACKET_STRING;

const string RELCOND_STRING_REGEX = SYMBOL_LEFT_BRACKET_STRING + RELREF_STRING_REGEX + SYMBOL_LEFT_BRACKET_STRING + "\\s*" + AND_STRING + "*" + "\\s+" + RELREF_STRING_REGEX + "\\s*" + SYMBOL_RIGHT_BRACKET_STRING + ASTERIK + SYMBOL_RIGHT_BRACKET_STRING;
const string SUCH_THAT_CL_REGEX = SUCH_THAT_STRING + "\\s+" + RELCOND_STRING_REGEX;
const string TEMPORARY = SYMBOL_LEFT_BRACKET_STRING + SUCH_THAT_CL_REGEX + SYMBOL_RIGHT_BRACKET_STRING;
const string SUCH_THAT_CL_EXTENDED_REGEX = TEMPORARY + SYMBOL_LEFT_BRACKET_STRING + "\\s*" + TEMPORARY + "\\s*" + SYMBOL_RIGHT_BRACKET_STRING + ASTERIK;
const string PATTERN_CL_REGEX = PATTERN_STRING + "\\s+" +
"([a-zA-Z])([a-zA-Z]|\\d|\#)*[ ]{0,1}\\(\\s*((([a-zA-Z])([a-zA-Z]|\\d|\\#)*)|(\\_)|(\"([a-zA-Z])([a-zA-Z]|\\d|\\#)*\"))\\s*,\\s*(\\_\"([a-zA-Z])(\\w)*((\\+|\\*|\\-)\\w+)*\"\\_|\\_|\"([a-zA-Z])(\\w)*((\\+|\\*|\\-)\\w+)*\")\\s*\\)";
const string BRACKETED_SYNONYM = SYMBOL_LEFT_BRACKET_STRING + SYNONYM_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING;
const string GENERAL_PATTERN_CL_REGEX = SYMBOL_LEFT_BRACKET_STRING + PATTERN_STRING + "\\s+" + SYNONYM_STRING_REGEX + "\\s*" + "\\(" + ".+?" + "\\)" + SYMBOL_RIGHT_BRACKET_STRING;
const string GENERAL_PATTERN_CL_EXTENDED_REGEX = SYMBOL_LEFT_BRACKET_STRING + GENERAL_PATTERN_CL_REGEX + SYMBOL_RIGHT_BRACKET_STRING
+ SYMBOL_LEFT_BRACKET_STRING + SYMBOL_LEFT_BRACKET_STRING + "\\s*" + AND_STRING + "\\s+" + SYMBOL_RIGHT_BRACKET_STRING + "?" + "\\s*" + GENERAL_PATTERN_CL_REGEX + SYMBOL_RIGHT_BRACKET_STRING + ASTERIK;

const string IF_PATTERN_REGEX = "\\s*" + BRACKETED_SYNONYM + "\\s*" + "\\(" + "\\s*" + VARREF_STRING_REGEX + "\\s*" + "," + "\\s*" + "_" + "\\s*" + "," + "\\s*" + "_" + "\\s*" + "\\)";
const string WHILE_PATTERN_REGEX = "\\s*" + BRACKETED_SYNONYM + "\\s*" + "\\(" + "\\s*" + VARREF_STRING_REGEX + "\\s*" + "," + "\\s*" + "_" + "\\s*" + "\\)";
const string ASSIGN_PATTERN_REGEX = "\\s*" + BRACKETED_SYNONYM + "\\s*" + "\\(" + "\\s*" + VARREF_STRING_REGEX + "\\s*" + "," + "\\s*" + "([^\\.\\,\\@\\$\\%\\&\\^\\'\\=\\~\\`\/\]+)" + "\\s*" + "\\)";
const string SUB_MATCH_REGEX = "([\\w][\\w\\d\#]*\\s+[\\w][\\w\\d\#]*)";
const string IF_PATTERN_AND_REGEX = SYMBOL_LEFT_BRACKET_STRING + IF_PATTERN_REGEX + SYMBOL_RIGHT_BRACKET_STRING + SYMBOL_LEFT_BRACKET_STRING + "\\s*" + SYMBOL_LEFT_BRACKET_STRING + AND_STRING + "\\s+" + SYMBOL_RIGHT_BRACKET_STRING + "{0,1}"
+ SYMBOL_RIGHT_BRACKET_STRING + IF_PATTERN_REGEX + SYMBOL_RIGHT_BRACKET_STRING + ASTERIK;
const string PARTIAL_PATTERN_REGEX = "(and\\s+)(.+)";

const string ELEM_REGEX = SYMBOL_LEFT_BRACKET_STRING  + IDENT_STRING_REGEX + OR + ATTRREF_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING;
const string TUPLE_REGEX = ELEM_REGEX + OR + SYMBOL_LEFT_BRACKET_STRING + "\\<" + "\\s*" + ELEM_REGEX + "\\s*" + SYMBOL_LEFT_BRACKET_STRING + "\\s*" + "," + "\\s*" + ELEM_REGEX + + "\\s*" + SYMBOL_RIGHT_BRACKET_STRING + ASTERIK + "\\>" + SYMBOL_RIGHT_BRACKET;
const string ONLY_TUPLE_REGEX = SYMBOL_LEFT_BRACKET_STRING + "\\<" + "\\s*" + ELEM_REGEX + "\\s*" + SYMBOL_LEFT_BRACKET_STRING + "\\s*" + "," + "\\s*" + ELEM_REGEX + +"\\s*" + SYMBOL_RIGHT_BRACKET_STRING + ASTERIK + "\\>" + SYMBOL_RIGHT_BRACKET;
//const string SELECT_INITIAL_REGEX = SYMBOL_LEFT_BRACKET_STRING + SELECT_STRING + "\\s+" + "([a-zA-Z])([a-zA-Z]|\\d|\\#)*|(BOOLEAN)" + OR + ATTRREF_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING;
const string SELECT_INITIAL_REGEX = SYMBOL_LEFT_BRACKET_STRING + SELECT_STRING + "\\s+" + SYMBOL_LEFT_BRACKET_STRING + "BOOLEAN" + OR + TUPLE_REGEX + SYMBOL_RIGHT_BRACKET_STRING + SYMBOL_RIGHT_BRACKET_STRING;
const string INVALID_BRACKET_EXEPTION = "Invalid Bracket Exception";

using namespace std;
QueryValidator::QueryValidator()
{
	//Add in the elements into the unordered map that will be used for with clauses
	withClauseTypeBank[PROCNAME] = STRTYPE;
	withClauseTypeBank[VARNAME] = STRTYPE;
	withClauseTypeBank[VALUE] = INTTYPE;
	withClauseTypeBank[STMTNUM] = INTTYPE;
	withClauseTypeBank[PROG_LINE_STRING] = INTTYPE;
	withClauseTypeBank[STRING_LITERAL] = STRTYPE;
	withClauseTypeBank[NUMBER_STRING] = INTTYPE;

	attrNameBank[PROCNAME] = { PROCEDURE_STRING, CALL_STRING };
	attrNameBank[VARNAME] = { VARIABLE_STRING };
	attrNameBank[VALUE] = { CONSTANT_STRING };
	attrNameBank[STMTNUM] = { STMT_STRING, ASSIGN_STRING, IF_STRING, WHILE_STRING, CALL_STRING };
}
//With a given str, loop thru to find ;
bool QueryValidator::parseInput(string str) {
	//Upon taking in a string query, clear the following
	queryStatement = QueryStatement();
	synonymAndEntityList = vector<SynonymEntityPair>();

	str = removeDuplicatesWhiteSpaces(str);

	if (str.find(SYMBOL_SEMI_COLON_STRING)) {
		bool valQuery = false;
		size_t pos;
		std::string token;

		while ((pos = str.find(SYMBOL_SEMI_COLON_STRING) != std::string::npos)) {
			pos = str.find(SYMBOL_SEMI_COLON_STRING);
			token = str.substr(ZERO, pos);
			string toCheck = token + SYMBOL_SEMI_COLON_STRING;
			toCheck = Util::trim(toCheck);
			if (!isValidDeclarationRegex(toCheck)) {
				return false;
			}
			if (!isEntityAndSynonym(token)) {
				return false;
			}
			str.erase(ZERO, pos + ONE);
		}
		cout << "hi";
		//After the loop ends, comes the select query
		if (str.find(SELECT_STRING) != string::npos) {
			return parseQueryLine(str);
		} else {
			return false;
		}
	} else {
		cout << WRONG_SYNTAX_ERROR;
		return false;
	}
}


//This function parses the Entity And Synonym and checks for the validity of Entity
bool QueryValidator::isEntityAndSynonym(string currentString) {
	if (isValidEntity(currentString)) {
		addSynonymEntityList();
		return true;
	} else {
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
	} else {
		return true;
	}
}

//This function checks for the validity of the declaration by comparing with the the entityList given
//Returns true if there is a match, else returns false
bool QueryValidator::isValidEntity(string currentString) {
	currentString = Util::trim(currentString);
	//Places the first vector with the unchecked entity, and subsequent vectors with synonyms
	std::vector<string> splitString = splitBySymbol(currentString, SYMBOL_WHITESPACE);
	//checks if the entity exists, if so, do parsing on the declarations
	if (inEntityList(splitString.front())) {
		if (parseDeclaration(splitString)) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

//Checks if a given entity is in the EntityTable
bool QueryValidator::inEntityList(string entity) {

	for (size_t i = ZERO; i < validEntities.size(); i++) {
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
	synonymsStr = removeSymbols(synonymsStr, TAB_STRING);
	//Split the synonymsStr by comma to obtain relevant synonyms create object SynonymEntityPair and places in the "EntTable"
	vector<string> synonymsAttached = splitBySymbol(synonymsStr, SYMBOL_COMMA);
	synonymAndEntityList.push_back(SynonymEntityPair(reqEntity, synonymsAttached));
	return true;
}

bool QueryValidator::isValidQueryLine(string selectString) {

	vector<string> initialVector;
	initialVector.push_back(selectString);

	initialVector = splitToSentences(selectString);
	//After splitting do validation different parts of the vector
	if (isValidSelect(initialVector) && isValidOthers(initialVector)) {
		return true;
	} else {
		return false;

	}
}
//Posssible str received here are:
//pattern a(,) pattern(,)
//pattern ifs(,,) and pattern a()
//pattern w(_,_) and pattern a() pattern ifs(,,)
bool QueryValidator::isValidPattern(string str) {
	//First Util::trim it
	str = Util::trim(str);

	//Extract out all the pattern
	vector<string> vecPattern = extractPatternClauses(str);

	if (vecPattern[ZERO] == INVALID_BRACKET_EXEPTION) {
		return false;
	}
	for (size_t i = ZERO; i < vecPattern.size(); i++) {
		string currentStr = vecPattern.at(i);
		//Do Util::trimming on this string
		currentStr = Util::trim(currentStr);
		//if string consists of AND
		if (currentStr.find(AND_STRING) != string::npos) {
			//Remove Leading 'And' if it exists
			if (isLeadingAnd(currentStr)) {
				currentStr = removeLeadingAnd(currentStr);
				currentStr = PATTERN_STRING + WHITESPACE_STRING + currentStr;
			}
		}
		//Checks generally up to before first occurence of left bracket
		if (!isValidGeneralPatternRegex(currentStr)) {
			return false;
		}
		//What i should do: remove leading pattern
		//With given string, first remove the word pattern
		currentStr = currentStr.substr(PATTERN_STRING_LENGTH, currentStr.length() - PATTERN_STRING_LENGTH);
		//Util::trim it again
		currentStr = Util::trim(currentStr);

		//To be used as identification of the pattern regex
		bool isAssign = false;
		bool isIf = false;
		bool isWhile = false;

		int idxFirstLeftBracket = currentStr.find(SYMBOL_LEFT_BRACKET_STRING);
		string synPattern = currentStr.substr(ZERO, idxFirstLeftBracket);
		synPattern = Util::trim(synPattern);
		string entPattern = getCorrespondingEntity(synPattern);

		if (entPattern != ASSIGN_STRING && entPattern != IF_STRING && entPattern != WHILE_STRING) {
			return false;
		} else if (entPattern == ASSIGN_STRING) {
			if (!isValidAddAssignPattern(currentStr, synPattern)) {
				return false;
			}
		} else if (entPattern == IF_STRING) {
			if (!isValidAddIfPattern(currentStr, synPattern)) {
				return false;
			}
		} else if (entPattern == WHILE_STRING) {
			if (!isValidAddWhilePattern(currentStr, synPattern)) {
				return false;
			}
		} else {
			return false;
		}
	}
	return true;
}

//This function handles the Assign pattern scenario
bool QueryValidator::isValidAddAssignPattern(string str, string synPattern) {
	//Cannot really use regex due to brackets, so perform manual check on each arguments
	//However can check generally for the first arg
	if (!isAssignPatternRegex(str)) {
		return false;
	}

	//Find the first occurence of left bracket
	int idxFirstLeftBracket = str.find(SYMBOL_LEFT_BRACKET_STRING);

	//Get the string without the synPattern
	string curr = str.substr(idxFirstLeftBracket, str.length() - idxFirstLeftBracket);

	//Remove outer 2 brackets
	curr = removeOuterParentheses(curr);

	//Split the string by comma
	vector<string> assignVecPattern = splitBySymbol(curr, SYMBOL_COMMA);

	string arg1 = assignVecPattern.at(ZERO);
	string arg2 = assignVecPattern.at(ONE);

	//Firstly trim them
	arg1 = Util::trim(arg1);
	arg2 = Util::trim(arg2);

	//Then for each arg, check wad type they are
	bool arg1UnderScore = false;
	bool arg1Variable = false;
	bool arg1StringLiteral = false;
	bool arg2UnderScore = false;
	bool arg2Exact = false;
	bool arg2Substring = false;

	if (isWildcard(arg1)) {
		arg1UnderScore = true;
	} else if (isQuotationIdentRegex(arg1)) {
		arg1StringLiteral = true;
		//At this pt do some cleaning up
		arg1 = removeSymbols(arg1, INVERTED_COMMA_STRING);
		arg1 = Util::trim(arg1);
	} else if (isVariableSynonym(arg1)) {
		arg1Variable = true;
	} else {
		return false;
	}
	if (isWildcard(arg2)) {
		arg2UnderScore = true;
	} else if (isValidExpr(arg2)) {
		arg2Exact = true;
		arg2 = arg2.substr(1, arg2.length() - 2);
		arg2 = Util::trim(arg2);
		arg2 = Util::insertBrackets(arg2);
	} else if (isValidExprUnder(arg2)) {
		arg2Substring = true;
		arg2 = removeUnderScoreAndQuotation(arg2);
		arg2 = removeSymbols(arg2, WHITESPACE_STRING);
		arg2 = removeSymbols(arg2, TAB_STRING);
		arg2 = Util::insertBrackets(arg2);
	} else {
		return false;
	}
	if ((!arg1Variable && !arg1UnderScore && !arg1StringLiteral) || (!arg2UnderScore && !arg2Exact
		&& !arg2Substring)) {
		return false;
	}

	//At this pt, arg1 and arg2 must be valid so we call a function

	addAssignPatternQueryElement(arg1, arg2, ASSIGN_STRING, synPattern, arg1UnderScore, arg1Variable, arg1StringLiteral, arg2UnderScore, arg2Exact
		, arg2Substring);
	return true;
}

void QueryValidator::addAssignPatternQueryElement(string arg1, string arg2, string ent, string syn, bool arg1UnderScore, bool arg1Variable,
	bool arg1StringLiteral, bool arg2UnderScore, bool arg2Exact, bool arg2Substring) {
	if (arg1UnderScore) {
		if (arg2UnderScore) {
			QueryElement assignPatternQueryElement = QueryElement(arg1, arg2, EMPTY_STRING, ent, syn, WILDCARD_STRING, WILDCARD_STRING, EMPTY_STRING, EMPTY_STRING);
			queryStatement.addPatternQuery(assignPatternQueryElement);
		} else if (arg2Exact) {
			QueryElement assignPatternQueryElement = QueryElement(arg1, arg2, EMPTY_STRING, ent, syn, WILDCARD_STRING, EXACT_STRING, EMPTY_STRING, EMPTY_STRING);
			queryStatement.addPatternQuery(assignPatternQueryElement);
		} else if (arg2Substring) {
			QueryElement assignPatternQueryElement = QueryElement(arg1, arg2, EMPTY_STRING, ent, syn, WILDCARD_STRING, SUBSTRING_STRING, EMPTY_STRING, EMPTY_STRING);
			queryStatement.addPatternQuery(assignPatternQueryElement);
		}
	} else if (arg1Variable) {
		if (arg2UnderScore) {
			QueryElement assignPatternQueryElement = QueryElement(arg1, arg2, EMPTY_STRING, ent, syn, SYNONYM_STRING, WILDCARD_STRING, EMPTY_STRING, VARIABLE_STRING);
			queryStatement.addPatternQuery(assignPatternQueryElement);
		} else if (arg2Exact) {
			QueryElement assignPatternQueryElement = QueryElement(arg1, arg2, EMPTY_STRING, ent, syn, SYNONYM_STRING, EXACT_STRING, EMPTY_STRING, VARIABLE_STRING);
			queryStatement.addPatternQuery(assignPatternQueryElement);
		} else if (arg2Substring) {
			QueryElement assignPatternQueryElement = QueryElement(arg1, arg2, EMPTY_STRING, ent, syn, SYNONYM_STRING, SUBSTRING_STRING, EMPTY_STRING, VARIABLE_STRING);
			queryStatement.addPatternQuery(assignPatternQueryElement);
		}
	} else if (arg1StringLiteral) {
		if (arg2UnderScore) {
			QueryElement assignPatternQueryElement = QueryElement(arg1, arg2, EMPTY_STRING, ent, syn, VARIABLE_STRING, WILDCARD_STRING, EMPTY_STRING, EMPTY_STRING);
			queryStatement.addPatternQuery(assignPatternQueryElement);
		} else if (arg2Exact) {
			QueryElement assignPatternQueryElement = QueryElement(arg1, arg2, EMPTY_STRING, ent, syn, VARIABLE_STRING, EXACT_STRING, EMPTY_STRING, EMPTY_STRING);
			queryStatement.addPatternQuery(assignPatternQueryElement);
		} else if (arg2Substring) {
			QueryElement assignPatternQueryElement = QueryElement(arg1, arg2, EMPTY_STRING, ent, syn, VARIABLE_STRING, SUBSTRING_STRING, EMPTY_STRING, EMPTY_STRING);
			queryStatement.addPatternQuery(assignPatternQueryElement);
		}
	}
}


//This method checks if given string is a well-formed expression
bool QueryValidator::isValidExpr(string str) {
	//Several checks to perform here
	//First ensure that no. of left brackets == no. of right brackets
	//Next ensure that onli string,integers and brackets are allowed
	//Next ensure that within a bracket, it is not empty
	//Next ensure that when an operator is encountered, LHS can onli be a ')', string/integer. RHS can onli be string/integer '('
	//Next ensure that when a string/integer is encounter LHS can never be a ')', RHS can never be '('
	//Given string will be sth like "wwwww"
	//Trim it first
	str = Util::trim(str);

	if (!isExactString(str)) {
		return false;
	} else {
		str = str.substr(ONE, str.length() - 2);
		return isBalancedParantheses(str);
	}
}

//This function takes in a string str and checks if LHS = RHS
bool QueryValidator::isBalancedParantheses(string str) {
	return Util::isValidExpression(str);
}
//This method checks if a given string a well-formed expression with underscores at both ends
bool QueryValidator::isValidExprUnder(string str) {
	str = Util::trim(str);

	if (!isSubstring(str)) {
		return false;
	}
	else {
		str = removeUnderScoreAndQuotation(str);
		return isBalancedParantheses(str);
	}
}

string QueryValidator::removeUnderScoreAndQuotation(string str) {
	str = removeSymbols(str, INVERTED_COMMA_STRING);
	str = removeSymbols(str, UNDER_SCORE_STRING);

	return Util::trim(str);
}



//This function handles the while pattern scenario
bool QueryValidator::isValidAddWhilePattern(string str, string synPattern) {

	//Returns false if it doesnt matches the while regex
	if (!isValidWhilePatternRegex(str)) {
		return false;
	} else {
		int idxFirstLeftBracket = str.find(SYMBOL_LEFT_BRACKET_STRING);

		string curr = str.substr(idxFirstLeftBracket, str.length() - idxFirstLeftBracket);

		curr = Util::trim(curr);

		curr = curr.substr(ONE, curr.length() - TWO);

		vector<string> whilePatternVec = splitBySymbol(curr, SYMBOL_COMMA);
		string arg1 = whilePatternVec.at(ZERO);
		//trim arg1
		arg1 = Util::trim(arg1);

		bool arg1UnderScore = false;
		bool arg1Variable = false;
		bool arg1StringLiteral = false;

		if (isWildcard(arg1)) {
			arg1UnderScore = true;
			addWhilePatternQueryElement(arg1, arg1UnderScore, arg1Variable, arg1StringLiteral, synPattern);
			return true;
		} else if (isQuotationIdentRegex(arg1)) {
			arg1StringLiteral = true;
			arg1 = removeSymbols(arg1, INVERTED_COMMA_STRING);
			arg1 = Util::trim(arg1);
			addWhilePatternQueryElement(arg1, arg1UnderScore, arg1Variable, arg1StringLiteral, synPattern);
			return true;
		} else if (isVariableSynonym(arg1)) {
			arg1Variable = true;
			addWhilePatternQueryElement(arg1, arg1UnderScore, arg1Variable, arg1StringLiteral, synPattern);
			return true;
		} else {
			return false;
		}
	}
}

//This function checks if the given pattern that matches the if regex is of the allowed semantics
//Possible if pattern : pattern ifs(x,_,_)		pattern ifs("x",_,_)		pattern ifs(1,_,_)		pattern ifs(_,_,_)
//While taking note that the entity before ( must be an 'if design entity
//This function assumes that the string received here will be
//ifs(....)		or ifs(.....)	or ifs	(....)	or ifs	(....)
//With pattern as the leading word
bool QueryValidator::isValidAddIfPattern(string str, string synPattern) {
	//If it does not matches the if regex
	if (!isValidIfPatternRegex(str)) {
		return false;
	} else {
		//Since regex passed, try and get wad is contained inside the string now
		//Find the occurence of the first left bracket
		int idxFirstLeftBracket = str.find(SYMBOL_LEFT_BRACKET_STRING);
		//Since it passed the regex, i am sure that arg2 and arg 3 will be wildcard i.e. underscore string
		//So all i need to do is find out what is arg1 
		string curr = str.substr(idxFirstLeftBracket, str.length() - idxFirstLeftBracket);
		//Remove the brackets
		curr = Util::trim(curr);
		curr = curr.substr(ONE, curr.length() - TWO);

		vector<string> ifPatternVec = splitBySymbol(curr, SYMBOL_COMMA);
		string arg1 = ifPatternVec.at(ZERO);
		//trim arg1
		arg1 = Util::trim(arg1);

		//Possible identities of arg1 entRef: stringliterals, synonym variable, number, underscore_String
		bool arg1UnderScore = false;
		bool arg1Variable = false;
		bool arg1StringLiteral = false;

		if (isWildcard(arg1)) {
			arg1UnderScore = true;
			addIfPatternQueryElement(arg1, arg1UnderScore, arg1Variable, arg1StringLiteral, synPattern);
			return true;
		} else if (isQuotationIdentRegex(arg1)) {
			arg1StringLiteral = true;
			arg1 = removeSymbols(arg1, INVERTED_COMMA_STRING);
			arg1 = Util::trim(arg1);
			addIfPatternQueryElement(arg1, arg1UnderScore, arg1Variable, arg1StringLiteral, synPattern);
			return true;
		} else if (isVariableSynonym(arg1)) {
			arg1Variable = true;
			addIfPatternQueryElement(arg1, arg1UnderScore, arg1Variable, arg1StringLiteral, synPattern);
			return true;
		} else {
			return false;
		}
	}
}
//This functions takes in arg1 and 4 boolean and 1 string and creates a	QueryElement to be added to the QueryStatement
void QueryValidator::addIfPatternQueryElement(string arg1, bool arg1Underscore, bool arg1Variable, bool arg1StringLiteral, string synPattern) {
	//If arg1 is a boolean, just add
	if (arg1Underscore) {
		QueryElement ifQueryElement = QueryElement(UNDER_SCORE_STRING, UNDER_SCORE_STRING, UNDER_SCORE_STRING, IF_STRING, synPattern, WILDCARD_STRING, WILDCARD_STRING, WILDCARD_STRING, EMPTY_STRING);
		queryStatement.addPatternQuery(ifQueryElement);
	} else if (arg1StringLiteral) {
		QueryElement ifQueryElement = QueryElement(arg1, UNDER_SCORE_STRING, UNDER_SCORE_STRING, IF_STRING, synPattern, VARIABLE_STRING, WILDCARD_STRING, WILDCARD_STRING, EMPTY_STRING);
		queryStatement.addPatternQuery(ifQueryElement);
	} else if (arg1Variable) {
		QueryElement ifQueryElement = QueryElement(arg1, UNDER_SCORE_STRING, UNDER_SCORE_STRING, IF_STRING, synPattern, SYNONYM_STRING, WILDCARD_STRING, WILDCARD_STRING, VARIABLE_STRING);
		queryStatement.addPatternQuery(ifQueryElement);
	}
}

void QueryValidator::addWhilePatternQueryElement(string arg1, bool arg1Underscore, bool arg1Variable, bool arg1StringLiteral, string synPattern) {
	if (arg1Underscore) {
		QueryElement whileQueryElement = QueryElement(UNDER_SCORE_STRING, UNDER_SCORE_STRING, EMPTY_STRING, WHILE_STRING, synPattern, WILDCARD_STRING, WILDCARD_STRING, EMPTY_STRING, EMPTY_STRING);
		queryStatement.addPatternQuery(whileQueryElement);
	} else if (arg1StringLiteral) {
		QueryElement whileQueryElement = QueryElement(arg1, UNDER_SCORE_STRING, EMPTY_STRING, WHILE_STRING, synPattern, VARIABLE_STRING, WILDCARD_STRING, EMPTY_STRING, EMPTY_STRING);
		queryStatement.addPatternQuery(whileQueryElement);
	} else if (arg1Variable) {
		QueryElement whileQueryElement = QueryElement(arg1, UNDER_SCORE_STRING, EMPTY_STRING, WHILE_STRING, synPattern, SYNONYM_STRING, WILDCARD_STRING, EMPTY_STRING, VARIABLE_STRING);
		queryStatement.addPatternQuery(whileQueryElement);
	}
}
bool QueryValidator::isLeadingAnd(string str) {
	if (str.find(AND_STRING) == ZERO) {
		if (isPartialPatternRegex(str)) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}
//This function removes the leading pattern string, then trim it and returns this trimmed string
string QueryValidator::removeLeadingPatternString(string str) {

	str = str.substr(PATTERN_STRING.length(), str.length() - PATTERN_STRING.length());

	return Util::trim(str);
}
string QueryValidator::removeLeadingAnd(string str) {

	str = Util::trim(str);

	if (str.find(AND_STRING) == 0) {
		str = str.substr(THREE, str.length() - THREE);
	}
	return str;
}
//This functions checks if the given str is of the design entity
bool QueryValidator::isVariableSynonym(string str) {
	string ent = getCorrespondingEntity(str);
	return(ent == VARIABLE_STRING);
}
bool QueryValidator::isWildcard(string arg) {
	return (arg == UNDER_SCORE_STRING);
}
//This function takes in an argument(2) and checks if it is an exact string
bool QueryValidator::isExactString(string arg2) {
	if ((arg2.at(ZERO) == DOUBLE_QUOTATION) && (arg2.at(arg2.length() - 1) == DOUBLE_QUOTATION)) {
		return true;
	}
	else {
		return false;
	}
	return true;
}
//This function function checks for the select portion of the select query and ensure that the synonym
bool QueryValidator::isValidSelect(vector<string> vectorClauses) {
	//First element of this vector gives the select clause
	string selectClause = vectorClauses.at(ZERO);

	int toAdd = ZERO;
	selectClause = Util::trim(selectClause);
	if (!isValidSelectInitialRegex(selectClause)) {
		return false;
	}

	string resultCl = selectClause.substr(SELECT_STRING.length(), selectClause.length() - SELECT_STRING.length());
	resultCl = Util::trim(resultCl);
	resultCl = removeSymbols(resultCl, WHITESPACE_STRING);
	resultCl = removeSymbols(resultCl, TAB_STRING);

	//If its a synonym
	if (isIdent(resultCl) && isValidSynonym(resultCl)) {
		string ent = getCorrespondingEntity(resultCl);
		addSelectQueryElement(ent, resultCl, SYNONYM_STRING, SYNONYM_STRING);
		return true;
	} else if (resultCl == BOOLEAN_STRING) {
		addSelectQueryElement(EMPTY_STRING, EMPTY_STRING, BOOLEAN_STRING, EMPTY_STRING);
		return true;
	} else if (isAttrRef(resultCl)) {
		//Example p.procName, c.value etc
		vector<string> resultVec = splitBySymbol(resultCl, DOT_CHAR);
		string syn = resultVec.at(ZERO);
		string attrName = resultVec.at(ONE);
		//First check that synonym is declared and is a corresponding entity
		if (isValidSynonym(syn) && isValidCorrespondingEntity(syn, attrName)) {
			string ent = getCorrespondingEntity(syn);
			addSelectQueryElement(ent, syn, ATTRREF_STRING, attrName);
			return true;
		} else {
			return false;
		}
	} else if(isOnlyTuple(resultCl)) {
		//a,b.stmt#,c,d
		//Then remove the '<' and '>'
		resultCl = resultCl.substr(ONE, resultCl.length() - TWO);
		vector<string> resultVec = splitBySymbol(resultCl, SYMBOL_COMMA);
		if (isValidCorrespondingTupleEntities(resultVec)) {
			string allSyn = extractAllSyn(resultVec);		// a,b,c,d
			string allEnt = extractAllEnt(resultVec);
			string allSynAttr = extractAllSynAttr(resultVec);
			addSelectQueryElement(allEnt, allSyn, TUPLE_STRING, allSynAttr);
			return true;
		}
		else {
			return false;
		}
	} else {
		return false;
	}
}
string QueryValidator::extractAllSynAttr(vector<string> resultVec) {
	string toReturn;
	for (size_t i = 0; i < resultVec.size(); i++) {
		if (isAttrRef(resultVec.at(i))) {
			vector<string> tempVec = splitBySymbol(resultVec.at(i), DOT_CHAR);
			string attrName = tempVec.at(ONE);
			toReturn += attrName + COMMA_STRING;
		} else {
			toReturn += SYNONYM_STRING + COMMA_STRING;
		}
	}
	return toReturn.substr(ZERO, toReturn.length() - ONE);
}
bool QueryValidator::isValidCorrespondingTupleEntities(vector<string> resultVec) {
	for (size_t i = 0; i < resultVec.size(); i++) {
		if (isAttrRef(resultVec.at(i))) {
			vector<string> tempVec = splitBySymbol(resultVec.at(i), DOT_CHAR);
			string syn = tempVec.at(ZERO);
			string attrName = tempVec.at(ONE);
			if (!isValidCorrespondingEntity(syn, attrName)) {
				return false;
			}
		} else {
			
			string ent = getCorrespondingEntity(resultVec.at(i));
			if (ent == INVALID_STRING) {
				return false;
			}
		}
	}
	return true;
}
string QueryValidator::extractAllEnt(vector<string> resultVec) {
	string toReturn;
	for (size_t i = 0; i < resultVec.size(); i ++) {
		if (isAttrRef(resultVec.at(i))) {
			vector<string> tempVec = splitBySymbol(resultVec.at(i), DOT_CHAR);
			string ent = getCorrespondingEntity(tempVec.at(ZERO));
			toReturn += ent + COMMA_STRING;
		} else {
			string ent = getCorrespondingEntity(resultVec.at(i));
			toReturn += ent + COMMA_STRING;
		}
	}
	return toReturn.substr(ZERO, toReturn.length() - ONE);
}
string QueryValidator::extractAllSyn(vector<string> resultVec) {
	string toReturn;
	for (size_t i = 0; i < resultVec.size();  i++) {
		if (isAttrRef(resultVec.at(i))) {
			vector<string> tempVec = splitBySymbol(resultVec.at(i), DOT_CHAR);
			toReturn += tempVec.at(ZERO) + COMMA_STRING;
		}
		else {
			toReturn += resultVec.at(i) + COMMA_STRING;
		}
	}
	return toReturn.substr(ZERO, toReturn.length() - ONE);
}
bool QueryValidator::isValidCorrespondingEntity(string synonym, string attrName) {
	string ent = getCorrespondingEntity(synonym);
	
	vector<string> permittedEntities = attrNameBank[attrName];

	for (size_t i = 0; i < permittedEntities.size(); i++) {
		if (permittedEntities[i] == ent) {
			return true;
		}
	}
	return false;
}
void QueryValidator::addSelectQueryElement(string ent, string syn, string selectType, string str) {
	queryStatement.addSelectQuery(QueryElement(ent, syn, selectType, str));
}

void QueryValidator::addSuchThatQueryElement(QueryElement qe) {
	queryStatement.addSuchThatQuery(qe);
}

//isValid others currently accepts such that and pattern clause
bool QueryValidator::isValidOthers(vector<string> vec) {
	//This implies its a short query i.e. stmt s
	if (vec.size() < 2) {
		return true;
	} else {
		string selectStr = vec.at(ZERO);
		selectStr = Util::trim(selectStr);
		vector<string> vecSplit = splitBySymbol(selectStr, SYMBOL_WHITESPACE);
		//Obtain the synonym
		string syn = vecSplit.at(ONE);

		//loop through every string to check if the first index and last index is a whitespace
		//If so, remove them
		for (size_t k = ONE; k < vec.size(); k++) {
			Util::trim(vec.at(k));
		}
		vector<string> temp = vec;
		for (size_t i = ONE; i < vec.size(); i++) {
			if (vec.at(i).find(SELECT_STRING) != std::string::npos) {
				return false;
			} else if (vec.at(i).find(SUCH_THAT_STRING) != std::string::npos) {
				if (!isValidSuchThat(vec.at(i))) {
					return false;
				}
			} else if (vec.at(i).find(PATTERN_STRING) != std::string::npos) {
				if (!isValidPattern(vec.at(i))) {
					return false;
				}
			} else if (vec.at(i).find(WITH_STRING) != std::string::npos) {
				if (!isValidWith(vec.at(i))) {
					return false;
				}
			}
		}
		return true;
	}
}


//This function takes in e.g. str = Follows, type = stmt, idx = 1
bool QueryValidator::checkRelationshipTable(string rel, string type, int idx) {
	if (relationshipTable.isValidArg(rel, type, idx)) {
		return true;
	} else {
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
			if (tempVec.at(j) == syn) {
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
vector<string> QueryValidator::splitToSentences(string strToSplit) {
	string curr = strToSplit;
	vector<string> results;
	int currIdx = ZERO;

	size_t posSuchThat = curr.find(SUCH_THAT_STRING);

	size_t posPattern = curr.find(PATTERN_STRING);

	size_t posWith = curr.find(WITH_STRING);

	//Compare to get the one with smallest index
	//Then find the occurence of the one with smallest index
	size_t posInterest;
	string clauseInterest;
	if ((posSuchThat != std::string::npos) && (posSuchThat < posPattern) && (posSuchThat < posWith)) {
		posInterest = posSuchThat;
		clauseInterest = SUCH_THAT_STRING;
	} else if ((posPattern != std::string::npos) && (posPattern < posSuchThat) && (posPattern < posWith)) {
		posInterest = posPattern;
		clauseInterest = PATTERN_STRING;
	} else if ((posWith != std::string::npos) && (posWith < posSuchThat) && (posWith < posSuchThat)) {
		posInterest = posWith;
		clauseInterest = WITH_STRING;
	}
	//If Select clauses are sth like Select s, just return
	if ((posSuchThat == std::string::npos) && (posPattern == std::string::npos)
		&& (posWith == std::string::npos)) {
		results.push_back(curr);
		return results;
	}

	size_t nextPosInterest = posInterest;
	string nextClauseInterest = clauseInterest;

	int count = ZERO;
	while (currIdx != -1) {
		string toPush = curr.substr(ZERO, nextPosInterest);
		toPush = Util::trim(toPush);
		toPush = trim(toPush);
		results.push_back(toPush);
		//Get the next instance of the string
		curr = curr.substr(nextPosInterest, curr.length());

		size_t nextPosSuchThat = curr.find(SUCH_THAT_STRING);

		size_t nextPosPattern = curr.find(PATTERN_STRING);

		size_t nextPosWith = curr.find(WITH_STRING);

		if ((nextPosSuchThat != std::string::npos) && (nextPosSuchThat != ZERO)
			&& (((nextPosPattern == ZERO) && (nextPosSuchThat < nextPosWith)) || ((nextPosWith == ZERO) && (nextPosSuchThat < nextPosPattern)))) {
			nextPosInterest = nextPosSuchThat;
			nextClauseInterest = SUCH_THAT_STRING;
		} else if ((nextPosPattern != std::string::npos) && (nextPosPattern != ZERO)
			&& (((nextPosSuchThat == ZERO) && (nextPosPattern < nextPosWith)) || ((nextPosWith == ZERO) && (nextPosPattern > nextPosWith)))) {
			nextPosInterest = nextPosPattern;
			nextClauseInterest = PATTERN_STRING;

		} else if ((nextPosWith != std::string::npos) && (nextPosWith != ZERO)
			&& (((nextPosSuchThat == ZERO) && (nextPosPattern > nextPosWith)) || ((nextPosPattern == ZERO) && (nextPosSuchThat > nextPosWith)))) {
			nextPosInterest = nextPosWith;
			nextClauseInterest = WITH_STRING;

		} else if (((nextPosSuchThat == ZERO) && (nextPosWith == std::string::npos) && (nextPosPattern == std::string::npos))
			|| ((nextPosPattern == ZERO) && (nextPosSuchThat == std::string::npos) && (nextPosWith == std::string::npos))
			|| ((nextPosWith == ZERO) && (nextPosSuchThat == std::string::npos) && (nextPosPattern == std::string::npos))) {
			toPush = curr.substr(ZERO, curr.length());
			toPush = Util::trim(toPush);
			toPush = trim(toPush);
			results.push_back(toPush);
			break;
		} else {
			//Sth went wrong
			break;
		}
		currIdx = nextPosInterest;
	}
	return results;
}

vector<string> QueryValidator::extractWithClauses(string str) {
	regex withRegex(ATTRCOMPARE_STRING_REGEX);
	smatch m;
	string temp;
	vector<string> toReturnVec;

	while (regex_search(str, m, withRegex)) {
		temp = m[ZERO];
		toReturnVec.push_back(temp);
		str = m.suffix().str();
	}
	return toReturnVec;
}

vector<string> QueryValidator::extractPatternClauses(string str) {
	
	size_t pos = ZERO;
	string curr = str;
	vector<string> vecPattern;
	try {
		//These are some example strings: 
		//pattern a(_,_) pattern ifs(_,_)
		//pattern a(_,_) and ifs(_,_) pattern ifs(_,_)
		//pattern a(_,_) pattern ifs(_,_) and ifs(_,_)
		//While pos is not end of string
		while (pos != string::npos) {
			//Try to find the next occurence of pattern
			size_t initFound = curr.find(PATTERN_STRING);
			size_t found = curr.find(PATTERN_STRING, initFound + ONE);
			size_t foundAnd = curr.find(AND_STRING, ONE);
			//Means end of string, so just push in and break
			if (found == string::npos && foundAnd == string::npos) {
				curr = Util::trim(curr);
				if (curr == "") {
					break;
				}
				vecPattern.push_back(curr);
				break;
			}
			else {
				//curr is now pattern(....)...
				string currTwo = curr.substr(ZERO, found);
				size_t lastBracket = currTwo.find_last_of(SYMBOL_RIGHT_BRACKET_STRING);
				currTwo = currTwo.substr(ZERO, lastBracket + ONE);
				currTwo = Util::trim(currTwo);
				if (currTwo.find(AND_STRING) == string::npos) {
					if (lastBracket == string::npos) {
						throw (INVALID_BRACKET_EXEPTION);
					}
					vecPattern.push_back(currTwo);
					curr = curr.substr(lastBracket + ONE, curr.length() - lastBracket);
					pos = lastBracket + ONE;
				}
				else {
					if (lastBracket == string::npos) {
						throw (INVALID_BRACKET_EXEPTION);
					}
					vecPattern = extractAndPattern(currTwo, vecPattern);
					curr = curr.substr(lastBracket + ONE, curr.length() - lastBracket);
					pos = lastBracket + ONE;
				}
			}
		}
	}
	catch (string INVALID_BRACKET_EXEPTION) {
		cout << INVALID_BRACKET_EXEPTION;
		vector<string> invalidVec = { INVALID_BRACKET_EXEPTION };
		return invalidVec;
	}
	return vecPattern;
}

//At this point the string consists of 'And' string
vector<string> QueryValidator::extractAndPattern(string currTwo, vector<string>vecPattern) {
	size_t pos = ZERO;
	//While there are no more 'And' in the given string
	while (pos != string::npos) {
		size_t nextAnd = currTwo.find(AND_STRING, ONE);
		string currThree = currTwo.substr(ZERO, nextAnd);
		vecPattern.push_back(currThree);
		if (nextAnd == string::npos) {
			break;
		}
		currTwo = currTwo.substr(nextAnd, currTwo.length() - nextAnd);
	}
	return vecPattern;
}
string QueryValidator::removeSymbols(string str, string symbolToRemove) {
	std::regex pattern(symbolToRemove);
	std::string toReturn = std::regex_replace(str, pattern, "");
	return toReturn;
}

string QueryValidator::removeDuplicatesWhiteSpaces(string str) {
	size_t pos;
	while ((pos = str.find("  ")) != std::string::npos)
		str = str.replace(pos, 2, " ");
	return str;
}
string QueryValidator::removeOuterParentheses(string str) {
	str = str.substr(ONE, str.length() - ONE);
	str = str.substr(ZERO, str.length() - ONE);
	return str;
}
string QueryValidator::trim(string str) {
	return regex_replace(str, regex("^ +| +$|( ) +"), "$1");
}
//Takes in a string and check if it matches the declaration regex
//Returns true if matches, else false
bool QueryValidator::isValidDeclarationRegex(string str) {
	regex declareRegex("((stmt|assign|while|variable|constant|prog_line|procedure|if|call|stmtLst)\\s+(([a-zA-Z])([a-zA-Z]|\\d|\#)*)\\s*(\\,\\s*([a-zA-Z])([a-zA-Z]|\\d|\#)*)*)\\;");
	return regex_match(str, declareRegex);
}
//Takes in a string and checks if it matches the such that regex
//Retunrs true if matches, else false
bool QueryValidator::isValidSuchThatRegex(string str) {
	regex suchThatRegex(SUCH_THAT_CL_REGEX);
	return regex_match(str, suchThatRegex);
}
bool QueryValidator::isValidSuchThaExtendedRegex(string str) {
	regex suchThatExtendedRegex(SUCH_THAT_CL_EXTENDED_REGEX);
	return regex_match(str, suchThatExtendedRegex);
}
bool QueryValidator::isValidWithRegex(string str) {
	regex withRegex(WITH_CL_REGEX);
	return regex_match(str, withRegex);
}
bool QueryValidator::isValidWithExtendedRegex(string str) {
	regex withExtendedRegex(WITH_CL_EXTENDED_REGEX);
	return regex_match(str, withExtendedRegex);
}
bool QueryValidator::isValidAttrCondRegex(string str) {
	regex attrCondRegex(ATTRCOND_STRING_REGEX);
	return regex_match(str, attrCondRegex);
}
bool QueryValidator::isValidAttrCompareRegex(string str) {
	regex attrCompareRegex(ATTRCOMPARE_STRING_REGEX);
	return regex_match(str, attrCompareRegex);
}
bool QueryValidator::isValidLeadTrail(string str) {
	str = Util::trim(str);
	if (str == AND_STRING) {
		return true;
	} else {
		if (str == "") {
			return true;
		} else {
			return false;
		}
	}
}
bool QueryValidator::isValidAttRefRegex(string str) {
	regex attrRefRegex(ATTRREF_STRING_REGEX);
	return regex_match(str, attrRefRegex);
}
bool QueryValidator::isValidIfMultiplePatternRegex(string str) {
	regex ifPatternAndRegex(IF_PATTERN_AND_REGEX);
	return regex_match(str, ifPatternAndRegex);
}
bool QueryValidator::isValidSelectInitialRegex(string str) {
	regex patternRegex(SELECT_INITIAL_REGEX);
	return regex_match(str, patternRegex);
}
bool QueryValidator::isValidGeneralPatternRegex(string str) {
	regex generalPatternRegex(GENERAL_PATTERN_CL_REGEX);
	return regex_match(str, generalPatternRegex);
}
bool QueryValidator::isValidWhilePatternRegex(string str) {
	regex whilePatternRegex(WHILE_PATTERN_REGEX);
	return regex_match(str, whilePatternRegex);
}
bool QueryValidator::isValidIfPatternRegex(string str) {
	regex ifPatternRegex(IF_PATTERN_REGEX);
	return regex_match(str, ifPatternRegex);
}
bool QueryValidator::isQuotationIdentRegex(string str) {
	regex quotationRegex(QUOTATION_IDENT_STRING_REGEX);
	return regex_match(str, quotationRegex);
}
bool QueryValidator::isPartialPatternRegex(string str) {
	regex partialPatternRegex(PARTIAL_PATTERN_REGEX);
	return regex_match(str, partialPatternRegex);
}
bool QueryValidator::isAssignPatternRegex(string str) {
	regex assignPatternRegex(ASSIGN_PATTERN_REGEX);
	return regex_match(str, assignPatternRegex);
}
bool QueryValidator::isString(string str) {
	regex isStringRegex(STRING_REGEX);
	return regex_match(str, isStringRegex);
}
bool QueryValidator::isWhiteSpaceTab(string str) {
	regex isWhiteSpaceTabRegex(WHITESPACE_TAB_REGEX);
	return regex_match(str, isWhiteSpaceTabRegex);
}
bool QueryValidator::isSubstring(string str) {
	regex subStringRegex(SUBSTRING_REGEX);
	return regex_match(str, subStringRegex);
}
bool QueryValidator::isValidTuple(string str) {
	regex tupleRegex(TUPLE_REGEX);
	return regex_match(str, tupleRegex);
}
bool QueryValidator::isIdent(string str) {
	regex identRegex(IDENT_STRING_REGEX);
	return regex_match(str, identRegex);
}
bool QueryValidator::isOnlyTuple(string str) {
	regex onlyTupleRegex(ONLY_TUPLE_REGEX);
	return regex_match(str, onlyTupleRegex);
}