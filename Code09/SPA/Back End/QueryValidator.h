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
	void addPatternQueryElement(string arg1, string arg2, string ent, string syn, bool arg1Variable, bool arg1Wildcard, bool arg1Synonym, bool arg2Substring, bool arg2FullString, bool arg2Wilcard);
	void addSuchThatQueryElement(QueryElement qe);
	void addSynonymEntityList();

	bool isValidSelect(vector<string> vectorClauses);
	bool isValidOthers(vector<string> others);
	bool isValidPattern(string str, string syn);
	bool isValidSynonym(string syn);
	bool inEntityList(string entity);
	bool parseDeclaration(vector<string> splitString);
	bool isValidWith(string str);
	void addWithQueryElement(string arg1, string arg2, string arg1Type, string arg2Type, string arg1Ent, string arg2Ent, string arg1Synonym, string arg2Synonym);
	bool isAttrRef(string arg);
	bool isSameType(string arg1, string arg2, bool arg1AttrRef, bool arg2AttrRef, string arg1AttrName, string arg2AttrName, string arg1Identity, string arg2Identity);
	string extractIdentity(string arg);
	bool checkRelationshipTable(string, string, int);
	bool parseQueryLine(string);

public:
	QueryValidator();
	bool isValidDeclarationRegex(string str);
	bool isValidSuchThatRegex(string str);
	bool isValidSuchThaExtendedRegex(string str);
	bool isValidWithRegex(string str);
	bool isValidWithExtendedRegex(string str);
	bool isValidAttrCondRegex(string str);
	bool isValidAttRefRegex(string str);
	bool isValidAttrCompareRegex(string str);
	bool isValidPatternRegex(string str);
	bool isValidSelectInitialRegex(string str);
	bool isVariable(string str);
	bool isQuotationIdentRegex(string str);
	bool parseInput(string str);
	bool isValidModifiesP(string str);
	bool isEntityAndSynonym(string);
	bool isValidSuchThat(string str);
	bool addSuchThatQueryElement(bool arg1_NUM, bool arg1_UNDER, bool arg1_STRING_LITERAL, bool arg2_NUM, bool arg2_UNDER, bool arg2_STRING_LITERAL, string relType, string arg1, string arg2, string type1, string type2);
	bool isValidEntity(string);

	bool isVariableArg1(string arg1);
	bool isSubstringArg2(string arg2);
	bool isWildcard(string arg);
	bool isExactString(string arg2);
	bool isValidQueryLine(string selectString);
	bool isValidSynDesignEntity(string syn);
	bool is_number(string str);

	string changeLowerCase(string str);
	string removeSymbols(string str, string symbolToRemove);
	string removeDuplicatesWhiteSpaces(string str);
	string trim(string str);
	string trimPatternArgs(string str);

	vector<string> extractSuchThatClauses(string str);
	vector<string> extractWithClauses(string str);
	QueryStatement getQueryStatement();
	vector<string> splitToSentences(string strToSplit);
	vector<string> splitBySymbol(string str, char symbol);
};