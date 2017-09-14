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
	
	string getCorrespondingEntity(string syn);
	string removeSymbols(string str, string symbolToRemove);
	
	vector<string> validEntities;
	vector<string> split(string, char);
	vector<string> split(string str, string symbolSplitWith);
	vector<string> split(vector<string> vectorToSplit, string strToSplitWith);
	vector<string> splitStatement(vector<string> currentVector);
	vector<string> declarationString;
	vector<string> queryString;


	void parseLine(string, string);
	void setSynonymList(vector<SynonymEntityPair> synonymList);
	void parseQueryLine(string);
	void addSelectQueryElement(string ent, string syn);
	void addPatternQueryElement(string arg1, string arg2, string ent, string syn);

	void addSuchThatQueryElement(QueryElement qe);
	
	bool isValidSelect(vector<string> vectorClauses);
	bool isValidOthers(vector<string> vector);
	bool isValidSuchThat(string str, string syn);
	bool addSuchThatQueryElement(bool arg1_NUM, bool arg1_UNDER, bool arg2_NUM, bool arg2_UNDER, string relType, string arg1, string arg2, string type1, string type2);
	bool is_number(string);
	bool isValidQueryLine(string selectString);
	bool isValidPattern(string str, string syn);
	
	bool parseEntityAndSynonym(string);
	bool checkEntityAndSynonym(string);
	bool isValidEntity(string);
	bool isValidSynonym(string);
	bool inEntityList(string entity);
	bool parseDeclaration(vector<string> splitString);

	bool checkRelationshipTable(string, string, int);
public:
	QueryValidator();
	int getNumClauses();
	string changeLowerCase(string str);
	void startParsing();
};
