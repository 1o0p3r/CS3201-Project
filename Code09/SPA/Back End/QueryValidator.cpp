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
const string STMTREF_STRING_REGEX = "((([a-zA-Z])([a-zA-Z]|\\d|\#)*)|(\_)|(\\d+))"; //Nth wrg here
const string LINEREF_STRING_REGEX = STMTREF_STRING_REGEX;
const string ENTREF_STRING_REGEX = "((([a-zA-Z])([a-zA-Z]|\\d|\#)*)|(\_)|(\\d+)|(\"\\s*([a-zA-Z])([a-zA-Z]|\\d|\#)*\\s*\"))"; //Nth wrg here either
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


//Regexs for such that relationships
const string TEMP_MODIFIESP_STRING_REGEX = MODIFIES_STRING + SYMBOL_LEFT_BRACKET_STRING + "\\(" + "\\s*" + ENTREF_STRING_REGEX + "\\s*"
+ "," + "\\s*" + ENTREF_STRING_REGEX + "\\s*" + "\\)" + SYMBOL_RIGHT_BRACKET_STRING;
const string TEMP_MODIFIESS_STRING_REGEX = MODIFIES_STRING + SYMBOL_LEFT_BRACKET_STRING + "\\(" + "\\s*" + STMTREF_STRING_REGEX + "\\s*"
+ "," + "\\s*" + ENTREF_STRING_REGEX + "\\s*" + "\\)" + SYMBOL_RIGHT_BRACKET_STRING;

const string TEMP_USESP_STRING_REGEX = USES_STRING + SYMBOL_LEFT_BRACKET_STRING + "\\(" + "\\s*" + ENTREF_STRING_REGEX + "\\s*"
+ "," + "\\s*" + ENTREF_STRING_REGEX + "\\s*" + "\\)" + SYMBOL_RIGHT_BRACKET_STRING;
const string TEMP_USESS_STRING_REGEX = USES_STRING + SYMBOL_LEFT_BRACKET_STRING + "\\(" + "\\s*" + STMTREF_STRING_REGEX + "\\s*"
+ "," + "\\s*" + ENTREF_STRING_REGEX + "\\s*" + "\\)" + SYMBOL_RIGHT_BRACKET_STRING;

const string TEMP_CALLS_STRING_REGEX = CALLS_STRING + SYMBOL_LEFT_BRACKET_STRING + "\\(" + "\\s*" + ENTREF_STRING_REGEX + "\\s*"
+ "," + "\\s*" + ENTREF_STRING_REGEX + "\\s*" + "\\)" + SYMBOL_RIGHT_BRACKET_STRING;
const string TEMP_CALLST_STRING_REGEX = CALLS_STRING + "\\*" + SYMBOL_LEFT_BRACKET_STRING + "\\(" + "\\s*" + ENTREF_STRING_REGEX + "\\s*"
+ "," + "\\s*" + ENTREF_STRING_REGEX + "\\s*" + "\\)" + SYMBOL_RIGHT_BRACKET_STRING;

const string TEMP_PARENT_STRING_REGEX = PARENT_STRING + SYMBOL_LEFT_BRACKET_STRING + "\\(" + "\\s*" + STMTREF_STRING_REGEX + "\\s*"
+ "," + "\\s*" + STMTREF_STRING_REGEX + "\\s*" + "\\)" + SYMBOL_RIGHT_BRACKET_STRING;
const string TEMP_PARENTT_STRING_REGEX = PARENT_STRING + "\\*" + SYMBOL_LEFT_BRACKET_STRING + "\\(" + "\\s*" + STMTREF_STRING_REGEX + "\\s*"
+ "," + "\\s*" + STMTREF_STRING_REGEX + "\\s*" + "\\)" + SYMBOL_RIGHT_BRACKET_STRING;

const string TEMP_FOLLOWS_STRING_REGEX = FOLLOWS_STRING + SYMBOL_LEFT_BRACKET_STRING + "\\(" + "\\s*" + STMTREF_STRING_REGEX + "\\s*"
+ "," + "\\s*" + STMTREF_STRING_REGEX + "\\s*" + "\\)" + SYMBOL_RIGHT_BRACKET_STRING;
const string TEMP_FOLLOWST_STRING_REGEX = FOLLOWS_STRING + "\\*" + SYMBOL_LEFT_BRACKET_STRING + "\\(" + "\\s*" + STMTREF_STRING_REGEX + "\\s*"
+ "," + "\\s*" + STMTREF_STRING_REGEX + "\\s*" + "\\)" + SYMBOL_RIGHT_BRACKET_STRING;

