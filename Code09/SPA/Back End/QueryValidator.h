#pragma once
#include <string>
#include <vector>
#include <SynonymEntityPair.h>
#include <RelationshipTable.h>
#include <Relationship.h>
#include <QueryStatement.h>
#include <QueryElement.h>
#include <regex>

using std::string;
using std::vector;


class QueryValidator
{
private:
	int numClauses;
	RelationshipTable relationshipTable;
	QueryStatement queryStatement; 
	vector<SynonymEntityPair> synonymAndEntityList; 
	vector<string> validEntities = { "stmt", "assign", "while",
		"variable", "constant", "prog_line", "procedure", "stmtLst", "call", "if"};
	unordered_map<string, string> withClauseTypeBank;
	unordered_map<string, vector<string>> attrNameBank;
	vector<string> declarationString;
	vector<string> queryString;
	void addSynonymEntityList();

	bool isValidSelect(vector<string> vectorClauses);

	string extractAllSynAttr(vector<string> resultVec);

	bool isValidCorrespondingTupleEntities(vector<string> resultVec);

	string extractAllEnt(vector<string> resultCl);

	string extractAllSyn(vector<string> resultVec);

	bool isValidOthers(vector<string> others);
	bool isVariableSynonym(string str);
	bool isValidSynonym(string syn);
	bool inEntityList(string entity);
	bool parseDeclaration(vector<string> splitString);
	bool isValidWith(string str);
	bool isAttrRef(string arg);
	bool isSameType(string arg1, string arg2, bool arg1AttrRef, bool arg2AttrRef, string arg1AttrName, string arg2AttrName, string arg1Identity, string arg2Identity);
	bool checkRelationshipTable(string, string, int);
	bool parseQueryLine(string);
	bool isLeadingAnd(string str);
	bool isValidAddAssignPattern(string str, string synPattern);
	bool isValidAddWhilePattern(string str, string synPattern);
	bool isValidAddIfPattern(string str, string synPattern);
	bool isWhiteSpaceTab(string str);
	bool isString(string str);
	bool isBalancedParantheses(string str);
	bool isValidLeadTrail(string str);
	bool is_number(string str);
	bool isSubstring(string str);
	
	bool isValidAttrCondRegex(string str);
	bool isValidAttRefRegex(string str);
	bool isValidAttrCompareRegex(string str);
	bool isQuotationIdentRegex(string str);
	bool isValidGeneralPatternRegex(string str);
	bool isValidIfMultiplePatternRegex(string str);

	bool addSuchThatQueryElement(bool arg1_NUM, bool arg1_UNDER, bool arg1_STRING_LITERAL, bool arg2_NUM, bool arg2_UNDER, bool arg2_STRING_LITERAL, string relType, string arg1, string arg2, string type1, string type2);
	
	void addSelectQueryElement(string ent, string syn, string selectType, string str);
	void addSuchThatQueryElement(QueryElement qe);
	void addWithQueryElement(string arg1Type, string arg2Type, string arg1Ent, string arg2Ent, string arg1Synonym, string arg2Synonym);
	void addIfPatternQueryElement(string arg1, bool arg1Underscore, bool arg1Number, bool arg1Variable, bool arg1StringLiteral, string synPattern);
	void addWhilePatternQueryElement(string arg1, bool arg1Underscore, bool arg1Number, bool arg1Variable, bool arg1StringLiteral, string synPattern);
	void addAssignPatternQueryElement(string arg1, string arg2, string ent, string syn, bool arg1UnderScore, bool arg1Number, bool arg1Variable, bool arg1StringLiteral, bool arg2UnderScore, bool arg2Exact, bool arg2Substring);
	
	string extractIdentity(string arg);


	vector<string> extractAndPattern(string currTwo, vector<string> VecPattern);

	string removeSymbols(string str, string symbolToRemove);
	string trim(string str);
	string removeDuplicatesWhiteSpaces(string str);
	string removeOuterParentheses(string str);
	string removeLeadingPatternString(string str);
	string removeLeadingAnd(string str);
	string removeUnderScoreAndQuotation(string str);
	string getCorrespondingEntity(string syn);
	vector<string> splitBySymbol(string str, char symbol);
public:
	QueryValidator();
	bool parseInput(string str);
	bool isValidQueryLine(string selectString);
	bool isValidSelectInitialRegex(string str);
	bool isValidDeclarationRegex(string str);
	bool isValidSuchThat(string str);
	bool isCornerRelation(string relation);
	bool isValidSuchThatRegex(string str);
	bool isValidSuchThaExtendedRegex(string str);
	bool isValidWithRegex(string str);
	bool isValidWithExtendedRegex(string str);

	bool isValidPattern(string str);
	bool isPartialPatternRegex(string str);
	bool isValidWhilePatternRegex(string str);
	bool isValidIfPatternRegex(string str);
	bool isAssignPatternRegex(string str);

	bool isEntityAndSynonym(string);
	bool isValidEntity(string);
	bool isWildcard(string arg);
	bool isExactString(string arg2);
	bool isValidExpr(string str);
	bool isValidExprUnder(string str);
	bool isValidTuple(string str);

	bool isIdent(string str);

	bool isOnlyTuple(string str);

	vector<string> extractPatternClauses(string str);
	vector<string> extractSuchThatClauses(string str);
	vector<string> extractWithClauses(string str);
	vector<string> splitToSentences(string strToSplit);
	QueryStatement getQueryStatement();
	bool isValidCorrespondingEntity(string synonym, string attrName);
	
};