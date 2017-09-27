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
	vector<SynonymEntityPair> synonymAndEntityList; //Has to be refresh after each query
	
	//The following is reserved for future iterations												
	/** 
	vector<string> validEntities = { "procedure", "stmtLst", "stmt", "assign", "call", "while", "if",
		"variable", "constant", "prog_line"};
	**/
	
	vector<string> validEntities = { "stmt", "assign", "while", 
		"variable", "constant", "prog_line"};

	string getCorrespondingEntity(string syn);

	vector<string> splitBySymbol(string str, char symbol);
	vector<string> splitStatement(vector<string> currentVector);
	vector<string> declarationString;
	vector<string> queryString;

	//void setSynonymList(vector<SynonymEntityPair> synonymList);
	void addSelectQueryElement(string ent, string syn);
	void addPatternQueryElement(string arg1, string arg2, string ent, string syn, bool arg1Variable, bool arg1Wildcard, bool arg1Synonym, bool arg2Substring, bool arg2FullString, bool arg2Wilcard);
	void addSuchThatQueryElement(QueryElement qe);
	
	bool isValidSelect(vector<string> vectorClauses);
	bool isValidOthers(vector<string> others);
	bool isValidPattern(string str, string syn);
	
	bool isValidSynonym(string syn);
	bool inEntityList(string entity);
	bool parseDeclaration(vector<string> splitString);
	bool checkRelationshipTable(string , string, int);
	void addSynonymEntityList();
	bool parseQueryLine(string);

public:
	QueryValidator();
	bool isValidDeclarationRegex(string str);
	bool isValidSuchThatRegex(string str);
	bool isValidPatternRegex(string str);
	bool isValidSelectInitialRegex(string str);
	bool isVariable(string str);
	bool parseInput(string str);
	bool isValidModifiesP(string str);
	bool isEntityAndSynonym(string);
	bool isValidSuchThat(string str, string syn);
	bool addSuchThatQueryElement(bool arg1_NUM, bool arg1_UNDER, bool arg2_NUM, bool arg2_UNDER, bool arg2_VARIABLE, string relType, string arg1, string arg2, string type1, string type2);
	bool isValidEntity(string);

	bool isVariableArg1(string arg1);
	bool isSubstringArg2(string arg2);
	bool isWildcard(string arg);	
	bool isExactString(string arg2);
	bool isValidQueryLine(string selectString);
	bool isValidSynDesignEntity(string syn);
	bool is_number(string str);
	
	void startParsing(string str);
	
	string changeLowerCase(string str);
	string removeSymbols(string str, string symbolToRemove);
	string removeDuplicatesWhiteSpaces(string str);
	string trim(string str);
	string trimPatternArgs(string str);
	
	vector<string> split(vector<string> vectorToSplit, string strToSplitWith);
	QueryStatement getQueryStatement();
};