const string TEMP_NEXT_STRING_REGEX = NEXT_STRING + SYMBOL_LEFT_BRACKET_STRING + "\\(" + "\\s*" + LINEREF_STRING_REGEX + "\\s*"
+ "," + "\\s*" + STMTREF_STRING_REGEX + "\\s*" + "\\)" + SYMBOL_RIGHT_BRACKET_STRING;
const string TEMP_NEXTT_STRING_REGEX = NEXT_STRING + "\\*" + SYMBOL_LEFT_BRACKET_STRING + "\\(" + "\\s*" + LINEREF_STRING_REGEX + "\\s*"
+ "," + "\\s*" + STMTREF_STRING_REGEX + "\\s*" + "\\)" + SYMBOL_RIGHT_BRACKET_STRING;

const string TEMP_RELREF_STRING_REGEX = SYMBOL_LEFT_BRACKET_STRING + SYMBOL_LEFT_BRACKET_STRING + TEMP_MODIFIESS_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING
+ OR + SYMBOL_LEFT_BRACKET_STRING + TEMP_USESS_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING
+ OR + SYMBOL_LEFT_BRACKET_STRING + TEMP_PARENTT_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING
+ OR + SYMBOL_LEFT_BRACKET_STRING + TEMP_PARENT_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING
+ OR + SYMBOL_LEFT_BRACKET_STRING + TEMP_FOLLOWS_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING
+ OR + SYMBOL_LEFT_BRACKET_STRING + TEMP_FOLLOWST_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING
+ OR + SYMBOL_LEFT_BRACKET_STRING + TEMP_NEXT_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING
+ OR + SYMBOL_LEFT_BRACKET_STRING + TEMP_NEXTT_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING
+ OR + SYMBOL_LEFT_BRACKET_STRING + TEMP_CALLS_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING
+ OR + SYMBOL_LEFT_BRACKET_STRING + TEMP_CALLST_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING
+ OR + SYMBOL_LEFT_BRACKET_STRING + TEMP_MODIFIESP_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING
+ OR + SYMBOL_LEFT_BRACKET_STRING + TEMP_USESP_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING
+ SYMBOL_RIGHT_BRACKET_STRING;

const string TEMP_RELCOND_STRING_REGEX = SYMBOL_LEFT_BRACKET_STRING + TEMP_RELREF_STRING_REGEX + SYMBOL_LEFT_BRACKET_STRING + "\\s*" + AND_STRING + "*" + "\\s+" + TEMP_RELREF_STRING_REGEX + "\\s*" + SYMBOL_RIGHT_BRACKET_STRING + ASTERIK + SYMBOL_RIGHT_BRACKET_STRING;

const string TEMP_ITR2_SUCH_THAT_CL_REGEX = SUCH_THAT_STRING + "\\s+" + TEMP_RELCOND_STRING_REGEX;
const string TEMPORARY = SYMBOL_LEFT_BRACKET_STRING + TEMP_ITR2_SUCH_THAT_CL_REGEX + SYMBOL_RIGHT_BRACKET_STRING;

const string TEMP_ITR2_SUCH_THAT_CL_EXTENDED_REGEX = TEMPORARY + SYMBOL_LEFT_BRACKET_STRING + "\\s*" + TEMPORARY + "\\s*" + SYMBOL_RIGHT_BRACKET_STRING + ASTERIK;

const string PATTERN_CL_REGEX = PATTERN_STRING + "\\s+" + 
"([a-zA-Z])([a-zA-Z]|\\d|\#)*[ ]{0,1}\\(\\s*((([a-zA-Z])([a-zA-Z]|\\d|\\#)*)|(\\_)|(\"([a-zA-Z])([a-zA-Z]|\\d|\\#)*\"))\\s*,\\s*(\\_\"([a-zA-Z])(\\w)*((\\+|\\*|\\-)\\w+)*\"\\_|\\_|\"([a-zA-Z])(\\w)*((\\+|\\*|\\-)\\w+)*\")\\s*\\)";

const string BRACKETED_SYNONYM = SYMBOL_LEFT_BRACKET_STRING + SYNONYM_STRING_REGEX + SYMBOL_RIGHT_BRACKET_STRING;

const string TEMP_ITR2_GENERAL_PATTERN_CL_REGEX = SYMBOL_LEFT_BRACKET_STRING + PATTERN_STRING + "\\s+" + SYNONYM_STRING_REGEX + "\\s*" + "\\(" + ".+?" + "\\)" + SYMBOL_RIGHT_BRACKET_STRING;

