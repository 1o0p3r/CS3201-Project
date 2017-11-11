//This class purpose is to store the process of validation of such that clauses and nothing else
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
const string SUCH_THAT_STRING = "such that";
const string FOLLOWS_STRING = "Follows";
const string FOLLOWS_STAR_STRING = "Follows*";
const string PARENT_STRING = "Parent";
const string PARENT_STAR_STRING = "Parent*";
const string MODIFIES_STRING = "Modifies";
const string USES_STRING = "Uses";
const string CALLS_STRING = "Calls";
const string NEXT_STRING = "Next";
const string NEXT_STAR_STRING = "Next*";
const string AFFECTS_STRING = "Affects";
const string AFFECTS_STAR_STRING = "Affects*";
const string CALL_STRING = "call";
const string UNDER_SCORE_STRING = "_";
const string WILDCARD_STRING = "wildcard";
const string SYMBOL_LEFT_BRACKET_STRING = "(";
const string SYMBOL_RIGHT_BRACKET_STRING = ")";
const string NUMBER_STRING = "number";
const string INVERTED_COMMA_STRING = "\"";
const string EXACT_STRING = "exact";
const string SUBSTRING_STRING = "substring";
const string VARIABLE_STRING = "variable";
const string SYNONYM_STRING = "synonym";
const string INVALID_STRING = "invalid";
const string STR_STRING = "string";
const string BOOLEAN_STRING = "BOOLEAN";

const string SUCH_THAT = "suchThat";
const string OR = "|";
const string EMPTY_STRING = "empty";
const string AND_STRING = "and";
const string ASTERIK = "*";
const string EQUAL_STRING = "=";
const string TAB_STRING = "\\t";

suchThatHandler::suchThatHandler()
{
}

