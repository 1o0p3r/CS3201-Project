//This method exists Only to contain the process of handling with clauses while still using existing methods in QueryValidator.cpp
#include "WithHandler.h"
#include <QueryValidator.h>
#include <vector>
#include <Util.h>
const int ZERO = 0;
const int ONE = 1;
const int TWO = 2;
const string WITH_STRING = "with";
const string PROCNAME = "procName";
const string VARNAME = "varName";
const string VALUE = "value";
const string STMTNUM = "stmt#";
const string STMT = "stmt";
const string STRING_LITERAL = "stringLiteral";

const string STRTYPE = "strType";
const string INTTYPE = "intType";
const string SYMBOL_SEMI_COLON_STRING = ";";
const string UNDER_SCORE_STRING = "_";
const string SYMBOL_LEFT_BRACKET_STRING = "(";
const string SYMBOL_RIGHT_BRACKET_STRING = ")";

const string EMPTY_STRING = "empty";
const string ASSIGN_STRING = "assign";
const string WHILE_STRING = "while";
const string IF_STRING = "if";
const string AND_STRING = "and";
const string ASTERIK = "*";
const string EQUAL_STRING = "=";
const string OR = "OR";

const char DOT_CHAR = '.';
const char INVERTED_COMMA = '\"';
const char WHITESPACE_CHAR = ' ';
const char DOUBLE_QUOTATION = '\"';
const char EQUAL_CHAR = '=';
const string INVERTED_COMMA_STRING = "\"";

const string VARIABLE_STRING = "variable";
const string SYNONYM_STRING = "synonym";
const string INTEGER_STRING = "integer";
const string INVALID_STRING = "invalid";
const string PROCEDURE_STRING = "procedure";
const string PROG_LINE_STRING = "prog_line";
const string STR_STRING = "string";
const string BOOLEAN_STRING = "BOOLEAN";
const string CALL_STRING = "call";
const string NUMBER_STRING = "number";