const string TEMP_ITR2_GENERAL_PATTERN_CL_EXTENDED_REGEX = SYMBOL_LEFT_BRACKET_STRING + TEMP_ITR2_GENERAL_PATTERN_CL_REGEX + SYMBOL_RIGHT_BRACKET_STRING
+ SYMBOL_LEFT_BRACKET_STRING + SYMBOL_LEFT_BRACKET_STRING + "\\s*" + AND_STRING +  "\\s+" + SYMBOL_RIGHT_BRACKET_STRING + "?" + "\\s*" + TEMP_ITR2_GENERAL_PATTERN_CL_REGEX + SYMBOL_RIGHT_BRACKET_STRING + ASTERIK;


const string IF_PATTERN_REGEX = "\\s*" + BRACKETED_SYNONYM + "\\s*" + "\\(" + "\\s*" + ENTREF_STRING_REGEX + "\\s*" + "," + "\\s*" + "_" + "\\s*" + "," + "\\s*" + "_" + "\\s*" + "\\)";
const string WHILE_PATTERN_REGEX = "\\s*" + BRACKETED_SYNONYM + "\\s*" + "\\(" + "\\s*" + ENTREF_STRING_REGEX + "\\s*" + "," + "\\s*" + "_" + "\\s*" + "\\)";
const string ASSIGN_PATTERN_REGEX = "\\s*" + BRACKETED_SYNONYM + "\\s*" + "\\(" + "\\s*" + ENTREF_STRING_REGEX + "\\s*" + "," + "\\s*" + "([^\\.\\,\\@\\$\\%\\&\\^\\'\\=\\~\\`\/\]+)" + "\\s*" + "\\)";

const string SUB_MATCH_REGEX = "([\\w][\\w\\d\#]*\\s+[\\w][\\w\\d\#]*)";
const string IF_PATTERN_AND_REGEX = SYMBOL_LEFT_BRACKET_STRING + IF_PATTERN_REGEX + SYMBOL_RIGHT_BRACKET_STRING + SYMBOL_LEFT_BRACKET_STRING + "\\s*" + SYMBOL_LEFT_BRACKET_STRING + AND_STRING + "\\s+" + SYMBOL_RIGHT_BRACKET_STRING + "{0,1}"
+ SYMBOL_RIGHT_BRACKET_STRING + IF_PATTERN_REGEX + SYMBOL_RIGHT_BRACKET_STRING + ASTERIK;

const string PARTIAL_PATTERN_REGEX = "(and\\s+pattern)(.+)";

const string SELECT_INITIAL_REGEX = SYMBOL_LEFT_BRACKET_STRING + SELECT_STRING + "\\s+" + "([a-zA-Z])([a-zA-Z]|\\d|\\#)*||(BOOLEAN)" + SYMBOL_RIGHT_BRACKET_STRING;


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
			toCheck = trim(toCheck);
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
	}
	else {
		return false;

	}
}

