#include "suchThatHandler.h"

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

suchThatHandler::suchThatHandler()
{
}

bool QueryValidator::isValidSuchThat(string str) {
	int toAdd = ZERO;

	//Remove the additional leading and trailing whitespace
	str = Util::trim(str);

	//If the str is a valid str regex just proceed
	if (isValidSuchThatRegex(str) || isValidSuchThaExtendedRegex(str)) {
		vector<string> suchThatRelVec = extractSuchThatClauses(str);
		for (int idx = 0; idx < suchThatRelVec.size(); idx++) {
			string currentString = suchThatRelVec.at(idx);
			vector<string> tempVec = splitBySymbol(currentString, SYMBOL_LEFT_BRACKET);
			string relation = tempVec.at(ZERO);
			string argsWithoutBracket = tempVec.at(ONE).substr(ZERO, tempVec.at(ONE).length() - 1);
			argsWithoutBracket = removeSymbols(argsWithoutBracket, WHITESPACE_STRING);
			vector<string> args = splitBySymbol(argsWithoutBracket, SYMBOL_COMMA);
			string arg1 = args.at(ZERO);
			string arg2 = args.at(ONE);

			string arg1Ent = getCorrespondingEntity(arg1);	//Attempts to get corresponding entity for each entity
			string arg2Ent = getCorrespondingEntity(arg2);	//E.g. Follows(s,4).	type1= stmt, type2 = number

			bool arg1Valid = false;
			bool arg2Valid = false;
			bool arg1_NUM = false;
			bool arg1_UNDER = false;
			bool arg1_STRING_LITERAL = false;

			bool arg2_UNDER = false;
			bool arg2_NUM = false;
			bool arg2_STRING_LITERAL = false;

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
				else if (isQuotationIdentRegex(arg1)) {
					arg1Valid = checkRelationshipTable(relation, STR_STRING, ONE);
					if (arg1Valid) {
						arg1_STRING_LITERAL = true;
					}
					else {
						arg1_STRING_LITERAL = false;
					}
				}
				else {
					arg1Valid = false;
				}
			}
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
				else if (isQuotationIdentRegex(arg2)) {
					arg2Valid = checkRelationshipTable(relation, STR_STRING, TWO);
					if (arg2Valid) {
						arg2_STRING_LITERAL = true;
					}
					else {
						arg2_STRING_LITERAL = false;
					}
				}
				else {
					arg2Valid = false;
				}
			}
			//If both are valid and true, create the clause
			if (arg1Valid && arg2Valid) {
				if (!addSuchThatQueryElement(arg1_NUM, arg1_UNDER, arg1_STRING_LITERAL, arg2_NUM, arg2_UNDER, arg2_STRING_LITERAL, relation, arg1, arg2, arg1Ent, arg2Ent)) {
					return false;
				}
			}
			else {
				return false;
			}
		}
	}
	else {
		return false;
	}
	return true;
}
bool QueryValidator::addSuchThatQueryElement(bool arg1_NUM, bool arg1_UNDER, bool arg1_STRING_LITERAL, bool arg2_NUM, bool arg2_UNDER, bool arg2_STRING_LITERAL, string relType, string arg1, string arg2, string arg1Ent, string arg2Ent) {

	//QueryElement parameters:
	//Arg1, Arg1Type, Arg1Ent, Arg2, Arg2Type, arg2Ent, relType
	//Implies that arg1 is a synonym
	if (arg1_NUM == false && arg1_UNDER == false && arg1_STRING_LITERAL == false) {
		//Implies that arg 1 is snyonym and arg2 is also a synonym
		if (arg2_NUM == false && arg2_UNDER == false && arg2_STRING_LITERAL == false) {
			QueryElement queryElement = QueryElement(arg1, SYNONYM_STRING, arg1Ent, arg2, SYNONYM_STRING, arg2Ent, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		//Implies that arg1 is synonym and arg2 is a wildcard
		else if (arg2_NUM == false && arg2_UNDER == true && arg2_STRING_LITERAL == false) {
			//Implies that the clause for arg1 is not a num/under, arg2 is not a num, arg2 is an UNDER
			QueryElement queryElement = QueryElement(arg1, SYNONYM_STRING, arg1Ent, UNDER_SCORE_STRING, WILDCARD_STRING, EMPTY_STRING, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		//Implies that arg1 is synonym, arg 2 is an integer i.e. number
		else if (arg2_NUM == true && arg2_UNDER == false && arg2_STRING_LITERAL == false) {
			QueryElement queryElement = QueryElement(arg1, SYNONYM_STRING, arg1Ent, arg2, NUMBER_STRING, EMPTY_STRING, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		//Implies that arg1 is synonym, arg2 is a stringLiteral
		else if (arg2_NUM == false && arg2_UNDER == false && arg2_STRING_LITERAL == true) {
			arg2 = removeSymbols(arg2, INVERTED_COMMA_STRING);
			arg2 = Util::trim(arg2);
			QueryElement queryElement = QueryElement(arg1, SYNONYM_STRING, arg1Ent, arg2, VARIABLE_STRING, EMPTY_STRING, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		else {
			return false;
		}
	}
	//Implies that arg1 is a num
	else if (arg1_NUM == true && arg1_UNDER == false && arg1_STRING_LITERAL == false) {
		//Implies that arg1 is a number and arg2 is a synonym
		if (arg2_NUM == false && arg2_UNDER == false && arg2_STRING_LITERAL == false) {
			QueryElement queryElement = QueryElement(arg1, NUMBER_STRING, EMPTY_STRING, arg2, SYNONYM_STRING, arg2Ent, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		//Implies that arg1 is a number and arg2 is a wildcard i.e underscore
		else if (arg2_NUM == false && arg2_UNDER == true && arg2_STRING_LITERAL == false) {
			QueryElement queryElement = QueryElement(arg1, NUMBER_STRING, EMPTY_STRING, UNDER_SCORE_STRING, WILDCARD_STRING, EMPTY_STRING, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		//Implies that arg1 is a number and arg2 is a number
		else if (arg2_NUM == true && arg2_UNDER == false && arg2_STRING_LITERAL == false) {
			QueryElement queryElement = QueryElement(arg1, NUMBER_STRING, EMPTY_STRING, arg2, NUMBER_STRING, EMPTY_STRING, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		//Implies taht arg1 is a number and arg2 is a variable type i.e. "IDENT"
		else if (arg2_NUM == false && arg2_UNDER == false && arg2_STRING_LITERAL == true) {
			arg2 = removeSymbols(arg2, INVERTED_COMMA_STRING);
			arg2 = Util::trim(arg2);
			QueryElement queryElement = QueryElement(arg1, NUMBER_STRING, EMPTY_STRING, arg2, VARIABLE_STRING, EMPTY_STRING, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		else {
			return false;
		}
	}
	//Implies that arg1 is a wildcard i.e. underscore
	else if (arg1_NUM == false && arg1_UNDER == true && arg1_STRING_LITERAL == false) {
		//Implies taht arg1 is a wildcard and arg 2 is a synonym
		if (arg2_NUM == false && arg2_UNDER == false && arg2_STRING_LITERAL == false) {
			QueryElement queryElement = QueryElement(UNDER_SCORE_STRING, WILDCARD_STRING, EMPTY_STRING, arg2, SYNONYM_STRING, arg2Ent, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		//Implies that arg1 is a wild card and arg 2 is a wildcard
		else if (arg2_NUM == false && arg2_UNDER == true && arg2_STRING_LITERAL == false) {
			QueryElement queryElement = QueryElement(UNDER_SCORE_STRING, WILDCARD_STRING, EMPTY_STRING, UNDER_SCORE_STRING, WILDCARD_STRING, EMPTY_STRING, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		//Implies that arg1 is a wildcard and arg2 is a number i.e. integer
		else if (arg2_NUM == true && arg2_UNDER == false && arg2_STRING_LITERAL == false) {
			QueryElement queryElement = QueryElement(UNDER_SCORE_STRING, WILDCARD_STRING, EMPTY_STRING, arg2, NUMBER_STRING, EMPTY_STRING, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		//Implies that arg1 is a wildcard and arg2 is a variable i.e. "IDENT"
		else if (arg2_NUM == false && arg2_UNDER == false && arg2_STRING_LITERAL == true) {
			arg2 = removeSymbols(arg2, INVERTED_COMMA_STRING);
			arg2 = Util::trim(arg2);
			QueryElement queryElement = QueryElement(UNDER_SCORE_STRING, WILDCARD_STRING, EMPTY_STRING, arg2, VARIABLE_STRING, EMPTY_STRING, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		else {
			return false;
		}
	}
	else if (arg1_NUM == false && arg1_UNDER == false && arg1_STRING_LITERAL == true) {
		//Implies taht arg1 is a stringLiteral and arg 2 is a synonym
		if (arg2_NUM == false && arg2_UNDER == false && arg2_STRING_LITERAL == false) {
			arg1 = removeSymbols(arg1, INVERTED_COMMA_STRING);
			arg1 = Util::trim(arg1);
			QueryElement queryElement = QueryElement(arg1, VARIABLE_STRING, EMPTY_STRING, arg2, SYNONYM_STRING, arg2Ent, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		//Implies that arg1 is a string literal and arg2 is a wildcard
		else if (arg2_NUM == false && arg2_UNDER == true && arg2_STRING_LITERAL == false) {
			arg1 = removeSymbols(arg1, INVERTED_COMMA_STRING);
			arg1 = Util::trim(arg1);
			QueryElement queryElement = QueryElement(arg1, VARIABLE_STRING, EMPTY_STRING, UNDER_SCORE_STRING, WILDCARD_STRING, EMPTY_STRING, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}//Implies that arg1 is a wildcard and arg2 is a number i.e. integer
		else if (arg2_NUM == true && arg2_UNDER == false && arg2_STRING_LITERAL == false) {
			arg1 = removeSymbols(arg1, INVERTED_COMMA_STRING);
			arg1 = Util::trim(arg1);
			QueryElement queryElement = QueryElement(arg1, VARIABLE_STRING, EMPTY_STRING, arg2, NUMBER_STRING, EMPTY_STRING, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		//Implies that arg1 is a wildcard and arg2 is a variable i.e. "IDENT"
		else if (arg2_NUM == false && arg2_UNDER == false && arg2_STRING_LITERAL == true) {
			arg1 = removeSymbols(arg1, INVERTED_COMMA_STRING);
			arg2 = removeSymbols(arg2, INVERTED_COMMA_STRING);
			arg1 = Util::trim(arg1);
			arg2 = Util::trim(arg2);
			QueryElement queryElement = QueryElement(arg1, VARIABLE_STRING, EMPTY_STRING, arg2, VARIABLE_STRING, EMPTY_STRING, relType);
			addSuchThatQueryElement(queryElement);
			return true;
		}
		else {
			return true;
		}
	}
	else {
		return false;
	}
}
//This method extracts all the relationship involed in the such that clauses and returns them in a vector
vector<string> QueryValidator::extractSuchThatClauses(string str) {
	regex suchThatRelRegex(TEMP_RELREF_STRING_REGEX);
	smatch m;
	string temp;
	vector<string> toReturnVec;

	while (regex_search(str, m, suchThatRelRegex)) {
		temp = m[0];
		toReturnVec.push_back(temp);
		str = m.suffix().str();
	}
	return toReturnVec;
}