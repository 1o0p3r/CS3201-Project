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


	string getCorrespondingEntity(string syn);

	vector<string> declarationString;
	vector<string> queryString;

	//void setSynonymList(vector<SynonymEntityPair> synonymList);
	void addSelectQueryElement(string ent, string syn, string selectType);
	void addSuchThatQueryElement(QueryElement qe);
	void addSynonymEntityList();

	string extractIdentity(string arg);
	bool isValidSelect(vector<string> vectorClauses);
	bool isValidOthers(vector<string> others);
	bool isVariableSynonym(string str);
	bool isValidSynonym(string syn);
	bool inEntityList(string entity);
	bool parseDeclaration(vector<string> splitString);
	bool isValidWith(string str);
	void addWithQueryElement(string arg1, string arg2, string arg1Type, string arg2Type, string arg1Ent, string arg2Ent, string arg1Synonym, string arg2Synonym);
	bool isAttrRef(string arg);
	bool isSameType(string arg1, string arg2, bool arg1AttrRef, bool arg2AttrRef, string arg1AttrName, string arg2AttrName, string arg1Identity, string arg2Identity);
	bool checkRelationshipTable(string, string, int);
	bool parseQueryLine(string);

	bool isValidAddAssignPattern(string str, string synPattern);
	bool isValidAddWhilePattern(string str, string synPattern);
	bool isValidAddIfPattern(string str, string synPattern);
	bool addSuchThatQueryElement(bool arg1_NUM, bool arg1_UNDER, bool arg1_STRING_LITERAL, bool arg2_NUM, bool arg2_UNDER, bool arg2_STRING_LITERAL, string relType, string arg1, string arg2, string type1, string type2);
	void addIfPatternQueryElement(string arg1, bool arg1Underscore, bool arg1Number, bool arg1Variable, bool arg1StringLiteral, string synPattern);
	void addWhilePatternQueryElement(string arg1, bool arg1Underscore, bool arg1Number, bool arg1Variable, bool arg1StringLiteral, string synPattern);
	void addAssignPatternQueryElement(string arg1, string arg2, string ent, string syn, bool arg1UnderScore, bool arg1Number, bool arg1Variable, bool arg1StringLiteral, bool arg2UnderScore, bool arg2Exact, bool arg2Substring);
public:
	QueryValidator();
	bool isValidDeclarationRegex(string str);
	bool isValidSuchThatRegex(string str);
	bool isValidSuchThaExtendedRegex(string str);
	bool isValidWithRegex(string str);
	bool isValidWithExtendedRegex(string str);
	bool isValidAttrCondRegex(string str);
	bool isValidAttRefRegex(string str);
	bool isValidIfMultiplePatternRegex(string str);
	bool isValidAttrCompareRegex(string str);
	bool isValidSelectInitialRegex(string str);
	bool isValidGeneralPatternRegex(string str);
	bool isValidWhilePatternRegex(string str);
	bool isValidIfPatternRegex(string str);
	bool isVariable(string str);
	bool isQuotationIdentRegex(string str);
	bool isPartialPatternRegex(string str);
	bool isAssignPatternRegex(string str);
	bool parseInput(string str);
	bool isEntityAndSynonym(string);
	bool isValidSuchThat(string str);
	bool isString(string str);
	bool isWhiteSpaceTab(string str);
	bool isSubstring(string str);
	bool isValidEntity(string);
	bool isVariableArg1(string arg1);
	bool isSubstringArg2(string arg2);
	bool isWildcard(string arg);
	bool isExactString(string arg2);
	bool isValidQueryLine(string selectString);
	bool isValidSynDesignEntity(string syn);
	bool isValidPatternIter2(string str);
	bool isValidExpr(string str);
	bool isBalancedParantheses(string str);
	bool isValidExprUnder(string str);
	bool isLeadingAnd(string str);
	bool isSubMatchArg2Pattern(string str);
	bool isValidLeadingCheck(string str);
	bool isValidLeadTrail(string str);
	bool is_number(string str);

	string changeLowerCase(string str);
	string removeSymbols(string str, string symbolToRemove);
	string removeDuplicatesWhiteSpaces(string str);
	string removeOuterParentheses(string str);
	string trim(string str);
	string trimPatternArgs(string str);

	string removeLeadingPatternString(string str);
	string removeLeadingAnd(string str);
	string removeUnderScoreAndQuotation(string str);



	vector<string> extractSuchThatClauses(string str);
	vector<string> extractWithClauses(string str);
	vector<string> extractPattern(string str);
	vector<string> splitToSentences(string strToSplit);
	vector<string> splitBySymbol(string str, char symbol);
	
	QueryStatement getQueryStatement();
};