bool QueryValidator::isValidSynDesignEntity(string synPattern) {
	synPattern = removeSymbols(synPattern, WHITESPACE_STRING);
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


//Posssible str received here are:
//pattern a(,) pattern(,)
//pattern ifs(,,) and pattern a()
//pattern w(_,_) and pattern a() pattern ifs(,,)
bool QueryValidator::isValidPatternIter2(string str) {
	//First trim it
	str = trim(str);

	//Extract out all the pattern
	vector<string> vecPattern = extractPattern(str);

	for (size_t i = 0; i < vecPattern.size(); i++) {

		if (!isValidLeadingCheck(str)) {
			return false;
		}
		else {
			//Do trimming on this string
			str = Util::trim(str);
			//if string consists of AND
			if (str.find(AND_STRING) != string::npos) {
				//Remove Leading 'And' if it exists
				if (isLeadingAnd(str)) {
					str = removeLeadingAnd(str);
				}
				//Checks generally up to before first occurence of left bracket
				if (!isValidGeneralPatternRegex(str)) {
					return false;
				}
				//What i should do: remove leading pattern
				//With given string, first remove the word pattern
				str = str.substr(PATTERN_STRING_LENGTH, str.length() - PATTERN_STRING_LENGTH);
				//Trim it again
				str = Util::trim(str);

				//To be used as identification of the pattern regex
				bool isAssign = false;
				bool isIf = false;
				bool isWhile = false;

				int idxFirstLeftBracket = str.find(SYMBOL_LEFT_BRACKET_STRING);
				string synPattern = str.substr(ZERO, idxFirstLeftBracket);
				string entPattern = getCorrespondingEntity(str);

				if (entPattern != ASSIGN_STRING && entPattern != IF_STRING && entPattern != WHILE_STRING) {
					return false;
				}
				else if (entPattern == ASSIGN_STRING) {
					return isValidAddAssignPattern(str, synPattern);
				}
				else if (entPattern == IF_STRING) {
					return isValidAddIfPattern(str, synPattern);
				}
				else if (entPattern == WHILE_STRING) {
					return isValidAddWhilePattern(str, synPattern);
				}
				else {
					return false;
				}

			}
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
	bool arg1Number = false;
	bool arg1Variable = false;
	bool arg1StringLiteral = false;
	bool arg2UnderScore = false;
	bool arg2Exact = false;
	bool arg2Substring = false;

	if (isWildcard(arg1)) {
		arg1UnderScore = true;
	}
	else if (is_number(arg1)) {
		arg1Number = true;
	}
	else if (isQuotationIdentRegex(arg1)) {
		arg1StringLiteral = true;
	}
	else if (isVariableSynonym(arg1)) {
		arg1Variable = true;
	}
	else {
		return false;
	}
	if (isWildcard(arg2)) {
		arg2UnderScore = true;
	}
	else if (isValidExpr(str)) {
		arg2Exact = true;
	}
	else if (isValidExprUnder(str)) {
		arg2Substring = true;
	}
	else {
		return false;
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


}

//This method checks if a given string a well-formed expression with underscores at both ends
bool QueryValidator::isValidExprUnder(string str) {

}
//This function handles the while pattern scenario
bool QueryValidator::isValidAddWhilePattern(string str, string synPattern) {

	//Returns false if it doesnt matches the while regex
	if (!isValidWhilePatternRegex(str)) {
		return false;
	}
	else {
		int idxFirstLeftBracket = str.find(SYMBOL_LEFT_BRACKET_STRING);

		string curr = str.substr(idxFirstLeftBracket, str.length() - idxFirstLeftBracket);

		curr = removeSymbols(curr, SYMBOL_LEFT_BRACKET_STRING);
		curr = removeSymbols(curr, SYMBOL_RIGHT_BRACKET_STRING);

		vector<string> whilePatternVec = splitBySymbol(curr, SYMBOL_COMMA);
		string arg1 = whilePatternVec.at(ZERO);
		//trim arg1
		arg1 = Util::trim(arg1);

		bool arg1UnderScore = false;
		bool arg1Number = false;
		bool arg1Variable = false;
		bool arg1StringLiteral = false;

		if (isWildcard(arg1)) {
			arg1UnderScore = true;
			addWhilePatternQueryElement(arg1, arg1UnderScore, arg1Number, arg1Variable, arg1StringLiteral, synPattern);
			return true;
		}
		else if (is_number(arg1)) {
			arg1Number = true;
			addWhilePatternQueryElement(arg1, arg1UnderScore, arg1Number, arg1Variable, arg1StringLiteral, synPattern);
			return true;
		}
		else if (isQuotationIdentRegex(arg1)) {
			arg1StringLiteral = true;
			arg1 = removeSymbols(arg1, INVERTED_COMMA_STRING);
			arg1 = Util::trim(arg1);
			addWhilePatternQueryElement(arg1, arg1UnderScore, arg1Number, arg1Variable, arg1StringLiteral, synPattern);
			return true;
		}
		else if (isVariableSynonym(arg1)) {
			arg1Variable = true;
			addWhilePatternQueryElement(arg1, arg1UnderScore, arg1Number, arg1Variable, arg1StringLiteral, synPattern);
			return true;
		}
		else {
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
	}
	else {
		//Since regex passed, try and get wad is contained inside the string now
		//Find the occurence of the first left bracket
		int idxFirstLeftBracket = str.find(SYMBOL_LEFT_BRACKET_STRING);
		//Since it passed the regex, i am sure that arg2 and arg 3 will be wildcard i.e. underscore string
		//So all i need to do is find out what is arg1 
		string curr = str.substr(idxFirstLeftBracket, str.length() - idxFirstLeftBracket);
		//Remove the brackets
		curr = removeSymbols(curr, SYMBOL_LEFT_BRACKET_STRING);
		curr = removeSymbols(curr, SYMBOL_RIGHT_BRACKET_STRING);
		
		vector<string> ifPatternVec = splitBySymbol(curr, SYMBOL_COMMA);
		string arg1 = ifPatternVec.at(ZERO);
		//trim arg1
		arg1 = Util::trim(arg1);

		//Possible identities of arg1 entRef: stringliterals, synonym variable, number, underscore_String
		bool arg1UnderScore = false;
		bool arg1Number = false;
		bool arg1Variable = false;
		bool arg1StringLiteral = false;

		if (isWildcard(arg1)) {
			arg1UnderScore = true;
			addIfPatternQueryElement(arg1, arg1UnderScore, arg1Number, arg1Variable, arg1StringLiteral, synPattern);
			return true;
		}
		else if (is_number(arg1)) {
			arg1Number = true;
			addIfPatternQueryElement(arg1, arg1UnderScore, arg1Number, arg1Variable, arg1StringLiteral, synPattern);
			return true;
		}
		else if (isQuotationIdentRegex(arg1)) {
			arg1StringLiteral = true;
			arg1 = removeSymbols(arg1, INVERTED_COMMA_STRING);
			arg1 = Util::trim(arg1);
			addIfPatternQueryElement(arg1, arg1UnderScore, arg1Number, arg1Variable, arg1StringLiteral, synPattern);
			return true;
		}
		else if (isVariableSynonym(arg1)) {
			arg1Variable = true;
			addIfPatternQueryElement(arg1, arg1UnderScore, arg1Number, arg1Variable, arg1StringLiteral, synPattern);
			return true;
		}
		else {
			return false;
		}
	}
}
//This functions takes in arg1 and 4 boolean and 1 string and creates a	QueryElement to be added to the QueryStatement
void QueryValidator::addIfPatternQueryElement(string arg1, bool arg1Underscore, bool arg1Number, bool arg1Variable, bool arg1StringLiteral, string synPattern) {
	//If arg1 is a boolean, just add
	if (arg1Underscore) {
		QueryElement ifQueryElement = QueryElement(UNDER_SCORE_STRING, UNDER_SCORE_STRING, UNDER_SCORE_STRING, IF_STRING, synPattern, WILDCARD_STRING, WILDCARD_STRING, WILDCARD_STRING, EMPTY_STRING);
		queryStatement.addPatternQuery(ifQueryElement);
	}
	else if (arg1Number) {
		QueryElement ifQueryElement = QueryElement(arg1, UNDER_SCORE_STRING, UNDER_SCORE_STRING, IF_STRING, synPattern, NUMBER_STRING, WILDCARD_STRING, WILDCARD_STRING, EMPTY_STRING);
		queryStatement.addPatternQuery(ifQueryElement);
	}
	else if (arg1StringLiteral) {
		QueryElement ifQueryElement = QueryElement(arg1, UNDER_SCORE_STRING, UNDER_SCORE_STRING, IF_STRING, synPattern, VARIABLE_STRING, WILDCARD_STRING, WILDCARD_STRING, EMPTY_STRING);
		queryStatement.addPatternQuery(ifQueryElement);
	}
	else if (arg1Variable) {
		QueryElement ifQueryElement = QueryElement(arg1, UNDER_SCORE_STRING, UNDER_SCORE_STRING, IF_STRING, synPattern, SYNONYM_STRING, WILDCARD_STRING, WILDCARD_STRING, VARIABLE_STRING);
		queryStatement.addPatternQuery(ifQueryElement);
	}
}

void QueryValidator::addWhilePatternQueryElement(string arg1, bool arg1Underscore, bool arg1Number, bool arg1Variable, bool arg1StringLiteral, string synPattern) {
	if (arg1Underscore) {
		QueryElement whileQueryElement = QueryElement(UNDER_SCORE_STRING, UNDER_SCORE_STRING, EMPTY_STRING, IF_STRING, synPattern, WILDCARD_STRING, WILDCARD_STRING, WILDCARD_STRING, EMPTY_STRING);
		queryStatement.addPatternQuery(whileQueryElement);
	}
	else if (arg1Number) {
		QueryElement whileQueryElement = QueryElement(arg1, UNDER_SCORE_STRING, EMPTY_STRING, IF_STRING, synPattern, NUMBER_STRING, WILDCARD_STRING, WILDCARD_STRING, EMPTY_STRING);
		queryStatement.addPatternQuery(whileQueryElement);
	}
	else if (arg1StringLiteral) {
		QueryElement whileQueryElement = QueryElement(arg1, UNDER_SCORE_STRING, EMPTY_STRING, IF_STRING, synPattern, VARIABLE_STRING, WILDCARD_STRING, WILDCARD_STRING, EMPTY_STRING);
		queryStatement.addPatternQuery(whileQueryElement);
	}
	else if (arg1Variable) {
		QueryElement whileQueryElement = QueryElement(arg1, UNDER_SCORE_STRING, EMPTY_STRING, IF_STRING, synPattern, SYNONYM_STRING, WILDCARD_STRING, WILDCARD_STRING, VARIABLE_STRING);
		queryStatement.addPatternQuery(whileQueryElement);
	}
}
bool QueryValidator::isLeadingAnd(string str) {
	if (str.find(AND_STRING) == 0) {
		if (isPartialPatternRegex(str)) {
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
//This function removes the leading pattern string, then trim it and returns this trimmed string
string QueryValidator::removeLeadingPatternString(string str) {

	str = str.substr(PATTERN_STRING.length(), str.length()-PATTERN_STRING.length());

	return Util::trim(str);
}
string QueryValidator::removeLeadingAnd(string str) {
	
	int idxPatternIntitial = str.find(PATTERN_STRING);

	string curr = str.substr(idxPatternIntitial, str.length()- idxPatternIntitial);
	return curr;
}
//This functions checks the leading area of the str i.e. it can onli be whitespace or the word 'and' of one occurence
bool QueryValidator::isValidLeadingCheck(string str) {
	//Try to find the index of the first occurence of pattern, take note of it
	int patternFirst = str.find(PATTERN_STRING);
	//Next get the substring till before occurence of pattern
	string before = str.substr(ZERO, patternFirst);
	if (patternFirst == ZERO) {
		return true;
	}
	else {
		if (!isValidLeadTrail(str)) {
			return false;
		}
		else {
			return true;
		}
	}

}

bool QueryValidator::isValidPattern(string str, string syn) {
	//Idea: From given string, ignore Pattern, obtain (arg1, arg2)
	//Example of current string : pattern a(v,"procs*ifs")	or pattern a(_,_"procs*while"_)
	str = trim(str);
	//str is now a(...,...)
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
		if (arg2Substring || arg2Exact) {
			arg2 = Util::insertBrackets(arg2);
		}

		addPatternQueryElement(arg1, arg2, ent, synDesignEnt, arg1Variable, arg1Wildcard, arg1Synonym, arg2Substring, arg2Exact, arg2Wildcard);
		return true;
	}
}

//This functions checks if the given str is of the design entity
bool QueryValidator::isVariableSynonym(string str) {
	string ent = getCorrespondingEntity(str);
	return(ent == VARIABLE_STRING);
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
	if ((arg2.at(0) == DOUBLE_QUOTATION) && (arg2.at(arg2.length() - 1) == DOUBLE_QUOTATION)) {
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
		addSelectQueryElement(ent, syn, SYNONYM_STRING);
		return true;
	}
	else if (syn == BOOLEAN_STRING) {
		addSelectQueryElement(EMPTY_STRING, EMPTY_STRING, BOOLEAN_STRING);
		return true;
	}
	else {
		cout << INVALID_SYNONYM_QUERIED_ERROR;
		return false;
	}
}
void QueryValidator::addSelectQueryElement(string ent, string syn, string selectType) {
	queryStatement.addSelectQuery(QueryElement(ent, syn, selectType));
}


//The following function
void QueryValidator::addPatternQueryElement(string arg1, string arg2, string ent, string syn, bool arg1Variable, bool arg1Wildcard, bool arg1Synonym, bool arg2Substring, bool arg2FullString, bool arg2Wilcard) {

	//Implies that arg1 is wildcard i.e. _
	if (arg1Wildcard) {
		if (arg2Substring) {
			queryStatement.addPatternQuery(QueryElement(arg1, arg2, EMPTY_STRING, ent, syn, WILDCARD_STRING, SUBSTRING_STRING, EMPTY_STRING, EMPTY_STRING));
		}
		else if (!arg2Substring && arg2FullString) {
			queryStatement.addPatternQuery(QueryElement(arg1, arg2, EMPTY_STRING, ent, syn, WILDCARD_STRING, EXACT_STRING, EMPTY_STRING, EMPTY_STRING));
		}
		else if (!arg2Substring && !arg2FullString && arg2Wilcard) {
			queryStatement.addPatternQuery(QueryElement(arg1, arg2, EMPTY_STRING, ent, syn, WILDCARD_STRING, WILDCARD_STRING, EMPTY_STRING, EMPTY_STRING));
		}
		else {
			//cout << "Error occured";
			exit(0);
		}
	}
	else if (arg1Variable) {
		if (arg2Substring) {
			queryStatement.addPatternQuery(QueryElement(arg1, arg2, EMPTY_STRING ,ent, syn, VARIABLE_STRING, SUBSTRING_STRING, EMPTY_STRING, EMPTY_STRING));
		}
		else if (!arg2Substring && arg2FullString) {
			queryStatement.addPatternQuery(QueryElement(arg1, arg2, EMPTY_STRING, ent, syn, VARIABLE_STRING, EXACT_STRING, EMPTY_STRING, EMPTY_STRING));
		}
		else if (!arg2Substring && !arg2FullString && arg2Wilcard) {
			queryStatement.addPatternQuery(QueryElement(arg1, arg2, EMPTY_STRING, ent, syn, VARIABLE_STRING, WILDCARD_STRING, EMPTY_STRING, EMPTY_STRING));
		}
		else {
			//cout << "Error occured";
			exit(0);
		}
	}
	else if (arg1Synonym) {
		if (arg2Substring) {
			queryStatement.addPatternQuery(QueryElement(arg1, arg2, EMPTY_STRING, ent, syn, SYNONYM_STRING, SUBSTRING_STRING, EMPTY_STRING, EMPTY_STRING));
		}
		else if (!arg2Substring && arg2FullString) {
			queryStatement.addPatternQuery(QueryElement(arg1, arg2, EMPTY_STRING, ent, syn, SYNONYM_STRING, EXACT_STRING, EMPTY_STRING, EMPTY_STRING));
		}
		else if (!arg2Substring && !arg2FullString && arg2Wilcard) {
			queryStatement.addPatternQuery(QueryElement(arg1, arg2, EMPTY_STRING, ent, syn, SYNONYM_STRING, WILDCARD_STRING, EMPTY_STRING, EMPTY_STRING));
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
			if (vec.at(i).find(SELECT_STRING) != std::string::npos) {
				return false;
			}
			else if (vec.at(i).find(SUCH_THAT_STRING) != std::string::npos) {
				if (!isValidSuchThat(vec.at(i))) {
					return false;
				}
			}
			else if (vec.at(i).find(PATTERN_STRING) != std::string::npos) {
				if (!isValidPattern(vec.at(i), syn)) {
					return false;
				}
			}
			else if (vec.at(i).find(WITH_STRING) != std::string::npos) {
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
			if (changeLowerCase(tempVec.at(j)) == changeLowerCase(syn)) {
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
	int currIdx = 0;

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
	}
	else if ((posPattern != std::string::npos) && (posPattern < posSuchThat) && (posPattern < posWith)) {
		posInterest = posPattern;
		clauseInterest = PATTERN_STRING;
	}
	else if ((posWith != std::string::npos) && (posWith < posSuchThat) && (posWith < posSuchThat)) {
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

	int count = 0;
	while (currIdx != -1) {
		string toPush = curr.substr(0, nextPosInterest);
		toPush = trim(toPush);
		results.push_back(toPush);
		//Get the next instance of the string
		curr = curr.substr(nextPosInterest, curr.length());

		size_t nextPosSuchThat = curr.find(SUCH_THAT_STRING);

		size_t nextPosPattern = curr.find(PATTERN_STRING);

		size_t nextPosWith = curr.find(WITH_STRING);

		if ((nextPosSuchThat != std::string::npos) && (nextPosSuchThat != 0)
			&& (((nextPosPattern == 0) && (nextPosSuchThat < nextPosWith)) || ((nextPosWith == 0) && (nextPosSuchThat < nextPosPattern)))) {
			nextPosInterest = nextPosSuchThat;
			nextClauseInterest = SUCH_THAT_STRING;
		}
		else if ((nextPosPattern != std::string::npos) && (nextPosPattern != 0)
			&& (((nextPosSuchThat == 0) && (nextPosPattern < nextPosWith)) || ((nextPosWith == 0) && (nextPosPattern > nextPosWith)))) {
			nextPosInterest = nextPosPattern;
			nextClauseInterest = PATTERN_STRING;

		}
		else if ((nextPosWith != std::string::npos) && (nextPosWith != 0)
			&& (((nextPosSuchThat == 0) && (nextPosPattern > nextPosWith)) || ((nextPosPattern == 0) && (nextPosSuchThat > nextPosWith)))) {
			nextPosInterest = nextPosWith;
			nextClauseInterest = WITH_STRING;

		}
		else if (((nextPosSuchThat == 0) && (nextPosWith == std::string::npos) && (nextPosPattern == std::string::npos))
			|| ((nextPosPattern == 0) && (nextPosSuchThat == std::string::npos) && (nextPosWith == std::string::npos))
			|| ((nextPosWith == 0) && (nextPosSuchThat == std::string::npos) && (nextPosPattern == std::string::npos))) {
			toPush = curr.substr(0, curr.length());
			toPush = trim(toPush);
			results.push_back(toPush);
			break;
		}
		else {
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
		temp = m[0];
		toReturnVec.push_back(temp);
		str = m.suffix().str();
	}
	return toReturnVec;
}
//This Method extracts out individual pattern from given string and places them into a vector of string
vector<string> QueryValidator::extractPattern(string str) {
	size_t pos=0;
	string curr = str;
	vector<string> vecPattern;
	//While pos is not end of string
	while (pos != string::npos) {
		//Try to find the next occurence of pattern
		size_t found = curr.find(PATTERN_STRING, pos+1);
		
		//Means end of string, so just push in and break
		if (found == string::npos) {
			vecPattern.push_back(curr);
			break;
		}
		//Means a match has been found
		else {

			//curr is now pattern(....)...
			string currTwo = curr.substr(ZERO, found);
			size_t lastBracket = currTwo.find_last_of(SYMBOL_RIGHT_BRACKET_STRING);
			currTwo = currTwo.substr(ZERO, lastBracket+ONE);
			vecPattern.push_back(currTwo);
			curr = curr.substr(lastBracket+ONE, curr.length() - lastBracket);
			pos = lastBracket+ONE;
		}
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
string QueryValidator::changeLowerCase(string str) {
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
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
	regex suchThatRegex(TEMP_ITR2_SUCH_THAT_CL_REGEX);
	return regex_match(str, suchThatRegex);
}
bool QueryValidator::isValidSuchThaExtendedRegex(string str) {
	regex suchThatExtendedRegex(TEMP_ITR2_SUCH_THAT_CL_EXTENDED_REGEX);
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
	regex leadTrailRegex(LEAD_TRAIL);
	return regex_match(str, leadTrailRegex);
}
bool QueryValidator::isValidAttRefRegex(string str) {
	regex attrRefRegex(ATTRREF_STRING_REGEX);
	return regex_match(str, attrRefRegex);
}
bool QueryValidator::isValidIfMultiplePatternRegex(string str) {
	regex ifPatternAndRegex(IF_PATTERN_AND_REGEX);
	return regex_match(str, ifPatternAndRegex);
}
bool QueryValidator::isValidPatternRegex(string str) {
	regex patternRegex(PATTERN_CL_REGEX);
	return regex_match(str, patternRegex);
}
bool QueryValidator::isValidSelectInitialRegex(string str) {
	regex patternRegex(SELECT_INITIAL_REGEX);
	return regex_match(str, patternRegex);
}
bool QueryValidator::isValidGeneralPatternRegex(string str) {
	regex generalPatternRegex(TEMP_ITR2_GENERAL_PATTERN_CL_REGEX);
	return regex_match(str, generalPatternRegex);
}
bool QueryValidator::isValidWhilePatternRegex(string str) {
	regex whilePatternRegex(WHILE_PATTERN_REGEX);
	return regex_match(str, whilePatternRegex);
}
bool QueryValidator::isValidPatternExtendedRegex(string str) {
	regex patternExtendedRegex(TEMP_ITR2_GENERAL_PATTERN_CL_EXTENDED_REGEX);
	return regex_match(str, patternExtendedRegex);
}
bool QueryValidator::isValidIfPatternRegex(string str) {
	regex ifPatternRegex(IF_PATTERN_REGEX);
	return regex_match(str, ifPatternRegex);
}
bool QueryValidator::isVariable(string str) {
	regex patternRegex(VARIABLE_STRING_REGEX);
	return regex_match(str, patternRegex);
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
string QueryValidator::trimPatternArgs(string str) {
	int idxLeft = str.find(SYMBOL_LEFT_BRACKET_STRING);
	str = str.substr(idxLeft + ONE, str.length() - idxLeft - TWO);
	str = removeSymbols(str, WHITESPACE_STRING);
	return str;
}
bool::QueryValidator::isSubMatchArg2Pattern(string str) {
	std::cmatch m;
	regex subMatchRegex(SUB_MATCH_REGEX);
	regex_match(str, subMatchRegex);
}