WithHandler::WithHandler()
{

}
//This method checks if the with clauses is a valid with clause
//Returns true if valid, else returns false
bool QueryValidator::isValidWith(string str) {
	str = Util::trim(str);
	//If there is a match with a regex simply proceed to compare
	if (isValidWithRegex(str) || isValidWithExtendedRegex(str)) {
		vector<string> vecWith = extractWithClauses(str);
		//Loop through for every with clauses and compare LHS AND RHS
		for (size_t i = ZERO; i < vecWith.size(); i++) {
			//Split the string by equal symbol
			string currWith = vecWith.at(i);
			vector<string> argsWith = splitBySymbol(currWith, EQUAL_CHAR);
			string arg1 = argsWith.at(ZERO);
			string arg2 = argsWith.at(ONE);
			string arg1Ent, arg1Syn, arg1attrName, arg1Identity;
			string arg2Ent, arg2Syn, arg2attrName, arg2Identity;
			bool arg1AttrRef = false;
			bool arg2AttrRef = false;
			arg1 = Util::trim(arg1);
			arg2 = Util::trim(arg2);
			//Implies that e.g. v.varName as a whole is a valid, so i can go and get its respective entity, synonym, etc
			if (isAttrRef(arg1)) {
				//Split by dot
				vector<string> arg1Vec = splitBySymbol(arg1, DOT_CHAR);
				//Gets the synonym
				arg1Syn = arg1Vec.at(ZERO);
				//Gets the Entity
				arg1Ent = getCorrespondingEntity(arg1Syn);
				//Gets the attrRef
				arg1attrName = arg1Vec.at(ONE);
				arg1AttrRef = true;
			} else {
				//Implies that arg1 is either a synonym, stringLiteral or integer
				arg1Identity = extractIdentity(arg1);
				if ((arg1Identity != STRING_LITERAL) && (arg1Identity != NUMBER_STRING) && (arg1Identity != PROG_LINE_STRING) && !resultBoolean) {
					return false;
				} else if ((arg1Identity != STRING_LITERAL) && (arg1Identity != NUMBER_STRING) && (arg1Identity != PROG_LINE_STRING) && resultBoolean) {
					queryStatement.setInvalidQueryBoolean();
					return true;
				}
			}
			if (isAttrRef(arg2)) {
				//Split by dot
				vector<string> arg2Vec = splitBySymbol(arg2, DOT_CHAR);
				//Gets the synonym
				arg2Syn = arg2Vec.at(ZERO);
				//Gets the Entity
				arg2Ent = getCorrespondingEntity(arg2Syn);
				//Gets the attrRef
				arg2attrName = arg2Vec.at(ONE);
				arg2AttrRef = true;
			} else {
				arg2Identity = extractIdentity(arg2);
				if ((arg2Identity != STRING_LITERAL) && (arg2Identity != NUMBER_STRING) && (arg2Identity != PROG_LINE_STRING) && !resultBoolean) {
					return false;
				} else if((arg2Identity != STRING_LITERAL) && (arg2Identity != NUMBER_STRING) && (arg2Identity != PROG_LINE_STRING) && resultBoolean) {
					queryStatement.setInvalidQueryBoolean();
					return true;
				}
			}
			bool bothSameType = isSameType(arg1, arg2, arg1AttrRef, arg2AttrRef, arg1attrName, arg2attrName, arg1Identity, arg2Identity);
			//Do comparison between LHS and RHS
			if (bothSameType) {
				//Do cleaning up for stringLiteral
				if (arg1Identity == STRING_LITERAL || arg2Identity == STRING_LITERAL) {
					arg1 = removeSymbols(arg1, INVERTED_COMMA_STRING);
					arg2 = removeSymbols(arg2, INVERTED_COMMA_STRING);
				}
				if (arg1 == arg2) {
					continue;
				} else {
					bool bothStrLiteral = bothStringLiteral(arg1Identity, arg2Identity);
					bool bothNum = bothNumber(arg1Identity, arg2Identity);
					if (bothStrLiteral && resultBoolean) {
						queryStatement.setInvalidQueryBoolean();
						return true;
					} else if(bothStrLiteral && !resultBoolean) {
						return false;
					} else if (bothNum && resultBoolean) {
						queryStatement.setInvalidQueryBoolean();
						return true;
					} else if(bothNum && !resultBoolean) {
						return false;
					}
				}
				//If arg1 and 2 are both attrRref	
				if (arg1AttrRef && arg2AttrRef) {
					//E.g. p.procName = v.varName
					addWithQueryElement(arg1attrName, arg2attrName, arg1Ent, arg2Ent, arg1Syn, arg2Syn);
				} else if (arg1AttrRef && !arg2AttrRef) {
					//E.g. p.procName = "sth"
					addWithQueryElement(arg1attrName, arg2Identity, arg1Ent, arg2Ent, arg1Syn, arg2);
				} else if (!arg1AttrRef && arg2AttrRef) {
						//E.g. "sth" = p.procName
					addWithQueryElement(arg1Identity, arg2attrName, arg1Ent, arg2Ent, arg1, arg2Syn);
				} else {
					//E.g. n = 1
					addWithQueryElement(arg1Identity, arg2Identity, arg1Ent, arg2Ent, arg1, arg2);
				}
			} else if (!bothSameType && resultBoolean) {
				queryStatement.setInvalidQueryBoolean();
				return true;
			} else {
				return false;
			}
		}
		return true;
	} else {
		if (!resultBoolean) {
			return false;
		} else {
			queryStatement.setInvalidQueryBoolean();
			return true;
		}
	}
}
//This function takes in 2 argument and checks if both are stringLiterals
//Returns true if so, else returns false
bool QueryValidator::bothStringLiteral(string arg1Identity, string arg2Identity) {
	if (arg1Identity == STRING_LITERAL && arg1Identity == arg2Identity) {
		return true;
	} else {
		return false;
	}
}
//This function takes in 2 argument and checks if both are numbers
//Returns true if so, else returns false
bool QueryValidator::bothNumber(string arg1Identity, string arg2Identity) {
	if (arg1Identity == NUMBER_STRING && arg1Identity == arg2Identity) {
		return true;
	}
	else {
		return false;
	}
}
//This function creates a with QueryElement and adds to QueryStatement
void QueryValidator::addWithQueryElement(string arg1Type, string arg2Type, string arg1Ent, string arg2Ent, string arg1Synonym,
	string arg2Synonym) {
	QueryElement queryElement = QueryElement(arg1Type, arg2Type, arg1Ent, arg2Ent, arg1Synonym, arg2Synonym, WITH_STRING);
	queryStatement.addWithQuery(queryElement);

	bool arg1StringLiteralOrNum = false;
	bool arg2StringLiteralOrNum = false;

	if (isStringLiteralOrNumber(arg1Synonym)) {
		arg1StringLiteralOrNum = true;
	}
	if (isStringLiteralOrNumber(arg2Synonym)) {
		arg2StringLiteralOrNum = true;
	}
}
//This function checks if given string is a stringLiteral or Number
//Returns true if either is true, else returns false
bool QueryValidator::isStringLiteralOrNumber(string syn) {
	return (isQuotationIdentRegex(syn) || is_number(syn));
}