//This function takes in a given string and checks that it is a valid such that clause
//Returns true if valid, else returns false
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
			string relation = Util::trim(tempVec.at(ZERO));
			string argsWithoutBracket = tempVec.at(ONE).substr(ZERO, tempVec.at(ONE).length() - ONE);
			argsWithoutBracket = removeSymbols(argsWithoutBracket, WHITESPACE_STRING);
			argsWithoutBracket = removeSymbols(argsWithoutBracket, TAB_STRING);
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
				arg1Valid = checkRelationshipTable(relation, arg1Ent, ONE);
			} else {
				//If no correspond entity was obtained, try to check if it is a number or wildcard
				if (is_number(arg1)) {
					//If exists in table and part of declared relationship, create a clause and add it to query tree
					arg1Valid = checkRelationshipTable(relation, NUMBER_STRING, ONE);
					if (arg1Valid) {
						arg1_NUM = true;
					} else {
						arg1_NUM = false;
					}
				} else if (arg1 == UNDER_SCORE_STRING) {
					arg1Valid = checkRelationshipTable(relation, WILDCARD_STRING, ONE);
					if (arg1Valid) {
						arg1_UNDER = true;
					} else {
						arg1_UNDER = false;
					}
				} else if (isQuotationIdentRegex(arg1)) {
					arg1Valid = checkRelationshipTable(relation, STR_STRING, ONE);
					if (arg1Valid) {
						arg1_STRING_LITERAL = true;
					} else {
						arg1_STRING_LITERAL = false;
					}
				} else {
					arg1Valid = false;
				}
			}
			if (arg2Ent != INVALID_STRING) {
				arg2Valid = checkRelationshipTable(relation, arg2Ent, TWO);
			} else {
				if (is_number(arg2)) {
					arg2Valid = checkRelationshipTable(relation, NUMBER_STRING, TWO);
					if (arg2Valid) {
						arg2_NUM = true;
					} else {
						arg2_NUM = false;
					}
				} else if (arg2 == UNDER_SCORE_STRING) {
					arg2Valid = checkRelationshipTable(relation, WILDCARD_STRING, TWO);
					if (arg2Valid) {
						arg2_UNDER = true;
					} else {
						arg2_UNDER = false;
					}
				} else if (isQuotationIdentRegex(arg2)) {
					arg2Valid = checkRelationshipTable(relation, STR_STRING, TWO);
					if (arg2Valid) {
						arg2_STRING_LITERAL = true;
					} else {
						arg2_STRING_LITERAL = false;
					}
				} else {
					arg2Valid = false;
				}
			}
			if (!isAllowedParametersCheck(arg1, arg1Ent, arg1_STRING_LITERAL, arg2, arg2Ent, arg2_STRING_LITERAL, relation)) {
				if (!resultBoolean) {
					return false;
				} else {
					queryStatement.setInvalidQueryBoolean();
					return true;
				}
			}
			//If both are valid and true, create the clause
			if (arg1Valid && arg2Valid) {
				if (!addSuchThatQueryElement(arg1_NUM, arg1_UNDER, arg1_STRING_LITERAL, arg2_NUM, arg2_UNDER, arg2_STRING_LITERAL, relation, arg1, arg2, arg1Ent, arg2Ent)) {
					return false;
				}
			} else {
				if (!resultBoolean) {
					return false;
				} else {
					queryStatement.setInvalidQueryBoolean();
					return true;
				}
			}
		}
	} else {
		if (!resultBoolean) {
			return false;
		} else {
			queryStatement.setInvalidQueryBoolean();
			return true;
		}
	}
	return true;
}
//This function takes in and do further checking on the argument to ensure that argument 1 and argument 2 are not arguments that do not make sense, e.g. Follows(s,s)
//Returns true if valid, else returns false
bool QueryValidator::isAllowedParametersCheck(string arg1, string arg1Ent, bool arg1_STRING_LITERAL, string arg2, string arg2Ent, bool arg2_STRING_LITERAL, string relation) {
	//Do further checking only if arg1 equals to arg2
	if (arg1 == arg2) {
		if (resultBoolean && !isCornerRelation(relation) && arg1 != UNDER_SCORE_STRING) {
			queryStatement.setInvalidQueryBoolean();
			return true;
		} //Then check the relationships, the only such that clauses with same arg1 and arg2 are Next*, Affects*
		else if (isCornerRelation(relation)) {
			return true;
		} else {
			if (arg1 == UNDER_SCORE_STRING) {
				return true;
			} else {
				return false;
			}
		}
	} else {
		return true;
	}
}
//This function takes in a given string and checks that given string is Modifes|Uses|Next*|Affects*|Affects
//Returns true if is indeed one of the relation, else returns false
bool QueryValidator::isCornerRelation(string relation) {
	if ((relation == MODIFIES_STRING) || (relation == USES_STRING) || (relation == NEXT_STAR_STRING) || (relation == AFFECTS_STAR_STRING) || (relation == AFFECTS_STRING)) {
		return true;
	}
	return false;
}
//This function takes in a given string and checks if it is either Affects|Affects*|Next
//Returns true if it is indeed one of the relation, else returns false
bool QueryValidator::isHardType(string rel) {
	return (rel == AFFECTS_STRING || rel == AFFECTS_STAR_STRING || rel == NEXT_STAR_STRING);
}
//This function adds a such that QueryElement into the QueryStatement Object
//Returns true if successful addition is done, else returns false
bool QueryValidator::addSuchThatQueryElement(bool arg1_NUM, bool arg1_UNDER, bool arg1_STRING_LITERAL, bool arg2_NUM, bool arg2_UNDER, bool arg2_STRING_LITERAL, string relType, string arg1, string arg2, string arg1Ent, string arg2Ent) {
	//QueryElement parameters:
	//Arg1, Arg1Type, Arg1Ent, Arg2, Arg2Type, arg2Ent, relType
	//Implies that arg1 is a synonym
	if (!arg1_NUM && !arg1_UNDER && !arg1_STRING_LITERAL) {
		//Implies that arg 1 is snyonym and arg2 is also a synonym
		if (!arg2_NUM && !arg2_UNDER && !arg2_STRING_LITERAL) {
			QueryElement queryElement = QueryElement(arg1, SYNONYM_STRING, arg1Ent, arg2, SYNONYM_STRING, arg2Ent, relType, SUCH_THAT);
			if (isHardType(relType)) {
				queryStatement.addHardQueryElement(queryElement);
				queryStatement.addHardMultiMap(arg1, ONE, queryStatement.getHardQueryElementsSize() - ONE);
				queryStatement.addHardMultiMap(arg2, TWO, queryStatement.getHardQueryElementsSize() - ONE);
			} else {
				queryStatement.addNormalQueryElement(queryElement);
				queryStatement.addNormalMultiMap(arg1, ONE, queryStatement.getNormalQueryElementsSize() - ONE);
				queryStatement.addNormalMultiMap(arg2, TWO, queryStatement.getNormalQueryElementsSize() - ONE);
			}
			return true;
		} else if (!arg2_NUM && arg2_UNDER && !arg2_STRING_LITERAL) {
			//Implies that the clause for arg1 is not a num/under, arg2 is not a num, arg2 is an UNDER
			QueryElement queryElement = QueryElement(arg1, SYNONYM_STRING, arg1Ent, UNDER_SCORE_STRING, WILDCARD_STRING, EMPTY_STRING, relType, SUCH_THAT);
			if (isHardType(relType)) {
				queryStatement.addHardQueryElement(queryElement);
				queryStatement.addHardMultiMap(arg1, ONE, queryStatement.getHardQueryElementsSize() - ONE);
			} else {
				queryStatement.addNormalQueryElement(queryElement);
				queryStatement.addNormalMultiMap(arg1, ONE, queryStatement.getNormalQueryElementsSize() - ONE);
			}
			return true;
		} else if (arg2_NUM && !arg2_UNDER && !arg2_STRING_LITERAL) {
			QueryElement queryElement = QueryElement(arg1, SYNONYM_STRING, arg1Ent, arg2, NUMBER_STRING, EMPTY_STRING, relType, SUCH_THAT);
			if (isHardType(relType)) {
				queryStatement.addHardQueryElement(queryElement);
				queryStatement.addHardMultiMap(arg1, ONE, queryStatement.getHardQueryElementsSize() - ONE);
			} else {
				queryStatement.addNormalQueryElement(queryElement);
				queryStatement.addNormalMultiMap(arg1, ONE, queryStatement.getNormalQueryElementsSize() - ONE);
			}
			return true;
		} else if (!arg2_NUM && !arg2_UNDER  && arg2_STRING_LITERAL) {
			arg2 = removeSymbols(arg2, INVERTED_COMMA_STRING);
			arg2 = Util::trim(arg2);
			QueryElement queryElement = QueryElement(arg1, SYNONYM_STRING, arg1Ent, arg2, VARIABLE_STRING, EMPTY_STRING, relType, SUCH_THAT);
			if (isHardType(relType)) {
				queryStatement.addHardQueryElement(queryElement);
				queryStatement.addHardMultiMap(arg1, ONE, queryStatement.getHardQueryElementsSize() - ONE);
			} else {
				queryStatement.addNormalQueryElement(queryElement);
				queryStatement.addNormalMultiMap(arg1, ONE, queryStatement.getNormalQueryElementsSize() - ONE);
			}
			return true;
		} else {
			return false;
		}
	} else if (arg1_NUM && !arg1_UNDER && !arg1_STRING_LITERAL) {
		//Implies that arg1 is a number and arg2 is a synonym
		if (!arg2_NUM && !arg2_UNDER && !arg2_STRING_LITERAL) {
			QueryElement queryElement = QueryElement(arg1, NUMBER_STRING, EMPTY_STRING, arg2, SYNONYM_STRING, arg2Ent, relType, SUCH_THAT);
			if (isHardType(relType)) {
				queryStatement.addHardQueryElement(queryElement);
				queryStatement.addHardMultiMap(arg2, TWO, queryStatement.getHardQueryElementsSize() - ONE);
			} else {
				queryStatement.addNormalQueryElement(queryElement);
				queryStatement.addNormalMultiMap(arg2, TWO, queryStatement.getNormalQueryElementsSize() - ONE);
			}
			return true;
		} else if (!arg2_NUM && arg2_UNDER && !arg2_STRING_LITERAL) {
			QueryElement queryElement = QueryElement(arg1, NUMBER_STRING, EMPTY_STRING, UNDER_SCORE_STRING, WILDCARD_STRING, EMPTY_STRING, relType, SUCH_THAT);
			if (isHardType(relType)) {
				queryStatement.addHardQueryElement(queryElement);
			} else {
				queryStatement.addNormalQueryElement(queryElement);
			}
			return true;
		} else if (arg2_NUM && !arg2_UNDER && !arg2_STRING_LITERAL) {
			QueryElement queryElement = QueryElement(arg1, NUMBER_STRING, EMPTY_STRING, arg2, NUMBER_STRING, EMPTY_STRING, relType, SUCH_THAT);
			if (isHardType(relType)) {
				queryStatement.addHardQueryElement(queryElement);
			} else {
				queryStatement.addNormalQueryElement(queryElement);
			}
			return true;
		} else if (!arg2_NUM && !arg2_UNDER  && arg2_STRING_LITERAL) {
			arg2 = removeSymbols(arg2, INVERTED_COMMA_STRING);
			arg2 = Util::trim(arg2);
			QueryElement queryElement = QueryElement(arg1, NUMBER_STRING, EMPTY_STRING, arg2, VARIABLE_STRING, EMPTY_STRING, relType, SUCH_THAT);
			if (isHardType(relType)) {
				queryStatement.addHardQueryElement(queryElement);
			} else {
				queryStatement.addNormalQueryElement(queryElement);
			}
			return true;
		} else {
			return false;
		}
	} else if (!arg1_NUM && arg1_UNDER && !arg1_STRING_LITERAL) {
		//Implies taht arg1 is a wildcard and arg 2 is a synonym
		if (!arg2_NUM && !arg2_UNDER && !arg2_STRING_LITERAL) {
			QueryElement queryElement = QueryElement(UNDER_SCORE_STRING, WILDCARD_STRING, EMPTY_STRING, arg2, SYNONYM_STRING, arg2Ent, relType, SUCH_THAT);
			if (isHardType(relType)) {
				queryStatement.addHardQueryElement(queryElement);
				queryStatement.addHardMultiMap(arg2, TWO, queryStatement.getHardQueryElementsSize() - ONE);
			} else {
				queryStatement.addNormalQueryElement(queryElement);
				queryStatement.addNormalMultiMap(arg2, TWO, queryStatement.getNormalQueryElementsSize() - ONE);
			}
			return true;
		} else if (!arg2_NUM && arg2_UNDER && !arg2_STRING_LITERAL) {
			QueryElement queryElement = QueryElement(UNDER_SCORE_STRING, WILDCARD_STRING, EMPTY_STRING, UNDER_SCORE_STRING, WILDCARD_STRING, EMPTY_STRING, relType, SUCH_THAT);
			if (isHardType(relType)) {
				queryStatement.addHardQueryElement(queryElement);
			} else {
				queryStatement.addNormalQueryElement(queryElement);
			}
			return true;
		} else if (arg2_NUM && !arg2_UNDER && !arg2_STRING_LITERAL) {
			QueryElement queryElement = QueryElement(UNDER_SCORE_STRING, WILDCARD_STRING, EMPTY_STRING, arg2, NUMBER_STRING, EMPTY_STRING, relType, SUCH_THAT);
			if (isHardType(relType)) {
				queryStatement.addHardQueryElement(queryElement);
			} else {
				queryStatement.addNormalQueryElement(queryElement);
			}
			return true;
		} else if (!arg2_NUM && !arg2_UNDER  && arg2_STRING_LITERAL) {
			arg2 = removeSymbols(arg2, INVERTED_COMMA_STRING);
			arg2 = Util::trim(arg2);
			QueryElement queryElement = QueryElement(UNDER_SCORE_STRING, WILDCARD_STRING, EMPTY_STRING, arg2, VARIABLE_STRING, EMPTY_STRING, relType, SUCH_THAT);
			if (isHardType(relType)) {
				queryStatement.addHardQueryElement(queryElement);
			} else {
				queryStatement.addNormalQueryElement(queryElement);
			}
			return true;
		} else {
			return false;
		}
	} else if (!arg1_NUM && !arg1_UNDER && arg1_STRING_LITERAL) {
		//Implies taht arg1 is a stringLiteral and arg 2 is a synonym
		if (!arg2_NUM && !arg2_UNDER && !arg2_STRING_LITERAL) {
			arg1 = removeSymbols(arg1, INVERTED_COMMA_STRING);
			arg1 = Util::trim(arg1);
			QueryElement queryElement = QueryElement(arg1, VARIABLE_STRING, EMPTY_STRING, arg2, SYNONYM_STRING, arg2Ent, relType, SUCH_THAT);
			if (isHardType(relType)) {
				queryStatement.addHardQueryElement(queryElement);
				queryStatement.addHardMultiMap(arg2, TWO, queryStatement.getHardQueryElementsSize() - ONE);
			} else {
				queryStatement.addNormalQueryElement(queryElement);
				queryStatement.addNormalMultiMap(arg2, TWO, queryStatement.getNormalQueryElementsSize() - ONE);
			}
			return true;
		} else if (!arg2_NUM && arg2_UNDER && !arg2_STRING_LITERAL) {
			arg1 = removeSymbols(arg1, INVERTED_COMMA_STRING);
			arg1 = Util::trim(arg1);
			QueryElement queryElement = QueryElement(arg1, VARIABLE_STRING, EMPTY_STRING, UNDER_SCORE_STRING, WILDCARD_STRING, EMPTY_STRING, relType, SUCH_THAT);
			if (isHardType(relType)) {
				queryStatement.addHardQueryElement(queryElement);
			} else {
				queryStatement.addNormalQueryElement(queryElement);
			}
			return true;
		} else if (arg2_NUM && !arg2_UNDER && !arg2_STRING_LITERAL) {
			arg1 = removeSymbols(arg1, INVERTED_COMMA_STRING);
			arg1 = Util::trim(arg1);
			QueryElement queryElement = QueryElement(arg1, VARIABLE_STRING, EMPTY_STRING, arg2, NUMBER_STRING, EMPTY_STRING, relType, SUCH_THAT);
			if (isHardType(relType)) {
				queryStatement.addHardQueryElement(queryElement);
			} else {
				queryStatement.addNormalQueryElement(queryElement);
			}
			return true;
		} else if (!arg2_NUM && !arg2_UNDER  && arg2_STRING_LITERAL) {
			arg1 = removeSymbols(arg1, INVERTED_COMMA_STRING);
			arg2 = removeSymbols(arg2, INVERTED_COMMA_STRING);
			arg1 = Util::trim(arg1);
			arg2 = Util::trim(arg2);
			QueryElement queryElement = QueryElement(arg1, VARIABLE_STRING, EMPTY_STRING, arg2, VARIABLE_STRING, EMPTY_STRING, relType, SUCH_THAT);
			if (isHardType(relType)) {
				queryStatement.addHardQueryElement(queryElement);
			} else {
				queryStatement.addNormalQueryElement(queryElement);
			}
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}
