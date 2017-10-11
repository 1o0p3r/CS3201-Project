#include "WithHandler.h"
#include <QueryValidator.h>
#include <vector>
#include <Util.h>
const int ZERO = 0;
const int ONE = 1;
const string WITH_STRING = "with";
const string PROCNAME = "procName";
const string VARNAME = "varName";
const string VALUE = "value";
const string STMTNUM = "stmt#";
const string STRING_LITERAL = "stringLiteral";

const string STRTYPE = "strType";
const string INTTYPE = "intType";
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
const string NUMBER_STRING = "number";


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

WithHandler::WithHandler()
{

}



//This method checks if the with clauses is a valid with clause by initially checking with the regex 
//Then proceed to check if LHS = RHS in terms of type
bool QueryValidator::isValidWith(string str) {
	str = Util::trim(str);

	//If there is a match with a regex simply proceed to compare
	if (isValidWithRegex(str) || isValidWithExtendedRegex(str)) {
		vector<string> vecWith = extractWithClauses(str);

		//Loop through for every with clauses and compare LHS AND RHS
		for (size_t i = 0; i < vecWith.size(); i++) {
			//Split the string by equal symbol
			string currWith = vecWith.at(i);
			vector<string> argsWith = splitBySymbol(currWith, EQUAL_CHAR);

			//Gets the LHS and RHS
			string arg1 = argsWith.at(ZERO);
			string arg2 = argsWith.at(ONE);

			string arg1Ent, arg1Syn, arg1attrName, arg1Identity;
			string arg2Ent, arg2Syn, arg2attrName, arg2Identity;
			bool arg1AttrRef = false;
			bool arg2AttrRef = false;
			//Checks to be done sequentially in this order: 
			//Checks if arg1 arg2 is and attrRef, if so check if the synonym is a valid one and get the corresponding entity and checks if it is a valid entity for the attrRef
			//Checks if it is a prog_line as well
			//Then proceed to check LHS and RHS

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
			}
			else {
				//Implies that arg1 is either a synonym, stringLiteral or integer
				arg1Identity = extractIdentity(arg1);
				if ((arg1Identity != STRING_LITERAL) && (arg1Identity != NUMBER_STRING) && (arg1Identity != PROG_LINE_STRING)) {
					return false;
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
			}
			else {
				arg2Identity = extractIdentity(arg2);

				if ((arg2Identity != STRING_LITERAL) && (arg2Identity != NUMBER_STRING) && (arg2Identity != PROG_LINE_STRING)) {
					return false;
				}

			}
			//Do comparison between LHS and RHS
			if (isSameType(arg1, arg2, arg1AttrRef, arg2AttrRef, arg1attrName, arg2attrName, arg1Identity, arg2Identity)) {
				//Do cleaning up for stringLiteral
				if (arg1Identity == STRING_LITERAL) {
					arg1 = removeSymbols(arg1, INVERTED_COMMA_STRING);
				}
				if (arg2Identity == STRING_LITERAL) {
					arg2 = removeSymbols(arg2, INVERTED_COMMA_STRING);
				}
				//If arg1 and 2 are both attrRref
				if (arg1AttrRef && arg2AttrRef) {
					addWithQueryElement(arg1, arg2, arg1attrName, arg2attrName, arg1Ent, arg2Ent, arg1Syn, arg2Syn);
				}
				//Only arg1 is attrRef and arg2 could be 
				else if (arg1AttrRef && !arg2AttrRef) {
					addWithQueryElement(arg1, arg2, arg1attrName, arg2Identity, arg1Ent, arg2Ent, arg1Syn, arg2Syn);
				}
				else if (!arg1AttrRef && arg2AttrRef) {
					addWithQueryElement(arg1, arg2, arg1Identity, arg2attrName, arg1Ent, arg2Ent, arg1Syn, arg2Syn);
				}
				else {
					addWithQueryElement(arg1, arg2, arg1Identity, arg2Identity, arg1Ent, arg2Ent, arg1Syn, arg2Syn);
				}
			}
			else {
				return false;
			}
		}
		return true;
	}
	else {
		return false;
	}
}
void QueryValidator::addWithQueryElement(string arg1, string arg2, string arg1Type, string arg2Type, string arg1Ent, string arg2Ent, string arg1Synonym,
	string arg2Synonym) {
	QueryElement queryElement = QueryElement(arg1, arg2, arg1Type, arg2Type, arg1Ent, arg2Ent, arg1Synonym, arg2Synonym);

	queryStatement.addWithQuery(queryElement);
}
//Checks if it is attrRef
bool QueryValidator::isAttrRef(string arg) {
	if (isValidAttRefRegex(arg)) {
		vector<string> argVec = splitBySymbol(arg, DOT_CHAR);
		string argSyn = argVec.at(0);
		string argAttrName = argVec.at(1);

		string argEnt = getCorrespondingEntity(argSyn);

		//Only match will be procedure
		if (argAttrName == PROCNAME) {
			//Can be either call or procedure
			if (argEnt == PROCEDURE_STRING || argEnt == CALL_STRING) {
				return true;
			}
		}
		//Only match will be variable
		else if (argAttrName == VARNAME) {
			return(argEnt == VARIABLE_STRING);
		}
		//Only match will be constant
		else if (argAttrName == VALUE) {
			return(argEnt == CONSTANT_STRING);
		}
		//Only matches will be stmt i.e. assign, while, if
		else if (argAttrName == STMTNUM) {
			if ((argEnt == ASSIGN_STRING) || (argEnt == WHILE_STRING) || (argEnt == IF_STRING) || (argEnt == CALL_STRING)) {
				return true;
			}
			else {
				return false;
			}
		}
		return false;
	}
	else {
		return false;
	}
}
//This function compare LHS and RHS 
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
		}
		//else RHS is not an attrRef
		else {
			//LHS IS attrRef, RHS is not attrRef
			string arg1Type = withClauseTypeBank[arg1AttrName];
			//RHS is not attrRef, so it can be either a synonym, stringliteral or integer
			string arg2TypeTemp = arg2Identity;
			string arg2Type = withClauseTypeBank[arg2TypeTemp];

			return(arg1Type == arg2Type);
		}
	}
	//else if LHS is not an attrRef
	else {
		//If LHS IS not attrRef, RHS is an attrRef
		if (arg2AttrRef) {
			string arg1Type = withClauseTypeBank[arg1Identity];
			string arg2Type = withClauseTypeBank[arg2AttrName];

			return (arg1Type == arg2Type);
		}
		else {
			//LHS and RHS are both not attrRef, means arg1 and arg 2 are both default things like n,1,"hi"

			//First check: if arg1 identity is number
			if (arg1Identity == NUMBER_STRING) {
				//If both are numbers and are not equal to each other
				if ((arg2Identity == NUMBER_STRING) && (arg1 != arg2)) {
					return false;
				}
				else if ((arg2Identity == NUMBER_STRING) && (arg1 == arg2)) {
					return true;
				}
				else if ((arg2Identity == PROG_LINE_STRING)) {
					return true;
				}
				//Arg2Identity must be a string, so must be a mismatch
				else {
					return false;
				}
			}
			else if (arg1Identity == PROG_LINE_STRING) {
				//Arg 1 is progline and arg2 is number
				if (arg2Identity == NUMBER_STRING) {
					return true;
				}
				//Arg1 is progline and arg2 is progLine
				else if (arg2Identity == PROG_LINE_STRING) {
					return true;
				}
				//Arg2 must be a string
				else {
					return false;
				}
			}
			//Arg1 is a string, only other match is a string
			else {
				if (arg2Identity != STRING_LITERAL) {
					return false;
				}
				else {
					return true;
				}
			}

		}
	}
}

//This function finds the identity of the given argument i.e. either a synonym, stringliterl or integer
string QueryValidator::extractIdentity(string arg) {
	if (isQuotationIdentRegex(arg)) {
		return STRING_LITERAL;
	}
	else if (is_number(arg)) {
		return NUMBER_STRING;
	}
	//Else must be synonym
	else {
		if (isValidSynonym(arg)) {
			string toReturn = getCorrespondingEntity(arg);
			return toReturn;
		}
	}
}