//This function checks given string is attrRef
//Returns true if so, else returns false
bool QueryValidator::isAttrRef(string arg) {
	if (isValidAttRefRegex(arg)) {
		vector<string> argVec = splitBySymbol(arg, DOT_CHAR);
		string argSyn = argVec.at(ZERO);
		string argAttrName = argVec.at(ONE);

		string argEnt = getCorrespondingEntity(argSyn);

		if (isValidCorrespondingEntity(argSyn, argAttrName)) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}
//This function compare LHS and RHS and checks for their type as indicated on handbook i.e. both must be same Type: stringType or intType
//Returns true if they are same type, else returns false
bool QueryValidator::isSameType(string arg1, string arg2, bool arg1AttrRef, bool arg2AttrRef, string arg1AttrName, string arg2AttrName
	, string arg1Identity, string arg2Identity) {
	//First check if LHS is an attrRef
	if (arg1AttrRef) {
		//If RHS is attrRef
		if (arg2AttrRef) {
			//LHS and RHS are both attrRef
			string arg1Type = withClauseTypeBank[arg1AttrName];
			string arg2Type = withClauseTypeBank[arg2AttrName];
			return (arg1Type == arg2Type);
		} else {
			//LHS IS attrRef, RHS is not attrRef
			string arg1Type = withClauseTypeBank[arg1AttrName];
			//RHS is not attrRef, so it can be either a synonym, stringliteral or integer
			string arg2TypeTemp = arg2Identity;
			string arg2Type = withClauseTypeBank[arg2TypeTemp];
			return(arg1Type == arg2Type);
		}
	} else {
		//If LHS IS not attrRef, RHS is an attrRef
		if (arg2AttrRef) {
			string arg1Type = withClauseTypeBank[arg1Identity];
			string arg2Type = withClauseTypeBank[arg2AttrName];
			return (arg1Type == arg2Type);
		} else {
			//LHS and RHS are both not attrRef, means arg1 and arg 2 are both default things like n,1,"hi"
			//First check: if arg1 identity is number
			if (arg1Identity == NUMBER_STRING) {
				//If both are numbers and are not equal to each other
				if ((arg2Identity == NUMBER_STRING) && (arg1 != arg2) && !resultBoolean) {
					return false;
				} else if ((arg2Identity == NUMBER_STRING) && (arg1 != arg2) && resultBoolean) {
					return true;
				} else if ((arg2Identity == NUMBER_STRING) && (arg1 == arg2)) {
					return true;
				} else if ((arg2Identity == PROG_LINE_STRING)) {
					return true;
				} else {
					return false;
				}
			} else if (arg1Identity == PROG_LINE_STRING) {
				//Arg 1 is progline and arg2 is number
				if (arg2Identity == NUMBER_STRING) {
					return true;
				} else if (arg2Identity == PROG_LINE_STRING) {
					return true;
				} else {
					return false;
				}
			} else {
				if (arg2Identity != STRING_LITERAL) {
					return false;
				} else {
					return true;
				}
			}

		}
	}
}

//This function finds the identity of the given argument i.e. either a synonym, stringliterl or integer
//Returns the identity string
string QueryValidator::extractIdentity(string arg) {
	if (isQuotationIdentRegex(arg)) {
		return STRING_LITERAL;
	} else if (is_number(arg)) {
		return NUMBER_STRING;
	} else {
		if (isValidSynonym(arg)) {
			string toReturn = getCorrespondingEntity(arg);
			return toReturn;
		} else {
			return INVALID_STRING;
		}
	}
}